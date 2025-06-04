#ifndef MODEL_MAIN_H
#define MODEL_MAIN_H

// STL >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
#include <cmath>
#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <optional>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

// PROJECT'S HEADERS >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
#include "include/common.h"

// CONSTANTS >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

// STRUCTS >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
namespace s21 {
class WireframeObject {
 protected:
  struct Coordinate {
    float x{0.0f}, y{0.0f}, z{0.0f};
    bool IsValid() const {
      return !std::isnan(x) && !std::isnan(y) && !std::isnan(z);
    }
  };
  struct TextureCoordinate {
    float u{0.0f}, v{0.0f};
    bool IsValid() const {
      return !std::isnan(u) && !std::isnan(v) && u >= 0.0f && u <= 1.0f &&
             v >= 0.0f && v <= 1.0f;
    }
  };
  struct Face {
    Coordinate position[3];
    TextureCoordinate texture[3];
    Coordinate normal;
  };
  struct Counter {
    int v = 0, vt = 0, vn = 0, f = 0;
  };

 public:
  // Rule of three
  WireframeObject(const std::string file_path);
  ~WireframeObject();
  // WireframeObject(const WireframeObject &other);
  // WireframeObject &operator=(const WireframeObject &other);

  // Member functions
  void SetName(const std::string &new_name) { name = new_name; }
  std::string GetName() const { return name; }
  int GetId() const { return id; }
  static void ResetIdCounter() { next_id = 0; }

 protected:
  int AllocateMemory(std::ifstream &file);
  void GetValues(std::ifstream &file);
  void AssignName(const std::string file_path);

 protected:
  static int next_id;
  int id;
  std::string name;
  std::vector<Coordinate> vertices;
  std::vector<TextureCoordinate> textures;
  std::vector<Coordinate> normals;
  std::vector<Face> faces;
  Counter count;
  const size_t MAX_VERTICES = 10000;
  const size_t MAX_FACES = 100000;

 protected:
  using CheckFunction = bool (WireframeObject::*)(std::istringstream &);

  bool CheckVertex(std::istringstream &iss) {
    float x, y, z;
    if (!(iss >> x >> y >> z)) return false;
    count.v++;
    return true;
  }

  bool CheckTexture(std::istringstream &iss) {
    float u, v;
    if (!(iss >> u >> v)) return false;
    count.vt++;
    return true;
  }

  bool CheckNormal(std::istringstream &iss) {
    float x, y, z;
    if (!(iss >> x >> y >> z)) return false;
    count.vn++;
    return true;
  }

  bool CheckFace(std::istringstream &iss) {
    int v, vt, vn;
    char slash;
    for (int i = 0; i < 3; i++) {
      if (!(iss >> v >> slash >> vt >> slash >> vn)) return false;
      if (v > count.v || vt > count.vt || vn > count.vn) return false;
    }
    count.f++;
    return true;
  }

  bool ValidateCounters() const {
    return count.v > 0 && count.vt > 0 && count.vn > 0 && count.f > 0 &&
           count.v <= MAX_VERTICES && count.f <= MAX_FACES;
  }

  bool ParseCoordinate(std::istringstream &iss, Coordinate &out_coord) {
    if (!(iss >> out_coord.x >> out_coord.y >> out_coord.z)) {
      return false;
    }
    return out_coord.IsValid();
  }

  bool ParseTextureCoordinate(std::istringstream &iss,
                              TextureCoordinate &out_tex_coord) {
    if (!(iss >> out_tex_coord.u >> out_tex_coord.v)) {
      return false;
    }
    return out_tex_coord.IsValid();
  }

  bool ParseFace(std::istringstream &iss, Face &out_face) {
    char slash;
    int v, vt, vn;
    for (int i = 0; i < 3; i++) {
      if (!(iss >> v >> slash >> vt >> slash >> vn)) {
        return false;
      }
      if (v <= 0 || vt <= 0 || vn <= 0 || v > count.v || vt > count.vt ||
          vn > count.vn) {
        return false;
      }
      out_face.position[i] = vertices[v - 1];
      out_face.texture[i] = textures[vt - 1];
    }
    out_face.normal = normals[vn - 1];
    return true;
  }
};  // class WireframeObject

// FUNCTIONS >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
WireframeObject::WireframeObject(const std::string file_path) {
  std::ifstream file(file_path);
  if (!file.is_open()) {
    std::cout << "in WireframeObject: Could not open file"
              << "\n";
  } else {
    if (AllocateMemory(file) == ERROR_CODE_SUCCESS) {
      id = next_id++;
      GetValues(file);
      AssignName(file_path);
    } else {
      std::cout << "in WireframeObject: Bad .obj file"
                << "\n";
    }
    file.close();
  }
}

WireframeObject::~WireframeObject() {
  vertices.clear();
  textures.clear();
  normals.clear();
  faces.clear();
  count = Counter();
  id = -1;
  name.clear();
}

int WireframeObject::AllocateMemory(std::ifstream &file) {
  const std::map<std::string, CheckFunction> inspect_types = {
      {"v", &WireframeObject::CheckVertex},
      {"vt", &WireframeObject::CheckTexture},
      {"vn", &WireframeObject::CheckNormal},
      {"f", &WireframeObject::CheckFace}};

  std::string line;
  int result_code = ERROR_CODE_SUCCESS;

  while (std::getline(file, line)) {
    if (line.empty() || line[0] == '#') continue;

    std::istringstream iss(line);
    std::string prefix;
    iss >> prefix;

    auto it = inspect_types.find(prefix);
    if (it != inspect_types.end() && !(this->*(it->second))(iss)) {
      std::cout << "in AllocateMemory: Invalid" << prefix << "format in file"
                << "\n";
      result_code = ERROR_CODE_INVALID_FORMAT;
      break;
    }
  }

  if (!ValidateCounters()) {
    result_code = ERROR_CODE_INVALID_FORMAT;
  }

  if (result_code == ERROR_CODE_SUCCESS) {
    try {
      vertices.reserve(count.v);
      textures.reserve(count.vt);
      normals.reserve(count.vn);
      faces.reserve(count.f);
    } catch (const std::bad_alloc &) {
      result_code = ERROR_CODE_MEMORY_ERROR;
    }
  }

  file.clear();
  file.seekg(0, std::ios::beg);
  return result_code;
}

void WireframeObject::GetValues(std::ifstream &file) {
  std::string line;

  while (std::getline(file, line)) {
    if (line.empty() || line[0] == '#') continue;

    std::istringstream iss(line);
    std::string prefix;
    iss >> prefix;

    if (prefix == "v") {
      Coordinate vertex;
      if (ParseCoordinate(iss, vertex)) {
        vertices.push_back(vertex);
      } else {
        std::cout << "in GetValues: Invalid vertex format in file"
                  << "\n";
      }
    } else if (prefix == "vt") {
      TextureCoordinate texture;
      if (ParseTextureCoordinate(iss, texture)) {
        textures.push_back(texture);
      } else {
        std::cout << "in GetValues: Invalid texture coordinate format in file"
                  << "\n";
      }
    } else if (prefix == "vn") {
      Coordinate normal;
      if (ParseCoordinate(iss, normal)) {
        normals.push_back(normal);
      } else {
        std::cout << "in GetValues: Invalid normal format in file"
                  << "\n";
      }
    } else if (prefix == "f") {
      Face face;
      if (ParseFace(iss, face)) {
        faces.push_back(face);
      } else {
        std::cout << "in GetValues: Invalid face format in file"
                  << "\n";
      }
    }
  }
}

void WireframeObject::AssignName(const std::string file_path) {
  size_t last_slash = file_path.find_last_of("/\\");
  if (last_slash != std::string::npos) {
    name = file_path.substr(last_slash + 1);
  } else {
    name = file_path;
  }
}

int WireframeObject::next_id = 0;
}  // namespace s21

#endif  // MODEL_MAIN_H
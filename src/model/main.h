#ifndef INCLUDE_MAIN_H
#define INCLUDE_MAIN_H

// RELATED HEADER >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

// DEPENDENCIES >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>

// PROJECT'S HEADERS >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
#include "include/common.h"

// CONSTANTS >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

// STRUCTS >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
namespace s21 {
class WireframeObject {
 protected:
  struct Coordinate {
    float x, y, z;
  };
  struct TextureCoordinate {
    float u, v;
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
  void PrintInfo() const {
    std::cout << "Wireframe Object: " << name << "\n"
              << "Vertices: " << count.v << ", "
              << "Texture Coordinates: " << count.vt << ", "
              << "Normals: " << count.vn << ", "
              << "Faces: " << count.f << "\n";
  }

 protected:
  int AllocateMemory(std::ifstream &file);
  void GetValues(std::ifstream &file);
  void AssignName(const std::string file_path);
  // void ClearData();

 protected:
  // int id;
  std::string name;
  Coordinate *vertices;
  TextureCoordinate *textures;
  Coordinate *normals;
  Face *faces;
  Counter count;

 protected:
  // `using` is a C++ keyword that allows developer to create an alias for a
  // type.
  // 1. `using CheckFunction` - This creates a new type name called
  // `CheckFunction`
  // 2. `=` - Assigns the type definition
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
};  // class WireframeObject

// FUNCTIONS >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
WireframeObject::WireframeObject(const std::string file_path) {
  std::ifstream file(file_path);
  if (!file.is_open()) {
    std::cout << "in WireframeObject: Could not open file" << "\n";
  } else {
    if (AllocateMemory(file) == ERROR_CODE_SUCCESS) {
      GetValues(file);
      AssignName(file_path);
    } else {
      std::cout << "in WireframeObject: Bad .obj file" << "\n";
    }
    file.close();
  }
}

WireframeObject::~WireframeObject() {
  delete[] vertices;
  delete[] textures;
  delete[] normals;
  delete[] faces;
}

int WireframeObject::AllocateMemory(std::ifstream &file) {
  const std::map<std::string, CheckFunction> inspect_types = {
      {"v", &WireframeObject::CheckVertex},
      {"vt", &WireframeObject::CheckTexture},
      {"vn", &WireframeObject::CheckNormal},
      {"f", &WireframeObject::CheckFace}};

  std::string line, prefix;
  int result_code = ERROR_CODE_SUCCESS;

  while (std::getline(file, line)) {
    if (line.empty() || line[0] == '#') continue;

    std::istringstream iss(line);
    iss >> prefix;

    auto it = inspect_types.find(prefix);
    if (it != inspect_types.end()) {
      if (!(this->*(it->second))(iss)) {
        std::cout << "in AllocateMemory: Invalid" << prefix << "format in file" << "\n";
        result_code = ERROR_CODE_INVALID_FORMAT;
        break;
      }
    }
  }

  if (count.v == 0 || count.vt == 0 || count.vn == 0 || count.f == 0) {
    std::cout << "in AllocateMemory: Insufficient data in file" << "\n";
    return ERROR_CODE_INVALID_FORMAT;
  }

  if (result_code == ERROR_CODE_SUCCESS) {
    vertices = new Coordinate[count.v]();
    textures = new TextureCoordinate[count.vt]();
    normals = new Coordinate[count.vn]();
    faces = new Face[count.f]();
  }

  file.clear();
  file.seekg(0, std::ios::beg);
  return result_code;
}

void WireframeObject::GetValues(std::ifstream &file) {
  std::string line, prefix;
  int v_index = 0, vt_index = 0, vn_index = 0, f_index = 0;

  while (std::getline(file, line)) {
    if (line.empty() || line[0] == '#') continue;

    std::istringstream iss(line);
    iss >> prefix;

    if (prefix == "v") {
      iss >> vertices[v_index].x >> vertices[v_index].y >> vertices[v_index].z;
      v_index++;
    } else if (prefix == "vt") {
      iss >> textures[vt_index].u >> textures[vt_index].v;
      vt_index++;
    } else if (prefix == "vn") {
      iss >> normals[vn_index].x >> normals[vn_index].y >> normals[vn_index].z;
      vn_index++;
    } else if (prefix == "f") {
      char slash;
      int v = 1, vt = 1, vn = 1;
      for (int i = 0; i < 3; i++) {
        iss >> v >> slash >> vt >> slash >> vn;
        faces[f_index].position[i] = vertices[v - 1];
        faces[f_index].texture[i] = textures[vt - 1];
        faces[f_index].normal = normals[vn - 1];
      }
      f_index++;
    }
  }
  // Open debug log file
  std::ofstream debug_log("logs/debug.log", std::ios::app);
  if (debug_log.is_open()) {
    debug_log << "Object: " << name << " - Faces data:\n";
    for (int i = 0; i < count.f; i++) {
      debug_log << "Face " << i + 1 << ":\n";
      for (int j = 0; j < 3; j++) {
        debug_log << "  Vertex " << j + 1 << ": ("
                 << faces[i].position[j].x << ", "
                 << faces[i].position[j].y << ", "
                 << faces[i].position[j].z << ")\n";
        debug_log << "  Texture: ("
                 << faces[i].texture[j].u << ", "
                 << faces[i].texture[j].v << ")\n";
      }
      debug_log << "  Normal: ("
               << faces[i].normal.x << ", "
               << faces[i].normal.y << ", "
               << faces[i].normal.z << ")\n";
      debug_log << "\n";
    }
    debug_log.close();
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
}  // namespace s21

#endif  // INCLUDE_MAIN_H
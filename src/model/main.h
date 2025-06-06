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
#include "model/errors.h"

// CONSTANTS >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
const size_t MAX_VERTICES = 10000;
const size_t MAX_FACES = 100000;

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
  ErrorCode AllocateMemory(std::ifstream &file);
  void GetValues(std::ifstream &file) noexcept;
  void AssignName(const std::string file_path) noexcept;

 protected:
  static int next_id;
  int id;
  std::string name;
  std::vector<Coordinate> vertices;
  std::vector<TextureCoordinate> textures;
  std::vector<Coordinate> normals;
  std::vector<Face> faces;
  Counter count;

 protected:
  // helper functions
  using InputFunction = bool (WireframeObject::*)(std::istringstream &);
  using ParseFunction = void (WireframeObject::*)(std::istringstream &);
  bool CheckVertex(std::istringstream &iss);
  bool CheckTexture(std::istringstream &iss);
  bool CheckNormal(std::istringstream &iss);
  bool CheckFace(std::istringstream &iss);
  bool ValidateCounters() const;
  void ParseVertex(std::istringstream &iss);
  void ParseTextureCoordinate(std::istringstream &iss);
  void ParseNormal(std::istringstream &iss);
  void ParseFace(std::istringstream &iss);
};  // class WireframeObject

// FUNCTIONS >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
WireframeObject::WireframeObject(const std::string file_path) {
  std::ifstream file(file_path);
  if (!file.is_open()) {
    LogError("WireframeObject", "File not found");
  } else {
    if (AllocateMemory(file) == success_code) {
      id = next_id++;
      GetValues(file);
      AssignName(file_path);
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

ErrorCode WireframeObject::AllocateMemory(std::ifstream &file) {
  const std::map<std::string, InputFunction> inspect_types = {
      {"v", &WireframeObject::CheckVertex},
      {"vt", &WireframeObject::CheckTexture},
      {"vn", &WireframeObject::CheckNormal},
      {"f", &WireframeObject::CheckFace}};

  std::string line;
  ErrorCode result_code = success_code;

  while (std::getline(file, line)) {
    if (line.empty() || line[0] == '#') continue;

    std::istringstream iss(line);
    std::string prefix;
    iss >> prefix;

    auto it = inspect_types.find(prefix);
    if (it != inspect_types.end() && !(this->*(it->second))(iss)) {
      QString log_message = QString("Invalid format in line: %1")
                                .arg(QString::fromStdString(line));
      LogError("AllocateMemory", log_message);
      result_code = invalid_format;
      break;
    }
  }

  if (!ValidateCounters()) {
    result_code = invalid_format;
  }

  if (result_code == success_code) {
    try {
      vertices.reserve(count.v);
      textures.reserve(count.vt);
      normals.reserve(count.vn);
      faces.reserve(count.f);
    } catch (const std::bad_alloc &) {
      result_code = memory_error;
    }
  }

  file.clear();
  file.seekg(0, std::ios::beg);
  return result_code;
}

void WireframeObject::GetValues(std::ifstream &file) noexcept {
  const std::map<std::string, ParseFunction> parse_types = {
      {"v", &WireframeObject::ParseVertex},
      {"vt", &WireframeObject::ParseTextureCoordinate},
      {"vn", &WireframeObject::ParseNormal},
      {"f", &WireframeObject::ParseFace}};

  std::string line;

  while (std::getline(file, line)) {
    if (line.empty() || line[0] == '#') continue;

    std::istringstream iss(line);
    std::string prefix;
    iss >> prefix;

    auto it = parse_types.find(prefix);
    if (it != parse_types.end()) (this->*(it->second))(iss);
  }
}

void WireframeObject::AssignName(const std::string file_path) noexcept {
  size_t last_slash = file_path.find_last_of("/\\");
  if (last_slash != std::string::npos) {
    name = file_path.substr(last_slash + 1);
  } else {
    name = file_path;
  }
}

bool WireframeObject::CheckVertex(std::istringstream &iss) {
  Coordinate vertex;
  if (!(iss >> vertex.x >> vertex.y >> vertex.z)) return false;
  if (!vertex.IsValid()) return false;
  count.v++;
  return true;
}

bool WireframeObject::CheckTexture(std::istringstream &iss) {
  TextureCoordinate texture;
  if (!(iss >> texture.u >> texture.v)) return false;
  if (!texture.IsValid()) return false;
  count.vt++;
  return true;
}

bool WireframeObject::CheckNormal(std::istringstream &iss) {
  Coordinate normal;
  if (!(iss >> normal.x >> normal.y >> normal.z)) return false;
  if (!normal.IsValid()) return false;
  if (std::abs(normal.x) > 1.0f || std::abs(normal.y) > 1.0f ||
      std::abs(normal.z) > 1.0f) {
    return false;  // Normal vector components should be in [-1, 1]
  }
  count.vn++;
  return true;
}

bool WireframeObject::CheckFace(std::istringstream &iss) {
  int v, vt, vn;
  char slash;
  for (int i = 0; i < 3; i++) {
    if (!(iss >> v >> slash >> vt >> slash >> vn)) return false;
    if (v > count.v || vt > count.vt || vn > count.vn) return false;
    if (v < 1 || vt < 1 || vn < 1) return false;
  }
  count.f++;
  return true;
}

bool WireframeObject::ValidateCounters() const {
  return count.v > 0 && count.vt > 0 && count.vn > 0 && count.f > 0 &&
         count.v <= MAX_VERTICES && count.f <= MAX_FACES;
}

void WireframeObject::ParseVertex(std::istringstream &iss) {
  Coordinate vertex;
  iss >> vertex.x >> vertex.y >> vertex.z;
  vertices.push_back(vertex);
}

void WireframeObject::ParseTextureCoordinate(std::istringstream &iss) {
  TextureCoordinate texture;
  iss >> texture.u >> texture.v;
  textures.push_back(texture);
}

void WireframeObject::ParseNormal(std::istringstream &iss) {
  Coordinate normal;
  iss >> normal.x >> normal.y >> normal.z;
  normals.push_back(normal);
}

void WireframeObject::ParseFace(std::istringstream &iss) {
  Face face;
  char slash;
  int v, vt, vn;
  for (int i = 0; i < 3; i++) {
    iss >> v >> slash >> vt >> slash >> vn;
    face.position[i] = vertices[v - 1];
    face.texture[i] = textures[vt - 1];
  }
  face.normal = normals[vn - 1];
  faces.push_back(face);
}

int WireframeObject::next_id = 0;
}  // namespace s21

#endif  // MODEL_MAIN_H
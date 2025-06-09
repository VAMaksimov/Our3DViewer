#include "model/parser.h"

namespace s21 {
int WireframeObject::next_id = 0;

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
      LogError("AllocateMemory", "Invalid format in line: " + line);
      result_code = invalid_format;
      break;
    }
  }

  if (result_code == success_code && !ValidateCounters()) {
    result_code = invalid_format;
    LogError("AllocateMemory", invalid_format);
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
}  // namespace s21

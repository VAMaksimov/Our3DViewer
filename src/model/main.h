#ifndef INCLUDE_MAIN_H
#define INCLUDE_MAIN_H

// DEPENDENCIES >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
#include <fstream>
#include <sstream>
#include <string>

// CONSTANTS >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

// STRUCTS >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
namespace s21 {
typedef struct {
  float x;
  float y;
  float z;
} Coordinate;

typedef struct {
  float u;
  float v;
} TextureCoordinate;

typedef struct {
  float x;
  float y;
  float z;
} NormalVector;

typedef struct {
  Coordinate position[3];
  TextureCoordinate texture[3];
  NormalVector normal;
} Face;

typedef struct {
  int v = 0, vt = 0, vn = 0, f = 0;
} Counter;

class WireframeObject {
 public:
  // Rule of three
  WireframeObject(const std::string file_path);
  ~WireframeObject();
  WireframeObject(const WireframeObject &other);
  WireframeObject &operator=(const WireframeObject &other);

  // Member functions
  void SetName(const std::string &new_name) { name = new_name; }
  std::string GetName() const { return name; }

 protected:
  int id;
  std::string name;
  Coordinate *vertices;
  TextureCoordinate *textures;
  NormalVector *normals;
  Face *faces;
  Counter count;

 protected:
  int AllocateMemory(std::ifstream &file);
  void GetValues(std::ifstream &file);
  void ClearData();
};  // class WireframeObject

// FUNCTIONS >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
WireframeObject::WireframeObject(const std::string file_path) {
  std::ifstream file(file_path);
  if (!file.is_open()) {
    printf("Error in WireframeObject: Could not open file %s\n", file_path.c_str());
  } else {
    if (AllocateMemory(file))
      GetValues(file);
    file.close();
  }
}

int WireframeObject::AllocateMemory(std::ifstream &file) {
  std::string line, prefix;
  int result_code = 1;

  while (std::getline(file, line)) {
    if (line.empty() || line[0] == '#') continue;

    std::istringstream iss(line);
    iss >> prefix;

    if (prefix == "v")
      count.v++;
    else if (prefix == "vt")
      count.vt++;
    else if (prefix == "vn")
      count.vn++;
    else if (prefix == "f") {
      char slash;
      int v = 1, vt = 1, vn = 1;
      iss >> v >> slash >> vt >> slash >> vn;
      if (v > count.v || vt > count.vt || vn > count.vn) {
        printf("Error in AllocateMemory: Face index out of bounds in file");
        result_code = 0;
      }
      count.f++;
    }
  }

  if (result_code == 0) {
    ClearData();
  } else {
    vertices = new Coordinate[count.v]();
    textures = new TextureCoordinate[count.vt]();
    normals = new NormalVector[count.vn]();
    faces = new Face[count.f]();
  }
  // Reset file stream to the beginning
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
}
}  // namespace s21

#endif  // INCLUDE_MAIN_H
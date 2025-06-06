#ifndef MODEL_PARSER_H
#define MODEL_PARSER_H

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
}  // namespace s21

#endif  // MODEL_PARSER_H
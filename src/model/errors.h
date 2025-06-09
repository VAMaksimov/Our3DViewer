#ifndef MODEL_ERRORS_H
#define MODEL_ERRORS_H

#include <chrono>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <string>

namespace s21 {

typedef enum {
  success_code = 0,
  file_not_found,
  invalid_format,
  memory_error,
  unknown_error
} ErrorCode;

void LogError(const std::string& component, const std::string& message);
void LogError(const std::string& component, const ErrorCode status);
std::string GetStatusMessage(ErrorCode status);

static std::string GetCurrentTimestamp() {
  auto now = std::chrono::system_clock::now();
  auto time = std::chrono::system_clock::to_time_t(now);
  std::stringstream ss;
  ss << std::put_time(std::localtime(&time), "%Y-%m-%d %H:%M:%S");
  return ss.str();
}

static std::string FormatErrorMessage(const std::string& component,
                                      const std::string& message) {
  return "[" + GetCurrentTimestamp() + "] " + component + ": " + message + "\n";
}

}  // namespace s21

#endif  // MODEL_ERRORS_H
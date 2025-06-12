#include "model/errors.h"

namespace s21 {

void LogError(const std::string& component, const std::string& message) {
  std::string log_message = FormatErrorMessage(component, message);

  std::ofstream file("logs/debug.log", std::ios::app);
  if (file.is_open()) {
    file << log_message;
    file.close();
  }

  std::cerr << log_message;
}

void LogError(const std::string& component, const ErrorCode status) {
  std::string message = GetStatusMessage(status);
  LogError(component, message);
}

std::string GetStatusMessage(ErrorCode status) {
  switch (status) {
    case ErrorCode::success_code:
      return "Operation successful";
    case ErrorCode::file_not_found:
      return "File not found";
    case ErrorCode::invalid_format:
      return "Invalid file format";
    case ErrorCode::memory_error:
      return "Memory allocation error";
    default:
      return "Unknown error";
  }
}

std::string GetCurrentTimestamp() {
  auto now = std::chrono::system_clock::now();
  auto time = std::chrono::system_clock::to_time_t(now);
  std::stringstream ss;
  ss << std::put_time(std::localtime(&time), "%Y-%m-%d %H:%M:%S");
  return ss.str();
}

std::string FormatErrorMessage(const std::string& component,
                               const std::string& message) {
  return "[" + GetCurrentTimestamp() + "] " + component + ": " + message + "\n";
}

}  // namespace s21
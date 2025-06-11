#ifndef MODEL_ERRORS_H
#define MODEL_ERRORS_H

#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>
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

std::string GetCurrentTimestamp();

std::string FormatErrorMessage(const std::string& component,
                               const std::string& message);

}  // namespace s21

#endif  // MODEL_ERRORS_H
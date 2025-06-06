#include "model/errors.h"

namespace s21 {
static QString last_error_message;

void LogError(const QString& component, const QString& message) {
  QString log_message = FormatErrorMessage(component, message);
  last_error_message = log_message;

  QFile file("logs/debug.log");
  if (file.open(QIODevice::Append | QIODevice::Text)) {
    QTextStream stream(&file);
    stream << log_message;
    file.close();
  }

  qDebug() << log_message;
}

void LogError(const QString& component, const ErrorCode status) {
  QString message = GetStatusMessage(status);
  LogError(component, message);
}

QString GetStatusMessage(ErrorCode status) {
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

QString GetLastErrorMessage() {
  return last_error_message;
}
}  // namespace s21
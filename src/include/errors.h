#ifndef INCLUDE_ERRORS_H
#define INCLUDE_ERRORS_H

#include <QDateTime>
#include <QDebug>
#include <QFile>
#include <QString>

namespace s21 {
typedef enum {
  success_code = 0,
  file_not_found,
  invalid_format,
  memory_error,
  unknown_error
} ErrorCode;

class ErrorLogger {
 public:
  void LogError(const QString& component, const QString& message) {
    QString log_message =
        QString("[%1] %2: %3\n")
            .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"))
            .arg(component)
            .arg(message);

    QFile file("logs/debug.log");
    if (file.open(QIODevice::Append | QIODevice::Text)) {
      QTextStream stream(&file);
      stream << log_message;
      file.close();
    }

    qDebug() << log_message;
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
};  // class ErrorLogger
}  // namespace s21

#endif  // INCLUDE_ERRORS_H
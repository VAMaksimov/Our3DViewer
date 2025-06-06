#ifndef MODEL_ERRORS_H
#define MODEL_ERRORS_H

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

void LogError(const QString& component, const QString& message) {
  // .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"))
  QString log_message = QString("%2: %3\n").arg(component).arg(message);

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
}  // namespace s21

#endif  // MODEL_ERRORS_H
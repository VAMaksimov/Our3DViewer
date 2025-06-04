#ifndef INCLUDE_ERRORS_H
#define INCLUDE_ERRORS_H

#include <QDateTime>
#include <QDebug>
#include <QFile>
#include <QString>

namespace s21 {
enum ParserStatus {
  Success,
  FileNotFound,
  InvalidFormat,
  InsufficientData,
  MemoryError,
  InvalidVertex,
  InvalidTexture,
  InvalidNormal,
  InvalidFace
};

class ErrorLogger {
 public:
  static void LogError(const QString& component, const QString& message) {
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

  static QString GetStatusMessage(ParserStatus status) {
    switch (status) {
      case ParserStatus::Success:
        return "Operation successful";
      case ParserStatus::FileNotFound:
        return "File not found";
      case ParserStatus::InvalidFormat:
        return "Invalid file format";
      case ParserStatus::InsufficientData:
        return "Insufficient data in file";
      case ParserStatus::MemoryError:
        return "Memory allocation error";
      case ParserStatus::InvalidVertex:
        return "Invalid vertex data";
      case ParserStatus::InvalidTexture:
        return "Invalid texture coordinates";
      case ParserStatus::InvalidNormal:
        return "Invalid normal data";
      case ParserStatus::InvalidFace:
        return "Invalid face data";
      default:
        return "Unknown error";
    }
  }
};

struct ParserResult {
  ParserStatus status{ParserStatus::Success};
  QString message;

  bool isSuccess() const { return status == ParserStatus::Success; }
};
}  // namespace s21

#endif  // INCLUDE_ERRORS_H
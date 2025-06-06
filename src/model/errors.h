#ifndef MODEL_ERRORS_H
#define MODEL_ERRORS_H

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

/**
 * @brief Logs error messages to a file and outputs them to the console
 * @param component The component where the error occurred
 * @param message The error message to log
 *
 * @note `static` ensures that this function is only visible within this header.
 * It prevents multiple definitions if this header is included in multiple
 * translation units.
 */
void LogError(const QString& component, const QString& message);

void LogError(const QString& component, const ErrorCode status);

static QString FormatErrorMessage(const QString& component,
                                  const QString& message) {
  return QString("[%1] %2: %3\n")
      .arg(component)
      .arg(message)
      .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
}

QString GetStatusMessage(ErrorCode status);
QString GetLastErrorMessage();
}  // namespace s21

#endif  // MODEL_ERRORS_H
#include "model/errors.h"
#include "model/parser.h"
#include "view/viewer_widget.h"

int main(int argc, char *argv[]) {
  s21::LogError("main", "\n\n-- Application started --");
  QApplication app(argc, argv);
  s21::ViewerWidget viewerWidget;
  viewerWidget.show();
  return app.exec();
}

#include "model/parser.h"
#include "model/errors.h"
#include "view/viewer_widget.h"

int main(int argc, char *argv[]) {
  LogError("main", "\n-- Application started --");
  QApplication app(argc, argv);
  s21::ViewerWidget viewerWidget;
  viewerWidget.show();
  return app.exec();
}

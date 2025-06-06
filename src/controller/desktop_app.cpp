#include "model/parser.h"
#include "view/viewer_widget.h"

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);
  s21::ViewerWidget viewerWidget;
  viewerWidget.show();
  return app.exec();
}

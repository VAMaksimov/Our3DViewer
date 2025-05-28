#include "gui/desktop/qt_widget.h"

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);
  s21::TetrisWidget tetrisWidget;
  return app.exec();
}

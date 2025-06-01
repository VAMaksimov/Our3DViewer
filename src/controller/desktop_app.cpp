#include "view/qt_widget.h"

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);
  s21::TetrisWidget tetrisWidget;
  tetrisWidget.show();
  return app.exec();
}

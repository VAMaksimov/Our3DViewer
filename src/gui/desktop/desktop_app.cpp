#include "gui/desktop/qt_widget.h"

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);
  s21::TetrisWidget tetrisWidget;
  s21::SnakeWidget snakeWidget;

  printf("Do you wanna play snake or tetris\n");
  printf("Press 't' for tetris or 's' for snake\n");
  char choice = 't';
  scanf("%c", &choice);

  if (choice == 't') {
    printf("Starting Tetris... \n");
    tetrisWidget.show();
  } else {
    printf("Starting Snake... \n");
    snakeWidget.show();
  }
  return app.exec();
}

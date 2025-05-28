#ifndef INCLUDE_QT_WIDGET_H
#define INCLUDE_QT_WIDGET_H

#include <stdio.h>

#include <QApplication>
#include <QKeyEvent>    // Event for key presses/releases
#include <QPaintEvent>  // Event triggering painting
#include <QPainter>     // For custom drawing
#include <QTimer>
#include <QWidget>

#include "include/common.h"
#include "include/snake.h"
#include "include/tetris.h"

#define CELL_SIZE 15
#define STATS_AREA_SIZE 60

namespace s21 {
/**
 * @brief The SnakeWidget class is a custom QWidget that represents the
 * gameboard for the Snake game. It handles user input, rendering the game
 * state, and managing the game loop using a QTimer. The class inherits from
 * QWidget and provides methods for handling key events, painting the game
 * board, updating the game state, and displaying game statistics.
 *
 * @note Q_OBJECT macro is used to enable Qt's meta-object features, such as
 * signals and slots, which are essential for event handling and communication
 * between objects in Qt applications.
 */
class SnakeWidget : public QWidget {
  Q_OBJECT

 public:
  SnakeWidget(QWidget *parent = nullptr);
  ~SnakeWidget() override;

 protected:
  void keyPressEvent(QKeyEvent *event) override;
  void paintEvent(QPaintEvent *event) override;
  void PrintInfo(QPainter &painter, GameInfo_t *game_info, UserAction_t state);

 private slots:
  void gameTick();

 private:
  SnakeGame *snake_game;
  QTimer *game_timer;

  void StartGame();

  void TogglePause();
};  // class SnakeWidget

class TetrisWidget : public QWidget {
  Q_OBJECT

 public:
  TetrisWidget(QWidget *parent = nullptr);
  ~TetrisWidget() override;

 protected:
  void keyPressEvent(QKeyEvent *event) override;
  void paintEvent(QPaintEvent *event) override;
  void PrintInfo(QPainter &painter, GameInfo_t *game_info, UserAction_t state);
  void ShowFigure(QPainter &painter);
  void DisplayNextFigure(QPainter &painter);

 private slots:
  void gameTick();

 private:
  TetrisGame *tetris_game;
  QTimer *game_timer;

  void StartGame();

  void TogglePause();
};  // class TetrisWidget
}  // namespace s21

#endif  // INCLUDE_QT_WIDGET_H

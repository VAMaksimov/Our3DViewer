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
#include "include/tetris.h"

#define CELL_SIZE 15
#define STATS_AREA_SIZE 60

namespace s21 {
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

#ifndef INCLUDE_QT_WIDGET_H
#define INCLUDE_QT_WIDGET_H

#include <stdio.h>

#include <QApplication>
#include <QKeyEvent>    // Event for key presses/releases
#include <QPaintEvent>  // Event triggering painting
#include <QPainter>     // For custom drawing
#include <QTimer>
#include <QWidget>

#include "include/main.h"

namespace s21 {
class ViewerWidget : public QWidget {
  Q_OBJECT

 public:
  ViewerWidget(QWidget *parent = nullptr);
  ~ViewerWidget() override;

 protected:
  void keyPressEvent(QKeyEvent *event) override;
  void paintEvent(QPaintEvent *event) override;
  void PrintInfo(QPainter &painter, GameInfo_t *game_info, UserAction_t state);
  void ShowFigure(QPainter &painter);
  void DisplayNextFigure(QPainter &painter);
};  // class ViewerWidget
}  // namespace s21

#endif  // INCLUDE_QT_WIDGET_H

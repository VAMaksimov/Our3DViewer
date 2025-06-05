#ifndef VIEW_VIEWER_WIDGET_H
#define VIEW_VIEWER_WIDGET_H

#include <stdio.h>

#include <QApplication>
#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QLabel>
#include <memory>

#include "model/main.h"

namespace s21 {
/**
 * @brief Class creates a window with two panels:
 * Left panel containing an "Open File" button
 * Main panel showing basic information about the loaded wireframe object
 * 
 * @note OpenFile slot handles file selection and object creation,
 * while UpdateObjectInfo updates the display with basic object information.
 */
class ViewerWidget : public QWidget {
  Q_OBJECT

 public:
  ViewerWidget(QWidget *parent = nullptr);
  ~ViewerWidget() override;

 private slots:
  void OpenFile();
  void UpdateObjectInfo();

 private:
  void InitializeUI();
  void CreateLayouts();

  QPushButton* open_file_button;
  QLabel* object_info_label;
  QWidget* left_panel;
  QWidget* main_panel;
  std::unique_ptr<WireframeObject> current_object;
};

}  // namespace s21

#endif  // VIEW_VIEWER_WIDGET_H
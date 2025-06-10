#ifndef VIEW_VIEWER_WIDGET_H
#define VIEW_VIEWER_WIDGET_H

#include <stdio.h>

#include <QApplication>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include <QPushButton>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QWidget>
#include <memory>

#include "model/parser.h"

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
  ViewerWidget(QWidget* parent = nullptr);
  ~ViewerWidget() override;

 private slots:
  void OpenFile();
  void UpdateObjectInfo();

 private:
  /**
   * @brief Initializes the user interface elements of the viewer widget
   *
   * Creates and sets up the basic UI components including:
   * - Open file button
   * - Object information label
   * - Left panel widget
   * - Main panel widget
   *
   * Also establishes signal-slot connection for the "open file" button click
   * event
   */
  void InitializeWidgets();
  void DefineLayouts();
  void ShowError();

  QPushButton* open_file_button;
  QLabel* object_info_label;
  QWidget* left_panel;
  QWidget* main_panel;
  QTextEdit* log_viewer;
  Scene* main_viewer;
  std::unique_ptr<WireframeObject> current_object;
};

}  // namespace s21

#endif  // VIEW_VIEWER_WIDGET_H
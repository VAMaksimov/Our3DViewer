#include "view/viewer_widget.h"

namespace s21 {

ViewerWidget::ViewerWidget(QWidget* parent) : QWidget(parent) {
  InitializeUI();
  CreateLayouts();
}

ViewerWidget::~ViewerWidget() {}

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
void ViewerWidget::InitializeUI() {
  open_file_button = new QPushButton("Open File", this);
  object_info_label = new QLabel("No object loaded", this);
  left_panel = new QWidget(this);
  main_panel = new QWidget(this);

  connect(open_file_button, &QPushButton::clicked, this,
          &ViewerWidget::OpenFile);
}

void ViewerWidget::CreateLayouts() {
  // Left panel layout
  QVBoxLayout* leftLayout = new QVBoxLayout(left_panel);
  leftLayout->addWidget(open_file_button);
  leftLayout->addStretch();
  left_panel->setFixedWidth(200);

  // Main panel layout
  QVBoxLayout* main_layout = new QVBoxLayout(main_panel);
  main_layout->addWidget(object_info_label);
  main_layout->addStretch();

  // Main widget layout
  QHBoxLayout* main_widget_layout = new QHBoxLayout(this);
  main_widget_layout->addWidget(left_panel);
  main_widget_layout->addWidget(main_panel);
}

void ViewerWidget::OpenFile() {
  QString file_path = QFileDialog::getOpenFileName(this, "Open Object File", "",
                                                   "Object Files (*.obj)");
  if (!file_path.isEmpty()) {
    try {
      current_object =
          std::make_unique<WireframeObject>(file_path.toStdString());
      UpdateObjectInfo();
    } catch (const std::exception& e) {
      object_info_label->setText("Error loading file: " + QString(e.what()));
    }
  }
}

void ViewerWidget::UpdateObjectInfo() {
  if (current_object) {
    QString info = QString("Object Name: %1\nObject ID: %2")
                       .arg(QString::fromStdString(current_object->GetName()))
                       .arg(current_object->GetId());
    object_info_label->setText(info);
  }
}

}  // namespace s21

#include "view/viewer_widget.h"

namespace s21 {

ViewerWidget::ViewerWidget(QWidget* parent) : QWidget(parent) {
  InitializeUI();
  CreateLayouts();
}

ViewerWidget::~ViewerWidget() {}

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

void ViewerWidget::ShowError() {
  QString error_message = GetLastErrorMessage();
  if (!error_message.isEmpty()) {
    QMessageBox::critical(this, "Error", error_message);
  }
}

void ViewerWidget::OpenFile() {
  QString file_path = QFileDialog::getOpenFileName(this, "Open Object File", "",
                                                   "Object Files (*.obj)");
  current_object = std::make_unique<WireframeObject>(file_path.toStdString());
  UpdateObjectInfo();
}

void ViewerWidget::UpdateObjectInfo() {
  if (current_object->GetId() > 0) {
    QString info = QString("Object Name: %1\nObject ID: %2")
                       .arg(QString::fromStdString(current_object->GetName()))
                       .arg(current_object->GetId());
    object_info_label->setText(info);
  } else {
    ShowError();
  }
}

}  // namespace s21

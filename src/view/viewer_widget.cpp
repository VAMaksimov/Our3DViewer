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
  log_viewer = new QTextEdit(this);

  connect(open_file_button, &QPushButton::clicked, this,
          &ViewerWidget::OpenFile);
}

void ViewerWidget::CreateLayouts() {
  // Left panel layout
  QVBoxLayout* left_layout = new QVBoxLayout(left_panel);
  left_layout->addWidget(open_file_button);
  left_layout->addStretch();
  left_panel->setFixedWidth(200);

  // Main panel layout
  QVBoxLayout* main_layout = new QVBoxLayout(main_panel);
  main_layout->addWidget(object_info_label);
  main_layout->addStretch();
  log_viewer->setReadOnly(true);
  log_viewer->setFixedHeight(150);
  main_layout->addWidget(log_viewer);

  // Main widget layout
  QHBoxLayout* main_widget_layout = new QHBoxLayout(this);
  main_widget_layout->addWidget(left_panel);
  main_widget_layout->addWidget(main_panel);
}

void ViewerWidget::ShowError() {
  QString error_message;
  QFile file("logs/debug.log");
  if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    QTextStream in(&file);
    while (!in.atEnd()) {
      error_message = in.readLine();
    }
    file.close();
  }
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
  if (current_object->GetId() >= 0) {
    QString info = QString("Object Name: %1\nObject ID: %2")
                       .arg(QString::fromStdString(current_object->GetName()))
                       .arg(current_object->GetId());
    object_info_label->setText(info);
  } else {
    ShowError();
    UpdateLogViewer();
  }
}

void ViewerWidget::UpdateLogViewer() {
  QFile file("logs/debug.log");
  if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    QTextStream in(&file);
    log_viewer->setText(in.readAll());
    file.close();
  }
}

}  // namespace s21

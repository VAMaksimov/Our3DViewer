#include "view/viewer_widget.h"

#include "view/scene.h"

namespace s21 {

ViewerWidget::ViewerWidget(QWidget* parent) : QWidget(parent) {
  InitializeWidgets();
  DefineLayouts();
}

ViewerWidget::~ViewerWidget() {}

void ViewerWidget::InitializeWidgets() {
  open_file_button = new QPushButton("Open File", this);
  object_info_label = new QLabel("No object loaded", this);
  left_panel = new QWidget(this);
  main_panel = new QWidget(this);
  log_viewer = new QTextEdit(this);
  main_viewer = new Scene(this);

  connect(open_file_button, &QPushButton::clicked, this,
          &ViewerWidget::OpenFile);
}

void ViewerWidget::DefineLayouts() {
  SetLeftPanel();
  SetMainPanel();
  // QHBox - horizontal appends, QVBox - vertical appends
  QHBoxLayout* main_widget_layout = new QHBoxLayout(this);
  main_widget_layout->addWidget(left_panel);
  main_widget_layout->addWidget(main_panel);
}

void ViewerWidget::SetLeftPanel() {
  QVBoxLayout* left_layout = new QVBoxLayout(left_panel);
  left_layout->addWidget(open_file_button);
  left_layout->addWidget(object_info_label);
  left_layout->addStretch();
  left_panel->setFixedWidth(200);
}

void ViewerWidget::SetMainPanel() {
  QVBoxLayout* main_layout = new QVBoxLayout(main_panel);
  main_viewer->setFixedHeight(1000);
  main_viewer->setFixedWidth(1000);
  main_layout->addWidget(main_viewer);
  log_viewer->setReadOnly(true);
  log_viewer->setFixedHeight(150);
  main_layout->addWidget(log_viewer);
}

void ViewerWidget::ShowError() {
  std::ifstream file("logs/debug.log", std::ios::in);
  if (!file.is_open()) {
    QMessageBox::critical(this, "Error", "logs/debug.log not found. Try again");
    std::ofstream log_file("logs/debug.log", std::ios::out);
    log_file.close();
  } else {
    std::string error_message;
    GetLastLine(file, error_message);
    file.close();
    if (!error_message.isEmpty()) {
      QMessageBox::critical(this, "Error", error_message);
      log_viewer->append(QString::fromStdString(error_message));
    }
  }
}

void ViewerWidget::UpdateObjectInfo() {
  if (current_object->GetId() >= 0) {
    QString info = QString("Object Name: %1\nObject ID: %2\nNo of faces: %3")
                       .arg(QString::fromStdString(current_object->GetName()))
                       .arg(current_object->GetId())
                       .arg(current_object->GetFaces().size());
    object_info_label->setText(info);
    main_viewer->SetModel(current_object);
    main_viewer->update();
  } else {
    ShowError();
  }
}

// helper funcs
void GetLastLine(const std::ifstream& file, std::string& error_message) {
  std::string line;
  while (std::getline(file, line)) {
    error_message = line;
  }
}

}  // namespace s21

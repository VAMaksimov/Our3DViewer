#include "view/viewer_widget.h"

namespace s21 {

void ViewerWidget::OpenFile() {
  QString file_path = QFileDialog::getOpenFileName(this, "Open Object File", "",
                                                   "Object Files (*.obj)");
  current_object = std::make_unique<WireframeObject>(file_path.toStdString());
  UpdateObjectInfo();
}
}  // namespace s21

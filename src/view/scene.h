#ifndef VIEW_SCENE_H_
#define VIEW_SCENE_H_

#include <QOpenGLBuffer>
#include <QOpenGLWidget>
#include <QVector3D>

#include "model/parser.h"  // WireframeObject

namespace s21 {

class Scene : public QOpenGLWidget {
  Q_OBJECT

 public:
  Scene(QWidget* parent = nullptr);
  ~Scene() override;
  void SetModel(const std::shared_ptr<WireframeObject> model);

 protected:
  void initializeGL() override;
  void paintGL() override;

 protected:
  QOpenGLBuffer vbo_;
  std::shared_ptr<WireframeObject> model_ = nullptr;
};

}  // namespace s21

#endif  // VIEW_SCENE_H_

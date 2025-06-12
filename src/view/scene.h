#ifndef VIEW_SCENE_H_
#define VIEW_SCENE_H_

#include <QOpenGLBuffer>
#include <QOpenGLWidget>
#include <QVector3D>
#include <QWheelEvent>

#include "model/parser.h"

namespace s21 {

class Scene : public QOpenGLWidget {
    Q_OBJECT

public:
    Scene(QWidget* parent = nullptr);
    ~Scene() override;
    void SetModel(const WireframeObject& model) {
        model_ = std::make_shared<WireframeObject>(model);
        vbo_.bind();
        vbo_.allocate(model_->vertices.data(),
                      model_->vertices.size() * sizeof(QVector3D));
        vbo_.release();
    }
   void wheelEvent(QWheelEvent *event) override;
   void mousePressEvent(QMouseEvent* event) override;
   void mouseMoveEvent(QMouseEvent* event) override;
   void mouseReleaseEvent(QMouseEvent* event) override;

protected:
    void initializeGL() override;
    void paintGL() override;

protected:
    QOpenGLBuffer vbo_;
    std::shared_ptr<WireframeObject> model_ = nullptr;

private:
    float scale_factor_ = 1.0f;  // Начальный масштаб
    bool is_dragging = false;
    QPoint last_mouse_pos;
    QVector2D model_position;

   /* bool is_rotating = false;
    QPoint last_rotate_pos;
    S21Matrix rotation_matrix;  // Матрица поворота 4x4
    QVector3D rotation_angles;  // Углы поворота по осям X,Y,Z*/
};

// Примеры цветов:
// Белый: (1.0, 1.0, 1.0, 1.0)
// Чёрный: (0.0, 0.0, 0.0, 1.0)
// Красный: (1.0, 0.0, 0.0, 1.0)
// Зелёный: (0.0, 1.0, 0.0, 1.0)
// Синий: (0.0, 0.0, 1.0, 1.0)
// Прозрачный (если используется альфа-блендинг): (0.0, 0.0, 0.0, 0.0)

} // namespace s21


#endif  // VIEW_SCENE_H_

#ifndef VIEW_SCENE_H_
#define VIEW_SCENE_H_

#include <QOpenGLWidget>
#include <QOpenGLBuffer>
#include <QVector3D>
#include <QWheelEvent>
#include "model/parser.h" // WireframeObject

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


protected:
    void initializeGL() override;
    void paintGL() override;

protected:
    QOpenGLBuffer vbo_;
    std::shared_ptr<WireframeObject> model_ = nullptr;

private:
    float scale_factor_ = 1.0f;  // Начальный масштаб
};

} // namespace s21

#endif // VIEW_SCENE_H_

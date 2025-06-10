#ifndef VIEW_SCENE_H_
#define VIEW_SCENE_H_

#include <QOpenGLWidget>
#include <QOpenGLBuffer>
#include <QWheelEvent>
#include <QVector3D>

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


protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int w, int h) override;

protected:
    QOpenGLBuffer vbo_;
    std::shared_ptr<WireframeObject> model_;
};

} // namespace s21

#endif // VIEW_SCENE_H_

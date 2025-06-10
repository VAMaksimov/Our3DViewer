// #ifndef VIEW_VIEWER_WIDGET_H_
// #define VIEW_VIEWER_WIDGET_H_

// #include <QWidget>
// #include <memory>

// namespace s21 {

// class Scene : public QWidget {
//     Q_OBJECT

// public:
//     Scene(QWidget* parent = nullptr, WireframeObject aobject) : model_(aobject);
//     ~Scene() override;

// protected:
//     void initializeGL() override;
//     void paintGL() override;
//     // void resizeGL(int w, int h) override;

// protected:
//     // std::unique_ptr<Facade> renderer_;
//     WireframeObject model_;
// };

// } // namespace s21

// #endif  // VIEW_VIEWER_WIDGET_H_

// view/scene.h
#ifndef VIEW_SCENE_H_
#define VIEW_SCENE_H_

#include <QOpenGLWidget>
#include <QOpenGLBuffer>
#include <QVector3D>

#include "model/parser.h" // WireframeObject

namespace s21 {

class Scene : public QOpenGLWidget {
    Q_OBJECT

public:
    Scene(QWidget* parent = nullptr);
    ~Scene() override;

protected:
    void initializeGL() override;
    void paintGL() override;
    void SetModel(const WireframeObject& model) {
        model_ = std::make_shared<WireframeObject>(model);
        vbo_.bind();
        vbo_.allocate(model_->vertices.data(), 
                     model_->vertices.size() * sizeof(QVector3D));
        vbo_.release();
    }

protected:
    QOpenGLBuffer vbo_;
    std::shared_ptr<WireframeObject> model_;
};

Scene::Scene(QWidget* parent) : QOpenGLWidget(parent), vbo_(QOpenGLBuffer::VertexBuffer) {
    model_ = std::make_shared<WireframeObject>("");
}

Scene::~Scene() {
    vbo_.destroy();
}

void Scene::initializeGL() {
    vbo_.create();
    vbo_.bind();
    if (model_) {
        vbo_.allocate(model_->vertices.data(), 
                     model_->vertices.size() * sizeof(QVector3D));
    }
    vbo_.release();
}

void Scene::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    if (model_) {
        vbo_.bind();
        glVertexPointer(3, GL_FLOAT, 0, nullptr);
        glEnableClientState(GL_VERTEX_ARRAY);
        glDrawArrays(GL_POINTS, 0, model_->vertices.size());
        glDisableClientState(GL_VERTEX_ARRAY);
        vbo_.release();
    }
}

} // namespace s21

#endif // VIEW_SCENE_H_

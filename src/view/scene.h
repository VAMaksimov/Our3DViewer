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

#include <QtOpenGLWidgets/QOpenGLWidget>
#include <QOpenGLBuffer>
#include <QVector3D>

#include "model/parser.h" // WireframeObject

namespace s21 {

class Scene : public QOpenGLWidget {
    Q_OBJECT

public:
    Scene(const std::unique_ptr<WireframeObject> model = nullptr, QWidget* parent = nullptr);
    ~Scene() override;

    void SetModel(const WireframeObject& model);

protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int w, int h) override;

    // Mouse events for rotation and zoom
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;

private:
    // QOpenGLShaderProgram program_;
    QOpenGLBuffer vbo_;

    std::unique_ptr<WireframeObject> model_;
    QVector3D rotation_{0.0f, 0.0f, 0.0f};
    float scale_{1.0f};

    QPoint last_pos_;
};

} // namespace s21

#endif // VIEW_SCENE_H_

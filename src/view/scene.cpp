// #include "scene.h"

// namespace s21 {
// Scene(QWidget* parent = nullptr){

// }
// ~Scene() override {

// }

// protected:
// void initializeGL() override;
// void paintGL() override;
// // void resizeGL(int w, int h) override;

// protected:
// // std::unique_ptr<Facade> renderer_;
// WireframeObject model_;
// }

// view/scene.cpp

#include "scene.h"
#include <QOpenGLFunctions>
#include <QDebug>
#include <QMouseEvent>

namespace s21 {

Scene::Scene(QOpenGLWidget* parent) : QOpenGLWidget(parent) {}

Scene::~Scene() {
    makeCurrent();
    vbo_.destroy();
    doneCurrent();
}

void Scene::SetModel(const WireframeObject& model) {
    model_ = model;
    update(); // перерисовываем с новой моделью
}

void Scene::initializeGL() {
    initializeOpenGLFunctions();

    // Компилируем шейдеры
    const char* vertexShaderSource = R"(
        #version 330 core
        in vec3 vertexPosition;
        uniform mat4 matrix;
        void main() {
            gl_Position = matrix * vec4(vertexPosition, 1.0);
        }
    )";

    const char* fragmentShaderSource = R"(
        #version 330 core
        out vec4 fragColor;
        void main() {
            fragColor = vec4(1.0, 0.0, 0.0, 1.0); // красные точки
        }
    )";

    if (!program_.addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSource)) {
        qDebug() << "Vertex shader error";
    }

    if (!program_.addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSource)) {
        qDebug() << "Fragment shader error";
    }

    if (!program_.link()) {
        qDebug() << "Shader link error";
    }

    // Загружаем вершины в VBO
    vbo_.create();
    vbo_.bind();
    vbo_.allocate(model_->GetVertices().data(),
                  model_->GetVertices().size() * sizeof(WireframeObject::Coordinate));
    vbo_.release();
}

void Scene::paintGL() {
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_DEPTH_TEST);

    program_.bind();

    // Строим матрицу преобразования
    QMatrix4x4 matrix;
    matrix.perspective(45.0f, width() * 1.0f / height(), 0.1f, 100.0f);
    matrix.translate(0.0f, 0.0f, -3.0f);
    matrix.scale(scale_);
    matrix.rotate(rotation_.x(), 1.0f, 0.0f, 0.0f);
    matrix.rotate(rotation_.y(), 0.0f, 1.0f, 0.0f);

    program_.setUniformValue("matrix", matrix);

    // Подключаем буфер вершин
    vbo_.bind();
    GLuint posAttr = program_.attributeLocation("vertexPosition");
    glEnableVertexAttribArray(posAttr);
    glVertexAttribPointer(posAttr, 3, GL_FLOAT, GL_FALSE, sizeof(WireframeObject::Coordinate), nullptr);

    // Рисуем точки
    glPointSize(2.0f);
    glDrawArrays(GL_POINTS, 0, model_.GetVertices().size());

    vbo_.release();
    program_.release();
}

void Scene::resizeGL(int w, int h) {
    if (h == 0) h = 1;
    glViewport(0, 0, w, h);
}

// === Управление мышью ===
void Scene::mousePressEvent(QMouseEvent* event) {
    last_pos_ = event->pos();
}

void Scene::mouseMoveEvent(QMouseEvent* event) {
    if (event->buttons() & Qt::LeftButton) {
        rotation_.setX(rotation_.x() + 0.5f * (event->y() - last_pos_.y()));
        rotation_.setY(rotation_.y() + 0.5f * (event->x() - last_pos_.x()));
        update();
    }
    last_pos_ = event->pos();
}

void Scene::wheelEvent(QWheelEvent* event) {
    float delta = event->angleDelta().y();
    scale_ += delta * 0.001f;
    if (scale_ < 0.1f) scale_ = 0.1f;
    if (scale_ > 5.0f) scale_ = 5.0f;
    update();
}

} // namespace s21

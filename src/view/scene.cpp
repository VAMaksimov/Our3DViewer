
#include "view/scene.h"

namespace s21 {
Scene::Scene(QWidget* parent) : QOpenGLWidget(parent), vbo_(QOpenGLBuffer::VertexBuffer) {}

Scene::~Scene() {
    vbo_.destroy();
}

void Scene::initializeGL() {
    glEnable(GL_DEPTH_TEST); // Включаем глубинный тест
    vbo_.create();
    vbo_.bind();
    if (model_) {
        vbo_.allocate(model_->vertices.data(),
                      model_->vertices.size() * sizeof(QVector3D));
    }
    vbo_.release();
}

void Scene::wheelEvent(QWheelEvent *event) {
    // Увеличиваем или уменьшаем масштаб в зависимости от направления прокрутки
    if (event->angleDelta().y() > 0) {
        scale_factor_ *= 1.1f;  // Увеличение
    } else {
        scale_factor_ /= 1.1f;  // Уменьшение
    }

    update();
}

// void Scene::paintGL() {
//     glClear(GL_COLOR_BUFFER_BIT);
//     glLoadIdentity();

//     if (model_) {
//         vbo_.bind();
//         glVertexPointer(3, GL_FLOAT, 0, nullptr);
//         glEnableClientState(GL_VERTEX_ARRAY);
//         glDrawArrays(GL_POINTS, 0, model_->vertices.size());
//         glDisableClientState(GL_VERTEX_ARRAY);
//         vbo_.release();
//     }
// }

void Scene::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // Применяем масштабирование
    glScalef(scale_factor_, scale_factor_, scale_factor_);

    if (model_) {
        // Отрисовка треугольников с нормалями
        glBegin(GL_TRIANGLES);
        for (const auto& face : model_->faces) {
            for (int i = 0; i < 3; ++i) {
                glNormal3f(face.normal.x, face.normal.y, face.normal.z);
                glVertex3f(face.position[i].x, face.position[i].y, face.position[i].z);
            }
        }
        glEnd();
    }
}



}

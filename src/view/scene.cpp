
#include "view/scene.h"

namespace s21 {
Scene::Scene(QWidget* parent)
    : QOpenGLWidget(parent), vbo_(QOpenGLBuffer::VertexBuffer) {}

Scene::~Scene() { vbo_.destroy(); }

void Scene::SetModel(const std::shared_ptr<WireframeObject> model) {
  model_ = model;
  vbo_.bind();
  vbo_.allocate(model_->vertices.data(),
                model_->vertices.size() * sizeof(QVector3D));
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

void Scene::initializeGL() {
    // glClearColor(0.1f, 0.2f, 0.4f, 1.0f);
    glEnable(GL_DEPTH_TEST); // Включаем глубинный тест
    vbo_.create();
    vbo_.bind();
    if (model_) {
        vbo_.allocate(model_->vertices.data(),
                      model_->vertices.size() * sizeof(QVector3D));
    }
    vbo_.release();
}


void Scene::paintGL() {
    glClearColor(0.1f, 0.2f, 0.4f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glScalef(scale_factor_, scale_factor_, scale_factor_);

    glTranslatef(model_position.x(), model_position.y(), 0.0f);

    if (model_) {
        glColor3f(0.0f, 1.0f, 1.0f);  // Зеленый цвет
        glBegin(GL_LINES);
        for (const auto& face : model_->faces) {
            // Рёбра треугольника: 0-1, 1-2, 2-0
            for (int i = 0; i < 3; ++i) {
                int j = (i + 1) % 3;  // Следующая вершина (замыкаем треугольник)
                glVertex3f(face.position[i]->x, face.position[i]->y, face.position[i]->z);
                glVertex3f(face.position[j]->x, face.position[j]->y, face.position[j]->z);
            }
        }
        glEnd();
    }
}

void Scene::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        is_dragging = true;
        last_mouse_pos = event->pos();
    }
}

void Scene::mouseMoveEvent(QMouseEvent* event) {
    if (is_dragging) {
        // Вычисляем смещение мыши
        QPoint delta = event->pos() - last_mouse_pos;
        last_mouse_pos = event->pos();

       // -delta.y() потому что ось Y направлена вниз а в OpenGL - вверх
        model_position += scale_factor_ * (QVector2D(delta.x(), -delta.y()));
        update();
    }
}

void Scene::mouseReleaseEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        is_dragging = false;
    }
}


}
}  // namespace s21

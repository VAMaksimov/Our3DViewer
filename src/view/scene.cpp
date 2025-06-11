
#include "view/scene.h"

namespace s21 {
Scene::Scene(QWidget* parent) : QOpenGLWidget(parent), vbo_(QOpenGLBuffer::VertexBuffer) {}

Scene::~Scene() {
    vbo_.destroy();
}

// void Scene::initializeGL() {
//     glEnable(GL_DEPTH_TEST); // Включаем глубинный тест
//     vbo_.create();
//     vbo_.bind();
//     if (model_) {
//         vbo_.allocate(model_->vertices.data(),
//                       model_->vertices.size() * sizeof(QVector3D));
//     }
//     vbo_.release();
// }

void Scene::initializeGL() {
    initializeOpenGLFunctions();
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.2f, 0.2f, 0.3f, 1.0f);

    // === Компиляция шейдеров ===
    shaderProgram_.addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSource);
    shaderProgram_.addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSource);
    if (!shaderProgram_.link()) {
        qDebug() << "Failed to link shader program!";
        return;
    }

    // === Настройка освещения и материала ===
    shaderProgram_.bind();
    shaderProgram_.setUniformValue("light.position", QVector3D(1.0f, 1.0f, 1.0f));
    shaderProgram_.setUniformValue("light.ambient", QVector3D(0.2f, 0.2f, 0.2f));
    shaderProgram_.setUniformValue("light.diffuse", QVector3D(0.8f, 0.8f, 0.8f));
    shaderProgram_.setUniformValue("light.specular", QVector3D(1.0f, 1.0f, 1.0f));

    shaderProgram_.setUniformValue("material.ambient", QVector3D(0.1f, 0.1f, 0.5f));
    shaderProgram_.setUniformValue("material.diffuse", QVector3D(0.1f, 0.1f, 0.5f));
    shaderProgram_.setUniformValue("material.specular", QVector3D(1.0f, 1.0f, 1.0f));
    shaderProgram_.setUniformValue("material.shininess", 64.0f);
    shaderProgram_.release();

    if (model_) {
        std::vector<WireframeObject::Coordinate> perVertexNormals;

        // === Формируем плоский список нормалей по вершинам ===
        for (const auto& face : model_->faces) {
            for (int i = 0; i < 3; ++i) {
                perVertexNormals.push_back(face.normal[i]); // одна нормаль на вершину треугольника
            }
        }

        // === Проверяем, что количество совпадает ===
        if (perVertexNormals.size() != model_->vertices.size()) {
            qDebug() << "Mismatch in normals and vertices count!";
        }

        // === Создание VAO и VBO ===
        vao_.create();
        vao_.bind();

        // --- VBO для вершин ---
        vboVertices_.create();
        vboVertices_.bind();
        vboVertices_.allocate(model_->vertices.data(),
                              static_cast<int>(model_->vertices.size() * sizeof(WireframeObject::Coordinate)));
        GLint posAttr = shaderProgram_.attributeLocation("aPos");
        glEnableVertexAttribArray(posAttr);
        glVertexAttribPointer(posAttr, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
        vboVertices_.release();

        // --- VBO для нормалей ---
        vboNormals_.create();
        vboNormals_.bind();
        vboNormals_.allocate(perVertexNormals.data(),
                             static_cast<int>(perVertexNormals.size() * sizeof(WireframeObject::Coordinate)));
        GLint normalAttr = shaderProgram_.attributeLocation("aNormal");
        glEnableVertexAttribArray(normalAttr);
        glVertexAttribPointer(normalAttr, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
        vboNormals_.release();

        vao_.release();

        vertexCount_ = static_cast<int>(perVertexNormals.size());
    }
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

// void Scene::paintGL() {
//     glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//     glLoadIdentity();

//     // Применяем масштабирование
//     glScalef(scale_factor_, scale_factor_, scale_factor_);

//     if (model_) {
//         // Отрисовка треугольников с нормалями
//         glBegin(GL_TRIANGLES);
//         for (const auto& face : model_->faces) {
//             for (int i = 0; i < 3; ++i) {
//                 glNormal3f(face.normal.x, face.normal.y, face.normal.z);
//                 glVertex3f(face.position[i].x, face.position[i].y, face.position[i].z);
//             }
//         }
//         glEnd();
//     }
// }

void Scene::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    QMatrix4x4 modelMatrix;
    modelMatrix.scale(scale_factor_);

    QMatrix4x4 viewMatrix;
    viewMatrix.translate(0.0f, 0.0f, -3.0f); // имитация камеры

    QMatrix4x4 projMatrix;
    projMatrix.perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);

    shaderProgram_.bind();
    shaderProgram_.setUniformValue("projection", projMatrix);
    shaderProgram_.setUniformValue("view", viewMatrix);
    shaderProgram_.setUniformValue("model", modelMatrix);

    if (model_ && vertexCount_ > 0) {
        vao_.bind();
        glDrawArrays(GL_TRIANGLES, 0, vertexCount_);
        vao_.release();
    }

    shaderProgram_.release();
}


}

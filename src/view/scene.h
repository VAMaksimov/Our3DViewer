#ifndef VIEW_SCENE_H_
#define VIEW_SCENE_H_

#include <QOpenGLFunctions>
#include <QOpenGLWidget>
#include <QOpenGLBuffer>
#include <QVector3D>
#include <QWheelEvent>
#include "model/parser.h" // WireframeObject

#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QMatrix4x4>



namespace s21 {

class Scene : public QOpenGLWidget, protected QOpenGLFunctions {
    Q_OBJECT

public:
    // using Vertex = QOpenGLShader::Vertex;

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

    const char* vertexShaderSource = R"(
#version 120

attribute vec3 aPos;
attribute vec3 aNormal;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

varying vec3 fragNormal;
varying vec3 fragPos;

// Функция для приближенного расчёта обратной транспонированной матрицы
mat3 transposeInverse(mat4 m) {
    // Для простоты предположим, что матрица поворота без масштабирования/сдвигов
    return mat3(model);
}

void main() {
    fragPos = vec3(model * vec4(aPos, 1.0));
    fragNormal = normalize(mat3(model) * aNormal); // Упрощённая нормаль

    gl_Position = projection * view * model * vec4(aPos, 1.0);
}
)";

   const char* fragmentShaderSource = R"(
#version 120

varying vec3 fragNormal;
varying vec3 fragPos;

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

uniform Light light;
uniform Material material;

void main() {
    vec3 norm = normalize(fragNormal);
    vec3 lightDir = normalize(light.position - fragPos);

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * material.diffuse;

    vec3 viewDir = normalize(-fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * material.specular;

    vec3 result = light.ambient * material.ambient + diffuse + specular;
    gl_FragColor = vec4(result, 1.0);
}
)";



protected:
    void initializeGL() override;
    void paintGL() override;

protected:
    QOpenGLBuffer vbo_;
    std::shared_ptr<WireframeObject> model_ = nullptr;

private:
    QOpenGLShaderProgram shaderProgram_;
    QOpenGLVertexArrayObject vao_;
    QOpenGLBuffer vboVertices_, vboNormals_;

    int vertexCount_;
    float scale_factor_ = 1.0f;  // Начальный масштаб
};

} // namespace s21

#endif // VIEW_SCENE_H_

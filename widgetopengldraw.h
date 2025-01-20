#ifndef WIDGETOPENGLDRAW_H
#define WIDGETOPENGLDRAW_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>
#include <glm/glm.hpp>
#include <vector>

struct Vertex {
    glm::vec3 position;
    glm::vec2 texCoord;
    glm::vec3 normal;
};

struct Mesh {
    GLuint VAO;
    GLuint VBO;
    GLuint IBO;
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    glm::mat4 modelMatrix;
    QImage t;
};



class WidgetOpenGLDraw : public QOpenGLWidget {
    Q_OBJECT

public:
    WidgetOpenGLDraw(QWidget *parent = nullptr);
    ~WidgetOpenGLDraw();
    void loadModel();
    bool useOrthogonal = true;

    void addMesh(std::string filePath);
    std::vector<Mesh> meshes;
    void updateTexture();
    void addTexture(std::string filePath);
protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int w, int h) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

private:
    GLuint id_VAO_trikotnik, id_buffer_trikotnik, id_buffer_index;
    GLuint id_sencilni_program;
    QOpenGLFunctions_3_3_Core *gl = nullptr;

    glm::mat4 P, V, M = glm::mat4(1.0f);

    unsigned int currentMesh = 0;

    bool rightMousePressed = false;
    QPoint lastMousePos;

    glm::vec3 lightPos;
    glm::vec3 lightColor;
//    glm::vec3 objectColor;
    float ambientStrength;
    float specularStrength;
    float shininess;



    void PrevediSencilnike();
    void printProgramInfoLog(GLuint obj);
    void printShaderInfoLog(GLuint obj);
    bool loadOBJ(const std::string& filePath, std::vector<Vertex>& vertices, std::vector<unsigned int>& indices);
    void generateMesh(Mesh &mesh, std::vector<Vertex> &vertices, std::vector<unsigned int> &indices);
};

#endif // WIDGETOPENGLDRAW_H

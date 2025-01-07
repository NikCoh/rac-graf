#include "widgetopengldraw.h"
#include <iostream>
#include <glm/ext.hpp>
#include <QMouseEvent>
#include <QFileDialog>
#include <QApplication>
#include <fstream>
#include <sstream>
#include <QOpenGLVersionFunctionsFactory>


WidgetOpenGLDraw::WidgetOpenGLDraw(QWidget *parent) : QOpenGLWidget(parent) {
    setFocusPolicy(Qt::StrongFocus);
}

WidgetOpenGLDraw::~WidgetOpenGLDraw() {
    gl->glDeleteVertexArrays(1, &id_VAO_trikotnik);
    gl->glDeleteProgram(id_sencilni_program);
}

void WidgetOpenGLDraw::printProgramInfoLog(GLuint obj) {
    int infologLength = 0;
    gl->glGetProgramiv(obj, GL_INFO_LOG_LENGTH, &infologLength);
    if (infologLength > 0) {
        std::unique_ptr<char[]> infoLog(new char[infologLength]);
        int charsWritten = 0;
        gl->glGetProgramInfoLog(obj, infologLength, &charsWritten, infoLog.get());
        std::cerr << infoLog.get() << std::endl;
    }
}

void WidgetOpenGLDraw::printShaderInfoLog(GLuint obj) {
    int infologLength = 0;
    gl->glGetShaderiv(obj, GL_INFO_LOG_LENGTH, &infologLength);
    if (infologLength > 0) {
        std::unique_ptr<char[]> infoLog(new char[infologLength]);
        int charsWritten = 0;
        gl->glGetShaderInfoLog(obj, infologLength, &charsWritten, infoLog.get());
        std::cerr << infoLog.get() << std::endl;
    }
}

bool WidgetOpenGLDraw::loadOBJ(const std::string& filePath, std::vector<Vertex>& vertices, std::vector<unsigned int>& indices) {

    std::vector<glm::vec3> positions;
    std::vector<glm::vec2> texCoords;
    std::vector<glm::vec3> normals;

    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Failed to open OBJ file: " << filePath << std::endl;
        return false;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream stream(line);
        std::string prefix;
        stream >> prefix;

        if (prefix == "v") {
            glm::vec3 position;
            stream >> position.x >> position.y >> position.z;
            positions.push_back(position);
        }
        else if (prefix == "vt") {
            glm::vec2 texCoord;
            stream >> texCoord.x >> texCoord.y;
            texCoords.push_back(texCoord);
        }
        else if (prefix == "vn") {
            glm::vec3 normal;
            stream >> normal.x >> normal.y >> normal.z;
            normals.push_back(normal);
        }
        else if (prefix == "f") {
            unsigned int v1, t1, n1, v2, t2, n2, v3, t3, n3;
            char slash;

            stream >> v1 >> slash >> t1 >> slash >> n1;
            stream >> v2 >> slash >> t2 >> slash >> n2;
            stream >> v3 >> slash >> t3 >> slash >> n3;

            v1 -= 1; v2 -= 1; v3 -= 1;
            t1 -= 1; t2 -= 1; t3 -= 1;
            n1 -= 1; n2 -= 1; n3 -= 1;

            Vertex vertex1 = { positions[v1], texCoords[t1], normals[n1] };
            Vertex vertex2 = { positions[v2], texCoords[t2], normals[n2] };
            Vertex vertex3 = { positions[v3], texCoords[t3], normals[n3] };

            unsigned int currentIndex = vertices.size();
            vertices.push_back(vertex1);
            vertices.push_back(vertex2);
            vertices.push_back(vertex3);

            indices.push_back(currentIndex);
            indices.push_back(currentIndex + 1);
            indices.push_back(currentIndex + 2);
        }

    }

    return true;
}

void WidgetOpenGLDraw::PrevediSencilnike() {
    id_sencilni_program = gl->glCreateProgram();

    GLuint vs = gl->glCreateShader(GL_VERTEX_SHADER);
    std::string vss = R"(
    #version 330 core

    layout(location = 0) in vec3 in_Pos;
    layout(location = 1) in vec2 in_TexCoord;
    layout(location = 2) in vec3 in_Normal;

    out vec2 TexCoord;
    out vec3 Normal;
    out vec3 FragPos;

    uniform mat4 PVM;
    uniform mat4 Model;

    void main() {
        gl_Position = PVM * vec4(in_Pos, 1.0);

        TexCoord = in_TexCoord;

        Normal = mat3(transpose(inverse(Model))) * in_Normal;

        FragPos = vec3(Model * vec4(in_Pos, 1.0));
    }
)";

    const char *vssc = vss.c_str();
    gl->glShaderSource(vs, 1, &vssc, nullptr);
    gl->glCompileShader(vs);
    printShaderInfoLog(vs);
    gl->glAttachShader(id_sencilni_program, vs);

    GLuint fs = gl->glCreateShader(GL_FRAGMENT_SHADER);
    std::string fss = R"(
    #version 330 core

    in vec2 TexCoord;
    in vec3 Normal;
    in vec3 FragPos;

    out vec4 out_Color;

    uniform vec3 lightPos;
    uniform vec3 viewPos;
    uniform vec3 lightColor;
    uniform vec3 objectColor;
    uniform float ambientStrength;
    uniform float specularStrength;
    uniform float shininess;

    void main() {
        vec3 norm = normalize(Normal);
        vec3 lightDir = normalize(lightPos - FragPos);
        vec3 viewDir = normalize(viewPos - FragPos);

        vec3 ambient = ambientStrength * lightColor;

        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = diff * lightColor;

        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
        vec3 specular = specularStrength * spec * lightColor;

        vec3 result = (ambient + diffuse + specular) * objectColor;

        out_Color = vec4(result, 1.0);
    }
)";


    const char *fssc = fss.c_str();
    gl->glShaderSource(fs, 1, &fssc, nullptr);
    gl->glCompileShader(fs);
    printShaderInfoLog(fs);
    gl->glAttachShader(id_sencilni_program, fs);

    gl->glLinkProgram(id_sencilni_program);
    printProgramInfoLog(id_sencilni_program);
}

void WidgetOpenGLDraw::initializeGL() {
#if QT_VERSION >= 0x060000
    gl = QOpenGLVersionFunctionsFactory::get<QOpenGLFunctions_3_3_Core>(context());
#else
    gl = context()->versionFunctions<QOpenGLFunctions_3_3_Core>();
#endif

    if (!gl) {
        std::cerr << "Could not obtain required OpenGL context version";
        QApplication::exit(1);
    }

    PrevediSencilnike();

    lightPos = glm::vec3(0.0f, 10.0f, 0.0f);
    lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
    objectColor = glm::vec3(1.0f, 1.0f, 1.0f);

    ambientStrength = 0.1f;
    specularStrength = 0.1f;
    shininess = 32.0f;

    P = glm::perspective(glm::radians(45.0f), 16.0f / 9.0f, 0.1f, 100.0f);
    V = glm::lookAt(glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 vMatrix = glm::mat4(1.0f);

    vMatrix[0][0] = 0.999846f;  vMatrix[0][1] = -0.00171678f; vMatrix[0][2] = -0.0174986f;  vMatrix[0][3] = 0.0f;
    vMatrix[1][0] = 0.0022038f; vMatrix[1][1] = 0.99961f;    vMatrix[1][2] = 0.0278509f;   vMatrix[1][3] = 0.0f;
    vMatrix[2][0] = 0.0174439f; vMatrix[2][1] = -0.0278851f; vMatrix[2][2] = 0.999459f;    vMatrix[2][3] = 0.0f;
    vMatrix[3][0] = -0.5f;      vMatrix[3][1] = 0.0f;        vMatrix[3][2] = -5.5f;       vMatrix[3][3] = 1.0f;

    V = vMatrix;

    addMesh("C:/Users/cohni/Downloads/kolata haubata.obj");
    glm::mat4 matrix = glm::mat4(1.0f);
    matrix[0][0] = 0.0868244f;  matrix[0][1] = -0.0871558f; matrix[0][2] = 0.992405f; matrix[0][3] = 0.0f;
    matrix[1][0] = 0.00759615f; matrix[1][1] = 0.996195f;   matrix[1][2] = 0.0868242f; matrix[1][3] = 0.0f;
    matrix[2][0] = -0.996195f;  matrix[2][1] = 0.0f;        matrix[2][2] = 0.087156f;  matrix[2][3] = 0.0f;
    matrix[3][0] = 0.181742f;   matrix[3][1] = -1.61743f;   matrix[3][2] = 4.18257f;   matrix[3][3] = 1.0f;
    meshes[meshes.size()-1].modelMatrix = matrix;
    addMesh("C:/Users/cohni/Downloads/clovek.obj");
    meshes[meshes.size()-1].modelMatrix = matrix;
}

void WidgetOpenGLDraw::addMesh(std::string filePath){
    makeCurrent();
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    if (loadOBJ(filePath, vertices, indices)) {
        Mesh mesh;
        mesh.modelMatrix = glm::mat4(1.0f);
        generateMesh(mesh, vertices, indices);
        meshes.push_back(mesh);
    }
    doneCurrent();
}

void WidgetOpenGLDraw::paintGL() {
    gl->glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    gl->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    gl->glEnable(GL_DEPTH_TEST);

    gl->glUseProgram(id_sencilni_program);

    glm::mat4 MVP = P * V;

    gl->glUniform3fv(gl->glGetUniformLocation(id_sencilni_program, "lightPos"), 1, glm::value_ptr(lightPos));
    gl->glUniform3fv(gl->glGetUniformLocation(id_sencilni_program, "viewPos"), 1, glm::value_ptr(V));
    gl->glUniform3fv(gl->glGetUniformLocation(id_sencilni_program, "lightColor"), 1, glm::value_ptr(lightColor));
    gl->glUniform3fv(gl->glGetUniformLocation(id_sencilni_program, "objectColor"), 1, glm::value_ptr(objectColor));

    gl->glUniform1f(gl->glGetUniformLocation(id_sencilni_program, "ambientStrength"), ambientStrength);
    gl->glUniform1f(gl->glGetUniformLocation(id_sencilni_program, "specularStrength"), specularStrength);
    gl->glUniform1f(gl->glGetUniformLocation(id_sencilni_program, "shininess"), shininess);


    for (const auto& mesh : meshes) {
        gl->glBindVertexArray(mesh.VAO);

        glm::mat4 modelView = MVP * mesh.modelMatrix;
        gl->glUniformMatrix4fv(gl->glGetUniformLocation(id_sencilni_program, "PVM"), 1, GL_FALSE, glm::value_ptr(modelView));

        gl->glUniformMatrix4fv(gl->glGetUniformLocation(id_sencilni_program, "Model"), 1, GL_FALSE, glm::value_ptr(mesh.modelMatrix));

        gl->glDrawElements(GL_TRIANGLES, mesh.indices.size(), GL_UNSIGNED_INT, nullptr);

        gl->glBindVertexArray(0);
    }

    gl->glUseProgram(0);
}

void WidgetOpenGLDraw::resizeGL(int w, int h) {
    gl->glViewport(0, 0, w, h);
}

void WidgetOpenGLDraw::generateMesh(Mesh &mesh, std::vector<Vertex> &vertices, std::vector<unsigned int> &indices) {
    mesh.vertices = vertices;
    mesh.indices = indices;
    gl->glGenVertexArrays(1, &mesh.VAO);
    gl->glBindVertexArray(mesh.VAO);

    gl->glGenBuffers(1, &mesh.VBO);
    gl->glBindBuffer(GL_ARRAY_BUFFER, mesh.VBO);
    gl->glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

    gl->glGenBuffers(1, &mesh.IBO);
    gl->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.IBO);
    gl->glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    gl->glEnableVertexAttribArray(0);
    gl->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));

    gl->glEnableVertexAttribArray(1);
    gl->glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoord));

    gl->glEnableVertexAttribArray(2);
    gl->glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

    gl->glBindVertexArray(0);
}



void WidgetOpenGLDraw::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::RightButton) {
        rightMousePressed = true;
        lastMousePos = event->pos();
    }
    if (event->button() == Qt::LeftButton){
        if (meshes.size() > 0){
            currentMesh = (currentMesh + 1) % meshes.size();
            std::cout << "Currently editing: " << currentMesh << std::endl;
        }
    }
}

void WidgetOpenGLDraw::mouseMoveEvent(QMouseEvent *event) {
    if (rightMousePressed) {
        QPoint delta = event->pos() - lastMousePos;
        lastMousePos = event->pos();

        V = glm::rotate(V, glm::radians(delta.y() * 0.2f), glm::vec3(1, 0, 0));
        V = glm::rotate(V, glm::radians(delta.x() * 0.2f), glm::vec3(0, 1, 0));
        update();
    }
}

void WidgetOpenGLDraw::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == Qt::RightButton) {
        rightMousePressed = false;
    }
}



void WidgetOpenGLDraw::keyPressEvent(QKeyEvent *event) {
    const float cameraSpeed = 0.5f;
    const float translationSpeed = 0.2f;
    const float rotationSpeed = 5.0f;
    const float lightTranslationSpeed = 1.0f;
    const float colorSpeed = 0.05f;
    const float strengthSpeed = 0.1f;
    const float shinySpeed = 1.0f;

    glm::vec3 cameraPosition = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 translation = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f);

    if (event->key() == Qt::Key_Up) {
        cameraPosition.z -= cameraSpeed;
    } else if (event->key() == Qt::Key_Down) {
        cameraPosition.z += cameraSpeed;
    } else if (event->key() == Qt::Key_Left) {
        cameraPosition.x -= cameraSpeed;
    } else if (event->key() == Qt::Key_Right) {
        cameraPosition.x += cameraSpeed;
    } else if (event->key() == Qt::Key_Space) {
        cameraPosition.y += cameraSpeed;
    } else if (event->key() == Qt::Key_Shift) {
        cameraPosition.y -= cameraSpeed;
    } else if (event->key() == Qt::Key_P) {
        if (useOrthogonal) {
            float left = -10.0f, right = 10.0f;
            float bottom = -10.0f, top = 10.0f;
            float near = 0.1f, far = 100.0f;
            P = glm::ortho(left, right, bottom, top, near, far);
        } else {
            float fov = glm::radians(45.0f);
            float aspectRatio = 16.0f / 9.0f;
            float near = 0.1f, far = 100.0f;
            P = glm::perspective(fov, aspectRatio, near, far);
        }
        useOrthogonal = !useOrthogonal;
    } else if (event->key() == Qt::Key_Q) {
        translation.x -= translationSpeed;
    } else if (event->key() == Qt::Key_A) {
        translation.x += translationSpeed;
    } else if (event->key() == Qt::Key_W) {
        translation.y -= translationSpeed;
    } else if (event->key() == Qt::Key_S) {
        translation.y += translationSpeed;
    } else if (event->key() == Qt::Key_E) {
        translation.z -= translationSpeed;
    } else if (event->key() == Qt::Key_D) {
        translation.z += translationSpeed;
    } else if (event->key() == Qt::Key_R) {
        rotation.x -= rotationSpeed;
    } else if (event->key() == Qt::Key_F) {
        rotation.x += rotationSpeed;
    } else if (event->key() == Qt::Key_T) {
        rotation.y -= rotationSpeed;
    } else if (event->key() == Qt::Key_G) {
        rotation.y += rotationSpeed;
    } else if (event->key() == Qt::Key_Z) {
        rotation.z -= rotationSpeed;
    } else if (event->key() == Qt::Key_H) {
        rotation.z += rotationSpeed;
    } else if (event->key() == Qt::Key_U) {
        lightPos.x -= lightTranslationSpeed;
    } else if (event->key() == Qt::Key_J) {
        lightPos.x += lightTranslationSpeed;
    } else if (event->key() == Qt::Key_I) {
        lightPos.y -= lightTranslationSpeed;
    } else if (event->key() == Qt::Key_K) {
        lightPos.y += lightTranslationSpeed;
    } else if (event->key() == Qt::Key_O) {
        lightPos.z -= lightTranslationSpeed;
    } else if (event->key() == Qt::Key_L) {
        lightPos.z += lightTranslationSpeed;
    } else if (event->key() == Qt::Key_4) {
        lightColor.r = glm::max(0.0f, lightColor.r - colorSpeed);
    } else if (event->key() == Qt::Key_1) {
        lightColor.r = glm::min(1.0f, lightColor.r + colorSpeed);
    } else if (event->key() == Qt::Key_5) {
        lightColor.g = glm::max(0.0f, lightColor.g - colorSpeed);
    } else if (event->key() == Qt::Key_2) {
        lightColor.g = glm::min(1.0f, lightColor.g + colorSpeed);
    } else if (event->key() == Qt::Key_6) {
        lightColor.b = glm::max(0.0f, lightColor.b - colorSpeed);
    } else if (event->key() == Qt::Key_3) {
        lightColor.b = glm::min(1.0f, lightColor.b + colorSpeed);
    } else if (event->key() == Qt::Key_Slash) {
        objectColor.r = glm::max(0.0f, objectColor.r - colorSpeed);
    } else if (event->key() == Qt::Key_8) {
        objectColor.r = glm::min(1.0f, objectColor.r + colorSpeed);
    } else if (event->key() == Qt::Key_Asterisk) {
        objectColor.g = glm::max(0.0f, objectColor.g - colorSpeed);
    } else if (event->key() == Qt::Key_9) {
        objectColor.g = glm::min(1.0f, objectColor.g + colorSpeed);
    } else if (event->key() == Qt::Key_Minus) {
        objectColor.b = glm::max(0.0f, objectColor.b - colorSpeed);
    } else if (event->key() == Qt::Key_Plus) {
        objectColor.b = glm::min(1.0f, objectColor.b + colorSpeed);
    } else if (event->key() == Qt::Key_Y) {
        ambientStrength -= strengthSpeed;
    } else if (event->key() == Qt::Key_X) {
        ambientStrength += strengthSpeed;
    } else if (event->key() == Qt::Key_C) {
        specularStrength -= strengthSpeed;
    } else if (event->key() == Qt::Key_V) {
        specularStrength += strengthSpeed;
    } else if (event->key() == Qt::Key_B) {
        shininess -= shinySpeed;
    } else if (event->key() == Qt::Key_N) {
        shininess += shinySpeed;
    }

    if (meshes.size() > 0) {
        M = meshes[currentMesh].modelMatrix;

        M = glm::translate(M, translation);

        M = glm::rotate(M, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
        M = glm::rotate(M, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
        M = glm::rotate(M, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
        meshes[currentMesh].modelMatrix = M;

    }

    V = glm::translate(V, cameraPosition);
    update();
}


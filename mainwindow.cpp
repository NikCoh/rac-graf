#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <map>
#include <QFileDialog>
#include <iostream>
#include <QProcess>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    std::string filePath = QFileDialog::getOpenFileName(this, "Select a file", "", "All Files (*.*);;Text Files (*.txt)").toStdString();
    ui->widget->addMesh(filePath);
    ui->widget->update();

}

void MainWindow::on_pushButton_2_clicked()
{
    QString command = ui->lineEdit->text();
    std::cout << command.toStdString() << std::endl;
    QProcess process;
    QString pythonInterpreter = "python";
    QString scriptPath = "../../main.py";

    process.start(pythonInterpreter, QStringList() << scriptPath << command);

    if (!process.waitForFinished()) {
        qDebug() << "Failed to run the script:" << process.errorString();
    }

    std::cout << "Output:" << process.readAllStandardOutput().toStdString();

    //ukazi - oblaki
    std::map<QString, std::pair<QString, QString>> cloudModelMapping = {
                                                                        {"Voice Command Radio On", {"../../objects/oblak-radio_", "../../textures/green.png"}},
                                                                        {"Voice Command Radio Off", {"../../objects/oblak-radio_", "../../textures/red.png"}},
                                                                        {"Voice Command Air Conditioning On", {"../../objects/oblak-AC_", "../../textures/green.png"}},
                                                                        {"Voice Command Air Conditioning Off", {"../../objects/oblak-AC_", "../../textures/red.png"}},
                                                                        {"Voice Command Volume Up", {"../../objects/oblak-vol_", "../../textures/orange.png"}},
                                                                        {"Voice Command Volume Down", {"../../objects/oblak-vol_", "../../textures/blue.png"}},
                                                                        {"Voice Command Temperature Up", {"../../objects/oblak-temp_", "../../textures/orange.png"}},
                                                                        {"Voice Command Temperature Down", {"../../objects/oblak-temp_", "../../textures/blue.png"}},
                                                                        };

    if (cloudModelMapping.find(command) != cloudModelMapping.end()) {
        std::string modelPath = cloudModelMapping[command].first.toStdString();
        std::string texturePath = cloudModelMapping[command].second.toStdString();
        glm::mat4 matrix = glm::mat4(1.0f);
        matrix[0][0] = 1.0f;  matrix[0][1] = 0.0f; matrix[0][2] = 0.0f; matrix[0][3] = 0.0f;
        matrix[1][0] = 0.0f;  matrix[1][1] = 1.0f; matrix[1][2] = 0.0f; matrix[1][3] = 0.0f;
        matrix[2][0] = 0.0f;  matrix[2][1] = 0.0f; matrix[2][2] = 1.0f; matrix[2][3] = 0.0f;
        matrix[3][0] = -0.2f; matrix[3][1] = 0.0f; matrix[3][2] = 4.2f; matrix[3][3] = 1.0f;

        ui->widget->addMesh(modelPath + "1.obj");
        ui->widget->addTexture(texturePath);
        ui->widget->meshes[ui->widget->meshes.size() - 1].modelMatrix = matrix;

        ui->widget->update();

        QTimer::singleShot(500, this, [this, modelPath, texturePath, matrix]() {
            ui->widget->meshes.pop_back();
            ui->widget->addMesh(modelPath + "2.obj");
            ui->widget->addTexture(texturePath);
            ui->widget->meshes[ui->widget->meshes.size() - 1].modelMatrix = matrix;
            ui->widget->update();
        });

        QTimer::singleShot(1000, this, [this, modelPath, texturePath, matrix]() {
            ui->widget->meshes.pop_back();
            ui->widget->addMesh(modelPath + "3.obj");
            ui->widget->addTexture(texturePath);
            ui->widget->meshes[ui->widget->meshes.size() - 1].modelMatrix = matrix;
            ui->widget->update();
        });
        QTimer::singleShot(5000, this, [this]() {
            ui->widget->meshes.pop_back();
            ui->widget->update();
        });
    }
}


void MainWindow::on_pushButton_3_clicked()
{
    std::string filePath = QFileDialog::getOpenFileName(this, "Select a file", "", "All Files (*.*);;Text Files (*.txt)").toStdString();
    // ui->widget->addTexture(filePath);
    ui->widget->update();
}

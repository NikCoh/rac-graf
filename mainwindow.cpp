#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <iostream>
#include <QProces>
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

    glm::mat4 matrix = glm::mat4(1.0f);
    matrix[0][0] = 1.0f;  matrix[0][1] = 0.0f; matrix[0][2] = 0.0f; matrix[0][3] = 0.0f;
    matrix[1][0] = 0.0f;  matrix[1][1] = 1.0f; matrix[1][2] = 0.0f; matrix[1][3] = 0.0f;
    matrix[2][0] = 0.0f;  matrix[2][1] = 0.0f; matrix[2][2] = 1.0f; matrix[2][3] = 0.0f;
    matrix[3][0] = -0.2f; matrix[3][1] = 0.0f; matrix[3][2] = 4.2f; matrix[3][3] = 1.0f;

    ui->widget->addMesh("../../objects/oblak-AC_off.obj");

    ui->widget->meshes[ui->widget->meshes.size()-1].modelMatrix = matrix;

    ui->widget->update();

    QTimer::singleShot(10000, this, [this]() {

        ui->widget->meshes.pop_back();
        ui->widget->update();

    });
}


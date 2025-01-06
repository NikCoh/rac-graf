#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <iostream>
#include <QProces>

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

    qDebug() << "Output:" << process.readAllStandardOutput().toStdString();
}


#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <iostream>

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

}


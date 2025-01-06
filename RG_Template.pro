QT += core gui widgets opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
greaterThan(QT_MAJOR_VERSION, 5): QT += openglwidgets

TARGET = RG_Template
TEMPLATE = app

SOURCES += main.cpp mainwindow.cpp widgetopengldraw.cpp

HEADERS += mainwindow.h widgetopengldraw.h

FORMS += mainwindow.ui

CONFIG += c++17

INCLUDEPATH += C:\Users\cohni\Downloads\glm-1.0.1-light

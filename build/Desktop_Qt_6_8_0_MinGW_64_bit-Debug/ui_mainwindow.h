/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "widgetopengldraw.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionTest;
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout;
    WidgetOpenGLDraw *widget;
    QPushButton *pushButton;
    QHBoxLayout *horizontalLayout;
    QLineEdit *lineEdit;
    QPushButton *pushButton_2;
    QMenuBar *menuBar;
    QMenu *menuTest;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(937, 590);
        actionTest = new QAction(MainWindow);
        actionTest->setObjectName("actionTest");
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName("centralWidget");
        verticalLayout = new QVBoxLayout(centralWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName("verticalLayout");
        widget = new WidgetOpenGLDraw(centralWidget);
        widget->setObjectName("widget");
        widget->setMouseTracking(true);

        verticalLayout->addWidget(widget);

        pushButton = new QPushButton(centralWidget);
        pushButton->setObjectName("pushButton");

        verticalLayout->addWidget(pushButton);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName("horizontalLayout");
        lineEdit = new QLineEdit(centralWidget);
        lineEdit->setObjectName("lineEdit");

        horizontalLayout->addWidget(lineEdit);

        pushButton_2 = new QPushButton(centralWidget);
        pushButton_2->setObjectName("pushButton_2");

        horizontalLayout->addWidget(pushButton_2);


        verticalLayout->addLayout(horizontalLayout);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName("menuBar");
        menuBar->setGeometry(QRect(0, 0, 937, 21));
        menuTest = new QMenu(menuBar);
        menuTest->setObjectName("menuTest");
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName("mainToolBar");
        MainWindow->addToolBar(Qt::ToolBarArea::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName("statusBar");
        MainWindow->setStatusBar(statusBar);

        menuBar->addAction(menuTest->menuAction());
        menuTest->addAction(actionTest);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        actionTest->setText(QCoreApplication::translate("MainWindow", "&Test", nullptr));
        pushButton->setText(QCoreApplication::translate("MainWindow", "Add Object", nullptr));
        pushButton_2->setText(QCoreApplication::translate("MainWindow", "Send", nullptr));
        menuTest->setTitle(QCoreApplication::translate("MainWindow", "Test", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H

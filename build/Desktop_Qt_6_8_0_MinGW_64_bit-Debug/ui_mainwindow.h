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
#include <QtWidgets/QLabel>
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
    QHBoxLayout *horizontalLayout_2;
    QPushButton *pushButton;
    QPushButton *pushButton_3;
    QHBoxLayout *horizontalLayout;
    QLineEdit *lineEdit;
    QPushButton *pushButton_2;
    QLabel *instructionsLabel;
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

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        pushButton = new QPushButton(centralWidget);
        pushButton->setObjectName("pushButton");

        horizontalLayout_2->addWidget(pushButton);

        pushButton_3 = new QPushButton(centralWidget);
        pushButton_3->setObjectName("pushButton_3");

        horizontalLayout_2->addWidget(pushButton_3);


        verticalLayout->addLayout(horizontalLayout_2);

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

        instructionsLabel = new QLabel(centralWidget);
        instructionsLabel->setObjectName("instructionsLabel");
        QSizePolicy sizePolicy(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(instructionsLabel->sizePolicy().hasHeightForWidth());
        instructionsLabel->setSizePolicy(sizePolicy);
        instructionsLabel->setMinimumSize(QSize(800, 50));
        instructionsLabel->setWordWrap(true);

        verticalLayout->addWidget(instructionsLabel);

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
        pushButton_3->setText(QCoreApplication::translate("MainWindow", "Add Texture", nullptr));
        pushButton_2->setText(QCoreApplication::translate("MainWindow", "Send", nullptr));
        instructionsLabel->setText(QCoreApplication::translate("MainWindow", "Instructions:\n"
"        * Camera:        - Translate:    \342\206\222 +x, \342\206\220 -x, Space +y, Shift -y, \342\206\223 +z, \342\206\221 -z          - Projection:   P toggle\n"
"        * Object         - Translate:    A +x, Q -x,S +y, W -y, D +z, E -z                   - Rotate:       F +x, R -x, G +y, T -y, H +z, Z -z        - Color:        8 +r, / +g, 9 +b, ' -r, + -g, - -b\n"
"        * Light          - Translate:    J +x, U -x, K +y, I -y, L +z, O -z                  - Color:        1 +r, 2 +g, 3 +b, 4 -r, 5 -g, 6 -b        - Light:        X +ambient, Y -ambient, V +specular, C -specular        - Shininess:    N +shininess, B -shininess\n"
"        ", nullptr));
        menuTest->setTitle(QCoreApplication::translate("MainWindow", "Test", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H

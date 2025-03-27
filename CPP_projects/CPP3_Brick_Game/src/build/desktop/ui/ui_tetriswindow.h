/********************************************************************************
** Form generated from reading UI file 'tetriswindow.ui'
**
** Created by: Qt User Interface Compiler version 6.4.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TETRISWINDOW_H
#define UI_TETRISWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_TetrisWindow
{
public:
    QWidget *centralwidget;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *TetrisWindow)
    {
        if (TetrisWindow->objectName().isEmpty())
            TetrisWindow->setObjectName("TetrisWindow");
        TetrisWindow->resize(800, 600);
        centralwidget = new QWidget(TetrisWindow);
        centralwidget->setObjectName("centralwidget");
        TetrisWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(TetrisWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 800, 19));
        TetrisWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(TetrisWindow);
        statusbar->setObjectName("statusbar");
        TetrisWindow->setStatusBar(statusbar);

        retranslateUi(TetrisWindow);

        QMetaObject::connectSlotsByName(TetrisWindow);
    } // setupUi

    void retranslateUi(QMainWindow *TetrisWindow)
    {
        TetrisWindow->setWindowTitle(QCoreApplication::translate("TetrisWindow", "MainWindow", nullptr));
    } // retranslateUi

};

namespace Ui {
    class TetrisWindow: public Ui_TetrisWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TETRISWINDOW_H

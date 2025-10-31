/********************************************************************************
** Form generated from reading UI file 'snakewindow.ui'
**
** Created by: Qt User Interface Compiler version 6.4.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SNAKEWINDOW_H
#define UI_SNAKEWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SnakeWindow
{
public:
    QWidget *centralwidget;

    void setupUi(QMainWindow *SnakeWindow)
    {
        if (SnakeWindow->objectName().isEmpty())
            SnakeWindow->setObjectName("SnakeWindow");
        SnakeWindow->resize(800, 600);
        centralwidget = new QWidget(SnakeWindow);
        centralwidget->setObjectName("centralwidget");
        SnakeWindow->setCentralWidget(centralwidget);

        retranslateUi(SnakeWindow);

        QMetaObject::connectSlotsByName(SnakeWindow);
    } // setupUi

    void retranslateUi(QMainWindow *SnakeWindow)
    {
        SnakeWindow->setWindowTitle(QCoreApplication::translate("SnakeWindow", "MainWindow", nullptr));
    } // retranslateUi

};

namespace Ui {
    class SnakeWindow: public Ui_SnakeWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SNAKEWINDOW_H

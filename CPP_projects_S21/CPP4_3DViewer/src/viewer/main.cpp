#include <QApplication>

#include "mainwindow.h"
using namespace s21;

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);

  s21::ObjModelController controller;

  MainWindow w(controller);
  w.show();
  return a.exec();
}

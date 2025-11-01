#include "tetriswindow.h"

#include "fieldwidget.h"
#include "ui_tetriswindow.h"

TetrisWindow::TetrisWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::TetrisWindow), main_window(parent) {
  ui->setupUi(this);
  field_widget = new FieldWidget(this);
  setCentralWidget(field_widget);
  adjustSize();
}

TetrisWindow::~TetrisWindow() {
  delete ui;
  // if (field_widget) delete field_widget;
}

void TetrisWindow::closeEvent(QCloseEvent *event) {
  int ret =
      QMessageBox::warning(this, tr("Exit to Menu"),
                           tr("Are you sure you want to exit to main menu?"),
                           QMessageBox::Yes | QMessageBox::No, QMessageBox::No);

  if (ret == QMessageBox::Yes) {
    // Показываем главное меню вместо выхода из приложения
    if (main_window) {
      main_window->show();
      main_window->raise();
      main_window->activateWindow();
    }
    event->accept();
  } else {
    event->ignore();
  }
}

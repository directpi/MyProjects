#include "snakewindow.h"

#include "fieldwidget.h"
#include "ui_snakewindow.h"

SnakeWindow::SnakeWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::SnakeWindow), main_window(parent) {
  ui->setupUi(this);
  field_widget = new FieldWidget(this);
  setCentralWidget(field_widget);
  adjustSize();
}

SnakeWindow::~SnakeWindow() {
  delete ui;
  if (field_widget) delete field_widget;
}

void SnakeWindow::closeEvent(QCloseEvent *event) {
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

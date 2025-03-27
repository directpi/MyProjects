#include "snakewindow.h"

#include "fieldwidget.h"
#include "ui_snakewindow.h"

SnakeWindow::SnakeWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::SnakeWindow) {
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
      QMessageBox::warning(this, tr("Terminate Game"),
                           tr("Are you sure you want to terminate the game?"),
                           QMessageBox::Yes | QMessageBox::No, QMessageBox::No);

  if (ret == QMessageBox::Yes) {
    // snake_game_over();
    QApplication::quit();
  } else {
    event->ignore();
  }
}

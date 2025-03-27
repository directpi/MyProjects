#include "tetriswindow.h"

#include "fieldwidget.h"
#include "ui_tetriswindow.h"

TetrisWindow::TetrisWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::TetrisWindow) {
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
      QMessageBox::warning(this, tr("Terminate Game"),
                           tr("Are you sure you want to terminate the game?"),
                           QMessageBox::Yes | QMessageBox::No, QMessageBox::No);

  if (ret == QMessageBox::Yes) {
    // game_over();
    QCoreApplication::quit();
  } else {
    event->ignore();
  }
}

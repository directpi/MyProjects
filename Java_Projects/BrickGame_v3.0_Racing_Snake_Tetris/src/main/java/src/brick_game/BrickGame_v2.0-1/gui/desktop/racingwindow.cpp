#include "racingwindow.h"

#include <QApplication>
#include <QMessageBox>

#include "fieldwidget.h"
#include "ui_racingwindow.h"

RacingWindow::RacingWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::RacingWindow), main_window(parent) {
  ui->setupUi(this);
  field_widget = new FieldWidget(this);
  setCentralWidget(field_widget);
  adjustSize();
}

RacingWindow::~RacingWindow() {
  delete ui;
  if (field_widget) delete field_widget;
}

void RacingWindow::closeEvent(QCloseEvent *event) {
  int ret =
      QMessageBox::warning(this, tr("Exit to Menu"),
                           tr("Are you sure you want to exit to main menu?"),
                           QMessageBox::Yes | QMessageBox::No, QMessageBox::No);

  if (ret == QMessageBox::Yes) {
    // Останавливаем игру перед закрытием
    if (field_widget) {
      // FieldWidget автоматически остановит REST клиент в деструкторе
    }
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

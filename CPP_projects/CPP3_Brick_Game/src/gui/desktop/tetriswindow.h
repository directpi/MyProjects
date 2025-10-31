#ifndef TETRISWINDOW_H
#define TETRISWINDOW_H

#include <QDialog>
#include <QMainWindow>

#include "fieldwidget.h"

namespace Ui {
class TetrisWindow;
}

class TetrisWindow : public QMainWindow {
  Q_OBJECT

 public:
  explicit TetrisWindow(QWidget *parent = nullptr);
  ~TetrisWindow();

 protected:
  void closeEvent(QCloseEvent *event) override;

 private:
  Ui::TetrisWindow *ui;
  FieldWidget *field_widget;
};

#endif  // TETRISWINDOW_H

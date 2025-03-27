#ifndef SNAKEWINDOW_H
#define SNAKEWINDOW_H
#include <QDialog>
#include <QMainWindow>

#include "fieldwidget.h"

namespace Ui {
class SnakeWindow;
}

class SnakeWindow : public QMainWindow {
  Q_OBJECT

 public:
  explicit SnakeWindow(QWidget *parent = nullptr);
  ~SnakeWindow();

 protected:
  void closeEvent(QCloseEvent *event) override;

 private:
  Ui::SnakeWindow *ui;
  FieldWidget *field_widget;
};

#endif  // SNAKEWINDOW_H

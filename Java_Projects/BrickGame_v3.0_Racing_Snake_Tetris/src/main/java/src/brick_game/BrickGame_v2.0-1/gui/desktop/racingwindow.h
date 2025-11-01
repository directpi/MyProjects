#ifndef RACINGWINDOW_H
#define RACINGWINDOW_H
#include <QDialog>
#include <QMainWindow>

#include "fieldwidget.h"

namespace Ui {
class RacingWindow;
}

class RacingWindow : public QMainWindow {
  Q_OBJECT

 public:
  explicit RacingWindow(QWidget *parent = nullptr);
  ~RacingWindow();

 protected:
  void closeEvent(QCloseEvent *event) override;

 private:
  Ui::RacingWindow *ui;
  FieldWidget *field_widget;
  QWidget *main_window;
};

#endif  // RACINGWINDOW_H

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "fieldwidget.h"
#include "racingwindow.h"
#include "snakewindow.h"
#include "tetriswindow.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

 protected:
  void closeEvent(QCloseEvent *event) override;

 private slots:
  void on_tetrisButton_clicked();

  void on_snakeButton_clicked();

  void on_racingButton_clicked();

  void on_exitButton_clicked();

 public slots:
  void showMainMenu();

 private:
  Ui::MainWindow *ui;
  TetrisWindow *tetris_window;
  SnakeWindow *snake_window;
  RacingWindow *racing_window;
};
#endif  // MAINWINDOW_H

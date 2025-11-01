#include "mainwindow.h"

#include "QMessageBox"
#include "ui_mainwindow.h"

t_selector game_selector = Tetris;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow),
      tetris_window(nullptr),
      snake_window(nullptr),
      racing_window(nullptr) {
  ui->setupUi(this);
}

MainWindow::~MainWindow() {
  delete ui;
  if (tetris_window) delete tetris_window;
  if (snake_window) delete snake_window;
  if (racing_window) delete racing_window;
}

void MainWindow::on_tetrisButton_clicked() {
  if (tetris_window) {
    delete tetris_window;
    tetris_window = nullptr;
  }
  game_selector = Tetris;
  hide();
  tetris_window = new TetrisWindow(this);
  tetris_window->show();
  tetris_window->raise();
  tetris_window->activateWindow();
}

void MainWindow::on_snakeButton_clicked() {
  if (snake_window) {
    delete snake_window;
    snake_window = nullptr;
  }
  game_selector = Snake;
  hide();
  snake_window = new SnakeWindow(this);
  snake_window->show();
  snake_window->raise();
  snake_window->activateWindow();
}

void MainWindow::on_racingButton_clicked() {
  if (racing_window) {
    delete racing_window;
    racing_window = nullptr;
  }
  game_selector = Racing;
  hide();
  racing_window = new RacingWindow(this);
  racing_window->show();
  racing_window->raise();
  racing_window->activateWindow();
}

void MainWindow::showMainMenu() {
  show();
  raise();
  activateWindow();
}

void MainWindow::on_exitButton_clicked() { QApplication::quit(); }

void MainWindow::closeEvent(QCloseEvent *event) {
  int ret = QMessageBox::question(
      this, tr("Exit Application"),
      tr("Are you sure you want to exit the application?"),
      QMessageBox::Yes | QMessageBox::No, QMessageBox::No);

  if (ret == QMessageBox::Yes) {
    event->accept();
  } else {
    event->ignore();
  }
}

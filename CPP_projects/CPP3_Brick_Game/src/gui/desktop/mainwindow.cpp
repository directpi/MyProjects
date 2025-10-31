#include "mainwindow.h"

#include "QMessageBox"
#include "ui_mainwindow.h"

t_selector game_selector = Tetris;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow),
      tetris_window(nullptr),
      snake_window(nullptr) {
  ui->setupUi(this);
}

MainWindow::~MainWindow() {
  delete ui;
  if (tetris_window) delete tetris_window;
  if (snake_window) delete snake_window;
}

void MainWindow::on_tetrisButton_clicked() {
  close();
  if (tetris_window) {
    delete tetris_window;
    tetris_window = nullptr;
  }
  game_selector = Tetris;
  tetris_window = new TetrisWindow(this);
  tetris_window->show();
}

void MainWindow::on_snakeButton_clicked() {
  close();
  if (snake_window) {
    delete snake_window;
    snake_window = nullptr;
  }
  game_selector = Snake;
  snake_window = new SnakeWindow(this);
  snake_window->show();
}

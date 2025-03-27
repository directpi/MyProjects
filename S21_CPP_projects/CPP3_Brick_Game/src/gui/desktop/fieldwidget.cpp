#include "fieldwidget.h"

#include <cstring>
#include <filesystem>
#include <fstream>
#include <iostream>

UserAction_t user_action = Start;
GameInfo_t game_info = {nullptr, nullptr, 0, 0, 1, GAME_SPEED, 0};
long long prev_time = current_timestamp();

FieldWidget::FieldWidget(QWidget *parent) : QWidget{parent} {
  initializeGame();

  font.setPointSize(14);
  setFixedSize(600, 600);
  setFocusPolicy(Qt::StrongFocus);

  // timer to periodically update the matrix display
  timer = new QTimer(this);
  connect(timer, &QTimer::timeout, this, &FieldWidget::updateFieldDisplay);
  timer->start(30);  // ms
}

FieldWidget::~FieldWidget() {
  if (game_selector == Snake) {
    snake::snake_check_high_score(&game_info);
  }
  delete timer;
}

void FieldWidget::paintEvent(QPaintEvent *event) {
  Q_UNUSED(event);
  painter.begin(this);
  painter.setFont(font);

  handle_pause(&painter);
  handle_gameover(&painter);

  if (game_selector == Snake) {
    game_info = *snake::updateCurrentState();
    handle_win(&painter);
  } else if (game_selector == Tetris) {
    game_info = *updateCurrentState(user_action);

    long long current_time = current_timestamp();
    if (game_info.pause != (int)EXIT_STATE &&
        game_info.pause != (int)GAMEOVER_STATE &&
        game_info.pause != (int)PAUSE_STATE &&
        current_time > prev_time + game_info.speed) {
      game_info = *updateCurrentState(Down);
      prev_time = current_time;
    }
    user_action = Start;
  }

  for (int row = 0; row < 20 && game_info.field; ++row) {
    for (int col = 0; col < 10 && game_info.field; ++col) {
      if (game_info.field[row][col] == 1 &&
          game_info.pause != (int)GAMEOVER_STATE) {
        painter.setBrush(Qt::blue);
      } else if (game_info.field[row][col] == 2) {
        painter.setBrush(Qt::red);
      } else if (game_info.field[row][col] == 1 &&
                 game_info.pause == (int)GAMEOVER_STATE) {
        painter.setBrush(Qt::red);
      } else {
        painter.setBrush(Qt::white);
      }
      painter.drawRect(col * cellSize, row * cellSize, cellSize, cellSize);
    }
  }

  if (game_selector == Tetris && game_info.pause != (int)GAMEOVER_STATE) {
    for (int row = 0; row < 4 && game_info.next; row++) {
      for (int col = 0; col < 4 && game_info.next; col++) {
        if (game_info.next[row][col]) {
          painter.setBrush(Qt::blue);
        } else {
          painter.setBrush(Qt::white);
        }
        painter.drawRect(col * cellSize + 380, row * cellSize + 40, cellSize,
                         cellSize);
      }
    }
  }

  draw_information(&painter);
  painter.end();
}

void FieldWidget::draw_information(QPainter *painter) {
  painter->setPen(QPen(Qt::blue));
  if (game_selector == Tetris) painter->drawText(350, 25, "NEXT BLOCK");
  std::string score = "SCORE: " + std::to_string(game_info.score);
  painter->drawText(350, 200, score.data());
  std::string h_score = "MAX SCORE: " + std::to_string(game_info.high_score);
  if (game_info.high_score > game_info.score) {
    painter->drawText(350, 240, h_score.data());
  } else {
    painter->drawText(350, 240, score.data());
  }
  std::string level = "LEVEL: " + std::to_string(game_info.level);
  painter->drawText(350, 280, level.data());
  painter->drawText(350, 440, "MOVE: Arrows");
  painter->drawText(350, 480, "PAUSE: P");
  if (game_selector == Tetris) {
    painter->drawText(350, 520, "ROTATE: Space");
  } else {
    painter->drawText(350, 520, "SPEED: Space");
  }
  painter->drawText(350, 560, "EXIT: Esc");
  painter->setPen(QPen(Qt::black));
}

void FieldWidget::handle_pause(QPainter *painter) {
  if (game_info.pause == (int)PAUSE_STATE) {
    painter->setPen(QPen(Qt::red));
    painter->drawText(350, 320, "PAUSED");
    painter->drawText(350, 360, "Press P to continue");
    painter->setPen(QPen(Qt::black));
  }
}

void FieldWidget::handle_gameover(QPainter *painter) {
  if (game_info.pause == (int)GAMEOVER_STATE) {
    painter->setPen(QPen(Qt::red));
    painter->drawText(350, 320, "GAME OVER");
    painter->drawText(350, 360, "Press ENTER to try again");
    painter->drawText(350, 400, "Press ESCAPE to exit");
    painter->setPen(QPen(Qt::black));
  }
}

void FieldWidget::handle_win(QPainter *painter) {
  if (game_selector == Snake) {
    if (game_info.pause == (int)WIN_STATE) {
      painter->setPen(QPen(Qt::red));
      painter->drawText(350, 320, "WIN");
      painter->drawText(350, 360, "Press ENTER to try again");
      painter->drawText(350, 400, "Press ESCAPE to exit");
      painter->setPen(QPen(Qt::black));
    }
  }
}

void FieldWidget::keyPressEvent(QKeyEvent *event) {
  user_action = getSignal(event->key());

  if (game_selector == Snake) {
    bool hold = (user_action == UserAction_t::Action);
    snake::SnakeController::userInput(user_action, hold);
  }
  if (user_action == Terminate) {
    if (!game_info.pause) game_info.pause = (int)EXIT_STATE;
    handleTerminateGame();
    // QWidget::keyPressEvent(event);
  }
  if (game_selector == Tetris && user_action == Start &&
      game_info.pause == (int)GAMEOVER_STATE) {
    clearField(game_info.field);
    clearField(game_info.next);
    game_info.pause = (int)START_STATE;
  } else {
    QWidget::keyPressEvent(event);
  }
}

void FieldWidget::updateFieldDisplay() { update(); }

/**
 * @brief Transforms raw user input into static user action
 */
UserAction_t getSignal(int user_input) {
  UserAction_t key_code = Start;
  if (user_input == Qt::Key_Down)
    key_code = Down;
  else if (user_input == Qt::Key_Up)
    key_code = Up;
  else if (user_input == Qt::Key_Left)
    key_code = Left;
  else if (user_input == Qt::Key_Right)
    key_code = Right;
  else if (user_input == Qt::Key_Escape)
    key_code = Terminate;
  else if (user_input == Qt::Key_P)
    key_code = Pause;
  else if (user_input == Qt::Key_Return || user_input == Qt::Key_Enter)
    key_code = Start;
  else if (user_input == Qt::Key_Space)
    key_code = Action;
  return key_code;
}

void FieldWidget::handleTerminateGame() { QCoreApplication::quit(); }

void FieldWidget::initializeGame() { qt_game_info_init(); }
void qt_game_info_init() {
  size_t height = 20;
  size_t width = 10;
  game_info.field = new int *[height];
  for (size_t i = 0; i < height; ++i) game_info.field[i] = new int[width]{0};

  if (game_selector == Tetris) {
    game_info.next = new int *[4];
    for (size_t i = 0; i < 4; ++i) game_info.next[i] = new int[width]{0};
  }
}
long long current_timestamp() {
  struct timeval te;
  gettimeofday(&te, NULL);
  long long miliseconds = te.tv_sec * 1000LL + te.tv_usec / 1000;
  return miliseconds;
}
void clearField(int **matrix) {
  for (int i = 0; i < HEIGHT; i++) {
    for (int j = 0; j < WIDTH; j++) {
      matrix[i][j] = 0;
    }
  }
}

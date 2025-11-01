#include "fieldwidget.h"

#include <cstring>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <queue>
#include <vector>

UserAction_t user_action = Start;
GameInfo_t game_info = {nullptr, nullptr, 0, 0, 1, GAME_SPEED, 0};
long long prev_time = current_timestamp();

FieldWidget::FieldWidget(QWidget *parent)
    : QWidget{parent}, racing_client(nullptr) {
  initializeGame();

  font.setPointSize(14);
  setFixedSize(600, 600);
  setFocusPolicy(Qt::StrongFocus);

  // Инициализируем REST клиент для Racing
  if (game_selector == Racing) {
    racing_client = new s21::RacingRestClient();
    // Инициализируем поле
    for (int y = 0; y < HEIGHT; y++) {
      for (int x = 0; x < WIDTH; x++) {
        racing_field[y][x] = false;
      }
    }
    // Запускаем игру на сервере асинхронно (без блокировки UI)
    (void)racing_client->ensureStarted();
    // НЕ вызываем startGame() синхронно здесь, чтобы не вешать GUI
  }

  // timer to periodically update the matrix display
  timer = new QTimer(this);
  connect(timer, &QTimer::timeout, this, &FieldWidget::updateFieldDisplay);
  timer->start(30);  // ms
}

FieldWidget::~FieldWidget() {
  if (game_selector == Snake) {
    s21::snake_check_high_score(&game_info);
  }
  if (racing_client) {
    stopRacingGame();
    delete racing_client;
    racing_client = nullptr;
  }
  delete timer;
}

void FieldWidget::paintEvent(QPaintEvent *event) {
  Q_UNUSED(event);
  QPainter painter(this);
  painter.setFont(font);

  handle_pause(&painter);
  handle_gameover(&painter);

  if (game_selector == Snake) {
    game_info = *s21::updateCurrentState();
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
  } else if (game_selector == Racing) {
    // Обработка Racing через REST API
    if (racing_client) {
      racing_client->getState(racing_field, &game_info);
    }
    draw_racing_field(&painter);
    return;  // ВАЖНО: выходим, чтобы не рисовать стандартное поле
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
  if (game_selector == Racing) {
    std::string speedInfo = "SPEED: " + std::to_string(game_info.speed);
    painter->drawText(450, 280, speedInfo.c_str());
  }
  painter->drawText(350, 440, "MOVE: Arrows");
  painter->drawText(350, 480, "PAUSE: P");
  if (game_selector == Tetris) {
    painter->drawText(350, 520, "ROTATE: Space");
  } else if (game_selector == Racing) {
    painter->drawText(350, 520, "SPEED: Up");
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
    s21::SnakeController::userInput(user_action, hold);
  } else if (game_selector == Racing) {
    // Если Game Over, обрабатываем Enter/Esc напрямую
    if (game_info.pause == (int)GAMEOVER_STATE) {
      if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter) {
        // Перезапуск по аналогии с Java desktop: stopGame() → ensureStarted()
        // ВАЖНО: выполняем в фоне, чтобы не блокировать GUI
        if (racing_client) {
          s21::RacingRestClient *clientPtr = racing_client;
          std::thread([clientPtr]() {
            clientPtr->stopGame();
            (void)clientPtr->ensureStarted();
          }).detach();
        }
        // Не меняем локально pause; ждём подтверждения от сервера через
        // getState()
        return;
      } else if (event->key() == Qt::Key_Escape) {
        handleTerminateGame();
        return;
      }
    }
    // Пауза: отправляем как обычное действие без использования hold-тоггла
    if (event->key() == Qt::Key_P) {
      if (racing_client) {
        if (!racing_client->isGameRunning()) {
          (void)racing_client->ensureStarted();
        }
        racing_client->userInput(UserAction_t::Pause, /*hold=*/false);
      }
      return;
    }
    // Обработка клавиш для Racing через REST API (обычный режим)
    if (racing_client) {
      bool hold = (user_action == UserAction_t::Up);  // Up - ускорение (hold)
      racing_client->userInput(user_action, hold);
    }
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
    // } else if (game_selector == Racing && user_action == Start &&
    //            game_info.pause == (int)GAMEOVER_STATE) {
    //   // Рестарт Racing игры
    //   if (racing_client) {
    //     racing_client->resetGame();
    //     racing_client->startGame();
    //   }
    //   game_info.pause = (int)START_STATE;
  } else {
    QWidget::keyPressEvent(event);
  }
}

void FieldWidget::keyReleaseEvent(QKeyEvent *event) {
  if (game_selector == Racing && racing_client) {
    if (event->key() == Qt::Key_Up) {
      // Сообщаем серверу об окончании удержания ускорения
      racing_client->userInput(UserAction_t::Up, /*hold=*/false);
      return;
    }
  }
  QWidget::keyReleaseEvent(event);
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

void FieldWidget::handleTerminateGame() {
  QWidget *w = window();
  if (w) {
    w->close();
  } else {
    QCoreApplication::quit();
  }
}

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

void FieldWidget::draw_racing_field(QPainter *painter) {
  painter->setPen(QPen(Qt::black, 1));
  for (int y = 0; y < HEIGHT; y++) {
    for (int x = 0; x < WIDTH; x++) {
      int x_pos = x * cellSize;
      int y_pos = y * cellSize;
      painter->setBrush(QBrush(racing_field[y][x] ? Qt::blue : Qt::white));
      painter->drawRect(x_pos, y_pos, cellSize - 1, cellSize - 1);
    }
  }
  draw_information(painter);
}
void clearField(int **matrix) {
  for (int i = 0; i < HEIGHT; i++) {
    for (int j = 0; j < WIDTH; j++) {
      matrix[i][j] = 0;
    }
  }
}

void FieldWidget::stopRacingGame() {
  if (racing_client) {
    racing_client->stopGame();
  }
}

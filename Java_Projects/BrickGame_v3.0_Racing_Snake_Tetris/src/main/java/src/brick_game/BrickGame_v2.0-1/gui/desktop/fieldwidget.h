#ifndef FIELDWIDGET_H
#define FIELDWIDGET_H

#include <QCoreApplication>
#include <QKeyEvent>
#include <QLabel>
#include <QMessageBox>
#include <QPainter>
#include <QTimer>
#include <QWidget>

#include "../../brick_game/common/common_file.h"
#include "../../brick_game/common/defines.h"
#include "../../brick_game/racing/s21_racing_rest_client.hpp"
#include "../../brick_game/snake/controller/s21_controller.hpp"
#include "../../brick_game/snake/model/s21_model.hpp"
#include "../../brick_game/tetris/s21_tetris_back.h"
// #include "../../gui/cli/s21_gui.hpp"

UserAction_t getSignal(int user_input);

typedef enum {
  Tetris = 1,
  Snake = 2,
  Racing = 3,
} t_selector;

extern t_selector game_selector;

class FieldWidget : public QWidget {
  Q_OBJECT
 public:
  explicit FieldWidget(QWidget *parent = nullptr);
  ~FieldWidget();

 protected:
  void paintEvent(QPaintEvent *event) override;
  void keyPressEvent(QKeyEvent *event) override;
  void keyReleaseEvent(QKeyEvent *event) override;

 private slots:
  void updateFieldDisplay();

 private slots:
  void handleTerminateGame();

 private:
  void initializeGame();
  // void get_cwd();
  void draw_information(QPainter *painter);
  void handle_pause(QPainter *painter);
  void handle_gameover(QPainter *painter);
  void handle_win(QPainter *painter);
  void draw_racing_field(QPainter *painter);
  void stopRacingGame();
  const int cellSize = 30;
  QTimer *timer;
  QFont font;

  s21::RacingRestClient *racing_client;
  bool racing_field[HEIGHT][WIDTH];
  // Текущий оценённый центр игрока
  int playerCenterY = -1;
  int playerCenterX = -1;
  // Зафиксированный центр игрока для стабильной окраски
  int playerCenterYLocked = -1;
  int playerCenterXLocked = -1;
  // Карта стабильных цветов для соперников по колонке
  int opponentLaneColor[WIDTH] = {0};
};

void clearField(int **matrix);
void qt_game_info_init();
long long current_timestamp();
#endif  // FIELDWIDGET_H
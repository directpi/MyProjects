#include "../include/gui/mainwindow.h"

#include <QDateTime>
#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>
#include <QThread>
#include <QtGlobal>

#include "../../common/Utils.h"
#include "gui/DungeonView.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), game_(Game::create()) {
  ui->setupUi(this);
  setWindowTitle("MAZE and CAVE by Harkonex, Margarin, Directpi");

  // Оставляем только заголовок и кнопку закрытия
  setWindowFlags(Qt::Window | Qt::WindowTitleHint | Qt::WindowCloseButtonHint);

  // Устанавливаем начальный размер
  // resize(1024, 640);
  setFixedSize(1024, 650);

  show();

  caveTimer = new QTimer(this);
  connect(caveTimer, &QTimer::timeout, this, &MainWindow::updateCave);

  // Инициализация состояния кнопок
  ui->save_maze_btn->setEnabled(false);
  ui->find_way_btn->setEnabled(false);
  ui->ml_agent_training_btn->setEnabled(false);
  ui->start_cave_btn->setEnabled(false);
  ui->rows_create_spb->setValue(DEFAULT_ROWS);
  ui->cols_create_spb->setValue(DEFAULT_COLS);

  // Подключение сигналов
  connect(ui->generate_maze_btn, &QPushButton::pressed, this,
          &MainWindow::generateMaze);
  connect(ui->generate_cave_btn, &QPushButton::pressed, this,
          &MainWindow::generateCave);
  connect(ui->load_maze_btn, &QPushButton::pressed, this,
          &MainWindow::loadMazeFile);
  connect(ui->load_cave_btn, &QPushButton::pressed, this,
          &MainWindow::loadCaveFile);
  connect(ui->save_maze_btn, &QPushButton::pressed, this,
          &MainWindow::saveToFile);
  connect(ui->find_way_btn, &QPushButton::pressed, this,
          &MainWindow::showMazeWay);
  connect(ui->ml_agent_training_btn, &QPushButton::pressed, this,
          &MainWindow::showMazeWay);
  connect(ui->dungeon_view, &DungeonView::startUpdated, this,
          &MainWindow::updateStartPoint);
  connect(ui->dungeon_view, &DungeonView::finishUpdated, this,
          &MainWindow::updateFinishPoint);
  connect(ui->start_row_spb, QOverload<int>::of(&QSpinBox::valueChanged), this,
          &MainWindow::onStartPointChanged);
  connect(ui->start_col_spb, QOverload<int>::of(&QSpinBox::valueChanged), this,
          &MainWindow::onStartPointChanged);
  connect(ui->finish_row_spb, QOverload<int>::of(&QSpinBox::valueChanged), this,
          &MainWindow::onFinishPointChanged);
  connect(ui->finish_col_spb, QOverload<int>::of(&QSpinBox::valueChanged), this,
          &MainWindow::onFinishPointChanged);
#ifdef GPP_VERSION_15_OR_HIGHER
  connect(ui->step_checkBox, &QCheckBox::checkStateChanged, this,
          &MainWindow::onCheckboxStateChanged);
#else
  connect(ui->step_checkBox, &QCheckBox::stateChanged, this,
          &MainWindow::onCheckboxStateChanged);
#endif
  connect(ui->start_cave_btn, &QPushButton::clicked, this,
          &MainWindow::onStartCaveClicked);

  onCheckboxStateChanged(ui->step_checkBox->isChecked() ? Qt::Checked
                                                        : Qt::Unchecked);
  ui->start_cave_btn->setText(ui->step_checkBox->isChecked() ? "Step"
                                                             : "Start");
}

MainWindow::~MainWindow() {
  if (ui) {
    delete ui;
    ui = nullptr;
  }
}

void MainWindow::generateMaze() {
  try {
    int rows = ui->rows_create_spb->value();
    int cols = ui->cols_create_spb->value();

    // Генерация нового лабиринта
    game_->createDungeons(MAZE, rows, cols);

    // Получаем информацию о лабиринте
    auto dungeonInfo = game_->getGameInfo();
    if (auto* mazeInfo = std::get_if<MazeInfo>(&dungeonInfo)) {
      // Устанавливаем лабиринт в view
      ui->dungeon_view->setMaze(mazeInfo->rightWall, mazeInfo->downWall);

      // Устанавливаем новые ограничения для спинбоксов
      updateSpinBoxesRange(rows, cols);

      // Обновляем информацию о размере
      QString size = QString::number(rows) + "x" + QString::number(cols);
      ui->loaded_maze_size_lbl->setText(size);

      // Сбрасываем точки пути, если они были
      ui->dungeon_view->clearMazeSolution();
    }

    // Активируем кнопки
    ui->save_maze_btn->setEnabled(true);
    ui->find_way_btn->setEnabled(true);
    ui->ml_agent_training_btn->setEnabled(true);
    ui->start_cave_btn->setEnabled(false);
    ui->file_name_lbl->clear();

  } catch (const std::exception& e) {
    QMessageBox::critical(this, "Generation Error",
                          QString("Failed to generate maze: ") + e.what());
    resetMazeUI();
  }
}

void MainWindow::generateCave() {
  try {
    // Генерация случайных размеров от 1 до 50
    int rows = generateRandomInt(3, 50);
    int cols = generateRandomInt(3, 50);

    // Получаем параметры из UI
    int birthLimit = ui->birth_limit_spb->value();
    int deathLimit = ui->death_limit_spb->value();
    int chance = ui->chance_of_life_spb->value();

    // Создаем пещеру через game_
    game_->createCaveWithParams(rows, cols, birthLimit, deathLimit, chance);

    // Получаем информацию о пещере
    auto dungeonInfo = game_->getGameInfo();
    if (auto* caveInfo = std::get_if<CaveInfo>(&dungeonInfo)) {
      // Устанавливаем пещеру в view
      ui->dungeon_view->setCave(caveInfo->matrix);

      // Обновляем UI
      updateSpinBoxesRange(rows, cols);
      ui->loaded_maze_size_lbl->setText(QString("%1x%2").arg(rows).arg(cols));

      // Настройка кнопок
      ui->save_maze_btn->setEnabled(false);
      ui->find_way_btn->setEnabled(false);
      ui->ml_agent_training_btn->setEnabled(false);
      ui->file_name_lbl->clear();
      ui->start_cave_btn->setEnabled(true);
      ui->start_cave_btn->setText("Start");

      isEvolutionRunning = false;
    }

  } catch (const std::exception& e) {
    QMessageBox::critical(this, "Generation error",
                          QString("Failed to generate a cave: ") + e.what());
    ui->start_cave_btn->setEnabled(false);
    resetMazeUI();
  }
}

void MainWindow::loadFromFile(DungeonType type) {
  // Определяем путь к src/files относительно исполняемого файла
  QString defaultPath =
      QDir::cleanPath(QCoreApplication::applicationDirPath() + "/files");

  // Проверяем существует ли папка
  QDir dir(defaultPath);
  if (!dir.exists()) {
    qWarning() << "Directory does not exist:" << defaultPath;
    defaultPath = QDir::homePath();  // Используем домашнюю директорию как
                                     // запасной вариант
  }

  QString filter = (type == MAZE) ? "Maze Files (*.txt)" : "Cave Files (*.txt)";
  QString title = (type == MAZE) ? "Open Maze File" : "Open Cave File";

  QString file_path =
      QFileDialog::getOpenFileName(this, title, defaultPath, filter);

  if (!file_path.isEmpty()) {
    try {
      game_->createDungeons(type, file_path.toStdString());

      if (game_->getDungeon() == nullptr) {
        throw std::runtime_error("Game object is null");
      }

      auto dungeonInfo = game_->getGameInfo();

      int rows = 0;
      int cols = 0;

      if (type == MAZE) {
        if (auto* mazeInfo = std::get_if<MazeInfo>(&dungeonInfo)) {
          ui->dungeon_view->setMaze(mazeInfo->rightWall, mazeInfo->downWall);

          rows = mazeInfo->rightWall.size();
          cols = mazeInfo->rightWall[0].size();

          ui->save_maze_btn->setEnabled(true);
          ui->find_way_btn->setEnabled(true);
          ui->ml_agent_training_btn->setEnabled(true);
          ui->start_cave_btn->setEnabled(false);
        }
      } else if (type == CAVE) {
        if (auto* caveInfo = std::get_if<CaveInfo>(&dungeonInfo)) {
          ui->dungeon_view->setCave(caveInfo->matrix);
          rows = caveInfo->matrix.size();
          cols = caveInfo->matrix[0].size();
          ui->save_maze_btn->setEnabled(false);
          ui->find_way_btn->setEnabled(false);
          ui->ml_agent_training_btn->setEnabled(false);
          ui->start_cave_btn->setEnabled(true);
        }
      }

      // Обновляем UI элементы
      updateSpinBoxesRange(rows, cols);
      ui->loaded_maze_size_lbl->setText(QString("%1x%2").arg(rows).arg(cols));

      QFileInfo file_info(file_path);
      ui->file_name_lbl->setText(file_info.fileName());
      if (type == MAZE) {
        ui->save_maze_btn->setEnabled(true);
      }

    } catch (const std::exception& e) {
      QMessageBox::critical(this, "Load Error",
                            QString("Failed to load dungeon: ") + e.what());
      ui->dungeon_view->clearScene();
      ui->save_maze_btn->setEnabled(false);
      ui->find_way_btn->setEnabled(false);
      ui->file_name_lbl->clear();
      ui->loaded_maze_size_lbl->clear();
    }
  }
}

void MainWindow::saveToFile() {
  // Проверяем, есть ли что сохранять
  auto dungeonInfo = game_->getGameInfo();
  if (!std::holds_alternative<MazeInfo>(dungeonInfo)) {
    QMessageBox::warning(this, "Error", "No maze data to save");
    return;
  }

  // Диалог сохранения файла
  QString defaultPath =
      QCoreApplication::applicationDirPath() + "/../../src/files";
  QString fileName =
      "Maze_" + QDateTime::currentDateTime().toString("yyyy-MM-dd_hhmmss") +
      ".txt";
  QString filePath = QFileDialog::getSaveFileName(
      this, "Save Maze", QDir::cleanPath(defaultPath + "/" + fileName),
      "Text Files (*.txt)");

  if (!filePath.isEmpty()) {
    try {
      game_->saveToFile(filePath.toStdString());
      QMessageBox::information(this, "Success", "Maze saved successfully!");
      QFileInfo file_info(filePath);
      ui->file_name_lbl->setText(file_info.fileName());

    } catch (const std::exception& e) {
      QMessageBox::critical(this, "Error",
                            QString("Failed to save maze: ") + e.what());
    }
  }
}

void MainWindow::updateStartPoint(int x, int y) {
  ui->start_row_spb->setValue(x + 1);  // +1 для пользовательского отображения
  ui->start_col_spb->setValue(y + 1);
  ui->dungeon_view->clearMazeSolution();  // Очищаем путь
}

void MainWindow::updateFinishPoint(int x, int y) {
  ui->finish_row_spb->setValue(x + 1);
  ui->finish_col_spb->setValue(y + 1);
  ui->dungeon_view->clearMazeSolution();  // Очищаем путь
}

void MainWindow::showMazeWay() {
  try {
    // Определяем какая кнопка была нажата
    QPushButton* clickedButton = qobject_cast<QPushButton*>(sender());
    bool isMLAgent = (clickedButton == ui->ml_agent_training_btn);

    // Получаем координаты из интерфейса
    int start_row = ui->start_row_spb->value() - 1;
    int start_col = ui->start_col_spb->value() - 1;
    int finish_row = 0;
    int finish_col = 0;

    auto dungeonInfo = game_->getGameInfo();

    if (auto* mazeInfo = std::get_if<MazeInfo>(&dungeonInfo)) {
      if (isMLAgent) {
        finish_row = mazeInfo->rightWall.size() - 1;
        finish_col = mazeInfo->rightWall[0].size() - 1;
        // Обновляем UI
        ui->finish_row_spb->setValue(finish_row + 1);
        ui->finish_col_spb->setValue(finish_col + 1);
      } else {
        finish_row = ui->finish_row_spb->value() - 1;
        finish_col = ui->finish_col_spb->value() - 1;
      }
    } else {
      QMessageBox::warning(this, "Error", "No maze data available");
      return;
    }

    // Устанавливаем точки старта и финиша в DungeonView
    ui->dungeon_view->setStartPoint(start_row, start_col);
    ui->dungeon_view->setFinishPoint(finish_row, finish_col);

    std::vector<std::pair<int, int>> path;

    if (isMLAgent) {
      // Используем ML агента
      path =
          game_->runMLAgent({start_row, start_col}, {finish_row, finish_col});
    } else {
      // Используем стандартный алгоритм поиска пути
      path = game_->getPath({start_row, start_col}, {finish_row, finish_col});
    }

    // Отображаем путь
    if (!path.empty()) {
      ui->dungeon_view->paintMazeSolution(path);
    } else {
      QMessageBox::information(this, "Path",
                               "No path found between selected points");
    }
  } catch (const std::exception& e) {
    QMessageBox::critical(this, "Path Error",
                          QString("Failed to find path: ") + e.what());
  }
}

void MainWindow::onStartPointChanged() {
  int row = ui->start_row_spb->value() - 1;
  int col = ui->start_col_spb->value() - 1;
  ui->dungeon_view->setStartPoint(row, col);
  ui->dungeon_view->clearMazeSolution();  // Очищаем путь
}

void MainWindow::onFinishPointChanged() {
  int row = ui->finish_row_spb->value() - 1;
  int col = ui->finish_col_spb->value() - 1;
  ui->dungeon_view->setFinishPoint(row, col);
  ui->dungeon_view->clearMazeSolution();  // Очищаем путь
}

void MainWindow::resetMazeUI() {
  ui->dungeon_view->clearScene();
  ui->save_maze_btn->setEnabled(false);
  ui->find_way_btn->setEnabled(false);
  ui->ml_agent_training_btn->setEnabled(false);
  ui->file_name_lbl->clear();
  ui->loaded_maze_size_lbl->clear();

  // Возвращаем спинбоксы к значениям по умолчанию
  updateSpinBoxesRange(DEFAULT_ROWS, DEFAULT_COLS);
}

void MainWindow::onCheckboxStateChanged(int state) {
  if (state == Qt::Unchecked) {
    // Автоматический режим
    ui->start_cave_btn->setText(isEvolutionRunning ? "STOP" : "START");
    ui->delay_spb->setEnabled(true);

    // Если эволюция была в процессе, продолжаем в автоматическом режиме
    if (isEvolutionRunning) {
      caveTimer->start(ui->delay_spb->value());
    }
  } else {
    // Пошаговый режим
    ui->start_cave_btn->setText(isEvolutionRunning ? "NEXT STEP" : "START");
    ui->delay_spb->setEnabled(false);

    // Останавливаем таймер, но сохраняем состояние isEvolutionRunning
    caveTimer->stop();
  }
}

void MainWindow::updateSpinBoxesRange(int rows, int cols) {
  int max_row = rows;
  int max_col = cols;

  // Блокируем сигналы чтобы не вызывать слоты изменений
  ui->start_row_spb->blockSignals(true);
  ui->start_col_spb->blockSignals(true);
  ui->finish_row_spb->blockSignals(true);
  ui->finish_col_spb->blockSignals(true);

  // Устанавливаем новые диапазоны
  ui->start_row_spb->setMaximum(max_row);
  ui->start_col_spb->setMaximum(max_col);
  ui->finish_row_spb->setMaximum(max_row);
  ui->finish_col_spb->setMaximum(max_col);

  // Сбрасываем значения если они выходят за новые границы
  if (ui->start_row_spb->value() > max_row) ui->start_row_spb->setValue(0);
  if (ui->start_col_spb->value() > max_col) ui->start_col_spb->setValue(0);
  if (ui->finish_row_spb->value() > max_row)
    ui->finish_row_spb->setValue(max_row);
  if (ui->finish_col_spb->value() > max_col)
    ui->finish_col_spb->setValue(max_col);

  // Разблокируем сигналы
  ui->start_row_spb->blockSignals(false);
  ui->start_col_spb->blockSignals(false);
  ui->finish_row_spb->blockSignals(false);
  ui->finish_col_spb->blockSignals(false);

  // Очищаем текущий путь
  ui->dungeon_view->clearMazeSolution();
}

void MainWindow::onStartCaveClicked() {
  if (ui->step_checkBox->isChecked()) {
    // Пошаговый режим
    if (waitingForNextStep) {
      // Продолжаем выполнение
      emit nextStepRequested();
    } else {
      // Начинаем новую эволюцию
      isEvolutionRunning = true;
      performCaveStep();
    }
  } else {
    // Автоматический режим (переключатель Start/Stop)
    isEvolutionRunning = !isEvolutionRunning;
    ui->start_cave_btn->setText(isEvolutionRunning ? "STOP" : "START");
    ui->delay_spb->setEnabled(!isEvolutionRunning);

    if (isEvolutionRunning) {
      performCaveStep();
    }
  }
}

void MainWindow::performCaveStep() {
  if (!isEvolutionRunning) return;

  try {
    bool changesMade;
    do {
      // 1. Получаем текущее состояние пещеры
      CaveInfo caveInfo = std::get<CaveInfo>(game_->getGameInfo());

      // 2. Обновляем матрицу и параметры в Game
      game_->updateCaveMatrix(caveInfo.matrix);

      // Устанавливаем параметры эволюции
      if (auto* cave = dynamic_cast<Cave*>(game_->getDungeon())) {
        cave->setLimitBirth(ui->birth_limit_spb->value());
        cave->setLimitDeath(ui->death_limit_spb->value());
      }

      // 3. Выполняем шаг эволюции
      changesMade = game_->stepByStep();

      if (!changesMade) {
        QMessageBox::information(this, "Final", "The cave has stabilized");
        stopEvolution();
        return;
      }

      // 4. Получаем обновленное состояние
      CaveInfo updatedCaveInfo = std::get<CaveInfo>(game_->getGameInfo());
      ui->dungeon_view->setCave(updatedCaveInfo.matrix);

      // 5. Обработка разных режимов
      if (ui->step_checkBox->isChecked()) {  // Пошаговый режим
        waitingForNextStep = true;  // Ждём нажатия кнопки
        ui->start_cave_btn->setText("NEXT STEP");

        // Создаем локальный цикл ожидания
        QEventLoop loop;
        connect(this, &MainWindow::nextStepRequested, &loop, &QEventLoop::quit);
        loop.exec();

        waitingForNextStep = false;

        // Проверяем не было ли отмены
        if (!isEvolutionRunning) return;
      } else {
        // Автоматический режим
        QEventLoop loop;
        QTimer::singleShot(ui->delay_spb->value(), &loop, &QEventLoop::quit);
        loop.exec();
      }

      // Обновляем интерфейс
      QCoreApplication::processEvents();

    } while (isEvolutionRunning && changesMade);

  } catch (const std::exception& e) {
    QMessageBox::critical(this, "Error",
                          QString("Cave evolution error: ") + e.what());
    stopEvolution();
  }
}

void MainWindow::stopEvolution() {
  isEvolutionRunning = false;
  ui->start_cave_btn->setText("START");
  ui->delay_spb->setEnabled(true);
}

void MainWindow::updateCave() {
  if (ui->step_checkBox->isChecked()) {
    // В пошаговом режиме просто обновляем отображение
    auto dungeonInfo = game_->getGameInfo();
    if (auto* caveInfo = std::get_if<CaveInfo>(&dungeonInfo)) {
      ui->dungeon_view->setCave(caveInfo->matrix);
    }
  } else {
    // В автоматическом режиме выполняем шаг эволюции
    if (isEvolutionRunning) {
      performCaveStep();
    }
  }
}

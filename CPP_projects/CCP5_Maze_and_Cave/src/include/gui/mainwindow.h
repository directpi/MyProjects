#ifndef A1_MAZE_CPP_ID_1391674_TEAM_TL_HARKONEX_5C2B27DF_8751_4660_1_MAINWINDOW_H
#define A1_MAZE_CPP_ID_1391674_TEAM_TL_HARKONEX_5C2B27DF_8751_4660_1_MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QtGlobal>
#include <memory>

#include "../api/API.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

/**
 * @class MainWindow
 * @brief Основное окно приложения. Содержит элементы управления и область
 * отрисовки лабиринта
 */
class MainWindow : public QMainWindow {
  Q_OBJECT

  /// Количество строк по умолчанию для генерации лабиринта
  constexpr static int DEFAULT_ROWS = 10;
  /// Количество столбцов по умолчанию для генерации лабиринта
  constexpr static int DEFAULT_COLS = 10;

 public:
  /**
   * @brief Конструктор
   * @param[in] parent родитель
   */
  explicit MainWindow(QWidget *parent = nullptr);

  /**
   * @brief Деструктор
   */
  ~MainWindow() override;

 signals:
  void nextStepRequested();

 private slots:

  /**
   * @brief Слот для генерации лабиринта на заданное количество строк и
   * столбцов (указываются в спинбоксах)
   */
  void generateMaze();
  void generateCave();

  void loadMazeFile() { loadFromFile(MAZE); }
  void loadCaveFile() { loadFromFile(CAVE); }

  void onStartCaveClicked();
  void updateCave();

  /**
   * @brief Слот для загрузки лабиринта из файла
   */
  void loadFromFile(DungeonType type);
  /**
   * @brief Слот для сохранения лабиринта в файл
   */
  void saveToFile();

  /**
   * @brief Слот для обновления спинбоксов начальной точки решения лабиринта.
   * Вызывается, когда точка старта выбрана мышкой.
   * @param[in] x строка точки старта
   * @param[in] y столбец точки старта
   */
  void updateStartPoint(int x, int y);

  /**
   * @brief Слот для обновления спинбоксов конечной точки решения лабиринта.
   * Вызывается, когда точка финиша выбрана мышкой.
   * @param[in] x строка точки финиша
   * @param[in] y столбец точки финиша
   */
  void updateFinishPoint(int x, int y);

  /**
   * @brief Слот для генерации и отображения решения текущего лабиринта
   */
  void showMazeWay();

  void onStartPointChanged();
  void onFinishPointChanged();
  void onCheckboxStateChanged(int state);
  void updateSpinBoxesRange(int rows, int cols);
  void resetMazeUI();
  void stopEvolution();

 private:
  /// UI-форма
  Ui::MainWindow *ui = nullptr;

  std::unique_ptr<Game> game_ = nullptr;  // Указатель на объект Game

  void performCaveStep();
  void resetCaveGeneration();
  QTimer *caveTimer;
  bool isEvolutionRunning = false;
  bool isStepModeActive = false;
  bool waitingForNextStep = false;
};

#endif  // A1_MAZE_CPP_ID_1391674_TEAM_TL_HARKONEX_5C2B27DF_8751_4660_1_MAINWINDOW_H

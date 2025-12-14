#ifndef A1_MAZE_CPP_ID_1391674_TEAM_TL_HARKONEX_5C2B27DF_8751_4660_1_DUNGEON_VIEW_H
#define A1_MAZE_CPP_ID_1391674_TEAM_TL_HARKONEX_5C2B27DF_8751_4660_1_DUNGEON_VIEW_H

#include <QGraphicsView>
#include <QMap>
#include <QPainter>
#include <vector>

#include "../../include/api/API.h"
#include "../../include/common/TemplateMatrix.h"

class DungeonView : public QGraphicsView {
  Q_OBJECT

 public:
  explicit DungeonView(QWidget* parent = nullptr);
  ~DungeonView() override = default;

  void setMaze(const std::vector<std::vector<int>>& rightWalls,
               const std::vector<std::vector<int>>& downWalls);
  void setCave(const std::vector<std::vector<int>>& caveMatrix);
  void clearScene();
  void paintMazeSolution(std::vector<std::pair<int, int>> path);
  void setStartPoint(int row, int col);
  void setFinishPoint(int row, int col);
  void clearMazeSolution();

 signals:
  void startUpdated(int x, int y);
  void finishUpdated(int x, int y);

 protected:
  void mousePressEvent(QMouseEvent* event) override;
  void paintEvent(QPaintEvent* event) override;

 private:
  enum CellData { ROW = 0, COL = 1, PRESSED = 2 };

  constexpr static double WALL_THICKNESS = 2.0;
  constexpr static double PATH_THICKNESS = 3.0;
  const QColor WALL_COLOR = QColor(255, 215, 0);  // Qt::white;
  const QColor PATH_COLOR = Qt::blue;             // QColor(0, 229, 255);

  bool is_cave_ = false;
  std::vector<std::vector<int>> cave_matrix_;

  QGraphicsScene* scene_;
  QMap<QString, QGraphicsRectItem*> selected_points_;
  double cell_size_;
  std::vector<std::vector<int>> right_walls_;
  std::vector<std::vector<int>> down_walls_;

  QGraphicsRectItem* findCell(int row, int col);

  // Удаление точки по типу ("start" или "finish")
  void removePoint(const QString& type);

  // Установка новой точки
  void setPoint(const QString& type, QGraphicsRectItem* rect);
};

#endif  // A1_MAZE_CPP_ID_1391674_TEAM_TL_HARKONEX_5C2B27DF_8751_4660_1_DUNGEON_VIEW_H

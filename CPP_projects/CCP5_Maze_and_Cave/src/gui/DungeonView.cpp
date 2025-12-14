#include "../include/gui/DungeonView.h"

#include <QDebug>
#include <QGraphicsLineItem>
#include <QGraphicsRectItem>
#include <QMouseEvent>
#include <QPixmap>

DungeonView::DungeonView(QWidget* parent) : QGraphicsView(parent) {
  scene_ = new QGraphicsScene(this);
  setScene(scene_);
  setRenderHint(QPainter::Antialiasing);
  setDragMode(QGraphicsView::NoDrag);
  setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  setFrameShape(QFrame::NoFrame);
}

void DungeonView::setMaze(const std::vector<std::vector<int>>& rightWalls,
                          const std::vector<std::vector<int>>& downWalls) {
  clearScene();
  right_walls_ = rightWalls;
  down_walls_ = downWalls;

  int rows = right_walls_.size();
  int cols = rows > 0 ? right_walls_[0].size() : 0;

  scene_->setSceneRect(0, 0, rect().width(), rect().height());
  double cell_width = rect().width() / static_cast<double>(cols);
  double cell_height = rect().height() / static_cast<double>(rows);
  cell_size_ = std::min(cell_width, cell_height);

  double offset_x = (rect().width() - (cell_size_ * cols)) / 2.0;
  double offset_y = (rect().height() - (cell_size_ * rows)) / 2.0;

  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < cols; ++j) {
      double x = offset_x + j * cell_size_;
      double y = offset_y + i * cell_size_;

      QGraphicsRectItem* cell =
          scene_->addRect(x, y, cell_size_, cell_size_, QPen(Qt::NoPen));
      cell->setData(CellData::ROW, i);
      cell->setData(CellData::COL, j);
      cell->setData(CellData::PRESSED, false);
    }
  }
}

void DungeonView::setCave(const std::vector<std::vector<int>>& caveMatrix) {
  clearScene();
  is_cave_ = true;
  cave_matrix_ = caveMatrix;

  if (cave_matrix_.empty() || cave_matrix_[0].empty()) {
    qWarning() << "Empty cave matrix";
    return;
  }

  int rows = cave_matrix_.size();
  int cols = cave_matrix_[0].size();

  // Цвета
  QColor liveCellColor = Qt::darkGray;  // Живая клетка QColor(255, 215, 0);
  QColor borderColor = QColor(255, 215, 0);  // Цвет границы
  // QColor deadCellColor = Qt::white; // Мертвая клетка

  // Рассчитываем размер ячейки для отображения
  double cell_size = qMin(width() / cols, height() / rows);
  double offset_x = (width() - cell_size * cols) / 2.0;
  double offset_y = (height() - cell_size * rows) / 2.0;

  // Отрисовка пещеры
  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < cols; ++j) {
      if (cave_matrix_[i][j] == 1) {  // Живая клетка
        QGraphicsRectItem* cell = scene_->addRect(
            offset_x + j * cell_size, offset_y + i * cell_size, cell_size,
            cell_size, QPen(liveCellColor), QBrush(borderColor));
        Q_UNUSED(cell);
      }
      // else if (cave_matrix_[i][j] == 0) {  // Мертвая клетка
      //   QGraphicsRectItem* cell = scene_->addRect(
      //       offset_x + j * cell_size, offset_y + i * cell_size, cell_size,
      //       cell_size, QPen(deadCellColor), QBrush(deadCellColor));
      //   Q_UNUSED(cell);
      // }
    }
  }

  // Добавляем внешнюю рамку вокруг всей пещеры
  QGraphicsRectItem* border =
      scene_->addRect(offset_x, offset_y, cols * cell_size, rows * cell_size,
                      QPen(borderColor, 2));
  Q_UNUSED(border);

  update();  // Обновляем отрисовку
}

void DungeonView::paintMazeSolution(std::vector<std::pair<int, int>> path) {
  clearMazeSolution();
  if (path.empty() || right_walls_.empty()) return;

  const int rows = right_walls_.size();
  const int cols = right_walls_[0].size();

  const double cell_size = std::min(width() / cols, height() / rows);
  const double offset_x = (width() - cell_size * cols) / 2.0;
  const double offset_y = (height() - cell_size * rows) / 2.0;

  QPen path_pen(PATH_COLOR, PATH_THICKNESS, Qt::SolidLine, Qt::RoundCap);

  for (size_t i = 1; i < path.size(); ++i) {
    // Получаем координаты в порядке (row=y, col=x)
    const int y1 = path[i - 1].first;   // row
    const int x1 = path[i - 1].second;  // col
    const int y2 = path[i].first;       // row
    const int x2 = path[i].second;      // col

    // Преобразуем в координаты отрисовки Qt (x,y)
    const double qt_x1 = offset_x + x1 * cell_size + cell_size / 2;
    const double qt_y1 = offset_y + y1 * cell_size + cell_size / 2;
    const double qt_x2 = offset_x + x2 * cell_size + cell_size / 2;
    const double qt_y2 = offset_y + y2 * cell_size + cell_size / 2;

    scene_->addLine(qt_x1, qt_y1, qt_x2, qt_y2, path_pen);
  }
}

void DungeonView::paintEvent(QPaintEvent* event) {
  // Вызываем базовую реализацию отрисовки
  QGraphicsView::paintEvent(event);

  // Если лабиринт пустой - выходим
  if (right_walls_.empty()) return;

  QPainter painter(viewport());
  painter.setRenderHint(QPainter::Antialiasing);  // Добавляем сглаживание

  int rows = right_walls_.size();
  int cols = rows > 0 ? right_walls_[0].size() : 0;

  // Вычисляем смещение для центрирования лабиринта
  double offset_x = (rect().width() - (cell_size_ * cols)) / 2.0;
  double offset_y = (rect().height() - (cell_size_ * rows)) / 2.0;

  // Настраиваем перо для отрисовки стен
  QPen wall_pen(WALL_COLOR, WALL_THICKNESS);
  painter.setPen(wall_pen);

  // 1. Отрисовка внешних границ
  painter.drawRect(
      QRectF(offset_x, offset_y, cols * cell_size_, rows * cell_size_));

  // 2. Отрисовка внутренних стен
  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < cols; ++j) {
      double x = offset_x + j * cell_size_;
      double y = offset_y + i * cell_size_;

      // Правая стена (если есть)
      if (j < cols - 1 && right_walls_[i][j]) {  // Изменили условие
        painter.drawLine(QPointF(x + cell_size_, y),
                         QPointF(x + cell_size_, y + cell_size_));
      }

      // Нижняя стена (если есть)
      if (i < rows - 1 && down_walls_[i][j]) {  // Изменили условие
        painter.drawLine(QPointF(x, y + cell_size_),
                         QPointF(x + cell_size_, y + cell_size_));
      }
    }
  }
  // Отрисовываем иконки старта и финиша
  for (auto iter = selected_points_.begin(); iter != selected_points_.end();
       ++iter) {
    QGraphicsRectItem* cell = iter.value();
    // Загружаем соответствующую иконку
    QPixmap icon(":/icons/" + iter.key() + "_icon.png");
    QRectF cell_rect = cell->sceneBoundingRect();
    QPointF top_left = mapFromScene(cell_rect.topLeft());
    QSize size = cell_rect.size().toSize();
    // Рисуем иконку с сохранением пропорций
    painter.drawPixmap(top_left, icon.scaled(size, Qt::KeepAspectRatio,
                                             Qt::SmoothTransformation));
  }
}

void DungeonView::clearMazeSolution() {
  // Удаляем все элементы типа QGraphicsLineItem (линии пути)
  for (QGraphicsItem* item : scene()->items()) {
    if (item->type() == QGraphicsLineItem::Type) {
      scene()->removeItem(item);
      delete item;
    }
  }

  update();  // Обновляем отображение
}

void DungeonView::mousePressEvent(QMouseEvent* event) {
  QGraphicsView::mousePressEvent(event);

  QPointF scenePos = mapToScene(event->pos());
  QGraphicsItem* item = scene()->itemAt(scenePos, QTransform());
  if (!item) return;

  QGraphicsRectItem* rectItem = dynamic_cast<QGraphicsRectItem*>(item);
  if (!rectItem) return;

  int row = rectItem->data(CellData::ROW).toInt();
  int col = rectItem->data(CellData::COL).toInt();

  if (event->button() == Qt::LeftButton) {
    setPoint("start", rectItem);
    emit startUpdated(row, col);
  } else if (event->button() == Qt::RightButton) {
    setPoint("finish", rectItem);
    emit finishUpdated(row, col);
  }
  update();
}

void DungeonView::setStartPoint(int row, int col) {
  if (QGraphicsRectItem* cell = findCell(row, col)) {
    setPoint("start", cell);
  }
}

void DungeonView::setFinishPoint(int row, int col) {
  if (QGraphicsRectItem* cell = findCell(row, col)) {
    setPoint("finish", cell);
  }
}

QGraphicsRectItem* DungeonView::findCell(int row, int col) {
  for (QGraphicsItem* item : scene()->items()) {
    QGraphicsRectItem* rectItem = dynamic_cast<QGraphicsRectItem*>(item);
    if (rectItem && rectItem->data(CellData::ROW).toInt() == row &&
        rectItem->data(CellData::COL).toInt() == col) {
      return rectItem;
    }
  }
  return nullptr;
}

void DungeonView::setPoint(const QString& type, QGraphicsRectItem* rect) {
  // Сначала удаляем существующую точку этого типа
  if (selected_points_.contains(type)) {
    selected_points_[type]->setData(CellData::PRESSED, false);
    selected_points_.remove(type);
  }

  // Устанавливаем новую точку
  selected_points_[type] = rect;
  rect->setData(CellData::PRESSED, true);
  update();
}
void DungeonView::removePoint(const QString& type) {
  if (selected_points_.contains(type)) {
    selected_points_[type]->setData(CellData::PRESSED, false);
    selected_points_.remove(type);
    update();  // Перерисовываем сцену
  }
}

void DungeonView::clearScene() {
  // Очищаем все элементы сцены
  scene_->clear();

  // Очищаем данные пещеры
  is_cave_ = false;
  cave_matrix_.clear();

  // Очищаем выбранные точки
  selected_points_.clear();

  // Сбрасываем данные о стенах
  right_walls_.clear();
  down_walls_.clear();

  // Сбрасываем размер ячейки
  cell_size_ = 0;
}

#define GL_SILENCE_DEPRECATION

#include "myopenglwidget.h"

#include "../controller/controller.hpp"  // Добавляем заголовок контроллера
#include "../controller/strategies.hpp"  // Добавляем заголовок стратегий

s21::MyOpenGLWidget::MyOpenGLWidget(QWidget *parent)
    : QOpenGLWidget(parent),
      controller_(new ObjModelController) {  // Передаем контроллер
  setMouseTracking(true);
}

s21::MyOpenGLWidget::~MyOpenGLWidget() { controller_->freeModel(); }

void s21::MyOpenGLWidget::setController(ObjModelController *controller) {
  controller_ = controller;
  if (controller_) {
    // Подключение сигнала контроллера к методу обновления представления
    connect(controller_, &ObjModelController::modelUpdated, this,
            &MyOpenGLWidget::onUpdate);
  }
}

// Слот для обновления виджета
void s21::MyOpenGLWidget::onUpdate() {
  // Логика обновления графики после изменений в модели
  QOpenGLWidget::update();  // Вызов OpenGL обновления
}

void s21::MyOpenGLWidget::initializeGL() {
  initializeOpenGLFunctions();
  glClearColor(0.60392f, 0.60392f, 0.60392f, 1.0f);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_PROGRAM_POINT_SIZE);
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void s21::MyOpenGLWidget::resizeGL(int w, int h) { glViewport(0, 0, w, h); }

void s21::MyOpenGLWidget::resizeEvent(QResizeEvent *event) {
  QOpenGLWidget::resizeEvent(event);
  update();
}

void s21::MyOpenGLWidget::resetTransformationValues() {
  object_scale_ = 100;
  object_translate_ = {0, 0, 0};
  object_rotation_ = {0, 0, 0};
  max_vertex_ = 0;
}

void s21::MyOpenGLWidget::setObjectScale(const double &value) {
  object_scale_ = value;
  controller_->setStrategy(
      new ScaleStrategy(value));  // Устанавливаем стратегию масштабирования
  controller_->applyTransformation();  // Применяем трансформацию
}

double s21::MyOpenGLWidget::getCurrentPositionX() {
  return object_translate_.x;
}

void s21::MyOpenGLWidget::setObjectTranslateX(const double &value) {
  double deltaX = value - object_translate_.x;
  object_translate_.x = value;
  controller_->setStrategy(
      new MoveStrategy(deltaX, 0, 0));  // Передаем разницу по оси X
  controller_->applyTransformation();
}

void s21::MyOpenGLWidget::setObjectTranslateY(const double &value) {
  double deltaY = value - object_translate_.y;
  object_translate_.y = value;
  controller_->setStrategy(new MoveStrategy(
      0, deltaY, 0));  // Устанавливаем стратегию перемещения по Y
  controller_->applyTransformation();
}

void s21::MyOpenGLWidget::setObjectTranslateZ(const double &value) {
  double deltaZ = value - object_translate_.z;
  object_translate_.z = value;
  controller_->setStrategy(new MoveStrategy(
      0, 0, deltaZ));  // Устанавливаем стратегию перемещения по Z
  controller_->applyTransformation();
}

void s21::MyOpenGLWidget::setObjectRotationX(const double &value) {
  double deltaRotationX = value - object_rotation_.x;
  object_rotation_.x = value;
  controller_->setStrategy(
      new RotateStrategy(deltaRotationX, 0, 0));  // Передаем разницу по оси X
  controller_->applyTransformation();  // Применяем трансформацию
}

void s21::MyOpenGLWidget::setObjectRotationY(const double &value) {
  double deltaRotationY = value - object_rotation_.y;
  object_rotation_.y = value;
  controller_->setStrategy(
      new RotateStrategy(0, deltaRotationY, 0));  // Передаем разницу по оси X
  controller_->applyTransformation();  // Применяем трансформацию
}

void s21::MyOpenGLWidget::setObjectRotationZ(const double &value) {
  double deltaRotationZ = value - object_rotation_.z;
  object_rotation_.z = value;
  controller_->setStrategy(
      new RotateStrategy(0, 0, deltaRotationZ));  // Передаем разницу по оси X
  controller_->applyTransformation();  // Применяем трансформацию
}

void s21::MyOpenGLWidget::setVertexColor(const QColor &color) {
  vertex_color_ = color;
  update();
}

void s21::MyOpenGLWidget::setEdgeColor(const QColor &color) {
  edge_color_ = color;
  update();
}

void s21::MyOpenGLWidget::setBackgroundColor(const QColor &color) {
  if (background_color_ != color) {
    background_color_ = color;
    update();
  }
}

void s21::MyOpenGLWidget::setEdgesType(const int &value) {
  edges_type_ = value;
}

void s21::MyOpenGLWidget::setEdgesThickness(const int &value) {
  edges_thickness_ = value;
}

void s21::MyOpenGLWidget::setVerticesSize(const int &value) {
  vertices_size_ = value;
}

void s21::MyOpenGLWidget::setVertexDisplayType(const int &value) {
  vertex_display_type_ = value;
}

void s21::MyOpenGLWidget::setProjectionType(const int &value) {
  projection_type_ = value;
}

QColor s21::MyOpenGLWidget::getVertexColor() { return vertex_color_; }

QColor s21::MyOpenGLWidget::getEdgeColor() { return edge_color_; }

QColor s21::MyOpenGLWidget::getBackgroundColor() { return background_color_; }

int s21::MyOpenGLWidget::getVerticesSize() { return vertices_size_; }

int s21::MyOpenGLWidget::getEdgesThickness() { return edges_thickness_; }

int s21::MyOpenGLWidget::getProjectionType() { return projection_type_; }

int s21::MyOpenGLWidget::getVertexDisplayType() { return vertex_display_type_; }

int s21::MyOpenGLWidget::getEdgesType() { return edges_type_; }

double s21::MyOpenGLWidget::getObjectScale() { return object_scale_; }

s21::Vertex s21::MyOpenGLWidget::getObjectTranslate() {
  return object_translate_;
}

s21::Vertex s21::MyOpenGLWidget::getObjectRotation() {
  return object_rotation_;
}

void s21::MyOpenGLWidget::paintGL() {
  glClearColor(background_color_.redF(), background_color_.greenF(),
               background_color_.blueF(), 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  if (max_vertex_ == 0) {
    max_vertex_ = getMaxVertex();
  }

  // Устанавливаем проекцию в зависимости от выбранного типа
  if (projection_type_ == 0) {  // Перспективная проекция
    glFrustum(-max_vertex_, max_vertex_, -max_vertex_, max_vertex_, max_vertex_,
              max_vertex_ * 1000.0f);
    glTranslatef(0.0f, 0.0f, -2 * max_vertex_);
  } else {  // Ортогональная проекция
    glOrtho(-max_vertex_, max_vertex_, -max_vertex_, max_vertex_, -max_vertex_,
            1000.0f * max_vertex_);
    glTranslatef(0.0f, -max_vertex_ / 2, 0.0f);
  }

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  // Применяем перемещение
  glTranslatef(object_translate_.x, object_translate_.y, object_translate_.z);

  // Применяем вращение
  glRotatef(object_rotation_.x, 1.0f, 0.0f, 0.0f);
  glRotatef(object_rotation_.y, 0.0f, 1.0f, 0.0f);
  glRotatef(object_rotation_.z, 0.0f, 0.0f, 1.0f);

  // Применяем масштабирование
  glScalef(object_scale_, object_scale_, object_scale_);

  glEnableClientState(GL_VERTEX_ARRAY);
  const auto &vertices =
      controller_->getVertices();  // Получаем вершины через контроллер
  glVertexPointer(3, GL_DOUBLE, 0, vertices.data());

  if (vertex_display_type_ != 0) {
    paintVertex();
  }
  paintEdges();

  glDisableClientState(GL_VERTEX_ARRAY);
}

void s21::MyOpenGLWidget::updateGL() { paintGL(); }

float s21::MyOpenGLWidget::getMaxVertex() {
  const auto &vertices =
      controller_->getVertices();  // Получаем вершины через контроллер
  float max_vertex = 0.0f;
  for (const auto &vertex : vertices) {
    max_vertex = std::max(max_vertex, static_cast<float>(fabs(vertex.x)));
    max_vertex = std::max(max_vertex, static_cast<float>(fabs(vertex.y)));
    max_vertex = std::max(max_vertex, static_cast<float>(fabs(vertex.z)));
  }
  return max_vertex;
}

void s21::MyOpenGLWidget::paintVertex() {
  if (vertex_display_type_ == 1) {
    glEnable(GL_POINT_SMOOTH);
  }
  if (vertex_display_type_ == 2) {
    glDisable(GL_POINT_SMOOTH);
  }
  glPointSize(vertices_size_);
  glColor3f(vertex_color_.redF(), vertex_color_.greenF(),
            vertex_color_.blueF());
  const auto &vertices =
      controller_->getVertices();  // Получаем вершины через контроллер
  glDrawArrays(GL_POINTS, 0, vertices.size());
}

void s21::MyOpenGLWidget::paintEdges() {
  glLineWidth(edges_thickness_);
  glColor3f(edge_color_.redF(), edge_color_.greenF(), edge_color_.blueF());
  if (edges_type_ == 0) {
    paintSolidEdges();
  } else {
    paintDashedEdges();
  }
}

void s21::MyOpenGLWidget::paintSolidEdges() {
  glBegin(GL_LINES);
  const auto &edges =
      controller_->getEdges();  // Получаем грани через контроллер
  const auto &vertices =
      controller_->getVertices();  // Получаем вершины через контроллер
  for (const auto &edge : edges) {
    for (size_t j = 0; j < edge.vertexIndex.size() - 1; ++j) {
      int index1 = edge.vertexIndex[j];
      int index2 = edge.vertexIndex[j + 1];

      // Проверяем, что индексы вершин корректны
      if (index1 >= 0 && index1 < vertices.size() && index2 >= 0 &&
          index2 < vertices.size()) {
        Vertex vertex1 = vertices[index1];
        Vertex vertex2 = vertices[index2];

        glVertex3d(vertex1.x, vertex1.y, vertex1.z);
        glVertex3d(vertex2.x, vertex2.y, vertex2.z);
      }
    }
  }
  glEnd();
}

void s21::MyOpenGLWidget::paintDashedEdges() {
  glPushAttrib(GL_ENABLE_BIT);
  glLineStipple(1, 0x00FF);
  glEnable(GL_LINE_STIPPLE);

  glBegin(GL_LINES);
  const auto &edges =
      controller_->getEdges();  // Получаем грани через контроллер
  const auto &vertices =
      controller_->getVertices();  // Получаем вершины через контроллер
  for (const auto &edge : edges) {
    for (size_t j = 0; j < edge.vertexIndex.size() - 1; ++j) {
      int index1 = edge.vertexIndex[j];
      int index2 = edge.vertexIndex[j + 1];

      Vertex vertex1 = vertices[index1];
      Vertex vertex2 = vertices[index2];

      glVertex3d(vertex1.x, vertex1.y, vertex1.z);
      glVertex3d(vertex2.x, vertex2.y, vertex2.z);
    }
  }
  glEnd();

  glPopAttrib();
}

// Добавляем обработку событий мыши
void s21::MyOpenGLWidget::mousePressEvent(QMouseEvent *event) {
  if (event->button() == Qt::LeftButton) {
    last_mouse_position_ = event->pos();
  }
}

void s21::MyOpenGLWidget::mouseMoveEvent(QMouseEvent *event) {
  if (event->buttons() & Qt::LeftButton) {
    // Вычисляем разницу в положении мыши
    int dx = event->pos().x() - last_mouse_position_.x();
    int dy = event->pos().y() - last_mouse_position_.y();

    // Обновляем углы поворота
    object_rotation_.y += dx;  // Вращение вокруг оси Y
    object_rotation_.x += dy;  // Вращение вокруг оси X

    // Ограничиваем углы поворота в пределах от -180 до 180 градусов
    if (object_rotation_.x > 180) object_rotation_.x -= 360;
    if (object_rotation_.x < -180) object_rotation_.x += 360;
    if (object_rotation_.y > 180) object_rotation_.y -= 360;
    if (object_rotation_.y < -180) object_rotation_.y += 360;

    // Обновляем последнюю позицию мыши
    last_mouse_position_ = event->pos();

    // Перерисовываем виджет
    update();
  }
}

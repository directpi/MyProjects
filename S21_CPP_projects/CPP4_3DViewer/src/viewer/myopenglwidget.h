#ifndef MYOPENGLWIDGET_H
#define MYOPENGLWIDGET_H

#define GL_SILENCE_DEPRECATION  // Подавляем предупреждения об устаревании

#include <QColor>
#include <QMouseEvent>
#include <QOpenGLFunctions>
#include <QOpenGLWidget>
#include <QWheelEvent>

#include "../controller/controller.hpp"  // Заголовок контроллера
#include "../controller/strategies.hpp"  // Заголовок для MoveStrategy

namespace s21 {

class MyOpenGLWidget : public QOpenGLWidget, protected QOpenGLFunctions {
  Q_OBJECT

 public:
  explicit MyOpenGLWidget(QWidget *parent = nullptr);
  ~MyOpenGLWidget();

  // Метод для установки контроллера
  void setController(ObjModelController *controller);

  void setObjectScale(const double &value);
  void setObjectTranslateX(const double &value);
  void setObjectTranslateY(const double &value);
  void setObjectTranslateZ(const double &value);
  void setObjectRotationX(const double &value);
  void setObjectRotationY(const double &value);
  void setObjectRotationZ(const double &value);
  void setVertexColor(const QColor &color);
  void setEdgeColor(const QColor &color);
  void setBackgroundColor(const QColor &color);
  void setEdgesType(const int &value);
  void setEdgesThickness(const int &value);
  void setVerticesSize(const int &value);
  void setVertexDisplayType(const int &value);
  void setProjectionType(const int &value);

  QColor getVertexColor();
  QColor getEdgeColor();
  QColor getBackgroundColor();
  int getVerticesSize();
  int getEdgesThickness();
  int getProjectionType();
  int getVertexDisplayType();
  int getEdgesType();
  double getObjectScale();
  Vertex getObjectTranslate();
  Vertex getObjectRotation();
  float getMaxVertex();
  void resetTransformationValues();
  double getCurrentPositionX();

 public slots:
  void onUpdate();

 protected:
  void initializeGL() override;
  void resizeGL(int w, int h) override;
  void paintGL() override;
  void resizeEvent(QResizeEvent *event) override;
  void mousePressEvent(QMouseEvent *event) override;
  void mouseMoveEvent(QMouseEvent *event) override;

 private:
  void paintVertex();
  void paintEdges();
  void paintSolidEdges();
  void paintDashedEdges();

  void updateGL();

  float x_camera_rotation_;
  float y_camera_rotation_;
  double object_scale_;
  Vertex object_translate_;
  Vertex object_rotation_;
  float max_vertex_;
  QColor vertex_color_;
  QColor edge_color_;
  QColor background_color_;
  int edges_type_;
  int edges_thickness_;
  int vertices_size_;
  int vertex_display_type_;
  int projection_type_;
  QPoint last_mouse_position_;
  ObjModelController *controller_;  // Теперь это указатель
  MoveStrategy *moveStrategy_;  // Указатель на объект MoveStrategy
};

}  // namespace s21

#endif  // MYOPENGLWIDGET_H

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QColorDialog>
#include <QFileDialog>
#include <QMainWindow>
#include <QMessageBox>
#include <QObject>
#include <QSettings>
#include <QTimer>

#include "../controller/controller.hpp"  // Заголовок контроллера
#include "myopenglwidget.h"
#include "qgifimage.h"

using namespace s21;
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  explicit MainWindow(s21::ObjModelController &controller,
                      QWidget *parent = nullptr);
  ~MainWindow();

 private slots:
  void on_file_pushButton_clicked();
  void on_bgc_pushButton_clicked();
  void on_edge_c_pushButton_clicked();
  void on_vertex_c_pushButton_clicked();
  void updateColorForButton(QPushButton *button, MyOpenGLWidget *openGLWidget);
  void saveImage(const QString &format, const QString &filter,
                 const QString &extension);
  void on_jpg_pushButton_clicked();
  void on_bmp_pushButton_clicked();
  void on_gif_pushButton_clicked();
  void captureFrame();

  void on_none_radioButton_clicked();
  void on_circle_radioButton_clicked();
  void on_square_radioButton_clicked();
  void on_solid_radioButton_clicked();
  void on_dashed_radioButton_clicked();
  void on_persp_batton_clicked();
  void on_parall_batton_clicked();

  void on_x_Slider_move_valueChanged(int value);
  void on_spinBox_x_move_valueChanged(int value);
  void updateTranslationX(int value);
  void on_mx_pushButton_move_clicked();
  void on_px_pushButton_move_clicked();

  void on_y_Slider_move_valueChanged(int value);
  void on_spinBox_y_move_valueChanged(int value);
  void updateTranslationY(int value);
  void on_my_pushButton_move_clicked();
  void on_py_pushButton_move_clicked();

  void on_z_Slider_move_valueChanged(int value);
  void on_spinBox_z_move_valueChanged(int value);
  void updateTranslationZ(int value);
  void on_mz_pushButton_move_clicked();
  void on_pz_pushButton_move_clicked();

  void on_x_Slider_rotate_valueChanged(int value);
  void on_spinBox_x_rotate_valueChanged(int value);
  void updateRotationX(int value);
  void on_mx_pushButton_rotate_clicked();
  void on_px_pushButton_rotate_clicked();

  void on_y_Slider_rotate_valueChanged(int value);
  void on_spinBox_y_rotate_valueChanged(int value);
  void updateRotationY(int value);
  void on_my_pushButton_rotate_clicked();
  void on_py_pushButton_rotate_clicked();

  void on_z_Slider_rotate_valueChanged(int value);
  void on_spinBox_z_rotate_valueChanged(int value);
  void updateRotationZ(int value);
  void on_mz_pushButton_rotate_clicked();
  void on_pz_pushButton_rotate_clicked();

  void on_Slider_scale_valueChanged(int value);
  void on_spinBox_scale_valueChanged(int value);
  void on_min_pushButton_scale_clicked();
  void on_plus_pushButton_scale_clicked();

  void on_vertex_Slider_valueChanged(int value);
  void on_spinBox_vertex_valueChanged(int value);
  void on_m_pushButton_vertex_clicked();
  void on_p_pushButton_vertex_clicked();

  void on_edge_Slider_valueChanged(int value);
  void on_spinBox_edge_valueChanged(int value);
  void on_m_pushButton_edge_clicked();
  void on_p_pushButton_edge_clicked();

  void on_exit_pushButton_clicked();
  void on_reset_transform_pushButton_clicked();
  void reloadFile(const QString &fileName);

 private:
  void resetSliders();
  void saveSettings();
  void loadSetting();
  void loadVertexColor(const QSettings &settings);
  void loadEdgeColor(const QSettings &settings);
  void loadBGColor(const QSettings &settings);
  void loadSizeAndThickness(const QSettings &settings);
  void loadProjectionType(const QSettings &settings);
  void loadEdgesType(const QSettings &settings);
  void loadVertexDisplayType(const QSettings &settings);
  void initSliders();

  // // Добавляем объявление метода normalizeModel
  // void normalizeModel(ObjModel &model);

 private:
  Ui::MainWindow *ui;
  MyOpenGLWidget *openGLWidget;
  s21::ObjModelController *m_controller;  // Экземпляр контроллера

  QString currentFile;
  QGifImage gif;
  QTimer *recordTimer;
  QVector<QImage> frames;
  int frameCount;
};
#endif  // MAINWINDOW_H

#include "mainwindow.h"

#include "ui_mainwindow.h"

MainWindow::MainWindow(ObjModelController &controller, QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow),
      recordTimer(new QTimer(this)),
      frameCount(0) {
  ui->setupUi(this);
  setFixedSize(this->width(), this->height());
  connect(recordTimer, &QTimer::timeout, this, &MainWindow::captureFrame);
  // Передача контроллера в MyOpenGLWidget
  ui->openGLWidget->setController(&controller);
  m_controller = &controller;
  initSliders();
  loadSetting();
}

MainWindow::~MainWindow() {
  saveSettings();
  delete ui;
}

void MainWindow::initSliders() {
  // Слайдеры для перемещения
  ui->x_Slider_move->setRange(-50, 50);
  ui->x_Slider_move->setValue(0);
  ui->spinBox_x_move->setRange(-50, 50);
  ui->spinBox_x_move->setValue(0);
  connect(ui->x_Slider_move, SIGNAL(valueChanged(int)), this,
          SLOT(on_x_Slider_move_valueChanged(int)));
  connect(ui->spinBox_x_move, SIGNAL(valueChanged(int)), this,
          SLOT(on_spinBox_x_move_valueChanged(int)));
  // В MainWindow:
  connect(ui->x_Slider_move, &QSlider::valueChanged, ui->openGLWidget,
          &MyOpenGLWidget::onUpdate);

  ui->y_Slider_move->setRange(-50, 50);
  ui->y_Slider_move->setValue(0);
  ui->spinBox_y_move->setRange(-50, 50);
  ui->spinBox_y_move->setValue(0);
  connect(ui->y_Slider_move, SIGNAL(valueChanged(int)), this,
          SLOT(on_y_Slider_move_valueChanged(int)));
  connect(ui->spinBox_y_move, SIGNAL(valueChanged(int)), this,
          SLOT(on_spinBox_y_move_valueChanged(int)));
  // В MainWindow:
  connect(ui->y_Slider_move, &QSlider::valueChanged, ui->openGLWidget,
          &MyOpenGLWidget::onUpdate);

  ui->z_Slider_move->setRange(-50, 50);
  ui->z_Slider_move->setValue(0);
  ui->spinBox_z_move->setRange(-50, 50);
  ui->spinBox_z_move->setValue(0);
  connect(ui->z_Slider_move, SIGNAL(valueChanged(int)), this,
          SLOT(on_z_Slider_move_valueChanged(int)));
  connect(ui->spinBox_z_move, SIGNAL(valueChanged(int)), this,
          SLOT(on_spinBox_z_move_valueChanged(int)));
  // В MainWindow:
  connect(ui->z_Slider_move, &QSlider::valueChanged, ui->openGLWidget,
          &MyOpenGLWidget::onUpdate);

  // Слайдеры для вращения
  ui->x_Slider_rotate->setRange(-180, 180);
  ui->x_Slider_rotate->setValue(0);
  ui->spinBox_x_rotate->setRange(-180, 180);
  ui->spinBox_x_rotate->setValue(0);
  connect(ui->x_Slider_rotate, SIGNAL(valueChanged(int)), this,
          SLOT(on_x_Slider_rotate_valueChanged(int)));
  connect(ui->spinBox_x_rotate, SIGNAL(valueChanged(int)), this,
          SLOT(on_spinBox_x_rotate_valueChanged(int)));

  ui->y_Slider_rotate->setRange(-180, 180);
  ui->y_Slider_rotate->setValue(0);
  ui->spinBox_y_rotate->setRange(-180, 180);
  ui->spinBox_y_rotate->setValue(0);
  connect(ui->y_Slider_rotate, SIGNAL(valueChanged(int)), this,
          SLOT(on_y_Slider_rotate_valueChanged(int)));
  connect(ui->spinBox_y_rotate, SIGNAL(valueChanged(int)), this,
          SLOT(on_spinBox_y_rotate_valueChanged(int)));

  ui->z_Slider_rotate->setRange(-180, 180);
  ui->z_Slider_rotate->setValue(0);
  ui->spinBox_z_rotate->setRange(-180, 180);
  ui->spinBox_z_rotate->setValue(0);
  connect(ui->z_Slider_rotate, SIGNAL(valueChanged(int)), this,
          SLOT(on_z_Slider_rotate_valueChanged(int)));
  connect(ui->spinBox_z_rotate, SIGNAL(valueChanged(int)), this,
          SLOT(on_spinBox_z_rotate_valueChanged(int)));

  // Слайдеры для масштабирования
  ui->Slider_scale->setRange(50, 150);
  ui->Slider_scale->setValue(100);
  ui->spinBox_scale->setRange(50, 150);
  ui->spinBox_scale->setValue(100);
  connect(ui->Slider_scale, SIGNAL(valueChanged(int)), this,
          SLOT(on_Slider_scale_valueChanged(int)));
  connect(ui->spinBox_scale, SIGNAL(valueChanged(int)), this,
          SLOT(on_spinBox_scale_valueChanged(int)));
}

void MainWindow::on_file_pushButton_clicked() {
  QSettings settings("TKP", "3DViewer");
  settings.clear();
  settings.sync();

  QString fileName = QFileDialog::getOpenFileName(this, tr("Open 3D Model"), "",
                                                  tr("OBJ Files (*.obj)"));
  reloadFile(fileName);
}

void MainWindow::updateColorForButton(QPushButton *button,
                                      MyOpenGLWidget *openGLWidget) {
  QColor initialColor = button->palette().color(QPalette::Button);
  QColor color = QColorDialog::getColor(initialColor, this, "Select Color");

  if (color.isValid()) {
    button->setStyleSheet("");
    QPalette pal = button->palette();
    pal.setColor(QPalette::Button, color);
    button->setPalette(pal);
    button->setAutoFillBackground(true);

    if (button == ui->edge_c_pushButton) {
      openGLWidget->setEdgeColor(color);
    } else if (button == ui->vertex_c_pushButton) {
      openGLWidget->setVertexColor(color);
    } else if (button == ui->bgc_pushButton) {
      openGLWidget->setBackgroundColor(color);
    }
    button->update();
  }
}

void MainWindow::on_bgc_pushButton_clicked() {
  updateColorForButton(ui->bgc_pushButton, ui->openGLWidget);
  update();
}

void MainWindow::on_edge_c_pushButton_clicked() {
  updateColorForButton(ui->edge_c_pushButton, ui->openGLWidget);
  update();
}

void MainWindow::on_vertex_c_pushButton_clicked() {
  updateColorForButton(ui->vertex_c_pushButton, ui->openGLWidget);
  update();
}

void MainWindow::resetSliders() {
  ui->x_Slider_move->setValue(0);
  ui->y_Slider_move->setValue(0);
  ui->z_Slider_move->setValue(0);

  ui->x_Slider_rotate->setValue(0);
  ui->y_Slider_rotate->setValue(0);
  ui->z_Slider_rotate->setValue(0);

  ui->Slider_scale->setValue(100);
}

void MainWindow::saveSettings() {
  QSettings settings("TKP", "3DViewer");
  MyOpenGLWidget *openGLWidget =
      dynamic_cast<MyOpenGLWidget *>(ui->openGLWidget);

  if (openGLWidget) {
    QColor vertex_color = openGLWidget->getVertexColor();
    settings.setValue("vertexColorRed", vertex_color.redF());
    settings.setValue("vertexColorGreen", vertex_color.greenF());
    settings.setValue("vertexColorBlue", vertex_color.blueF());

    QColor edge_color = openGLWidget->getEdgeColor();
    settings.setValue("edgeColorRed", edge_color.redF());
    settings.setValue("edgeColorGreen", edge_color.greenF());
    settings.setValue("edgeColorBlue", edge_color.blueF());

    QColor background_color = openGLWidget->getBackgroundColor();
    settings.setValue("backgroundColorRed", background_color.redF());
    settings.setValue("backgroundColorGreen", background_color.greenF());
    settings.setValue("backgroundColorBlue", background_color.blueF());

    double vertices_size = openGLWidget->getVerticesSize();
    double edges_thickness = openGLWidget->getEdgesThickness();
    settings.setValue("verticesSize", vertices_size);
    settings.setValue("edgeThickness", edges_thickness);

    int projection_type = openGLWidget->getProjectionType();
    int edges_type = openGLWidget->getEdgesType();
    int vertex_display_type = openGLWidget->getVertexDisplayType();
    settings.setValue("projectionType", projection_type);
    settings.setValue("edgesType", edges_type);
    settings.setValue("vertexDisplayType", vertex_display_type);
  } else {
    qWarning("Unable to cast openGLWidget to MyOpenGLWidget.");
  }
}

void MainWindow::loadSetting() {
  QSettings settings("TKP", "3DViewer");
  loadVertexColor(settings);
  loadEdgeColor(settings);
  loadBGColor(settings);
  loadSizeAndThickness(settings);
  loadProjectionType(settings);
  loadEdgesType(settings);
  loadVertexDisplayType(settings);
}

void MainWindow::loadVertexColor(const QSettings &settings) {
  QColor vertex_color;
  vertex_color.setRedF(settings.value("vertexColorRed").toFloat());
  vertex_color.setGreenF(settings.value("vertexColorGreen").toFloat());
  vertex_color.setBlueF(settings.value("vertexColorBlue").toFloat());
  ui->openGLWidget->setVertexColor(vertex_color);
  ui->vertex_c_pushButton->setStyleSheet("background-color: " +
                                         vertex_color.name());
}

void MainWindow::loadEdgeColor(const QSettings &settings) {
  QColor edge_color;
  edge_color.setRedF(settings.value("edgeColorRed").toFloat());
  edge_color.setGreenF(settings.value("edgeColorGreen").toFloat());
  edge_color.setBlueF(settings.value("edgeColorBlue").toFloat());
  ui->openGLWidget->setEdgeColor(edge_color);
  ui->edge_c_pushButton->setStyleSheet("background-color: " +
                                       edge_color.name());
}

void MainWindow::loadBGColor(const QSettings &settings) {
  QColor background_color;
  background_color.setRedF(settings.value("backgroundColorRed").toFloat());
  background_color.setGreenF(settings.value("backgroundColorGreen").toFloat());
  background_color.setBlueF(settings.value("backgroundColorBlue").toFloat());
  ui->openGLWidget->setBackgroundColor(background_color);
  ui->bgc_pushButton->setStyleSheet("background-color: " +
                                    background_color.name());
}

void MainWindow::loadSizeAndThickness(const QSettings &settings) {
  ui->openGLWidget->setVerticesSize(settings.value("verticesSize").toDouble());
  ui->vertex_Slider->setValue(ui->openGLWidget->getVerticesSize());
  ui->openGLWidget->setEdgesThickness(
      settings.value("edgeThickness").toDouble());
  ui->edge_Slider->setValue(ui->openGLWidget->getEdgesThickness());
}

void MainWindow::loadProjectionType(const QSettings &settings) {
  ui->openGLWidget->setProjectionType(
      settings.value("projectionType", 0).toInt());
  if (ui->openGLWidget->getProjectionType() == 0) {
    ui->persp_batton->setChecked(true);
  } else {
    ui->parall_batton->setChecked(true);
  }
}

void MainWindow::loadEdgesType(const QSettings &settings) {
  ui->openGLWidget->setEdgesType(settings.value("edgesType", 0).toInt());
  if (ui->openGLWidget->getEdgesType() == 0) {
    ui->solid_radioButton->setChecked(true);
  } else {
    ui->dashed_radioButton->setChecked(true);
  }
}

void MainWindow::loadVertexDisplayType(const QSettings &settings) {
  ui->openGLWidget->setVertexDisplayType(
      settings.value("vertexDisplayType").toInt());
  if (ui->openGLWidget->getVertexDisplayType() == 0) {
    ui->none_radioButton->setChecked(true);
  } else if (ui->openGLWidget->getVertexDisplayType() == 1) {
    ui->circle_radioButton->setChecked(true);
  } else {
    ui->square_radioButton->setChecked(true);
  }
}

void MainWindow::saveImage(const QString &format, const QString &filter,
                           const QString &extension) {
  QImage image = ui->openGLWidget->grabFramebuffer();
  QFileDialog dialog(this);
  dialog.setWindowTitle(tr("Save Screen as %1").arg(format));
  dialog.setAcceptMode(QFileDialog::AcceptSave);
  dialog.setDirectory(QDir::currentPath());
  dialog.setNameFilter(filter);
  dialog.setFileMode(QFileDialog::AnyFile);

  if (dialog.exec()) {
    QString fileName = dialog.selectedFiles().first();
    if (!fileName.isEmpty()) {
      if (!fileName.endsWith(extension, Qt::CaseInsensitive)) {
        fileName += extension;
      }
      if (image.save(fileName, format.toLatin1().constData())) {
        QMessageBox::information(this, tr("OK"), tr("Successful saving"));
      } else {
        QMessageBox::critical(this, tr("Failed"),
                              tr("Could not save the image"));
      }
    }
  } else {
    QMessageBox::warning(this, tr("Cancelled"), tr("Save dialog cancelled"));
  }
}

void MainWindow::on_jpg_pushButton_clicked() {
  saveImage("JPEG", tr("JPEG (*.jpeg *.jpg)"), ".jpeg");
}

void MainWindow::on_bmp_pushButton_clicked() {
  saveImage("BMP", tr("BMP (*.bmp)"), ".bmp");
}

void MainWindow::on_gif_pushButton_clicked() {
  QString filePath = QFileDialog::getSaveFileName(this, tr("Save GIF"), "",
                                                  tr("GIF Files (*.gif)"));
  if (filePath.isEmpty()) {
    return;
  }

  if (!filePath.endsWith(".gif", Qt::CaseInsensitive)) {
    filePath += ".gif";
  }
  frames.clear();
  frameCount = 0;

  recordTimer->start(100);

  QTimer::singleShot(5000, this, [this, filePath]() {
    recordTimer->stop();

    gif.setDefaultDelay(100);
    for (const QImage &frame : frames) {
      gif.addFrame(frame);
    }
    gif.save(filePath);
    frames.clear();
  });
}

void MainWindow::captureFrame() {
  QImage frame = ui->openGLWidget->grabFramebuffer();
  if (!frame.isNull()) {
    frames.append(frame);
    frameCount++;
  }
}

void MainWindow::on_none_radioButton_clicked() {
  ui->openGLWidget->setVertexDisplayType(0);
  ui->openGLWidget->update();
}

void MainWindow::on_circle_radioButton_clicked() {
  ui->openGLWidget->setVertexDisplayType(1);
  ui->openGLWidget->update();
}

void MainWindow::on_square_radioButton_clicked() {
  ui->openGLWidget->setVertexDisplayType(2);
  ui->openGLWidget->update();
}

void MainWindow::on_solid_radioButton_clicked() {
  ui->openGLWidget->setEdgesType(0);
  ui->openGLWidget->update();
}

void MainWindow::on_dashed_radioButton_clicked() {
  ui->openGLWidget->setEdgesType(1);
  ui->openGLWidget->update();
}

void MainWindow::on_persp_batton_clicked() {
  ui->openGLWidget->setProjectionType(0);
  ui->openGLWidget->update();
}

void MainWindow::on_parall_batton_clicked() {
  ui->openGLWidget->setProjectionType(1);
  ui->openGLWidget->update();
}

// MOVE X
void MainWindow::updateTranslationX(int value) {
  double translation_value = (value / 100.0);
  ui->openGLWidget->setObjectTranslateX(translation_value);
  ui->openGLWidget->update();
}

void MainWindow::on_x_Slider_move_valueChanged(int value) {
  updateTranslationX(value);
  ui->spinBox_x_move->setValue(value);
}

void MainWindow::on_spinBox_x_move_valueChanged(int value) {
  updateTranslationX(value);
  ui->x_Slider_move->setValue(value);
}

void MainWindow::on_mx_pushButton_move_clicked() {
  int currentValue = ui->spinBox_x_move->value();
  ui->spinBox_x_move->setValue(currentValue - 5);
}

void MainWindow::on_px_pushButton_move_clicked() {
  int currentValue = ui->spinBox_x_move->value();
  ui->spinBox_x_move->setValue(currentValue + 5);
}

// MOVE Y
void MainWindow::updateTranslationY(int value) {
  double translation_value = (value / 100.0);
  ui->openGLWidget->setObjectTranslateY(translation_value);
  ui->openGLWidget->update();
}

void MainWindow::on_y_Slider_move_valueChanged(int value) {
  updateTranslationY(value);
  ui->spinBox_y_move->setValue(value);
}

void MainWindow::on_spinBox_y_move_valueChanged(int value) {
  updateTranslationY(value);
  ui->y_Slider_move->setValue(value);
}

void MainWindow::on_my_pushButton_move_clicked() {
  int currentValue = ui->spinBox_y_move->value();
  ui->spinBox_y_move->setValue(currentValue - 5);
}

void MainWindow::on_py_pushButton_move_clicked() {
  int currentValue = ui->spinBox_y_move->value();
  ui->spinBox_y_move->setValue(currentValue + 5);
}

// MOVE Z
void MainWindow::updateTranslationZ(int value) {
  double translation_value = (value / 100.0);
  ui->openGLWidget->setObjectTranslateZ(translation_value);
  ui->openGLWidget->update();
}

void MainWindow::on_z_Slider_move_valueChanged(int value) {
  updateTranslationZ(value);
  ui->spinBox_z_move->setValue(value);
}

void MainWindow::on_spinBox_z_move_valueChanged(int value) {
  updateTranslationZ(value);
  ui->z_Slider_move->setValue(value);
}

void MainWindow::on_mz_pushButton_move_clicked() {
  int currentValue = ui->spinBox_z_move->value();
  ui->spinBox_z_move->setValue(currentValue - 5);
}

void MainWindow::on_pz_pushButton_move_clicked() {
  int currentValue = ui->spinBox_z_move->value();
  ui->spinBox_z_move->setValue(currentValue + 5);
}

// ROTATE X
void MainWindow::updateRotationX(int value) {
  value = (value + 360) % 360;
  if (value > 180) {
    value -= 360;
  }
  value = value / 2;
  ui->openGLWidget->setObjectRotationX(value);
  ui->openGLWidget->update();
}

void MainWindow::on_spinBox_x_rotate_valueChanged(int value) {
  updateRotationX(value);
  ui->x_Slider_rotate->setValue(value);
}

void MainWindow::on_x_Slider_rotate_valueChanged(int value) {
  updateRotationX(value);
  ui->spinBox_x_rotate->setValue(value);
}

void MainWindow::on_mx_pushButton_rotate_clicked() {
  int currentValue = ui->spinBox_x_rotate->value();
  ui->spinBox_x_rotate->setValue(currentValue - 5);
}

void MainWindow::on_px_pushButton_rotate_clicked() {
  int currentValue = ui->spinBox_x_rotate->value();
  ui->spinBox_x_rotate->setValue(currentValue + 5);
}

// ROTATE Y
void MainWindow::updateRotationY(int value) {
  value = (value + 360) % 360;
  if (value > 180) {
    value -= 360;
  }
  value = value / 2;
  ui->openGLWidget->setObjectRotationY(value);
  ui->openGLWidget->update();
}

void MainWindow::on_y_Slider_rotate_valueChanged(int value) {
  updateRotationY(value);
  ui->spinBox_y_rotate->setValue(value);
}

void MainWindow::on_spinBox_y_rotate_valueChanged(int value) {
  updateRotationY(value);
  ui->y_Slider_rotate->setValue(value);
}

void MainWindow::on_my_pushButton_rotate_clicked() {
  int currentValue = ui->spinBox_y_rotate->value();
  ui->spinBox_y_rotate->setValue(currentValue - 5);
}

void MainWindow::on_py_pushButton_rotate_clicked() {
  int currentValue = ui->spinBox_y_rotate->value();
  ui->spinBox_y_rotate->setValue(currentValue + 5);
}

// ROTATE Z
void MainWindow::updateRotationZ(int value) {
  value = (value + 360) % 360;
  if (value > 180) {
    value -= 360;
  }
  value = value / 2;
  ui->openGLWidget->setObjectRotationZ(value);
  ui->openGLWidget->update();
}

void MainWindow::on_spinBox_z_rotate_valueChanged(int value) {
  updateRotationZ(value);
  ui->z_Slider_rotate->setValue(value);
}

void MainWindow::on_z_Slider_rotate_valueChanged(int value) {
  updateRotationZ(value);
  ui->spinBox_z_rotate->setValue(value);
}

void MainWindow::on_mz_pushButton_rotate_clicked() {
  int currentValue = ui->spinBox_z_rotate->value();
  ui->spinBox_z_rotate->setValue(currentValue - 5);
}

void MainWindow::on_pz_pushButton_rotate_clicked() {
  int currentValue = ui->spinBox_z_rotate->value();
  ui->spinBox_z_rotate->setValue(currentValue + 5);
}

// SCALE
void MainWindow::on_Slider_scale_valueChanged(int value) {
  double scale_value =
      value / 100.0;  // Преобразуем значение слайдера в масштаб
  ui->openGLWidget->setObjectScale(
      scale_value);  // Устанавливаем масштаб в виджете

  ui->openGLWidget->update();          // Обновляем виджет
  ui->spinBox_scale->setValue(value);  // Обновляем значение в SpinBox
}

void MainWindow::on_spinBox_scale_valueChanged(int value) {
  double scale_value = value / 100.0;  // Преобразуем значение SpinBox в масштаб
  ui->openGLWidget->setObjectScale(
      scale_value);  // Устанавливаем масштаб в виджете
  ui->Slider_scale->setValue(value);  // Обновляем значение в слайдере
}

void MainWindow::on_min_pushButton_scale_clicked() {
  int currentValue = ui->spinBox_scale->value();
  ui->spinBox_scale->setValue(currentValue - 10);
}

void MainWindow::on_plus_pushButton_scale_clicked() {
  int currentValue = ui->spinBox_scale->value();
  ui->spinBox_scale->setValue(currentValue + 10);
}

// VERTEX
void MainWindow::on_vertex_Slider_valueChanged(int value) {
  ui->openGLWidget->setVerticesSize(value);
  ui->openGLWidget->update();
  ui->spinBox_vertex->setValue(value);
}

void MainWindow::on_spinBox_vertex_valueChanged(int value) {
  ui->openGLWidget->setVerticesSize(value);
  ui->openGLWidget->update();
  ui->vertex_Slider->setValue(value);
}

void MainWindow::on_m_pushButton_vertex_clicked() {
  int currentValue = ui->spinBox_vertex->value();
  ui->spinBox_vertex->setValue(currentValue - 1);
}

void MainWindow::on_p_pushButton_vertex_clicked() {
  int currentValue = ui->spinBox_vertex->value();
  ui->spinBox_vertex->setValue(currentValue + 1);
}

// EDGE
void MainWindow::on_edge_Slider_valueChanged(int value) {
  ui->openGLWidget->setEdgesThickness(value);
  ui->openGLWidget->update();
  ui->spinBox_edge->setValue(value);
}

void MainWindow::on_spinBox_edge_valueChanged(int value) {
  ui->openGLWidget->setEdgesThickness(value);
  ui->openGLWidget->update();
  ui->edge_Slider->setValue(value);
}

void MainWindow::on_m_pushButton_edge_clicked() {
  int currentValue = ui->spinBox_edge->value();
  ui->spinBox_edge->setValue(currentValue - 1);
}

void MainWindow::on_p_pushButton_edge_clicked() {
  int currentValue = ui->spinBox_edge->value();
  ui->spinBox_edge->setValue(currentValue + 1);
}

// EXIT - RESET
void MainWindow::on_exit_pushButton_clicked() {
  this->close();
  qApp->quit();
}

void MainWindow::on_reset_transform_pushButton_clicked() {
  ui->openGLWidget->resetTransformationValues();
  resetSliders();
  if (!currentFile.isEmpty()) {
    reloadFile(currentFile);
  }
  ui->openGLWidget->update();
}

void MainWindow::reloadFile(const QString &fileName) {
  if (!fileName.isEmpty()) {
    currentFile = fileName;
    ui->openGLWidget->resetTransformationValues();
    resetSliders();

    QFileInfo fileInfo(fileName);
    ui->file_lineEdit->setText(fileName);
    ui->file_name_lineEdit->setText(fileInfo.fileName());

    QLocale currentLocale = QLocale::system();
    QLocale::setDefault(QLocale::C);

    // Очищаем предыдущую модель
    m_controller->freeModel();

    // Загружаем новую модель
    loadObjModel(fileName.toStdString());

    // Нормализация модели
    m_controller->normalize();  // Вызываем метод нормализации через контроллер

    ui->count_lineEdit_edge->setText(
        QString::number(m_controller->getNumEdges()));
    ui->count_lineEdit_vertex->setText(
        QString::number(m_controller->getNumVertices()));

    double scale_value = 1.0;

    ui->openGLWidget->setObjectScale(scale_value);

    ui->openGLWidget->update();  // Обновляем виджет

    QLocale::setDefault(currentLocale);
  }
}

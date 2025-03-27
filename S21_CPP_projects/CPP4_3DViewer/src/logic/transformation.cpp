#include "model.hpp"

namespace s21 {

/// @brief Changes coordinates so, the figure is about to move relative to 0,0,0
/// at dx, dy, dz values
/// @param dx
/// @param dy
/// @param dz
void ObjModel::moveModel(double dx, double dy, double dz) {
  tr.moveVector[0] = dx;
  tr.moveVector[1] = dy;
  tr.moveVector[2] = dz;
  fillTransformMatrix();
  transposeMatrix(tr.transformationMatrix);

  for (auto &vertex : vertices) {
    std::vector<double> transformed(4, 0);
    for (int j = 0; j < 4; ++j) {
      for (int k = 0; k < 4; ++k) {
        double value =
            k < 3 ? (k == 0 ? vertex.x : (k == 1 ? vertex.y : vertex.z)) : 1.0;
        transformed[j] += value * tr.transformationMatrix[k][j];
      }
    }
    vertex.x = transformed[0];
    vertex.y = transformed[1];
    vertex.z = transformed[2];
  }
}

/// @brief Scales figure relatively, current figure scale is always 1.0
/// @param scale e.g. 1.1 means increase coordinates values so, the figure is
/// bigger at 10%
void ObjModel::scaleModel(double scale) {
  if (scale <= 0) {
    throw std::invalid_argument("Scale factor must be greater than 0");
  }
  // Если новый масштаб отличается от текущего (с учетом погрешности)
  if (std::abs(scale - currentScale) >= 1e-6) {
    double tempScale = scale / currentScale;
    currentScale = scale;

    for (auto &vertex : vertices) {
      vertex.x *= tempScale;
      vertex.y *= tempScale;
      vertex.z *= tempScale;
    }
  }
}

/// @brief Changes coordinates values so, the figure center is 0,0,0 and max
/// crd no more than 0.5
void ObjModel::normalizeModel() {
  if (getNumVertices() == 0) return;

  double xCenter = (maxMin.minX + maxMin.maxX) / 2.0;
  double yCenter = (maxMin.minY + maxMin.maxY) / 2.0;
  double zCenter = (maxMin.minZ + maxMin.maxZ) / 2.0;

  // Находим максимальный размер модели
  double maxDimension =
      std::max(maxMin.maxX - maxMin.minX,
               std::max(maxMin.maxY - maxMin.minY, maxMin.maxZ - maxMin.minZ));

  // Нормализуем модель
  for (auto &vertex : vertices) {
    vertex.x = (vertex.x - xCenter) / maxDimension;
    vertex.y = (vertex.y - yCenter) / maxDimension;
    vertex.z = (vertex.z - zCenter) / maxDimension;
  }
}

/// @brief Changes coordinates so, the figure is about to rotate relative to
/// 0,0,0 at alphaX, alphaYy, alphaZ values. Alpha - should be set as degree
/// @param alphaX
/// @param alphaY
/// @param alphaZ
void ObjModel::rotateModel(double alphaX, double alphaY, double alphaZ) {
  alphaX = alphaX * M_PI / 180.0;
  alphaY = alphaY * M_PI / 180.0;
  alphaZ = alphaZ * M_PI / 180.0;

  fillRotationMatrix(tr.rotationMatrixX, alphaX, 0);
  fillRotationMatrix(tr.rotationMatrixY, alphaY, 1);
  fillRotationMatrix(tr.rotationMatrixZ, alphaZ, 2);

  applyRotation(tr.rotationMatrixX);
  applyRotation(tr.rotationMatrixY);
  applyRotation(tr.rotationMatrixZ);
}

/// @brief fills transformation matrix according to set move_vector values
void ObjModel::fillTransformMatrix() {
  // Инициализация единичной матрицы 4x4
  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 4; ++j) {
      tr.transformationMatrix[i][j] = (i == j ? 1.0 : 0.0);
    }
  }
  // Добавляем значения вектора перемещения
  tr.transformationMatrix[0][3] = tr.moveVector[0];
  tr.transformationMatrix[1][3] = tr.moveVector[1];
  tr.transformationMatrix[2][3] = tr.moveVector[2];
}

/// @brief Transposes a square matrix, result rewrites the original
/// @param matrix
void ObjModel::transposeMatrix(std::array<std::array<double, 4>, 4> &matrix) {
  // Транспонирование матрицы
  for (size_t i = 0; i < 4; ++i) {
    for (size_t j = i + 1; j < 4; ++j) {
      std::swap(matrix[i][j], matrix[j][i]);
    }
  }
}

/// @brief Fills rotation matrixes
/// @param matrix
/// @param alpha radians
/// @param axis 0 - X, 1 - Y, 2 - Z
void ObjModel::fillRotationMatrix(std::array<std::array<double, 3>, 3> &matrix,
                                  double alpha, int axis) {
  // Инициализация единичной матрицы
  for (int i = 0; i < 3; ++i) {
    for (int j = 0; j < 3; ++j) {
      matrix[i][j] = (i == j ? 1.0 : 0.0);
    }
  }
  // Проверка корректности значения оси вращения
  if (axis < 0 || axis > 2) {
    throw std::invalid_argument(
        "Invalid axis for rotation. Expected 0, 1, or 2.");
  }
  // Установка элементов матрицы для соответствующей оси вращения
  if (axis == 0) {  // Вращение вокруг оси X
    matrix[1][1] = std::cos(alpha);
    matrix[1][2] = -std::sin(alpha);
    matrix[2][1] = std::sin(alpha);
    matrix[2][2] = std::cos(alpha);
  } else if (axis == 1) {  // Вращение вокруг оси Y
    matrix[0][0] = std::cos(alpha);
    matrix[0][2] = std::sin(alpha);
    matrix[2][0] = -std::sin(alpha);
    matrix[2][2] = std::cos(alpha);
  } else if (axis == 2) {  // Вращение вокруг оси Z
    matrix[0][0] = std::cos(alpha);
    matrix[0][1] = -std::sin(alpha);
    matrix[1][0] = std::sin(alpha);
    matrix[1][1] = std::cos(alpha);
  }
}

/// @brief Fills rotation matrixes
/// @param rotationMatrix
void ObjModel::applyRotation(
    const std::array<std::array<double, 3>, 3> &rotationMatrix) {
  for (auto &vertex : vertices) {
    // Используем временные переменные для расчёта новых координат
    double tempX = rotationMatrix[0][0] * vertex.x +
                   rotationMatrix[0][1] * vertex.y +
                   rotationMatrix[0][2] * vertex.z;
    double tempY = rotationMatrix[1][0] * vertex.x +
                   rotationMatrix[1][1] * vertex.y +
                   rotationMatrix[1][2] * vertex.z;
    double tempZ = rotationMatrix[2][0] * vertex.x +
                   rotationMatrix[2][1] * vertex.y +
                   rotationMatrix[2][2] * vertex.z;

    // Обновляем координаты вершины
    vertex.x = tempX;
    vertex.y = tempY;
    vertex.z = tempZ;
  }
}
}  // namespace s21

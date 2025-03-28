#ifndef MODEL_HPP
#define MODEL_HPP

#include <algorithm>
#include <array>
#include <cmath>
#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <memory>  // Для std::unique_ptr
#include <sstream>
#include <stdexcept>  // Для исключений
#include <vector>

#define ERR 1
#define OK 0
#define START_SCALE 1

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

namespace s21 {

typedef struct Vertex {
  double x;
  double y;
  double z;
} Vertex;

typedef struct Polygon {
  std::vector<int> vertexIndex;  // Вектор для хранения вершин
  int amountPoly;                // Количество граней
} Polygon;

typedef struct MaxMin {
  double maxX;
  double maxY;
  double maxZ;
  double minX;
  double minY;
  double minZ;
} MaxMin;

typedef struct Angle {
  double alphaX;
  double alphaY;
  double alphaZ;
} Angle;

#include <vector>

typedef struct tMath {
  std::array<std::array<double, 4>, 4> transformationMatrix;
  std::array<std::array<double, 3>, 3> rotationMatrixX;
  std::array<std::array<double, 3>, 3> rotationMatrixY;
  std::array<std::array<double, 3>, 3> rotationMatrixZ;
  std::array<double, 3> moveVector;
} tMath;

class ObjModel {  // Singleton class
 private:
  // Поля класса
  std::vector<Vertex> vertices;  // Вектор для хранения вершин
  std::vector<Polygon> edges;  // Вектор для хранения граней
  size_t numVertices;          // Количество вершин
  size_t numEdges;             // Количество граней
  MaxMin maxMin;  // максимальные и минимальные значения координат
  Angle a;  // углы поворота относительно осей координат
  double currentScale;  // текущий масштаб
  tMath tr;  // структура для хранения матриц

  // Приватный конструктор модели
  ObjModel()
      : vertices(),
        edges(),
        numVertices(0),
        numEdges(0),
        currentScale(START_SCALE) {
    maxMin = {0, 0, 0, 0, 0, 0};
    a = {0, 0, 0};
    // Инициализация единичных матриц
    tr.transformationMatrix = {
        {{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}}};
    tr.rotationMatrixX = {{{1, 0, 0}, {0, 1, 0}, {0, 0, 1}}};
    tr.rotationMatrixY = tr.rotationMatrixX;  // Копируем единичную матрицу
    tr.rotationMatrixZ = tr.rotationMatrixX;
    tr.moveVector = {0, 0, 0};
  }

  // Запрещаем копирование и присваивание
  ObjModel(const ObjModel &) = delete;
  ObjModel &operator=(const ObjModel &) = delete;

  void fillTransformMatrix();
  void transposeMatrix(std::array<std::array<double, 4>, 4> &matrix);
  void fillRotationMatrix(std::array<std::array<double, 3>, 3> &matrix,
                          double alpha, int axis);
  void applyRotation(
      const std::array<std::array<double, 3>, 3> &rotationMatrix);

  // std::vector<Observer *> observers;  // Список наблюдателей

 public:
  ~ObjModel() { freeModel(); };

  // Метод для получения экземпляра
  static ObjModel &getInstance() {
    static ObjModel instance;
    return instance;
  }

  // Методы класса

  // Геттеры

  std::vector<Vertex> &getVertices() { return vertices; }
  std::vector<Polygon> &getEdges() { return edges; }
  size_t getNumVertices() { return numVertices; }
  size_t getNumEdges() { return numEdges; }
  MaxMin getMaxMin() { return maxMin; }
  Angle getAngle() { return a; }
  double getCurrentScale() { return currentScale; }
  std::array<std::array<double, 4>, 4> &getTransformationMatrix() {
    return tr.transformationMatrix;
  }
  std::array<std::array<double, 3>, 3> &getRotationMatrixX() {
    return tr.rotationMatrixX;
  }
  std::array<std::array<double, 3>, 3> &getRotationMatrixY() {
    return tr.rotationMatrixY;
  }
  std::array<std::array<double, 3>, 3> &getRotationMatrixZ() {
    return tr.rotationMatrixZ;
  }
  std::array<double, 3> &getMoveVector() { return tr.moveVector; }

  // Сеттеры

  void setVertices(std::vector<Vertex> &v) { vertices = v; }
  void setEdges(std::vector<Polygon> &e) { edges = e; }
  void setNumVertices(size_t value) { numVertices = value; }
  void setNumEdges(size_t value) { numEdges = value; }
  void setMaxMin(const MaxMin newMaxMin) { maxMin = newMaxMin; }
  void setAngle(const Angle &newAngle) { a = newAngle; }
  void setCurrentScale(double newScale) { currentScale = newScale; }

  void setTransformationMatrix(
      const std::array<std::array<double, 4>, 4> &matrix) {
    tr.transformationMatrix = matrix;
  }

  void setRotationMatrixX(const std::array<std::array<double, 3>, 3> &matrix) {
    tr.rotationMatrixX = matrix;
  }

  void setRotationMatrixY(const std::array<std::array<double, 3>, 3> &matrix) {
    tr.rotationMatrixY = matrix;
  }

  void setRotationMatrixZ(const std::array<std::array<double, 3>, 3> &matrix) {
    tr.rotationMatrixZ = matrix;
  }

  void setMoveVector(const std::array<double, 3> &vector) {
    tr.moveVector = vector;
  }

  void incrementNumVertices() { ++numVertices; }
  void incrementNumEdges() { ++numEdges; }
  void decrementNumEdges() { --numEdges; };

  // Функции преобразований модели

  void moveModel(double dx, double dy, double dz);
  void scaleModel(double scale);
  void rotateModel(double alphaX, double alphaY, double alphaZ);
  void normalizeModel();

  void freeModel();

  // Новый метод для получения грани по индексу
  Polygon getFacet(size_t index) const {
    if (index < edges.size()) {
      return edges[index];
    }
    throw std::out_of_range("Index out of range");
  }
  Vertex getVertexByIndex(size_t index) const {
    if (index < vertices.size()) {
      return vertices[index];
    } else {
      throw std::out_of_range("Vertex index out of range");
    }
  }
};

// Функция чтения файла и функции парсинга вершин и граней

void loadObjModel(std::string(filename));
void removeComments(std::string &line);
void parseVertices(std::string &line);
void parseEdges(const std::string &line);
void checkEdgePattern(const int *numVal, int *pattern, int curPattern,
                      int *priznak);
void finalizeEdge(Polygon *currentEdge);
void validateEdgeData(const std::string &line, Polygon *currentEdge,
                      bool isEdgeValid, ObjModel &model);
void addVertexToEdge(ObjModel &model, Polygon *&currentEdge, bool &isEdgeValid,
                     int v);
bool parseVertexToken(const std::string &token, int &v, int &vn, int &vt,
                      int &numVal, int &pattern, int &priznak);
void processEdgeLine(const std::string &line, ObjModel &model,
                     Polygon *&currentEdge, bool &isEdgeValid);

}  // namespace s21
#endif  // MODEL_HPP

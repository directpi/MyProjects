#include "model.hpp"

namespace s21 {

/// @brief loops over a file line by line, detects and parses vertices and edges
/// @param filename name of file
void loadObjModel(std::string(filename)) {
  std::ifstream file(filename);  // Используем ifstream для открытия файла
  if (!file.is_open()) {
    throw std::runtime_error("Не удалось открыть файл: " +
                             std::string(filename));
  }

  std::string line;
  while (std::getline(file, line)) {  // Чтение файла построчно
    removeComments(line);

    if (line.substr(0, 2) == "v ") {  // Если строка начинается с "v "
      parseVertices(line);  // Парсим вершины
    } else if (line.substr(0, 2) == "f ") {  // Если строка начинается с "f "
      parseEdges(line);                      // Парсим грани
    }
  }
  file.close();
}

/// @brief if a line consists of a comment - removes comment
/// @param line file line as a string
void removeComments(std::string &line) {
  size_t commentPos = line.find('#');
  if (commentPos != std::string::npos) {
    line.erase(commentPos);  // Удаляем все, начиная с символа '#'
  }
}

/// @brief clear vectors, free matrices, set default values to class fields
void ObjModel::freeModel() {
  // Очистка вектора граней
  if (!edges.empty()) {
    edges.clear();
    numEdges = 0;
  }
  // Очистка вектора вершин
  if (!vertices.empty()) {
    vertices.clear();
    numVertices = 0;
  }
  // Освобождение памяти для матриц (зануление элементов)
  for (auto &row : tr.transformationMatrix) {
    row.fill(0.0);
  }
  for (auto &row : tr.rotationMatrixX) {
    row.fill(0.0);
  }
  for (auto &row : tr.rotationMatrixY) {
    row.fill(0.0);
  }
  for (auto &row : tr.rotationMatrixZ) {
    row.fill(0.0);
  }
  tr.moveVector.fill(0.0);
  // Задаем значения по умолчанию
  maxMin = {};
  a = {};
  currentScale = START_SCALE;
}

/// @brief reads vertices from line and saves them to Vertices
/// @param line file line as a string
void parseVertices(std::string &line) {
  ObjModel &model = ObjModel::getInstance();

  // Инициализация координат
  double xCrd = 0, yCrd = 0, zCrd = 0, wCrd = 0;
  int priznak = 0;

  // Разбиваем строку на подстроки по пробелам
  std::istringstream stream(line);
  std::string token;
  int numVal = 0;

  // Читаем строку токен за токеном
  while (stream >> token && priznak != -1) {
    if (numVal == 1 && sscanf(token.c_str(), "%lf", &xCrd) == 1) {
      priznak++;
    } else if (numVal == 2 && sscanf(token.c_str(), "%lf", &yCrd) == 1) {
      priznak++;
    } else if (numVal == 3 && sscanf(token.c_str(), "%lf", &zCrd) == 1) {
      priznak++;
    } else if (numVal == 4 && sscanf(token.c_str(), "%lf", &wCrd) == 1) {
      // Для четвертой координаты ничего не делаем, но она может быть
    } else if (numVal > 0) {
      priznak = -1;  // Если больше 4 токенов, ошибка
    }
    numVal++;
  }

  // Если все координаты найдены, добавляем вершину
  if (priznak == 3) {
    Vertex vertex = {xCrd, yCrd, zCrd};
    model.getVertices().push_back(vertex);
    model.incrementNumVertices();

    // Обновляем min/max значения
    if (model.getNumVertices() == 1) {
      model.setMaxMin({xCrd, yCrd, zCrd, xCrd, yCrd, zCrd});
    } else {
      MaxMin currentMaxMin = model.getMaxMin();
      double temp_maxX = std::max(xCrd, currentMaxMin.maxX);
      double temp_maxY = std::max(yCrd, currentMaxMin.maxY);
      double temp_maxZ = std::max(zCrd, currentMaxMin.maxZ);
      double temp_minX = std::min(xCrd, currentMaxMin.minX);
      double temp_minY = std::min(yCrd, currentMaxMin.minY);
      double temp_minZ = std::min(zCrd, currentMaxMin.minZ);

      model.setMaxMin(
          {temp_maxX, temp_maxY, temp_maxZ, temp_minX, temp_minY, temp_minZ});
    }
  } else {
    // Если не удалось правильно распарсить координаты, выбрасываем исключение
    throw std::invalid_argument("Invalid vertex data: " + line);
  }
}

/// @brief reads polygons from line and saves them to Edges
/// @param line file line as a string
void parseEdges(const std::string &line) {
  if (line.empty()) {
    throw std::invalid_argument("Empty input line");
  }

  ObjModel &model = ObjModel::getInstance();

  Polygon *currentEdge = nullptr;
  bool isEdgeValid = false;

  processEdgeLine(line, model, currentEdge, isEdgeValid);
  validateEdgeData(line, currentEdge, isEdgeValid, model);
  finalizeEdge(currentEdge);
}

/// @brief run parseVertexToken and addVertexToEdge
/// @param line file line as a string
/// @param model singlton instance of the class
/// @param currentEdge current edge
/// @param isEdgeValid true if edge valid
void processEdgeLine(const std::string &line, ObjModel &model,
                     Polygon *&currentEdge, bool &isEdgeValid) {
  std::istringstream stream(line);
  std::string token;

  int v = 0, vn = 0, vt = 0, priznak = 0;
  int pattern = 0;
  int numVal = 0;

  while (stream >> token && priznak != -1) {
    priznak = 0;

    if (numVal > 0) {
      if (!parseVertexToken(token, v, vn, vt, numVal, pattern, priznak)) {
        throw std::invalid_argument("Error in edge format: " + line);
      }

      if (v < 1) priznak = -1;

      if (priznak == 1) {
        addVertexToEdge(model, currentEdge, isEdgeValid, v);
      } else if (priznak == -1) {
        if (currentEdge != nullptr && !currentEdge->vertexIndex.empty()) {
          currentEdge->vertexIndex.clear();
        }
        throw std::invalid_argument("Error in edge format: " + line);
      }
    }
    numVal++;
  }
}

/// @brief detect and parse edge pattern
/// @param token sequences of characters
/// @param v value for vertex
/// @param vn value for vertex normal
/// @param vt value for texture vertex
/// @param numVal count vertices in polygon
/// @param pattern pattern of polygon
/// @param priznak false or true
/// @return true or false
bool parseVertexToken(const std::string &token, int &v, int &vn, int &vt,
                      int &numVal, int &pattern, int &priznak) {
  if (sscanf(token.c_str(), "%d/%d/%d", &v, &vn, &vt) == 3) {
    checkEdgePattern(&numVal, &pattern, 1, &priznak);
  } else if (sscanf(token.c_str(), "%d/%d", &v, &vt) == 2) {
    checkEdgePattern(&numVal, &pattern, 2, &priznak);
  } else if (sscanf(token.c_str(), "%d//%d", &v, &vn) == 2) {
    checkEdgePattern(&numVal, &pattern, 3, &priznak);
  } else if (sscanf(token.c_str(), "%d", &v) == 1) {
    checkEdgePattern(&numVal, &pattern, 4, &priznak);
  } else {
    priznak = -1;
  }
  return priznak == -1 ? false : true;
}

/// @brief add vertex to edge
/// @param model singlton instance of the class
/// @param currentEdge current edge
/// @param isEdgeValid true or false
/// @param v vertex value from token
void addVertexToEdge(ObjModel &model, Polygon *&currentEdge, bool &isEdgeValid,
                     int v) {
  if (!isEdgeValid) {
    model.getEdges().emplace_back();
    model.incrementNumEdges();
    currentEdge = &model.getEdges().back();
    currentEdge->amountPoly = 0;
    isEdgeValid = true;
  }
  currentEdge->vertexIndex.push_back(v - 1);
  currentEdge->amountPoly++;
}

/// @brief
/// @param line file line as a string
/// @param currentEdge current edge
/// @param isEdgeValid true or false
/// @param model singlton instance of the class
void validateEdgeData(const std::string &line, Polygon *currentEdge,
                      bool isEdgeValid, ObjModel &model) {
  if (!isEdgeValid) {
    throw std::invalid_argument("Invalid edge data: " + line);
  }

  if (currentEdge != nullptr && currentEdge->vertexIndex.empty()) {
    model.getEdges().pop_back();
    model.decrementNumEdges();
  }
}

/// @brief copy first vertex to end and finalize edge
/// @param currentEdge current edge
void finalizeEdge(Polygon *currentEdge) {
  if (currentEdge != nullptr && currentEdge->amountPoly > 1) {
    currentEdge->vertexIndex.push_back(currentEdge->vertexIndex.front());
    currentEdge->amountPoly++;
  }
}

/// @brief find errors in patterns
/// @param numVal count of vertex
/// @param pattern new pattern
/// @param curPattern current pattern
/// @param priznak  =-1 if error
void checkEdgePattern(const int *numVal, int *pattern, int curPattern,
                      int *priznak) {
  if (*numVal > 1 && *pattern != curPattern) {
    *priznak = -1;
  } else {
    *pattern = curPattern;
    (*priznak)++;
  }
}
}  // namespace s21
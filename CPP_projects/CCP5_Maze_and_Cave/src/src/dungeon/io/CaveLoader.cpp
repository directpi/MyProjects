#include "../../../include/dungeon/io/CaveLoader.h"

#include <fstream>
#include <iostream>
#include <sstream>

std::unique_ptr<Dungeon> CaveLoader::load(const std::string& filepath) {
  CaveInfo info;
  std::ifstream stream;
  stream.open(filepath);

  if (!stream) {
    throw std::runtime_error("Filed to open file");
  }

  std::string firstStr;
  if (!getline(stream, firstStr)) {
    throw std::runtime_error("Error, first line is missing or file is empty");
  }

  int rows, cols;

  std::istringstream strStream(firstStr);
  if (!(strStream >> rows >> cols)) {
    throw std::runtime_error("Error reading cave dimensions");
  }

  if (char extraChar; strStream >> extraChar) {
    throw std::runtime_error("Error, extra characters in the first line");
  }

  if (rows < 1 || rows > 50 || cols < 1 || cols > 50) {
    throw std::runtime_error("Error, wrong cave dimensions");
  }

  auto cave = std::make_unique<Cave>(rows, cols);

  info.matrix.resize(rows);
  for (int i = 0; i < rows; i++) {
    info.matrix[i].resize(cols);
    for (int j = 0; j < cols; j++) {
      if (!(stream >> info.matrix[i][j])) {
        throw std::runtime_error("Error reading matrix data");
      }
    }
  }

  if (!stream.eof()) {
    throw std::runtime_error("Error, wrong file");
  }

  cave->setMatrix(info.matrix);

  stream.close();

  return cave;
}
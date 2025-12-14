#include "../../../include/dungeon/io/MazeLoader.h"

#include <fstream>
#include <sstream>

std::unique_ptr<Dungeon> MazeLoader::load(const std::string& filepath) {
  std::ifstream inputFile(filepath);
  if (!inputFile.is_open()) {
    throw std::runtime_error("Filed to open file");
  }

  size_t rows, cols;
  std::string first_line;

  if (!std::getline(inputFile, first_line)) {
    throw std::runtime_error("Error reading matrix size");
  }
  std::stringstream ss(first_line);

  if (!(ss >> rows >> cols)) {
    throw std::runtime_error("Error reading matrix size");
  }

  auto maze = std::make_unique<Maze>(rows, cols);

  Matrix& rightWalls = maze->getRightWalls();
  Matrix& downWalls = maze->getDownWalls();

  for (size_t i = 0; i < rows; ++i) {
    for (size_t j = 0; j < cols; ++j) {
      if (!(inputFile >> rightWalls(i, j))) {
        throw std::runtime_error("Error reading matrix data");
      }
    }
  }

  if (inputFile.eof()) {
    throw std::runtime_error("Error, wrong file");
  }

  for (size_t i = 0; i < rows; ++i) {
    for (size_t j = 0; j < cols; ++j) {
      if (!(inputFile >> downWalls(i, j))) {
        throw std::runtime_error("Error reading matrix data");
      }
    }
  }

  return maze;
}
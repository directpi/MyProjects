#include "../../../include/dungeon/io/MazeSaver.h"

#include <fstream>
#include <iostream>

#include "../../../include/dungeon/core/Maze.h"

void MazeSaver::save(const Dungeon& dungeon, const std::string& filepath) {
  const auto maze_ptr = dynamic_cast<const Maze*>(&dungeon);

  if (!maze_ptr) {
    std::cerr
        << "Ошибка: MazeSaver может сохранять только объекты типа Maze.\n";
    return;
  }

  std::ofstream outputFile(filepath);
  if (!outputFile.is_open()) {
    std::cerr << "Ошибка: не удалось открыть файл для записи: " << filepath
              << "\n";
    return;
  }
  outputFile << maze_ptr->getRows() << " " << maze_ptr->getCols() << "\n\n";

  const Matrix& rightWalls = maze_ptr->getRightWalls();
  for (size_t i = 0; i < maze_ptr->getRows(); ++i) {
    for (size_t j = 0; j < maze_ptr->getCols(); ++j) {
      outputFile << rightWalls(i, j)
                 << (j == maze_ptr->getCols() - 1 ? "" : " ");
    }
    outputFile << "\n";
  }
  outputFile << "\n";

  const Matrix& downWalls = maze_ptr->getDownWalls();
  for (size_t i = 0; i < maze_ptr->getRows(); ++i) {
    for (size_t j = 0; j < maze_ptr->getCols(); ++j) {
      outputFile << downWalls(i, j)
                 << (j == maze_ptr->getCols() - 1 ? "" : " ");
    }
    outputFile << "\n";
  }
}
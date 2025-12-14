#include "../../../include/dungeon/core/DungeonFactory.h"

#include "../../../include/dungeon/core/Cave.h"
#include "../../../include/dungeon/core/Maze.h"

std::unique_ptr<Dungeon> DungeonFactory::createDungeon(const DungeonType type,
                                                       size_t rows,
                                                       size_t cols) {
  std::unique_ptr<Dungeon> dungeon = nullptr;

  if (type == MAZE) {
    auto maze = std::make_unique<Maze>(rows, cols);
    maze->generateMaze();
    dungeon = std::move(maze);
  }
  // else if (type == CAVE) {
  //   auto cave = std::make_unique<Cave>(rows, cols);
  //   cave->generateCave();
  //   dungeon = std::move(cave);
  // }

  return dungeon;
}

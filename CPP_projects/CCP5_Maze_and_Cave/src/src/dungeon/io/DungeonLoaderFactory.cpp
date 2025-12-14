#include "../../../include/dungeon/io/DungeonLoaderFactory.h"

#include "../../../include/dungeon/io/MazeLoader.h"

std::unique_ptr<DungeonLoader> DungeonLoaderFactory::createLoader(
    const DungeonType type) {
  if (type == MAZE) {
    return std::make_unique<MazeLoader>();
  }
  if (type == CAVE) {
    return std::make_unique<CaveLoader>();
  }

  return nullptr;
}
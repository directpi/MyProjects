#include "../../../include/dungeon/io/DungeonSaverFactory.h"

#include "../../../include/dungeon/core/Maze.h"
#include "../../../include/dungeon/io/MazeSaver.h"

std::unique_ptr<DungeonSaver> DungeonSaverFactory::createSaver(
    const Dungeon& dungeonToSave) {
  if (dynamic_cast<const Maze*>(&dungeonToSave)) {
    return std::make_unique<MazeSaver>();
  }
  return nullptr;
}
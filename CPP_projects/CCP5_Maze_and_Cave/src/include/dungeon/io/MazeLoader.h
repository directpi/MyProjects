#ifndef MAZELOADER_H
#define MAZELOADER_H

#include "../core/Maze.h"
#include "DungeonLoader.h"

class MazeLoader final : public DungeonLoader {
 public:
  std::unique_ptr<Dungeon> load(const std::string& filepath) override;
};

#endif  // MAZELOADER_H

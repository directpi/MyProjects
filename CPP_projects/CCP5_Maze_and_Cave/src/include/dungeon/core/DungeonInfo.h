#ifndef DUNGEONINFO_H
#define DUNGEONINFO_H

#include <string>
#include <variant>
#include <vector>

#include "DungeonType.h"

typedef struct {
  const DungeonType type = DungeonType::MAZE;
  std::vector<std::vector<int>> rightWall;
  std::vector<std::vector<int>> downWall;
} MazeInfo;

typedef struct {
  const DungeonType type = DungeonType::CAVE;
  std::vector<std::vector<int>> matrix;
} CaveInfo;

using DungeonInfo = std::variant<MazeInfo, CaveInfo>;

#endif
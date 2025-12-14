#ifndef DUNGEON_H
#define DUNGEON_H

#include "DungeonInfo.h"

class Dungeon {
 public:
  virtual ~Dungeon() = default;
  virtual DungeonInfo getInfo() = 0;
};

#endif
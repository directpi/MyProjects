#ifndef DUNGEONFACTORY_H
#define DUNGEONFACTORY_H

#include <memory>

#include "Dungeon.h"
#include "DungeonType.h"

class DungeonFactory {
 public:
  static std::unique_ptr<Dungeon> createDungeon(DungeonType type, size_t rows,
                                                size_t cols);
};

#endif
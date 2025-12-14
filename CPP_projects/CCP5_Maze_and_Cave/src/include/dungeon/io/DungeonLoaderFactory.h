#ifndef DUNGEONLOADERFACTORY_H
#define DUNGEONLOADERFACTORY_H

#include <memory>

#include "../core/DungeonType.h"
#include "CaveLoader.h"
#include "DungeonLoader.h"

class DungeonLoaderFactory {
 public:
  static std::unique_ptr<DungeonLoader> createLoader(DungeonType type);
};

#endif  // DUNGEONLOADERFACTORY_H

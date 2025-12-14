#ifndef DUNGEONSAVERFACTORY_H
#define DUNGEONSAVERFACTORY_H

#include <memory>

#include "DungeonLoaderFactory.h"
#include "DungeonSaver.h"

class DungeonSaverFactory {
 public:
  static std::unique_ptr<DungeonSaver> createSaver(
      const Dungeon& dungeonToSave);
};
#endif  // DUNGEONSAVERFACTORY_H

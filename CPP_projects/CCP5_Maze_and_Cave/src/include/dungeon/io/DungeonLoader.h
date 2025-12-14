#ifndef DUNGEONLOADER_H
#define DUNGEONLOADER_H

#include <memory>
#include <string>

#include "../core/Dungeon.h"

class DungeonLoader {
 public:
  virtual ~DungeonLoader() = default;
  virtual std::unique_ptr<Dungeon> load(const std::string& source) = 0;
};

#endif  // DUNGEONLOADER_H

#ifndef DUNGEONSAVER_H
#define DUNGEONSAVER_H

#include <string>

#include "../core/Dungeon.h"

class DungeonSaver {
 public:
  virtual ~DungeonSaver() = default;

  virtual void save(const Dungeon& dungeon, const std::string& filepath) = 0;
};

#endif
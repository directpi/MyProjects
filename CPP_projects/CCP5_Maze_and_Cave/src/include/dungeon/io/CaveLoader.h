#ifndef CAVELOADER_H
#define CAVELOADER_H

#include <memory>

#include "../core/Cave.h"
#include "DungeonLoader.h"

class CaveLoader final : public DungeonLoader {
 public:
  std::unique_ptr<Dungeon> load(const std::string& filepath) override;
};

#endif  // CAVELOADER_H

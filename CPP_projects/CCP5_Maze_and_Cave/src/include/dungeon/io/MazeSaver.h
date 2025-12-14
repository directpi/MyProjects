#ifndef MAZESAVER_H
#define MAZESAVER_H

#include "DungeonSaver.h"

class MazeSaver final : public DungeonSaver {
 public:
  void save(const Dungeon& dungeon, const std::string& filepath) override;
};

#endif  // MAZESAVER_H

#ifndef DUNGEONPATHFINDER_H
#define DUNGEONPATHFINDER_H

#include <optional>
#include <vector>

#include "../../common/Point.h"

class DungeonPathfinder {
 public:
  virtual ~DungeonPathfinder() = default;

  virtual std::optional<std::vector<std::pair<int, int>>> findPath(
      std::pair<int, int> start, std::pair<int, int> end) = 0;
};
#endif  // DUNGEONPATHFINDER_H

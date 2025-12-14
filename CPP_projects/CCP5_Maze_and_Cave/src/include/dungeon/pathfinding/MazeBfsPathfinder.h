#ifndef MAZEBFSPATHFINDER_H
#define MAZEBFSPATHFINDER_H

#include "../core/Maze.h"
#include "DungeonPathfinder.h"

class MazeBfsPathfinder final : public DungeonPathfinder {
 public:
  explicit MazeBfsPathfinder(const Dungeon& maze);

  std::optional<std::vector<std::pair<int, int>>> findPath(
      std::pair<int, int> start, std::pair<int, int> end) override;

 private:
  const Maze* maze;
};

#endif  // MAZEBFSPATHFINDER_H

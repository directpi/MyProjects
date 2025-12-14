#ifndef A1_MAZE_CPP_ID_1391674_TEAM_TL_HARKONEX_5C2B27DF_8751_4660_1_API_H
#define A1_MAZE_CPP_ID_1391674_TEAM_TL_HARKONEX_5C2B27DF_8751_4660_1_API_H

#include "../../include/dungeon/core/DungeonFactory.h"
#include "../dungeon/core/Cave.h"
#include "../dungeon/pathfinding/MLAgent.h"

class Game {
 public:
  Game() = default;
  ~Game() = default;
  static std::unique_ptr<Game> create() { return std::make_unique<Game>(); }
  DungeonInfo getGameInfo() const;
  Dungeon* getDungeon() const { return dungeon.get(); }
  void createDungeons(DungeonType type, size_t rows, size_t cols);
  void createDungeons(DungeonType type, const std::string& fileName);
  std::vector<std::pair<int, int>> getPath(
      const std::pair<int, int>& start, const std::pair<int, int>& end) const;
  void saveToFile(const std::string& fileName) const;
  void createCaveWithParams(size_t rows, size_t cols, int birthLimit,
                            int deathLimit, int chance);
  void updateCaveMatrix(const std::vector<std::vector<int>>& newMatrix) const;
  bool stepByStep() const;
  std::vector<std::pair<int, int>> runMLAgent(
      const std::pair<int, int>& start,
      const std::pair<int, int>& finish) const;

  Game(const Game&) = delete;
  Game& operator=(const Game&) = delete;

 private:
  std::unique_ptr<Dungeon> dungeon;
  DungeonType dungeonType{};
};

#endif  // A1_MAZE_CPP_ID_1391674_TEAM_TL_HARKONEX_5C2B27DF_8751_4660_1_API_H

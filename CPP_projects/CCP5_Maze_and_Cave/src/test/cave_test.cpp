#include <vector>

#include "../include/api/API.h"
#include "test.h"

TEST(testCave, cave1) {
  const auto game = Game::create();
  game->createCaveWithParams(10, 10, 4, 3, 50);
  const auto cave = dynamic_cast<Cave *>(game->getDungeon());
  const std::vector<std::vector<int>> matrix = {
      {1, 0, 1, 0, 0, 0, 0, 1, 1, 0}, {0, 0, 1, 1, 0, 0, 0, 0, 0, 1},
      {0, 0, 1, 0, 1, 0, 1, 1, 0, 1}, {0, 1, 1, 1, 1, 1, 1, 0, 0, 0},
      {0, 0, 0, 1, 1, 0, 0, 1, 1, 1}, {0, 1, 0, 1, 0, 1, 0, 0, 0, 0},
      {1, 1, 0, 0, 0, 0, 0, 1, 0, 0}, {0, 0, 0, 0, 0, 0, 1, 0, 1, 1},
      {1, 0, 0, 0, 0, 1, 1, 0, 0, 0}, {0, 1, 1, 0, 0, 1, 1, 0, 0, 0}};

  cave->setMatrix(matrix);
  cave->setDelay(0);
  const auto delay = cave->getDelay();
  cave->setDelay(delay);

  while (cave->generateCell()) {
  }

  const auto dungeon = game->getGameInfo();
  const auto &caveMatrix = std::get<CaveInfo>(dungeon);

  const std::vector<std::vector<int>> result = {
      {1, 1, 1, 1, 1, 1, 1, 1, 1, 1}, {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
      {1, 1, 1, 1, 1, 1, 1, 1, 1, 1}, {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
      {1, 1, 1, 1, 1, 1, 0, 0, 1, 1}, {1, 1, 0, 0, 0, 0, 0, 0, 0, 1},
      {1, 0, 0, 0, 0, 0, 0, 0, 0, 1}, {1, 0, 0, 0, 0, 0, 0, 0, 1, 1},
      {1, 1, 0, 0, 0, 1, 1, 1, 1, 1}, {1, 1, 1, 1, 1, 1, 1, 1, 1, 1}};

  EXPECT_EQ(caveMatrix.matrix, result);
}
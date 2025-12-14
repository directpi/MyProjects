#include "../include/dungeon/core/Maze.h"

#include <vector>

#include "../include/api/API.h"
#include "test.h"

TEST(testMaze, MLAgent1) {
  const auto game = Game::create();
  game->createDungeons(MAZE, "files/maze_1.txt");
  const auto path = game->runMLAgent({0, 0}, {9, 9});

  const std::vector<std::pair<int, int>> result = {
      {0, 0}, {0, 1}, {0, 2}, {1, 2}, {2, 2}, {2, 1}, {3, 1}, {3, 2}, {3, 3},
      {4, 3}, {4, 4}, {3, 4}, {3, 5}, {2, 5}, {2, 4}, {2, 3}, {1, 3}, {0, 3},
      {0, 4}, {1, 4}, {1, 5}, {1, 6}, {1, 7}, {2, 7}, {3, 7}, {3, 8}, {2, 8},
      {1, 8}, {0, 8}, {0, 9}, {1, 9}, {2, 9}, {3, 9}, {4, 9}, {5, 9}, {6, 9},
      {6, 8}, {5, 8}, {5, 7}, {6, 7}, {6, 6}, {6, 5}, {7, 5}, {7, 6}, {8, 6},
      {9, 6}, {9, 7}, {9, 8}, {9, 9}};

  EXPECT_EQ(path, result);
}

TEST(testMaze, maze1) {
  const auto game = Game::create();
  game->createDungeons(MAZE, 10, 10);
  auto *maze = dynamic_cast<Maze *>(game->getDungeon());
  maze->generateMaze();
  auto info = game->getGameInfo();
}
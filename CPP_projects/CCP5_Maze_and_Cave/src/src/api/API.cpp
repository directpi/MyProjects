#include "../../include/api/API.h"

#include "../../include/dungeon/io/DungeonSaverFactory.h"
#include "../../include/dungeon/pathfinding/MazeBfsPathfinder.h"

void Game::createDungeons(const DungeonType type, const size_t rows,
                          const size_t cols) {
  this->dungeonType = type;
  this->dungeon = DungeonFactory::createDungeon(type, rows, cols);
}

void Game::createDungeons(const DungeonType type, const std::string& fileName) {
  this->dungeonType = type;
  this->dungeon = DungeonLoaderFactory::createLoader(type)->load(fileName);
}

std::vector<std::pair<int, int>> Game::getPath(
    const std::pair<int, int>& start, const std::pair<int, int>& end) const {
  MazeBfsPathfinder pathfinder(*(dungeon.get()));
  return pathfinder.findPath(start, end).value();
}

void Game::saveToFile(const std::string& fileName) const {
  DungeonSaverFactory::createSaver(*(dungeon.get()))
      .get()
      ->save(*(dungeon.get()), fileName);
}

DungeonInfo Game::getGameInfo() const { return this->dungeon->getInfo(); }

void Game::createCaveWithParams(size_t rows, size_t cols, const int birthLimit,
                                const int deathLimit, const int chance) {
  this->dungeonType = CAVE;
  auto cave = std::make_unique<Cave>(rows, cols);
  cave->setLimitBirth(birthLimit);
  cave->setLimitDeath(deathLimit);
  cave->setChanceBirth(chance);

  // Генерируем пещеру с текущими параметрами
  const auto caveMatrix = cave->generateCave();
  cave->setMatrix(caveMatrix);

  this->dungeon = std::move(cave);
}

void Game::updateCaveMatrix(
    const std::vector<std::vector<int>>& newMatrix) const {
  if (dungeon && dungeonType == CAVE) {
    if (auto* cave = dynamic_cast<Cave*>(dungeon.get())) {
      cave->setMatrix(newMatrix);
    }
  }
}

bool Game::stepByStep() const {
  auto* cave = dynamic_cast<Cave*>(dungeon.get());

  if (!cave) {
    throw std::runtime_error("Cave object is null");
  }

  return cave->generateCell();
}

std::vector<std::pair<int, int>> Game::runMLAgent(
    const std::pair<int, int>& start, const std::pair<int, int>& finish) const {
  auto* maze = dynamic_cast<Maze*>(dungeon.get());

  if (!maze) {
    throw std::runtime_error("Maze object is null");
  }

  MLAgent mlAgent(maze->getRows(), maze->getCols(), maze);
  mlAgent.trainAgent(start, finish);
  auto path = mlAgent.findPath(start, finish);

  return path;
}

#ifndef CAVE_H
#define CAVE_H

#include <vector>

#include "Dungeon.h"

class Cave final : public Dungeon {
 public:
  Cave(int rows, int cols);

  void setLimitBirth(int limit);
  void setLimitDeath(int limit);
  void setChanceBirth(int value);
  void setDelay(int value);

  void setMatrix(const std::vector<std::vector<int>> &matrix_);
  int getDelay() const;
  bool generateCell();
  bool hasChanges() const { return lastChanges; }

  std::vector<std::vector<int>> generateCave();
  DungeonInfo getInfo() override;

 private:
  int limitBirth;
  int limitDeath;
  int rows;
  int cols;
  int chanceBirth;
  int delay;
  std::vector<std::vector<int>> matrix;
  bool lastChanges = false;  // ✅ Флаг изменений
};

#endif  // CAVE_H

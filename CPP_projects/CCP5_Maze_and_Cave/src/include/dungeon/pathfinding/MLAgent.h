#ifndef MLAGENT_H
#define MLAGENT_H

#include <vector>

#include "../core/Maze.h"

#define ALPHA 0.1f
#define GAMMA 0.9f
#define NUM_EPISODES 7500
#define MAX_STEPS 3000
#define MAX_ACTIONS 4

class MLAgent {
 public:
  int numStates;
  int numActions;
  std::vector<std::vector<float>> qTable;

  MLAgent(const int rows, const int cols, Maze* m)
      : numStates(rows * cols),
        numActions(MAX_ACTIONS),
        qTable(numStates, std::vector<float>(numActions, 0.0)),
        maze(m) {}

  void trainAgent(const std::pair<int, int>& start,
                  const std::pair<int, int>& end);
  std::vector<std::pair<int, int>> findPath(
      const std::pair<int, int>& start, const std::pair<int, int>& end) const;

 private:
  Maze* maze;

  static float calculateDistance(int row, int col, int goalRow, int goalCol);
  int selectAction(int state, float epsilon) const;
  int getNextState(size_t state, size_t action, int& nextRow,
                   int& nextCol) const;
  void updateQValue(int state, int action, int nextState, float reward,
                    float distanceToGoal);
  float getReward(int nextState, int endRow, int endCol,
                  const std::vector<int>& visited) const;
};

#endif

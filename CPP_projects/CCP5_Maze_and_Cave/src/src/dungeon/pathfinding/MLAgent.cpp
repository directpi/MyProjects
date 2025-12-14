#include "../../../include/dungeon/pathfinding/MLAgent.h"

#include <complex>

#include "../../../include/api/API.h"
#include "../../../include/common/TemplateMatrix.h"
#include "../../../include/common/Utils.h"

float MLAgent::calculateDistance(const int row, const int col,
                                 const int goalRow, const int goalCol) {
  return std::sqrt((goalRow - row) * (goalRow - row) +
                   (goalCol - col) * (goalCol - col));
}

int MLAgent::selectAction(const int state, const float epsilon) const {
  if (state < 0 || state >= numStates) {
    throw std::invalid_argument("Invalid state");
  }

  // С вероятностью epsilon выбираем случайное действие
  if (const auto rand = generateRandomFloat(0.0f, 1.0f); rand < epsilon) {
    return generateRandomInt(0, 3);
  }

  // Иначе выбираем действие с максимальным Q-значением
  float maxValue = -std::numeric_limits<float>::max();
  int bestAction = -1;

  for (int action = 0; action < numActions; ++action) {
    int nextRow, nextCol;
    int nextState = getNextState(state, action, nextRow, nextCol);

    if (nextState != -1) {
      if (qTable[state][action] > maxValue) {
        maxValue = qTable[state][action];
        bestAction = action;
      }
    }
  }

  // Если все действия недопустимы, выбираем случайное
  if (bestAction == -1) {
    bestAction = generateRandomInt(0, 3);
  }

  return bestAction;
}

int MLAgent::getNextState(const size_t state, const size_t action, int& nextRow,
                          int& nextCol) const {
  size_t row = state / maze->getCols();
  size_t col = state % maze->getCols();
  Matrix& downWalls = maze->getDownWalls();
  Matrix& rightWalls = maze->getRightWalls();

  switch (action) {
    case 0:  // Вверх
      if (row > 0 && !downWalls(row - 1, col)) {
        --row;
      }
      break;
    case 1:  // Вправо
      if (col < maze->getCols() - 1 && !rightWalls(row, col)) {
        ++col;
      }
      break;
    case 2:  // Вниз
      if (row < maze->getRows() - 1 && !downWalls(row, col)) {
        ++row;
      }
      break;
    case 3:  // Влево
      if (col > 0 && !rightWalls(row, col - 1)) {
        --col;
      }
      break;
    default:
      return -1;
  }

  nextRow = static_cast<int>(row);
  nextCol = static_cast<int>(col);

  return static_cast<int>(row * maze->getCols() + col);
}

float MLAgent::getReward(const int nextState, const int endRow,
                         const int endCol,
                         const std::vector<int>& visited) const {
  const int nextRow = static_cast<int>(nextState / maze->getCols());
  const int nextCol = static_cast<int>(nextState % maze->getCols());

  if (nextRow == endRow && nextCol == endCol) {
    return 100.0f;  // Большое вознаграждение за достижение цели
  } else if (visited[nextState] > 0) {
    return -10.0f * static_cast<float>(
                        visited[nextState]);  // Штраф за повторное посещение
  } else {
    return -1.0f;  // Штраф за обычное движение
  }
}

void MLAgent::updateQValue(const int state, const int action,
                           const int nextState, const float reward,
                           const float distanceToGoal) {
  float maxNextQValue = qTable[nextState][0];
  for (int a = 1; a < numActions; ++a) {
    if (qTable[nextState][a] > maxNextQValue) {
      maxNextQValue = qTable[nextState][a];
    }
  }

  // Добавляем дополнительную награду за приближение к цели
  const float adjustedReward = reward - distanceToGoal;

  // Используем уравнение Беллмана
  qTable[state][action] +=
      ALPHA * (adjustedReward + GAMMA * maxNextQValue - qTable[state][action]);
}

void MLAgent::trainAgent(const std::pair<int, int>& start,
                         const std::pair<int, int>& end) {
  numStates = static_cast<int>(maze->getRows() * maze->getCols());

  for (int episode = 0; episode < NUM_EPISODES; ++episode) {
    int state = static_cast<int>(start.first * maze->getCols() + start.second);

    // Массив для отслеживания посещенных состояний
    std::vector<int> visited(numStates, 0);

    for (int step = 0; step < MAX_STEPS; ++step) {
      // Увеличиваем счетчик посещений для текущего состояния
      visited[state] += 1;
      const float epsilon =
          0.5f * std::exp(-static_cast<float>(episode) / 1000.0f);
      const int action = selectAction(state, epsilon);

      if (action < 0 || action >= MAX_ACTIONS) {
        continue;
      }

      int nextRow = -1, nextCol = -1;
      const int nextState = getNextState(state, action, nextRow, nextCol);

      if (nextState == -1) {
        continue;
      }

      const float reward = getReward(nextState, end.first, end.second, visited);
      const float distanceToGoal =
          calculateDistance(nextRow, nextCol, end.first, end.second);

      // Обновление Q-значения с учетом награды за приближение к цели
      updateQValue(state, action, nextState, reward, distanceToGoal);

      state = nextState;

      // Проверка достижения цели
      if (nextRow == end.first && nextCol == end.second) {
        break;
      }
    }
  }
}

std::vector<std::pair<int, int>> MLAgent::findPath(
    const std::pair<int, int>& start, const std::pair<int, int>& end) const {
  std::vector<std::pair<int, int>> path;
  path.push_back(start);

  int state = static_cast<int>(start.first * maze->getCols() + start.second);
  const int endState =
      static_cast<int>(end.first * maze->getCols() + end.second);

  // Проход по шагам для поиска кратчайшего пути
  for (int step = 0; step < MAX_STEPS && state != endState; ++step) {
    // Выбираем действие с максимальным Q-значением (ε = 0)
    const int action = selectAction(state, 0.0f);

    int nextRow, nextCol;
    int nextState = getNextState(state, action, nextRow, nextCol);

    if (nextState != -1) {
      path.emplace_back(nextRow, nextCol);

      // Переход в следующее состояние
      state = nextState;
    } else {
      break;
    }
  }

  return path;
}
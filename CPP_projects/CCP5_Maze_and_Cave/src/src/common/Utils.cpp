#include "../../include/common/Utils.h"

#include <random>

int generateRandomInt(const int from, const int to) {
  thread_local std::mt19937 gen(std::random_device{}());
  std::uniform_int_distribution<> distrib(from, to);

  return distrib(gen);
}

float generateRandomFloat(const float from, const float to) {
  thread_local std::mt19937 gen(std::random_device{}());
  std::uniform_real_distribution<float> distrib(from, to);

  return distrib(gen);
}

bool randomTrueFalse() { return generateRandomInt(0, 1); }

int getCounter() {
  static int counter = 1;
  return counter++;
}
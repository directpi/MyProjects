#ifndef STRATEGIES_HPP
#define STRATEGIES_HPP

#include "../logic/model.hpp"

namespace s21 {

// Интерфейс стратегии трансформации
class TransformationStrategy {
 public:
  virtual ~TransformationStrategy() = default;
  virtual void transform(ObjModel& model) = 0;
};

// Стратегия масштабирования
class ScaleStrategy : public TransformationStrategy {
 private:
  double scale;

 public:
  ScaleStrategy(double scale) : scale(scale) {}

  void transform(ObjModel& model) override { model.scaleModel(scale); }
};

// Стратегия перемещения
class MoveStrategy : public TransformationStrategy {
 private:
  double dx, dy, dz;

 public:
  MoveStrategy(double dx, double dy, double dz) : dx(dx), dy(dy), dz(dz) {}

  void transform(ObjModel& model) override { model.moveModel(dx, dy, dz); }
};

// Стратегия вращения
class RotateStrategy : public TransformationStrategy {
 private:
  double alphaX, alphaY, alphaZ;

 public:
  RotateStrategy(double alphaX, double alphaY, double alphaZ)
      : alphaX(alphaX), alphaY(alphaY), alphaZ(alphaZ) {}

  void transform(ObjModel& model) override {
    model.rotateModel(alphaX, alphaY, alphaZ);
  }
};
} //namespace s21
#endif  // STRATEGIES_HPP

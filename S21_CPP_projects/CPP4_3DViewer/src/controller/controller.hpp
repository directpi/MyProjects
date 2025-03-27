#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP

#include <QObject>

#include "../logic/model.hpp"
#include "strategies.hpp"

namespace s21 {

class ObjModelController : public QObject {
  Q_OBJECT  // Позволяет классу использовать сигналы и слоты
      private : ObjModel& model;  // Ссылка на модель
  std::unique_ptr<TransformationStrategy>
      strategy;  // Текущая стратегия трансформации

 public:
  ObjModelController() : model(ObjModel::getInstance()), strategy(nullptr) {}

  // Удаление конструктора копирования и оператора присваивания
  ObjModelController(const ObjModelController&) = delete;
  ObjModelController& operator=(const ObjModelController&) = delete;

  // Установка стратегии трансформации
  void setStrategy(TransformationStrategy* newStrategy) {
    strategy.reset(
        newStrategy);  // Используем reset() для присваивания новой стратегии
  }

  // Применение текущей стратегии
  void applyTransformation() {
    if (strategy) {
      strategy->transform(model);
      emit modelUpdated();  // Генерируем сигнал после применения трансформации
    }
  }

  // Методы для получения данных из модели
  std::vector<Vertex>& getVertices() { return model.getVertices(); }
  std::vector<Polygon>& getEdges() { return model.getEdges(); }
  size_t getNumVertices() { return model.getNumVertices(); }
  size_t getNumEdges() { return model.getNumEdges(); }
  Vertex getVertexByIndex(size_t index) const {
    return model.getVertexByIndex(index);
  }
  Polygon getFacet(size_t index) const { return model.getFacet(index); }
  void normalize() { model.normalizeModel(); }
  void freeModel() { model.freeModel(); }
  // Сигнал для уведомления об обновлениях в модели
 signals:
  void modelUpdated();
};

}  // namespace s21
#endif  // CONTROLLER_HPP

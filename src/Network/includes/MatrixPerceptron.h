#ifndef MATRIXPERCEPTRON_H
#define MATRIXPERCEPTRON_H

#include "ActivationFunction.h"
#include "Logger.h"
#include "Matrix.h"
#include "Perceptron.h"
namespace s21 {
class MatrixPerceptron : public Perceptron {
 public:
  MatrixPerceptron(const std::vector<int>& size_vector);
  int ForwardFeed() override;
  void Clear() override;
  void PrintConfig() override;
  void PrintValues(int L) override;
  void BackPropogation(int expect) override;
  void SaveWeights(std::string file_path) override;
  void ReadWeights(std::string file_path) override;
  void SetInput(const std::vector<double>& values) override;
  void WeightsUpdater(double learning_rate) override;
  void Initialize(const std::vector<int>& size_vector) override;

 private:
  using matrix = std::vector<std::vector<double>>;
  void InitOpenCLObjexts();
  Logger logger_{"MatrixPerceptron"};
  size_t layers_{};  // колличество слоев
  std::vector<int> neurons_count_;  // кол-во нейронов на скаждом слою
  s21::ActivationFunction activation_function_;  // функция активации
  std::vector<matrix> weigths_;                  // Матрицы весов
  std::vector<matrix> weigths_err_;  // Матрицы ошибок весов
  matrix bios_;                      // Веса смещения
  matrix neurons_val_;               // Значения нейронов
  matrix neurons_err_;  // Значения ошибок нейронов
};
}  // namespace s21

#endif  // MATRIXPERCEPTRON_H
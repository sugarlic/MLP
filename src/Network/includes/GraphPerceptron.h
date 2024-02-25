#ifndef GRAPHPERCEPTRON_H
#define GRAPHPERCEPTRON_H

#include <QImage>
#include <vector>

#include "Layer.h"
#include "Logger.h"
#include "Perceptron.h"
namespace s21 {
class GraphPerceptron : public Perceptron {
 public:
  GraphPerceptron(const std::vector<int>& size_vector);
  ~GraphPerceptron();
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
  void InitAllocators(const std::vector<int>& size_vector);
  void InitNeuronAllocator(size_t size);
  void InitWeightsAllocator(size_t size);
  Logger logger_{"GraphPerceptron"};
  std::vector<s21::Layer> layers_;
  void* alloc_memory_{nullptr};
  LinearAllocator* main_alloc_{nullptr};
  LinearAllocator* neuron_alloc_{nullptr};
  LinearAllocator* weights_alloc_{nullptr};
};
}  // namespace s21

#endif  // GRAPHPERCEPTRON_H
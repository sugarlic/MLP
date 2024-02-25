#ifndef LAYER_H
#define LAYER_H

#include <memory>

#include "LinearAllocator.h"
#include "Neuron.h"
#include "STLAdapter.h"
namespace s21 {
class Layer {
 public:
  using NeuronAlloc = STLAdaptor<Neuron, LinearAllocator>;
  Layer(size_t size, size_t prev_size, LinearAllocator& n_alloc,
        LinearAllocator& w_alloc);
  void Initialize(size_t size, s21::Layer& prev_layer);
  int Predict();
  void ForwardFeed(const s21::Layer& prev_layer);
  inline size_t GetSize() const { return neurons_.size(); };
  inline s21::Neuron& operator[](size_t i) { return neurons_[i]; };
  void Clear();
  std::vector<s21::Neuron, NeuronAlloc> neurons_;

 private:
  s21::ActivationFunction act_func_;
};

}  // namespace s21

#endif  // LAYER_H
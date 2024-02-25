#ifndef NEURON_H
#define NEURON_H

#include <Matrix.h>

#include "ActivationFunction.h"
#include "LinearAllocator.h"
#include "STLAdapter.h"

namespace s21 {
struct Neuron {
  Neuron(size_t prev_size, LinearAllocator& w_alloc)
      : weigths(w_alloc), weigths_err(w_alloc) {
    Initialize(prev_size);
  };
  void Initialize(size_t prev_size);
  void Clear();

  std::vector<double, STLAdaptor<double, LinearAllocator>> weigths;
  std::vector<double, STLAdaptor<double, LinearAllocator>> weigths_err;
  double value{};
  double value_err{};
  double bios{};
  s21::ActivationFunction activation_function;  // функция активации
};

}  // namespace s21

#endif  // NEURON_H
#include "Neuron.h"

void s21::Neuron::Initialize(size_t prev_size) {
  weigths.reserve(prev_size);
  weigths_err.reserve(prev_size);
  for (size_t i = 0; i < prev_size; ++i) {
    weigths.emplace_back(static_cast<double>(std::rand()) / RAND_MAX * 2.0 -
                         1.0);
    weigths_err.emplace_back(0);
  }

  bios = static_cast<double>(std::rand()) / RAND_MAX * 0.03 + 0.01;
}

void s21::Neuron::Clear() {
  value = 0;
  bios = 0;
  value_err = 0;
  weigths.clear();
}
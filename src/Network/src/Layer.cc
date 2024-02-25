#include "Layer.h"

s21::Layer::Layer(size_t size, size_t prev_size, LinearAllocator &n_alloc,
                  LinearAllocator &w_alloc)
    : neurons_(n_alloc) {
  neurons_.reserve(size);
  std::srand(std::time(nullptr));
  for (size_t i = 0; i < size; ++i) {
    neurons_.emplace_back(prev_size, w_alloc);
  }
}

void s21::Layer::ForwardFeed(const s21::Layer &prev_layer) {
  auto neuron_count = GetSize();
  for (size_t i = 0; i < neuron_count; ++i) {
    auto &neur = neurons_[i];
    auto &prev_neurs = prev_layer.neurons_;
    auto prev_neuron_count = prev_layer.GetSize();
    double tmp{};
    for (size_t j = 0; j < prev_neuron_count; ++j)
      tmp += neur.weigths[j] * prev_neurs[j].value;
    tmp += neur.bios;
    neur.value = act_func_.Use(tmp);
  }
}

int s21::Layer::Predict() {
  static auto comp = [](const Neuron &a, const Neuron &b) {
    return a.value < b.value;
  };
  auto largest = std::max_element(neurons_.begin(), neurons_.end(), comp);
  return std::distance(neurons_.begin(), largest);
}

void s21::Layer::Clear() {
  for (auto it : neurons_) {
    it.Clear();
  }
  neurons_.clear();
}

#include "GraphPerceptron.h"

#include <fstream>
#include <numeric>
namespace s21 {
GraphPerceptron::GraphPerceptron(const std::vector<int>& size_vector) {
  Initialize(size_vector);
}

GraphPerceptron::~GraphPerceptron() {
  layers_.clear();
  if (neuron_alloc_) delete neuron_alloc_;
  if (weights_alloc_) delete weights_alloc_;
  if (main_alloc_) delete main_alloc_;
  if (alloc_memory_) free(alloc_memory_);
}

void GraphPerceptron::Initialize(const std::vector<int>& size_vector) {
  try {
    const auto layers_c = size_vector.size();
    InitAllocators(size_vector);
    layers_.reserve(layers_c);
    hidden_l_count_ = layers_c < 2 ? 0 : layers_c - 2;
    layers_.push_back(
        Layer(size_vector[0], 0, *neuron_alloc_, *weights_alloc_));
    for (size_t i = 1; i < layers_c; ++i) {
      layers_.push_back(Layer(size_vector[i], size_vector[i - 1],
                              *neuron_alloc_, *weights_alloc_));
    }
    PrintConfig();
  } catch (...) {
    logger_.Log("Failed to init GraphPerceptron", Logger::LogLevel::kError);
  }
}
void GraphPerceptron::InitAllocators(const std::vector<int>& size_vector) {
  if (main_alloc_) {
    logger_.Log(
        "Can't reinit main allocator. Reinit whole perceptron with new "
        "size.",
        Logger::LogLevel::kError);
    throw;
  }
  layers_.clear();
  int total_weights = 0;
  int total_neurons =
      std::accumulate(size_vector.begin(), size_vector.end(), 0);
  for (size_t i = 1; i < size_vector.size(); ++i) {
    total_weights += size_vector[i] * size_vector[i - 1] *
                     2;  // 2 - vector of weights / vector of weights error
  }

  int total_bytes_neuron = total_neurons * sizeof(Neuron);
  int total_bytes_weights = total_weights * sizeof(double);
  int total_bytes = total_bytes_neuron + total_bytes_weights;
  alloc_memory_ = std::malloc(total_bytes);
  main_alloc_ = new LinearAllocator(total_bytes, alloc_memory_);
  InitNeuronAllocator(total_bytes_neuron);
  InitWeightsAllocator(total_bytes_weights);
}
void GraphPerceptron::InitNeuronAllocator(size_t size) {
  if (neuron_alloc_) {
    logger_.Log(
        "Can't reinit neuron allocator. Reinit whole perceptron with new "
        "size.",
        Logger::LogLevel::kError);
    throw;
  };
  neuron_alloc_ = new LinearAllocator(size, main_alloc_->Allocate(size));
}

void GraphPerceptron::InitWeightsAllocator(size_t size) {
  if (weights_alloc_) {
    logger_.Log(
        "Can't reinit weight allocator. Reinit whole perceptron with new "
        "size.",
        Logger::LogLevel::kError);
    throw;
  }
  weights_alloc_ = new LinearAllocator(size, main_alloc_->Allocate(size));
}

int GraphPerceptron::ForwardFeed() {
  for (size_t i = 1; i < layers_.size(); ++i) {
    layers_[i].ForwardFeed(layers_[i - 1]);
  }
  return layers_.back().Predict();
}

void GraphPerceptron::BackPropogation(int expect) {
  // выходной слой
  std::vector<double> correct(layers_.back().GetSize());
  correct[expect] = 1;
  auto& layer = layers_.back();
  for (size_t i = 0, neuron_count = layer.GetSize(); i < neuron_count; ++i) {
    auto& neuron = layer[i].value;
    layer[i].value_err = (correct[i] - neuron) * neuron * (1 - neuron);
  }

  // скрытыте слои
  for (int i = layers_.size() - 2; i > 0; --i) {
    auto& layer = layers_[i];
    auto& next_layer = layers_[i + 1];
    auto neuron_count = layer.GetSize();
    for (size_t j = 0; j < neuron_count; ++j) {
      double tmp{};
      auto next_neuron_count = next_layer.GetSize();
      for (size_t k = 0; k < next_neuron_count; ++k)
        tmp += next_layer[k].value_err * layer[k].weigths[j];
      auto& j_neuron = layer[j];
      j_neuron.value_err = tmp * j_neuron.value * (1 - j_neuron.value);
    }
  }
}

void GraphPerceptron::WeightsUpdater(double learning_rate) {
  double moment{0.1};

  for (int i = layers_.size() - 1; i > 0; --i) {
    auto& layer = layers_[i];
    auto& prev_layer = layers_[i - 1];
    auto neuron_count = layer.GetSize();
    auto prev_neuron_count = prev_layer.GetSize();
    for (size_t j = 0; j < prev_neuron_count; ++j) {
      auto& prev_neuron = prev_layer[j];
      for (size_t k = 0; k < neuron_count; ++k) {
        auto& neuron = layer[k];
        neuron.weigths_err[j] *= moment;
        double tmp = learning_rate * neuron.value_err * prev_neuron.value;
        neuron.weigths[j] += tmp;
        neuron.weigths_err[j] = tmp;
      }
    }
  }

  for (size_t i = 0; i < layers_.size() - 1; ++i) {
    auto& layer = layers_[i];
    auto& next_layer = layers_[i + 1];
    auto next_neuron_count = next_layer.GetSize();
    for (size_t k = 0; k < next_neuron_count; ++k)
      layer[k].bios += next_layer[k].value_err * learning_rate;
  }
}

void GraphPerceptron::SetInput(const std::vector<double>& values) {
  auto neuron_count = layers_[0].GetSize();
  for (size_t i = 0; i < neuron_count; ++i) layers_[0][i].value = values[i];
}

void GraphPerceptron::Clear() {
  layers_.clear();
  if (neuron_alloc_) delete neuron_alloc_;
  if (weights_alloc_) delete weights_alloc_;
  if (main_alloc_) delete main_alloc_;
  if (alloc_memory_) free(alloc_memory_);
  neuron_alloc_ = nullptr;
  weights_alloc_ = nullptr;
  main_alloc_ = nullptr;
  alloc_memory_ = nullptr;
}

void GraphPerceptron::SaveWeights(std::string file_path) {
  std::ofstream file;
  size_t saved_size = 0;
  file.open(file_path, std::ios::binary);
  if (!file.is_open()) {
    logger_.Log("Error saving file", Logger::LogLevel::kError);
    return;
  }
  PerceptronType file_p_type = kGraph;
  file.write(reinterpret_cast<const char*>(&file_p_type),
             sizeof(PerceptronType));
  size_t layers_size = layers_.size();

  file.write(reinterpret_cast<const char*>(&layers_size), sizeof(size_t));
  for (auto& layer : layers_) {
    if (&layer == &layers_.front() || &layer == &layers_.back()) continue;
    size_t neuron_count = layer.GetSize();
    file.write(reinterpret_cast<const char*>(&neuron_count), sizeof(size_t));
  }
  for (auto& layer : layers_) {
    size_t neuron_count = layer.GetSize();
    for (size_t i = 0; i < neuron_count; ++i) {
      auto& neuron = layer[i];
      auto& weights = neuron.weigths;
      size_t weights_size = weights.size();
      file.write(reinterpret_cast<const char*>(&weights_size), sizeof(size_t));
      file.write(reinterpret_cast<const char*>(weights.data()),
                 sizeof(double) * weights_size);
      file.write(reinterpret_cast<const char*>(&neuron.value), sizeof(double));
      file.write(reinterpret_cast<const char*>(&neuron.value_err),
                 sizeof(double));
      file.write(reinterpret_cast<const char*>(&neuron.bios), sizeof(double));
    }
  }
  saved_size = file.tellp();
  logger_.Log("Total saved bytes: " + std::to_string(saved_size) + " bytes.");
  logger_.Log("Weights were saved", Logger::LogLevel::kInfo);
  file.close();
}

void GraphPerceptron::ReadWeights(std::string file_path) {
  std::ifstream file(file_path);
  size_t read_size = 0;
  if (!file.is_open()) {
    logger_.Log("Error reading weights file", Logger::LogLevel::kError);
    return;
  }
  PerceptronType file_p_type = kMatrix;
  file.read(reinterpret_cast<char*>(&file_p_type), sizeof(PerceptronType));
  if (file_p_type != kGraph) {
    logger_.Log(
        "Error reading weights file. Wrong perceptron type in weight file.",
        Logger::LogLevel::kError);
    return;
  }
  Clear();
  size_t layers_size = 0;
  file.read(reinterpret_cast<char*>(&layers_size), sizeof(size_t));
  size_t h_layers_count = layers_size - 2;
  std::vector<size_t> h_values(h_layers_count);
  for (size_t i = 0; i < h_layers_count; ++i) {
    size_t val = 0;
    file.read(reinterpret_cast<char*>(&val), sizeof(size_t));
    h_values[i] = val;
  }
  logger_.Log("Reinitializing perceptron.");
  Initialize(Perceptron::CalcSizeVector(h_values));

  for (auto& layer : layers_) {
    size_t neuron_count = layer.GetSize();
    for (size_t i = 0; i < neuron_count; ++i) {
      auto& neuron = layer[i];
      size_t weights_size = 0;
      auto& weights = neuron.weigths;
      file.read(reinterpret_cast<char*>(&weights_size), sizeof(size_t));
      file.read(reinterpret_cast<char*>(weights.data()),
                sizeof(double) * weights_size);
      file.read(reinterpret_cast<char*>(&neuron.value), sizeof(double));
      file.read(reinterpret_cast<char*>(&neuron.value_err), sizeof(double));
      file.read(reinterpret_cast<char*>(&neuron.bios), sizeof(double));
      neuron.value_err = 0;
    }
  }
  read_size = file.tellg();
  logger_.Log("Total read bytes: " + std::to_string(read_size) + " bytes.");
  logger_.Log("Weights were loaded", Logger::LogLevel::kInfo);
  file.close();
}

void GraphPerceptron::PrintValues(int L) {
  for (size_t i = 0; i < layers_[L].GetSize(); ++i)
    logger_.Log(std::to_string(i) + " " + std::to_string(layers_[L][i].value));
}

void GraphPerceptron::PrintConfig() {
  static const std::string filler{"************************************"};
  logger_.Log(filler, Logger::LogLevel::kInfo);
  logger_.Log("Perceptron have " + std::to_string(layers_.size()) + " layers",
              Logger::LogLevel::kInfo);
  logger_.Log("SIZE: ", Logger::LogLevel::kInfo);
  std::string neurons{};
  for (size_t i = 0; i < layers_.size(); ++i)
    neurons += std::to_string(layers_[i].GetSize()) + ' ';
  logger_.Log(neurons, Logger::LogLevel::kInfo);
  logger_.Log(filler, Logger::LogLevel::kInfo);
}

}  // namespace s21

#include "MatrixPerceptron.h"

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <thread>

#include "FileManager.h"
namespace s21 {
using matrix = std::vector<std::vector<double>>;
MatrixPerceptron::MatrixPerceptron(const std::vector<int>& size_vector) {
  Initialize(size_vector);
}
void MatrixPerceptron::Initialize(const std::vector<int>& size_vector) {
  Clear();
  std::srand(std::time(nullptr));
  layers_ = size_vector.size();
  hidden_l_count_ = layers_ < 2 ? 0 : layers_ - 2;
  neurons_count_ = size_vector;
  bios_.resize(layers_ - 1);
  neurons_val_.resize(layers_);
  neurons_err_.resize(layers_);

  for (int i = 0, end = layers_ - 1; i != end; ++i) {
    weigths_.push_back(CreateMatrix(neurons_count_[i + 1], neurons_count_[i]));
    weigths_err_.push_back(
        CreateMatrix(neurons_count_[i + 1], neurons_count_[i]));
    RandFillMatrix(weigths_[i]);

    bios_[i].resize(neurons_count_[i + 1]);
    for (int j = 0, end = neurons_count_[i + 1]; j != end; ++j)
      bios_[i][j] = static_cast<double>(std::rand()) / RAND_MAX * 0.03 + 0.01;
  }

  for (int i = 0, end = layers_; i != end; ++i) {
    neurons_val_[i].resize(size_vector[i]);
    neurons_err_[i].resize(size_vector[i]);
  }
  PrintConfig();
}

void MatrixPerceptron::Clear() {
  layers_ = 0;
  neurons_count_.clear();
  weigths_.clear();
  weigths_err_.clear();
  bios_.clear();
  neurons_val_.clear();
}

void MatrixPerceptron::PrintConfig() {
  static const std::string filler{"************************************"};
  logger_.Log(filler, Logger::LogLevel::kInfo);
  logger_.Log("Perceptron have " + std::to_string(layers_) + " layers",
              Logger::LogLevel::kInfo);
  logger_.Log("SIZE: ", Logger::LogLevel::kInfo);
  std::string neurons{};
  for (size_t i = 0; i < layers_; ++i)
    neurons += std::to_string(neurons_count_[i]) + ' ';
  logger_.Log(neurons, Logger::LogLevel::kInfo);
  logger_.Log(filler, Logger::LogLevel::kInfo);
}

int MatrixPerceptron::ForwardFeed() {
  for (size_t i = 1; i < layers_; ++i) {
    neurons_val_[i] = MulVector(weigths_[i - 1], neurons_val_[i - 1]);
    SumVectors(neurons_val_[i], bios_[i - 1]);
    activation_function_.Use(neurons_val_[i]);
  }
  auto& last_l = neurons_val_.back();

  return std::distance(last_l.begin(),
                       std::max_element(last_l.begin(), last_l.end()));
}

void MatrixPerceptron::PrintValues(int L) {
  for (int i = 0; i < neurons_count_[L]; ++i)
    logger_.Log(std::to_string(i) + " " + std::to_string(neurons_val_[L][i]));
}

void MatrixPerceptron::BackPropogation(int expect) {
  // выходной слой
  std::vector<double> correct(neurons_count_[layers_ - 1]);
  correct[expect] = 1;
  for (int i = 0; i < neurons_count_[layers_ - 1]; ++i) {
    auto v = neurons_val_[layers_ - 1][i];
    neurons_err_[layers_ - 1][i] = (correct[i] - v) * v * (1 - v);
  }
  // скрытыте слои
  for (int i = layers_ - 2; i >= 0; --i) {
    for (int j = 0; j < neurons_count_[i]; ++j) {
      double tmp{};
      for (int k = 0; k < neurons_count_[i + 1]; ++k) {
        tmp += neurons_err_[i + 1][k] * weigths_[i][k][j];
      }
      neurons_err_[i][j] = tmp * neurons_val_[i][j] * (1 - neurons_val_[i][j]);
    }
  }
}

void MatrixPerceptron::WeightsUpdater(double learning_rate) {
  double moment{0.1};
  for (size_t i = 0; i < layers_ - 1; ++i) {
    auto& w = weigths_[i];
    for (int j = 0; j < neurons_count_[i + 1]; ++j) {
      auto err = neurons_err_[i + 1][j];
      double tmp{};
      for (int k = 0; k < neurons_count_[i]; ++k) {
        weigths_err_[i][j][k] *= moment;
        tmp = neurons_val_[i][k] * learning_rate * err + weigths_err_[i][j][k];
        w[j][k] += tmp;
        weigths_err_[i][j][k] = tmp;
      }
    }
  }

  for (size_t i = 0; i < layers_ - 1; ++i)
    for (int k = 0; k < neurons_count_[i + 1]; ++k)
      bios_[i][k] += neurons_err_[i + 1][k] * learning_rate;
}

void MatrixPerceptron::SaveWeights(std::string file_path) {
  std::ofstream file;
  file.open(file_path, std::ios::binary);
  if (!file.is_open()) {
    logger_.Log("Error saving file", Logger::LogLevel::kError);
    return;
  }
  PerceptronType file_p_type = kMatrix;
  file.write(reinterpret_cast<const char*>(&file_p_type),
             sizeof(PerceptronType));

  size_t layers_c = layers_ - 1;
  auto h_values_beg = ++neurons_count_.begin();
  size_t h_values_size = neurons_count_.size() - 2;

  file.write(reinterpret_cast<const char*>(&layers_c), sizeof(size_t));
  file.write(reinterpret_cast<const char*>(&h_values_size), sizeof(size_t));
  file.write(reinterpret_cast<const char*>(&(*h_values_beg)),
             sizeof(int) * h_values_size);
  for (size_t i = 0; i < layers_c; ++i) {
    size_t matrix_rows = weigths_[i].size();
    size_t matrix_cols = weigths_[i][0].size();
    size_t bios_size = neurons_count_[i + 1];
    file.write(reinterpret_cast<const char*>(&matrix_rows), sizeof(size_t));
    file.write(reinterpret_cast<const char*>(&matrix_cols), sizeof(size_t));
    file.write(reinterpret_cast<const char*>(&bios_size), sizeof(size_t));

    for (size_t j = 0; j < matrix_rows; ++j)
      file.write(reinterpret_cast<const char*>(weigths_[i][j].data()),
                 sizeof(double) * matrix_cols);

    file.write(reinterpret_cast<const char*>(bios_[i].data()),
               sizeof(double) * bios_size);
  }
  logger_.Log("Weights were saved");
}
void MatrixPerceptron::ReadWeights(std::string file_path) {
  // BUG won't open files containing 'x' or '%' chars
  std::ifstream file(file_path);
  if (!file.is_open()) {
    logger_.Log("Error reading weights file", Logger::LogLevel::kError);
    return;
  }
  PerceptronType file_p_type = kGraph;
  file.read(reinterpret_cast<char*>(&file_p_type), sizeof(PerceptronType));
  if (file_p_type != kMatrix) {
    logger_.Log(
        "Error reading weights file. Wrong perceptron type in weight file.",
        Logger::LogLevel::kError);
    return;
  }
  Clear();
  size_t layers_c = 0;
  size_t h_layers_count = 0;
  file.read(reinterpret_cast<char*>(&layers_c), sizeof(size_t));
  file.read(reinterpret_cast<char*>(&h_layers_count), sizeof(size_t));

  std::vector<int> h_values(h_layers_count);

  file.read(reinterpret_cast<char*>(h_values.data()),
            sizeof(int) * h_layers_count);
  logger_.Log("Reinitializing perceptron.");
  Initialize(Perceptron::CalcSizeVector({h_values.begin(), h_values.end()}));
  std::ifstream fin(file_path);
  for (size_t i = 0; i < layers_c; ++i) {
    size_t matrix_rows = 0;
    size_t matrix_cols = 0;
    size_t bios_size = 0;
    file.read(reinterpret_cast<char*>(&matrix_rows), sizeof(size_t));
    file.read(reinterpret_cast<char*>(&matrix_cols), sizeof(size_t));
    file.read(reinterpret_cast<char*>(&bios_size), sizeof(size_t));

    for (size_t j = 0; j < matrix_rows; ++j)
      file.read(reinterpret_cast<char*>(weigths_[i][j].data()),
                sizeof(double) * matrix_cols);

    file.read(reinterpret_cast<char*>(bios_[i].data()),
              sizeof(double) * bios_size);
  }
  logger_.Log("Weights were loaded");
}

void MatrixPerceptron::SetInput(const std::vector<double>& values) {
  for (int i = 0; i < neurons_count_[0]; i++) neurons_val_[0][i] = values[i];
}

}  // namespace s21

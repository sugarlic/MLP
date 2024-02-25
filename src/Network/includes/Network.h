#ifndef NETWORK_H
#define NETWORK_H

#include <QImage>
#include <mutex>
#include <string>
#include <vector>

#include "Logger.h"
#include "Perceptron.h"
namespace s21 {

struct NetworkArgs {
  std::string dataset_file;
  PerceptronType p_type;
  int new_size;
};

class Network {
 public:
  enum NetworkState {
    kTraining,
    kCrossTraining,
    kTesting,
    kIdle,
    kLoading,
    kSaving,
    kQuiting
  };
  Network(std::vector<int> size_vector) : size_vector_(size_vector) {
    SetState(kIdle);
    InitPerceptron();
  };
  Network(const Network& another) = delete;
  Network(Network&& another) = delete;
  Network& operator=(const Network& another) = delete;
  Network& operator=(Network&& another) = delete;
  std::vector<double> NetworkTest(double sample_part, NetworkArgs data);
  std::vector<double> NetworkTrain(int epochs_count, NetworkArgs data);
  std::vector<double> NetworkCrossValidationTrain(int group_count,
                                                  NetworkArgs data);
  void SaveWeights(std::string file_path);
  void ReadWeights(std::string file_path);
  void SetPerceptronType(PerceptronType p_type);
  PerceptronType GetPerceptronType();
  int AnalyzeImg(QImage img);
  ~Network() {
    if (perceptron_) delete perceptron_;
  }
  inline NetworkState GetState() {
    std::lock_guard<std::mutex> lock(state_mutex_);
    return state_;
  };
  inline void AbortTraining() { perceptron_->AbortTraining(); }
  inline int GetHiddenLayersCount() {
    if (!perceptron_) {
      logger_.Log(
          "Perceptron wasn't initialized. Can't get hidden layers count.",
          Logger::LogLevel::kError);
      throw;
    }
    return perceptron_->GetHiddenLayersCount();
  }

 private:
  NetworkState state_ = kLoading;
  mutable std::mutex state_mutex_;
  Logger logger_{"Network"};
  void ValidatePerceptron();
  void InitSizeVector(size_t new_size);
  void InitPerceptron();
  void ValidateState(std::string error_msg = "");
  inline void SetState(NetworkState state) {
    std::lock_guard<std::mutex> lock(state_mutex_);
    state_ = state;
  };

  std::vector<int> size_vector_;
  PerceptronType p_type_ = kGraph;
  Perceptron* perceptron_{};
};
}  // namespace s21

#endif  // NETWORK_H

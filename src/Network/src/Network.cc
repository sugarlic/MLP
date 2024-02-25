#include "Network.h"

#include <fstream>

#include "GraphPerceptron.h"
#include "MatrixPerceptron.h"
namespace s21 {

void Network::InitSizeVector(size_t new_size) {
  if (new_size == size_vector_.size() - 2) return;
  size_vector_ = Perceptron::CalcSizeVector(new_size);
  InitPerceptron();
}

std::vector<double> Network::NetworkTest(double sample_part, NetworkArgs data) {
  auto prev_state = GetState();
  std::vector<double> res;

  try {
    InitSizeVector(data.new_size);
    ValidateState("Can't start testing");
    SetPerceptronType(data.p_type);
    ValidatePerceptron();
    SetState(kTesting);
    res = perceptron_->Test(data.dataset_file, sample_part);
    SetState(kIdle);
  } catch (...) {
    SetState(prev_state);
    logger_.Log("Testing failed", Logger::LogLevel::kError);
  }
  return res;
}

std::vector<double> Network::NetworkTrain(int epochs_count, NetworkArgs data) {
  auto prev_state = GetState();
  std::vector<double> res;

  try {
    InitSizeVector(data.new_size);
    ValidateState("Can't start training");
    SetPerceptronType(data.p_type);
    ValidatePerceptron();
    SetState(kTraining);
    res = perceptron_->Train(data.dataset_file, epochs_count);
    SetState(kIdle);
  } catch (...) {
    SetState(prev_state);
    logger_.Log("Training failed", Logger::LogLevel::kError);
  }
  return res;
}

std::vector<double> Network::NetworkCrossValidationTrain(int group_count,
                                                         NetworkArgs data) {
  auto prev_state = GetState();
  std::vector<double> res;

  try {
    InitSizeVector(data.new_size);
    ValidateState("Can't start cross validated training");
    SetPerceptronType(data.p_type);
    ValidatePerceptron();
    SetState(kCrossTraining);
    res = perceptron_->CrossValidationTrain(data.dataset_file, group_count);
    SetState(kIdle);
  } catch (...) {
    SetState(prev_state);
    logger_.Log("Cross validated training failed", Logger::LogLevel::kError);
  }
  return res;
}

void Network::SaveWeights(std::string file_path) {
  auto prev_state = GetState();
  try {
    ValidateState("Can't start saving");
    ValidatePerceptron();
    SetState(kSaving);
    perceptron_->SaveWeights(file_path);
    SetState(kIdle);
  } catch (...) {
    SetState(prev_state);
    logger_.Log("Saving weights failed", Logger::LogLevel::kError);
  }
}

void Network::ReadWeights(std::string file_path) {
  auto prev_state = GetState();
  try {
    ValidateState("Can't start loading");
    ValidatePerceptron();
    std::ifstream file(file_path);
    if (!file.is_open()) {
      logger_.Log("Error reading weights file", Logger::LogLevel::kError);
      return;
    }
    PerceptronType file_p_type = kGraph;
    file.read(reinterpret_cast<char*>(&file_p_type), sizeof(PerceptronType));
    if (file_p_type != p_type_) {
      p_type_ = file_p_type;
      InitPerceptron();
    }
    SetState(kLoading);
    perceptron_->ReadWeights(file_path);
    SetState(kIdle);
  } catch (...) {
    SetState(prev_state);
    logger_.Log("Loading weights failed", Logger::LogLevel::kError);
  }
}

void Network::SetPerceptronType(PerceptronType p_type) {
  if (p_type == p_type_) return;
  p_type_ = p_type;
  InitPerceptron();
}

PerceptronType Network::GetPerceptronType() { return p_type_; }

int Network::AnalyzeImg(QImage img) {
  auto prev_state = GetState();
  int res = 0;
  try {
    ValidateState("Can't analyze image");
    ValidatePerceptron();
    SetState(kLoading);
    res = perceptron_->AnalyzeImg(img);
    SetState(kIdle);
  } catch (...) {
    SetState(prev_state);
    res = -1;
    logger_.Log("Analyzing image failed", Logger::LogLevel::kError);
  }
  return res;
}

void Network::ValidatePerceptron() {
  if (!perceptron_) {
    logger_.Log("Perceptron is not set", Logger::LogLevel::kError);
    throw "Perceptron is not set";
  }
}

void Network::InitPerceptron() {
  try {
    logger_.Log("Initializing perceptron");
    ValidateState("Can't init perceptron");
    if (perceptron_) delete perceptron_;
    if (p_type_ == kMatrix) perceptron_ = new MatrixPerceptron(size_vector_);
    if (p_type_ == kGraph) perceptron_ = new GraphPerceptron(size_vector_);
  } catch (...) {
  }
}
void Network::ValidateState(std::string error_msg) {
  if (GetState() == kIdle) return;
  logger_.Log("Network state isn't Idle. " + error_msg,
              Logger::LogLevel::kError);
  throw "Network state error";
};
}  // namespace s21

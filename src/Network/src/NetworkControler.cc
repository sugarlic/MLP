#include "NetworkControler.h"

namespace s21 {
std::vector<double> NetworkControler::NetworkTest(double sample_part,
                                                  NetworkArgs data) {
  return network_->NetworkTest(sample_part, data);
}

std::vector<double> NetworkControler::NetworkTrain(int epochs_count,
                                                   NetworkArgs data) {
  return network_->NetworkTrain(epochs_count, data);
}

std::vector<double> NetworkControler::NetworkCrossValidationTrain(
    int group_count, NetworkArgs data) {
  return network_->NetworkCrossValidationTrain(group_count, data);
}

Network::NetworkState NetworkControler::GetState() {
  return network_->GetState();
}

void NetworkControler::SaveWeights(std::string file_path) {
  network_->SaveWeights(file_path);
}

void NetworkControler::ReadWeights(std::string file_path) {
  network_->ReadWeights(file_path);
}

PerceptronType NetworkControler::GetPerceptronType() {
  return network_->GetPerceptronType();
}
void NetworkControler::AbortTraining() { network_->AbortTraining(); }
int NetworkControler::AnalyzeImg(QImage img) {
  return network_->AnalyzeImg(img);
}

int NetworkControler::GetHiddenLayersCount() {
  return network_->GetHiddenLayersCount();
}
}  // namespace s21

#ifndef NETWORKCONTROLER_H
#define NETWORKCONTROLER_H

#include "Network.h"

namespace s21 {
class NetworkControler {
 public:
  NetworkControler(std::shared_ptr<s21::Network> network)
      : network_{network} {};
  std::vector<double> NetworkTest(double sample_part, NetworkArgs data);
  std::vector<double> NetworkTrain(int epochs_count, NetworkArgs data);
  std::vector<double> NetworkCrossValidationTrain(int group_count,
                                                  NetworkArgs data);
  Network::NetworkState GetState();
  void SaveWeights(std::string file_path);
  void ReadWeights(std::string file_path);
  PerceptronType GetPerceptronType();
  void AbortTraining();
  int AnalyzeImg(QImage img);
  int GetHiddenLayersCount();

 private:
  std::shared_ptr<s21::Network> network_;
};
}  // namespace s21

#endif  // NETWORKCONTROLER_H

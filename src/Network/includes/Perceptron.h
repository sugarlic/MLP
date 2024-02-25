#ifndef PERCEPTRON_H
#define PERCEPTRON_H
#include <QImage>
#include <atomic>
#include <vector>

#include "FileManager.h"
#include "PerceptronType.h"
namespace s21 {
class Perceptron {
 public:
  Perceptron() = default;
  Perceptron(const Perceptron& another) = delete;
  Perceptron(Perceptron&& another) = delete;
  Perceptron& operator=(const Perceptron& another) = delete;
  Perceptron& operator=(Perceptron&& another) = delete;
  static std::vector<int> CalcSizeVector(size_t hidden_l_count = 2,
                                         size_t hidden_l_val = 100);
  static std::vector<int> CalcSizeVector(std::vector<size_t> hidden_l_values);
  int AnalyzeImg(QImage img);
  virtual int ForwardFeed() = 0;
  virtual void Clear() = 0;
  virtual void PrintConfig() = 0;
  virtual void PrintValues(int L) = 0;
  virtual void BackPropogation(int expect) = 0;
  virtual void SaveWeights(std::string file_path) = 0;
  virtual void ReadWeights(std::string file_path) = 0;
  virtual void SetInput(const std::vector<double>& values) = 0;
  virtual void WeightsUpdater(double learning_rate) = 0;
  virtual void Initialize(const std::vector<int>& size_vector) = 0;

  std::vector<double> Test(std::string dataset_file, double sample_part);
  std::vector<double> Train(std::string dataset_file, int epochs_count);
  std::vector<double> CrossValidationTrain(std::string dataset_file,
                                           int group_count);

  virtual ~Perceptron() {}
  inline void AbortTraining() { abort_task = true; }
  inline void ResetAbort() { abort_task = false; }
  inline int GetHiddenLayersCount() { return hidden_l_count_; }

 protected:
  int hidden_l_count_{0};
  void AnalyzeTraining(const std::vector<CharInfo>& dataset);
  std::vector<CharInfo> ReadDatasetFile(const std::string& file_path);
  std::atomic<bool> abort_task = false;

 private:
  Logger logger_{"Perceptron"};
  bool EndsWithBin(const std::string& str);
};
}  // namespace s21

#endif  // PERCEPTRON_H

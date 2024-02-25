#include "Perceptron.h"

#include <Matrix.h>

#include <QThread>
#include <chrono>
#include <cmath>
#include <iostream>
#include <sstream>

namespace s21 {

std::vector<int> Perceptron::CalcSizeVector(size_t hidden_l_count,
                                            size_t hidden_l_val) {
  std::vector<int> size_vector;
  constexpr int input_l_size = 28 * 28;  // 28 * 28 image size
  constexpr int output_l_size = 26;      // eng alphabet size
  size_vector.push_back(input_l_size);
  for (size_t i = 0; i < hidden_l_count; ++i)
    size_vector.push_back(hidden_l_val);
  size_vector.push_back(output_l_size);
  return size_vector;
}
std::vector<int> Perceptron::CalcSizeVector(
    std::vector<size_t> hidden_l_values) {
  std::vector<int> size_vector;
  constexpr int input_l_size = 28 * 28;  // 28 * 28 image size
  constexpr int output_l_size = 26;      // eng alphabet size
  size_vector.push_back(input_l_size);
  size_vector.insert(size_vector.end(), hidden_l_values.begin(),
                     hidden_l_values.end());
  size_vector.push_back(output_l_size);
  return size_vector;
}
bool Perceptron::EndsWithBin(const std::string& str) {
  if (str.length() >= 4) {
    std::string lastFourChars = str.substr(str.length() - 4);
    return lastFourChars == ".bin";
  }
  return false;
}
std::vector<double> Perceptron::Train(std::string dataset_file,
                                      int epochs_count) {
  std::vector<double> result;
  if (epochs_count <= 0) {
    logger_.Log("Epoch count < 0. Aborting.", Logger::LogLevel::kError);
    return result;
  }

  double true_ans_count = 0;
  std::chrono::duration<double> training_time;
  s21::FileManager file_manager;
  logger_.Log("Perceptron config for this training:");
  PrintConfig();
  auto data = ReadDatasetFile(dataset_file);

  for (int epoch = 0; epoch < epochs_count; ++epoch) {
    true_ans_count = 0;
    auto start_time = std::chrono::steady_clock::now();

    for (const auto& sample : data) {
      if (abort_task) {
        logger_.Log("Aborting training", Logger::LogLevel::kWarning);
        ResetAbort();
        return {};
      }

      SetInput(sample.pixels);
      double true_ans = sample.letter;
      double predict_ans = ForwardFeed();

      if (predict_ans == true_ans) {
        true_ans_count++;
      }
      BackPropogation(true_ans);
      WeightsUpdater(0.15 * exp(-epoch / 20.));
    }

    auto end_time = std::chrono::steady_clock::now();
    training_time = end_time - start_time;
    double true_ans_percentage = true_ans_count / data.size() * 100;

    std::string log =
        "\n\tEpoch id: " + std::to_string(epoch) +
        ";\n\tTrue answers: " + std::to_string(true_ans_percentage) +
        "%;\n\tTraining time: " + std::to_string(training_time.count());
    logger_.Log(log);

    result.push_back((100 - true_ans_percentage) / 100.f);
  }

  logger_.Log("Network training finished", Logger::LogLevel::kWarning);

  return result;
}

std::vector<double> Perceptron::Test(std::string dataset_file,
                                     double sample_part) {
  std::vector<double> res;
  if (sample_part <= 0 || sample_part > 1) return res;

  int true_ans_c{}, true_ans{}, predict_ans{};
  std::chrono::duration<double> time;
  s21::FileManager file_manager;
  auto data = ReadDatasetFile(dataset_file);
  auto t1 = std::chrono::steady_clock::now();

  logger_.Log("Starting test training on " + std::to_string(sample_part * 100) +
              "% of dataset data");

  for (int i = 0, end = data.size() * sample_part; i < end; ++i) {
    if (abort_task) {
      logger_.Log("Aborting test training", Logger::LogLevel::kWarning);
      ResetAbort();
      return {};
    }
    SetInput(data[i].pixels);
    true_ans = data[i].letter;
    predict_ans = ForwardFeed();
    if (predict_ans == true_ans) true_ans_c++;
  }
  AnalyzeTraining(data);
  auto t2 = std::chrono::steady_clock::now();
  time = t2 - t1;
  auto true_ans_perc = true_ans_c / (data.size() * sample_part) * 100;
  auto test_time = time.count();
  logger_.Log("Total true answers ratio: " + std::to_string(true_ans_perc) +
              "%");
  logger_.Log("Total testing time: " + std::to_string(test_time) + "s");
  res.push_back(true_ans_perc);
  res.push_back(test_time);
  logger_.Log("Finished test training");
  return res;
}

int Perceptron::AnalyzeImg(QImage img) {
  constexpr QSize max_size(512, 512);
  auto size = img.size();
  if (size.width() >= max_size.width() || size.height() >= max_size.height()) {
    logger_.Log("Max image size is 512x512", Logger::LogLevel::kError);
    throw Logger::LogLevel::kError;
  }
  img = img.scaled(28, 28);
  int w = img.size().width();
  int h = img.size().height();
  std::vector<double> data;
  for (int i = 0; i < w; i++) {
    for (int j = 0; j < h; j++) {
      data.push_back(img.pixelColor(i, j).green() / 255.);
    }
  }
  SetInput(data);
  return ForwardFeed();
}

std::vector<double> Perceptron::CrossValidationTrain(std::string dataset_file,
                                                     int group_count) {
  FileManager fm;
  std::vector<double> result;
  auto data = ReadDatasetFile(dataset_file);
  double true_ans_count{};
  double true_ans{};
  double predict_ans{};

  std::vector<std::vector<s21 ::CharInfo>> groups(group_count);
  for (int i = 0; i < group_count; ++i) {
    groups[i].resize(data.size() / group_count);
  }

  int group_ind{};
  int j{};
  for (size_t i = 0; i < data.size(); ++i) {
    groups[group_ind][j] = data[i];
    j++;
    if (i + 1 == (data.size() / group_count) * (group_ind + 1)) {
      j = 0;
      ++group_ind;
    }
  }
  logger_.Log("Starting network cross validated training. Please wait.",
              Logger::LogLevel::kWarning);
  for (size_t group = 0; group < groups.size(); ++group) {
    true_ans_count = 0;

    for (size_t i = 0; i < groups.size(); ++i) {
      if (i != group) {
        for (size_t j = 0; j < groups[i].size(); ++j) {
          if (abort_task) {
            logger_.Log("Aborting cross validated training",
                        Logger::LogLevel::kWarning);
            ResetAbort();
            return {};
          }
          SetInput(groups[i][j].pixels);

          true_ans = groups[i][j].letter;
          ForwardFeed();

          BackPropogation(true_ans);
          WeightsUpdater(0.3);
        }
      }
    }

    for (size_t j = 0; j < groups[group].size(); ++j) {
      if (abort_task) {
        logger_.Log("Aborting cross validated training",
                    Logger::LogLevel::kWarning);
        ResetAbort();
        return {};
      }

      SetInput(groups[group][j].pixels);
      true_ans = groups[group][j].letter;
      predict_ans = ForwardFeed();
      if (predict_ans == true_ans) true_ans_count++;
    }

    double true_ans_percentage = true_ans_count / groups[group].size() * 100;
    std::string log =
        "\n\tGroup id: " + std::to_string(group) +
        ";\n\tTrue answers: " + std::to_string(true_ans_percentage) + "%;";
    logger_.Log(log);
    result.push_back(100 - true_ans_percentage);
  }
  logger_.Log("Finished network cross validated training.");
  return result;
}

void Perceptron::AnalyzeTraining(const std::vector<CharInfo>& dataset) {
  constexpr size_t kNumOfLetters = 26;
  matrix confusion_mat(kNumOfLetters, std::vector<double>(kNumOfLetters, 0));
  logger_.Log("Analyzing perceptron state...");
  for (const auto& sample : dataset) {
    if (abort_task) {
      logger_.Log("Aborting training", Logger::LogLevel::kWarning);
      ResetAbort();
      return;
    }

    SetInput(sample.pixels);
    double true_ans = sample.letter;
    double predict_ans = ForwardFeed();
    confusion_mat[predict_ans][true_ans] += 1;
  }

  struct ConfusionData {
    int TP, TN, FP, FN;
    int letter;
  };
  std::vector<ConfusionData> c_data_v;
  c_data_v.reserve(kNumOfLetters);
  logger_.Log("Collecting confusion data...");
  for (size_t i = 0; i < kNumOfLetters; ++i) {
    int TP, TN, FP, FN;
    TP = confusion_mat[i][i];
    FN = FP = -TP;
    TN = 0;
    for (size_t j = 0; j < kNumOfLetters; ++j) {
      FP += confusion_mat[i][j];
      FN += confusion_mat[j][i];
    }
    for (size_t j = 0; j < kNumOfLetters; ++j)
      for (size_t k = 0; k < kNumOfLetters; ++k)
        if (j != i && k != i) TN += confusion_mat[j][k];
    c_data_v.push_back({TP, TN, FP, FN, (char)('A' + i)});
    logger_.Log(std::string("Analyzed letter '") + (char)('A' + i) + "'.");
    QThread::msleep(100);
  }
  // logger_.Log(log.str());
  // log.str("");
  std::stringstream log;
  log << std::setprecision(3);
  logger_.Log("Calculating results...");
  for (auto& cd : c_data_v) {
    double accuracy = 0;
    double recall = 0;
    double precision = 0;
    double f_measure = 0;
    accuracy = (double)(cd.TP + cd.TN) / (cd.TP + cd.TN + cd.FP + cd.FN);
    recall = (double)(cd.TP) / (cd.TP + cd.FN);
    recall = std::isnan(recall) ? 0 : recall;
    precision = (double)(cd.TP) / (cd.TP + cd.FP);
    precision = std::isnan(precision) ? 0 : precision;
    f_measure = 2 * (precision * recall * std::pow(10, 4)) /
                (precision * 100 + recall * 100);
    f_measure = std::isnan(f_measure) ? 0 : f_measure;
    log << "\n\tLetter:\t " << (char)cd.letter << "\n\tAccuracy:\t "
        << accuracy * 100 << "%\n\tRecall:\t " << recall * 100
        << "%\n\tPrecision:\t " << precision * 100 << "%\n\tF measure:\t "
        << f_measure << "%\n\n";
    QThread::msleep(100);
    logger_.Log(log.str());
    log.str("");
  }
  logger_.Log("Perceptron state analyzis is complete.");
}

std::vector<CharInfo> Perceptron::ReadDatasetFile(
    const std::string& file_path) {
  static s21::FileManager file_manager;
  std::vector<CharInfo> data;
  logger_.Log("Reading dataset file.", Logger::LogLevel::kInfo);
  if (EndsWithBin(file_path))
    data = file_manager.ReadDatasetFromBin(file_path);
  else
    data = file_manager.ReadDatasetFromTxt(file_path);

  logger_.Log("Dataset is loaded.", Logger::LogLevel::kInfo);
  return data;
}

}  // namespace s21

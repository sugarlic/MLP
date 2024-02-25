#ifndef TRAININGDIALOG_H
#define TRAININGDIALOG_H

#include <QCloseEvent>
#include <QSettings>
#include <QWidget>

#include "GraphView.h"
#include "Logger.h"
#include "NetworkControler.h"
#include "QtConcurrent/QtConcurrent"
namespace Ui {
class TrainingDialog;
}
namespace s21 {

class TrainingDialog : public QWidget {
  Q_OBJECT

 public:
  explicit TrainingDialog(NetworkControler *c, QWidget *parent = nullptr);
  ~TrainingDialog();
  QFuture<void> training_task;
 signals:
  void TrainingStarted();
  void TrainingFinished(std::vector<double>);
 private slots:
  void closeEvent(QCloseEvent *event) override;
  void showEvent(QShowEvent *event) override;
  void Start();
  void Return();
  void SetLayersN(int n);
  void SetPerceptronType();
  void AbortTraining();
  void LoadSettings();
  void SaveSettings();
  void LoadGraph(std::vector<double> data);
  void RecreateGraph();
  void FetchPerceptronSettings();

 private:
  GraphView *graph_ = nullptr;
  PerceptronType cur_p_type_;
  Logger logger_{"TrainingDialog"};
  QSettings settings_;
  NetworkControler *c_{};
  ::Ui::TrainingDialog *ui_;
  void SetupConnects();
  bool CheckControler();
};
}  // namespace s21

#endif  // TRAININGDIALOG_H

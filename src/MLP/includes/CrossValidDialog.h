#ifndef CROSSVALIDDIALOG_H
#define CROSSVALIDDIALOG_H

#include <QCloseEvent>
#include <QSettings>
#include <QWidget>

#include "Logger.h"
#include "NetworkControler.h"
#include "QtConcurrent/QtConcurrent"
namespace Ui {
class CrossValidDialog;
}
namespace s21 {

class CrossValidDialog : public QWidget {
  Q_OBJECT

 public:
  explicit CrossValidDialog(NetworkControler *c, QWidget *parent = nullptr);
  ~CrossValidDialog();
  QFuture<void> training_task;
 signals:
  void CrossValidStarted();
  void CrossValidFinished();
 private slots:
  void showEvent(QShowEvent *event) override;
  void closeEvent(QCloseEvent *event) override;
  void Start();
  void Return();
  void SetLayersN(int n);
  void SetPerceptronType();
  void AbortTraining();
  void LoadSettings();
  void SaveSettings();

 private:
  PerceptronType cur_p_type_;
  Logger logger_{"CrossValidDialog"};
  QSettings settings_;
  NetworkControler *c_{};
  ::Ui::CrossValidDialog *ui_;
  void SetupConnects();
  bool CheckControler();
};
}  // namespace s21

#endif  // CROSSVALIDDIALOG_H

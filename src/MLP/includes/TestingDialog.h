#ifndef TESTINGDIALOG_H
#define TESTINGDIALOG_H

#include <QCloseEvent>
#include <QSettings>
#include <QWidget>

#include "Logger.h"
#include "NetworkControler.h"
#include "QtConcurrent/QtConcurrent"
namespace Ui {
class TestingDialog;
}
namespace s21 {

class TestingDialog : public QWidget {
  Q_OBJECT

 public:
  explicit TestingDialog(NetworkControler *c, QWidget *parent = nullptr);
  ~TestingDialog();
  QFuture<void> testing_task;
 signals:
  void TestingStarted();
  void TestingFinished();
 private slots:
  void showEvent(QShowEvent *event) override;
  void closeEvent(QCloseEvent *event) override;
  void Start();
  void Return();
  void SetLayersN(int n);
  void SetPerceptronType();
  void AbortTesting();
  void LoadSettings();
  void SaveSettings();

 private:
  PerceptronType cur_p_type_;
  Logger logger_{"TestingDialog"};
  QSettings settings_;
  NetworkControler *c_{};
  ::Ui::TestingDialog *ui_;
  void SetupConnects();
  bool CheckControler();
};
}  // namespace s21

#endif  // TESTINGDIALOG_H

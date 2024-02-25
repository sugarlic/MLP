#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <Logger.h>
#include <Painter.h>

#include <QMainWindow>
#include <QSettings>

#include "CrossValidDialog.h"
#include "NetworkControler.h"
#include "TestingDialog.h"
#include "TrainingDialog.h"
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE
namespace s21 {
class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  explicit MainWindow(QWidget *parent = nullptr,
                      s21::NetworkControler *c = nullptr);
  ~MainWindow();
 private slots:
  void ShowPainter();
  void LoadWeights();
  void SaveWeights();
  void LoadBMPImage();
  void ShowTrainingDialog();
  void ShowTestingDialog();
  void ShowCrossValidDialog();

 private:
  void closeEvent(QCloseEvent *event) override;
  bool CheckControler();
  void SetWidgetsEnable(bool enabled);
  void SetupConnects();
  QSettings settings_;
  ::Ui::MainWindow *ui_;
  s21::NetworkControler *c_{};
  std::unique_ptr<Painter> painter_;
  std::unique_ptr<TrainingDialog> train_dialog_;
  std::unique_ptr<TestingDialog> test_dialog_;
  std::unique_ptr<CrossValidDialog> cross_valid_dialog_;
  Logger logger_{"MainWindow"};
};
}  // namespace s21

#endif  // MAINWINDOW_H

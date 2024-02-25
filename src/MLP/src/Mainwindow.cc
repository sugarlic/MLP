#include "Mainwindow.h"

#include <QDebug>
#include <QFileDialog>
#include <QFileInfo>

#include "FileManager.h"
#include "QThread"
#include "ui_mainwindow.h"
namespace s21 {
MainWindow::MainWindow(QWidget *parent, NetworkControler *c)
    : QMainWindow(parent), ui_(new ::Ui::MainWindow), c_{c} {
  ui_->setupUi(this);
  painter_ = std::make_unique<Painter>(c);
  train_dialog_ = std::make_unique<TrainingDialog>(c);
  test_dialog_ = std::make_unique<TestingDialog>(c);
  cross_valid_dialog_ = std::make_unique<CrossValidDialog>(c);

  logger_.SetOutputer(ui_->LogBoxTE);
  SetupConnects();
}

MainWindow::~MainWindow() { delete ui_; }

void MainWindow::ShowPainter() {
  painter_->Clear();
  painter_->show();
  painter_->raise();
}

void MainWindow::LoadWeights() {
  if (!CheckControler()) return;
  FileManager fm;
  auto path = fm.GetFilePath("(*.wt)");
  c_->ReadWeights(path.toStdString());
}

void MainWindow::SaveWeights() {
  if (!CheckControler()) return;
  FileManager fm;
  auto path = fm.GetSaveFilePath("(*.wt)");
  c_->SaveWeights(path.toStdString());
}

void MainWindow::LoadBMPImage() {
  if (!CheckControler()) return;
  FileManager fm;
  auto path = fm.GetFilePath("BMP (*.bmp)");
  auto image = fm.OpenBMP(path);
  if (image.isNull()) {
    logger_.Log("Bad file", Logger::LogLevel::kError);
    return;
  }
  int ans = c_->AnalyzeImg(image);
  if (ans == -1) return;
  std::string log = "Determined letter: ";
  log += (char)('A' + ans);
  logger_.Log(log, Logger::LogLevel::kInfo);
}

void MainWindow::ShowTrainingDialog() {
  train_dialog_->show();
  train_dialog_->raise();
}
void MainWindow::ShowTestingDialog() {
  test_dialog_->show();
  test_dialog_->raise();
}
void MainWindow::ShowCrossValidDialog() {
  cross_valid_dialog_->show();
  cross_valid_dialog_->raise();
}
void MainWindow::closeEvent(QCloseEvent *event) {
  if (CheckControler() && c_->GetState() != Network::kIdle) {
    logger_.Log(
        "Neural networks is still working. Please wait or abort current task.",
        Logger::LogLevel::kWarning);
    event->ignore();
    return;
  }
  if (train_dialog_ != nullptr) train_dialog_->close();
  if (test_dialog_ != nullptr) test_dialog_->close();
  if (cross_valid_dialog_ != nullptr) cross_valid_dialog_->close();
  if (painter_ != nullptr) painter_->close();
  event->accept();
}

bool MainWindow::CheckControler() {
  if (!c_) {
    logger_.Log("Missing network controler", Logger::LogLevel::kError);
    return false;
  }
  return true;
}

void MainWindow::SetWidgetsEnable(bool enabled) {
  ui_->WeightsSaveB->setEnabled(enabled);
  ui_->WeightsReadB->setEnabled(enabled);
  ui_->DrawB->setEnabled(enabled);
  ui_->LoadBMP_B->setEnabled(enabled);
}

void MainWindow::SetupConnects() {
  connect(ui_->StartTrainingB, &QPushButton::clicked, this,
          &MainWindow::ShowTrainingDialog);
  connect(ui_->TestingB, &QPushButton::clicked, this,
          &MainWindow::ShowTestingDialog);
  connect(ui_->CrossValidB, &QPushButton::clicked, this,
          &MainWindow::ShowCrossValidDialog);
  connect(ui_->DrawB, &QPushButton::clicked, this, &MainWindow::ShowPainter);
  connect(ui_->WeightsReadB, &QPushButton::clicked, this,
          &MainWindow::LoadWeights);
  connect(ui_->WeightsSaveB, &QPushButton::clicked, this,
          &MainWindow::SaveWeights);
  connect(ui_->LoadBMP_B, &QPushButton::clicked, this,
          &MainWindow::LoadBMPImage);

  connect(train_dialog_.get(), &TrainingDialog::TrainingStarted, this, [=]() {
    painter_->close();
    SetWidgetsEnable(false);
  });
  connect(train_dialog_.get(), &TrainingDialog::TrainingFinished, this,
          [=]() { SetWidgetsEnable(true); });

  connect(test_dialog_.get(), &TestingDialog::TestingStarted, this, [=]() {
    painter_->close();
    SetWidgetsEnable(false);
  });
  connect(test_dialog_.get(), &TestingDialog::TestingFinished, this,
          [=]() { SetWidgetsEnable(true); });

  connect(cross_valid_dialog_.get(), &CrossValidDialog::CrossValidStarted, this,
          [=]() {
            painter_->close();
            SetWidgetsEnable(false);
          });
  connect(cross_valid_dialog_.get(), &CrossValidDialog::CrossValidFinished,
          this, [=]() { SetWidgetsEnable(true); });
}
}  // namespace s21

#include "TestingDialog.h"

#include "FileManager.h"
#include "ui_TestingDialog.h"
namespace s21 {

TestingDialog::TestingDialog(NetworkControler *c, QWidget *parent)
    : QWidget(parent), c_(c), ui_(new ::Ui::TestingDialog) {
  ui_->setupUi(this);
  SetupConnects();
  LoadSettings();
}

TestingDialog::~TestingDialog() {
  if (CheckControler() && c_->GetState() == Network::kTraining) {
    AbortTesting();
    testing_task.waitForFinished();
  }
  delete ui_;
}
void TestingDialog::showEvent(QShowEvent *event) {
  if (!CheckControler()) return;
  auto p_type = c_->GetPerceptronType();
  if (p_type == kMatrix) ui_->MatrixRB->setChecked(true);
  if (p_type == kGraph) ui_->GraphRB->setChecked(true);
  cur_p_type_ = p_type;
  auto h_l_count = c_->GetHiddenLayersCount();
  ui_->HiddenLayersSB->setValue(h_l_count);
  event->accept();
}
void TestingDialog::closeEvent(QCloseEvent *event) {
  SaveSettings();
  event->accept();
}
void TestingDialog::SaveSettings() {
  settings_.beginGroup("Testing_Dialog");

  settings_.beginWriteArray("file_paths");
  int i = 0;
  for (auto &file_path : ui_->FileImportWid->ExportPathsQ()) {
    settings_.setArrayIndex(i++);
    settings_.setValue("path", file_path);
  }
  settings_.endArray();
  auto sample_part = ui_->SampleNSB->value();
  auto layers_n = ui_->HiddenLayersSB->value();
  settings_.setValue("sample_part", sample_part);
  settings_.setValue("layers_n", layers_n);
  if (CheckControler()) {
    auto perc_type = c_->GetPerceptronType();
    settings_.setValue("perc_type", perc_type);
  }
  settings_.endGroup();
}
void TestingDialog::LoadSettings() {
  settings_.beginGroup("Testing_Dialog");
  int size = settings_.beginReadArray("file_paths");
  std::vector<QString> paths;
  for (int i = 0; i < size; ++i) {
    settings_.setArrayIndex(i);
    paths.push_back(settings_.value("path").value<QString>());
  }
  settings_.endArray();

  int sample_part = settings_.value("sample_part").value<double>();
  int layers_n = settings_.value("layers_n").value<int>();
  auto perc_type = settings_.value("perc_type").value<s21::PerceptronType>();
  if (perc_type == kMatrix) ui_->MatrixRB->setChecked(true);
  if (perc_type == kGraph) ui_->GraphRB->setChecked(true);
  cur_p_type_ = perc_type;
  ui_->SampleNSB->setValue(sample_part);
  ui_->HiddenLayersSB->setValue(layers_n);
  settings_.endGroup();
  ui_->FileImportWid->ImportPaths(paths);
}
void TestingDialog::Start() {
  if (!CheckControler()) return;
  FileManager fm;
  auto path = ui_->FileImportWid->GetSelectedPath();
  if (path.empty()) {
    logger_.Log("Bad file path", Logger::LogLevel::kError);
    return;
  }
  ui_->StartB->setDisabled(true);
  ui_->GraphRB->setDisabled(true);
  ui_->MatrixRB->setDisabled(true);
  ui_->AbortB->setEnabled(true);
  auto sample = ui_->SampleNSB->value();
  auto layers_n = ui_->HiddenLayersSB->value();
  testing_task = QtConcurrent::run([=]() {
                   c_->NetworkTest(sample, {path, cur_p_type_, layers_n});
                 }).then([=]() {
    ui_->StartB->setEnabled(true);
    ui_->GraphRB->setEnabled(true);
    ui_->MatrixRB->setEnabled(true);
    ui_->AbortB->setDisabled(true);

    emit TestingFinished();
  });
  emit TestingStarted();
}

void TestingDialog::Return() { close(); }

// TODO add layers n editing in network
void TestingDialog::SetLayersN(int n) { Q_UNUSED(n) }

void TestingDialog::SetPerceptronType() {
  if (!CheckControler()) return;
  auto rb = static_cast<QRadioButton *>(sender());
  if (rb == ui_->MatrixRB) cur_p_type_ = kMatrix;
  if (rb == ui_->GraphRB) cur_p_type_ = kGraph;
}

void TestingDialog::AbortTesting() {
  if (c_->GetState() != Network::kTraining) return;
  c_->AbortTraining();
  ui_->AbortB->setDisabled(true);
}
bool s21::TestingDialog::CheckControler() {
  if (!c_) {
    logger_.Log("Missing network controler", Logger::LogLevel::kError);
    return false;
  }
  return true;
}

void s21::TestingDialog::SetupConnects() {
  connect(ui_->AbortB, &QPushButton::clicked, this,
          &TestingDialog::AbortTesting);
  connect(ui_->StartB, &QPushButton::clicked, this, &TestingDialog::Start);
  connect(ui_->ReturnB, &QPushButton::clicked, this, &TestingDialog::Return);
  connect(ui_->HiddenLayersSB, &QSpinBox::valueChanged, this,
          &TestingDialog::SetLayersN);
  connect(ui_->MatrixRB, &QRadioButton::clicked, this,
          &TestingDialog::SetPerceptronType);
  connect(ui_->GraphRB, &QRadioButton::clicked, this,
          &TestingDialog::SetPerceptronType);
  if (!CheckControler()) return;
  auto p_type = c_->GetPerceptronType();
  if (p_type == kMatrix) ui_->MatrixRB->setChecked(true);
  if (p_type == kGraph) ui_->GraphRB->setChecked(true);
}
}  // namespace s21

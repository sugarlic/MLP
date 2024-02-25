#include "CrossValidDialog.h"

#include "FileManager.h"
#include "ui_CrossValidDialog.h"
namespace s21 {

CrossValidDialog::CrossValidDialog(NetworkControler *c, QWidget *parent)
    : QWidget(parent), c_(c), ui_(new ::Ui::CrossValidDialog) {
  ui_->setupUi(this);
  SetupConnects();
  LoadSettings();
}

CrossValidDialog::~CrossValidDialog() {
  if (CheckControler() && c_->GetState() == Network::kTraining) {
    AbortTraining();
    training_task.waitForFinished();
  }
  delete ui_;
}
void CrossValidDialog::showEvent(QShowEvent *event) {
  if (!CheckControler()) return;
  auto p_type = c_->GetPerceptronType();
  if (p_type == kMatrix) ui_->MatrixRB->setChecked(true);
  if (p_type == kGraph) ui_->GraphRB->setChecked(true);
  cur_p_type_ = p_type;

  auto h_l_count = c_->GetHiddenLayersCount();
  ui_->HiddenLayersSB->setValue(h_l_count);
  event->accept();
}

void CrossValidDialog::closeEvent(QCloseEvent *event) {
  SaveSettings();
  event->accept();
}
void CrossValidDialog::SaveSettings() {
  settings_.beginGroup("CrossValidDialog");

  settings_.beginWriteArray("file_paths");
  int i = 0;
  for (auto &file_path : ui_->FileImportWid->ExportPathsQ()) {
    settings_.setArrayIndex(i++);
    settings_.setValue("path", file_path);
  }
  settings_.endArray();
  int group_n = ui_->GroupNSB->value();
  int layers_n = ui_->HiddenLayersSB->value();
  settings_.setValue("group_n", group_n);
  settings_.setValue("layers_n", layers_n);
  if (CheckControler()) {
    auto perc_type = c_->GetPerceptronType();
    settings_.setValue("perc_type", perc_type);
  }
  settings_.endGroup();
}
void CrossValidDialog::LoadSettings() {
  settings_.beginGroup("CrossValidDialog");
  int size = settings_.beginReadArray("file_paths");
  std::vector<QString> paths;
  for (int i = 0; i < size; ++i) {
    settings_.setArrayIndex(i);
    paths.push_back(settings_.value("path").value<QString>());
  }
  settings_.endArray();

  int group_n = settings_.value("group_n").value<int>();
  int layers_n = settings_.value("layers_n").value<int>();
  auto perc_type = settings_.value("perc_type").value<s21::PerceptronType>();
  if (perc_type == kMatrix) ui_->MatrixRB->setChecked(true);
  if (perc_type == kGraph) ui_->GraphRB->setChecked(true);
  cur_p_type_ = perc_type;
  ui_->GroupNSB->setValue(group_n);
  ui_->HiddenLayersSB->setValue(layers_n);
  settings_.endGroup();
  ui_->FileImportWid->ImportPaths(paths);
}
void CrossValidDialog::Start() {
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
  auto group_n = ui_->GroupNSB->value();
  auto layers_n = ui_->HiddenLayersSB->value();
  training_task =
      QtConcurrent::run([=]() {
        c_->NetworkCrossValidationTrain(group_n, {path, cur_p_type_, layers_n});
      }).then([=]() {
        ui_->StartB->setEnabled(true);
        ui_->GraphRB->setEnabled(true);
        ui_->MatrixRB->setEnabled(true);
        ui_->AbortB->setDisabled(true);

        emit CrossValidFinished();
      });
  emit CrossValidStarted();
}

void CrossValidDialog::Return() { close(); }

void CrossValidDialog::SetLayersN(int n) { Q_UNUSED(n) }

void CrossValidDialog::SetPerceptronType() {
  if (!CheckControler()) return;
  auto rb = static_cast<QRadioButton *>(sender());
  if (rb == ui_->MatrixRB) cur_p_type_ = kMatrix;
  if (rb == ui_->GraphRB) cur_p_type_ = kGraph;
}

void CrossValidDialog::AbortTraining() {
  if (c_->GetState() != Network::kCrossTraining) return;
  c_->AbortTraining();
  ui_->AbortB->setDisabled(true);
}
bool s21::CrossValidDialog::CheckControler() {
  if (!c_) {
    logger_.Log("Missing network controler", Logger::LogLevel::kError);
    return false;
  }
  return true;
}

void s21::CrossValidDialog::SetupConnects() {
  connect(ui_->AbortB, &QPushButton::clicked, this,
          &CrossValidDialog::AbortTraining);
  connect(ui_->StartB, &QPushButton::clicked, this, &CrossValidDialog::Start);
  connect(ui_->ReturnB, &QPushButton::clicked, this, &CrossValidDialog::Return);
  connect(ui_->HiddenLayersSB, &QSpinBox::valueChanged, this,
          &CrossValidDialog::SetLayersN);
  connect(ui_->MatrixRB, &QRadioButton::clicked, this,
          &CrossValidDialog::SetPerceptronType);
  connect(ui_->GraphRB, &QRadioButton::clicked, this,
          &CrossValidDialog::SetPerceptronType);
  if (!CheckControler()) return;
  auto p_type = c_->GetPerceptronType();
  if (p_type == kMatrix) ui_->MatrixRB->setChecked(true);
  if (p_type == kGraph) ui_->GraphRB->setChecked(true);
}
}  // namespace s21

#include "TrainingDialog.h"

#include "FileManager.h"
#include "ui_TrainingDialog.h"
namespace s21 {

TrainingDialog::TrainingDialog(NetworkControler *c, QWidget *parent)
    : QWidget(parent), c_(c), ui_(new ::Ui::TrainingDialog) {
  ui_->setupUi(this);
  SetupConnects();
  LoadSettings();
}

TrainingDialog::~TrainingDialog() {
  if (CheckControler() && c_->GetState() == Network::kTraining) {
    AbortTraining();
    training_task.waitForFinished();
  }
  if (graph_) {
    graph_->close();
    delete graph_;
  }
  delete ui_;
}

void TrainingDialog::closeEvent(QCloseEvent *event) {
  SaveSettings();
  event->accept();
}

void TrainingDialog::showEvent(QShowEvent *event) {
  if (!CheckControler()) return;
  auto p_type = c_->GetPerceptronType();
  if (p_type == kMatrix) ui_->MatrixRB->setChecked(true);
  if (p_type == kGraph) ui_->GraphRB->setChecked(true);
  cur_p_type_ = p_type;
  auto h_l_count = c_->GetHiddenLayersCount();
  ui_->HiddenLayersSB->setValue(h_l_count);
  event->accept();
}
void TrainingDialog::SaveSettings() {
  settings_.beginGroup("Training_Dialog");

  settings_.beginWriteArray("file_paths");
  int i = 0;
  for (auto &file_path : ui_->FileImportWid->ExportPathsQ()) {
    settings_.setArrayIndex(i++);
    settings_.setValue("path", file_path);
  }
  settings_.endArray();
  int epoch_n = ui_->EpochNSB->value();
  int layers_n = ui_->HiddenLayersSB->value();
  settings_.setValue("epoch_n", epoch_n);
  settings_.setValue("layers_n", layers_n);
  if (CheckControler()) {
    auto perc_type = c_->GetPerceptronType();
    settings_.setValue("perc_type", perc_type);
  }
  settings_.endGroup();
}

void TrainingDialog::LoadGraph(std::vector<double> data) {
  if (data.empty()) return;
  graph_->show();
  GraphView::GraphData d;
  for (size_t i = 0; i < data.size(); ++i) {
    d.first.push_back(i);
    d.second.push_back(data[i]);
  }
  graph_->PlotExpression(d);
}

void TrainingDialog::RecreateGraph() {
  if (graph_) {
    graph_->close();
    delete graph_;
  }
  graph_ = new GraphView();
}

void TrainingDialog::FetchPerceptronSettings() {
  auto p_type = c_->GetPerceptronType();
  if (p_type == kMatrix) ui_->MatrixRB->setChecked(true);
  if (p_type == kGraph) ui_->GraphRB->setChecked(true);
}
void TrainingDialog::LoadSettings() {
  settings_.beginGroup("Training_Dialog");
  int size = settings_.beginReadArray("file_paths");
  std::vector<QString> paths;
  for (int i = 0; i < size; ++i) {
    settings_.setArrayIndex(i);
    paths.push_back(settings_.value("path").value<QString>());
  }
  settings_.endArray();

  int epoch_n = settings_.value("epoch_n").value<int>();
  int layers_n = settings_.value("layers_n").value<int>();
  auto perc_type = settings_.value("perc_type").value<s21::PerceptronType>();
  if (perc_type == kMatrix) ui_->MatrixRB->setChecked(true);
  if (perc_type == kGraph) ui_->GraphRB->setChecked(true);
  cur_p_type_ = perc_type;
  ui_->EpochNSB->setValue(epoch_n);
  ui_->HiddenLayersSB->setValue(layers_n);
  settings_.endGroup();
  ui_->FileImportWid->ImportPaths(paths);
}
void TrainingDialog::Start() {
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
  auto epochs = ui_->EpochNSB->value();
  auto layers_n = ui_->HiddenLayersSB->value();

  RecreateGraph();

  training_task =
      QtConcurrent::run([=]() {
        return c_->NetworkTrain(epochs, {path, cur_p_type_, layers_n});
      }).then([=](std::vector<double> errors) {
        ui_->StartB->setEnabled(true);
        ui_->GraphRB->setEnabled(true);
        ui_->MatrixRB->setEnabled(true);
        ui_->AbortB->setDisabled(true);
        emit TrainingFinished(errors);
      });
  emit TrainingStarted();
}

void TrainingDialog::Return() { close(); }

void TrainingDialog::SetLayersN(int n) { Q_UNUSED(n) }

void TrainingDialog::SetPerceptronType() {
  if (!CheckControler()) return;
  auto rb = static_cast<QRadioButton *>(sender());
  if (rb == ui_->MatrixRB) cur_p_type_ = kMatrix;
  if (rb == ui_->GraphRB) cur_p_type_ = kGraph;
}

void TrainingDialog::AbortTraining() {
  if (c_->GetState() != Network::kTraining) return;
  c_->AbortTraining();
  ui_->AbortB->setDisabled(true);
}
bool s21::TrainingDialog::CheckControler() {
  if (!c_) {
    logger_.Log("Missing network controler", Logger::LogLevel::kError);
    return false;
  }
  return true;
}

void s21::TrainingDialog::SetupConnects() {
  connect(ui_->AbortB, &QPushButton::clicked, this,
          &TrainingDialog::AbortTraining);
  connect(ui_->StartB, &QPushButton::clicked, this, &TrainingDialog::Start);
  connect(ui_->ReturnB, &QPushButton::clicked, this, &TrainingDialog::Return);
  connect(ui_->HiddenLayersSB, &QSpinBox::valueChanged, this,
          &TrainingDialog::SetLayersN);
  connect(ui_->MatrixRB, &QRadioButton::clicked, this,
          &TrainingDialog::SetPerceptronType);
  connect(ui_->GraphRB, &QRadioButton::clicked, this,
          &TrainingDialog::SetPerceptronType);
  connect(this, &TrainingDialog::TrainingFinished, this,
          &TrainingDialog::LoadGraph);
  if (!CheckControler()) return;
  auto p_type = c_->GetPerceptronType();
  if (p_type == kMatrix) ui_->MatrixRB->setChecked(true);
  if (p_type == kGraph) ui_->GraphRB->setChecked(true);
}
}  // namespace s21

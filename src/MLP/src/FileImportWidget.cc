#include "FileImportWidget.h"

#include <QFileDialog>
#include <QFileInfo>

#include "FileManager.h"
#include "ui_FileImportWidget.h"
namespace s21 {
FileImportWidget::FileImportWidget(QWidget* parent)
    : QWidget(parent), ui(new Ui::FileImportWidget) {
  Setup();
}

FileImportWidget::~FileImportWidget() { delete ui; }

void FileImportWidget::ImportPaths(QVector<QString> paths) {
  ImportPaths((std::vector<QString>){paths.begin(), paths.end()});
}

void FileImportWidget::ImportPaths(std::vector<QString> paths) {
  std::vector<std::string> new_paths;
  for (const auto& path : paths) new_paths.push_back(path.toStdString());
  ImportPaths(new_paths);
}

void FileImportWidget::ImportPaths(std::vector<std::string> paths) {
  for (const auto& path : paths) {
    const auto str = path.c_str();
    if (!QFileInfo::exists(str)) continue;
    file_paths_ << str;
    ui->FileListCB->addItem(str);
  }
}

std::vector<std::string> FileImportWidget::ExportPaths() {
  return PathsToStdVector();
}

QVector<QString> FileImportWidget::ExportPathsQ() { return file_paths_; }

std::string FileImportWidget::GetSelectedPath() {
  auto file_path = ui->FileListCB->currentText().toStdString();
  return file_path;
}

void FileImportWidget::Setup() {
  ui->setupUi(this);
  connect(ui->OpenFileBut, &QAbstractButton::clicked, this,
          &FileImportWidget::ChooseFile);
}

void FileImportWidget::ChooseFile() {
  FileManager fm;
  auto file_path = fm.GetFilePath("Dataset (*.csv *.bin)");
  file_paths_ << file_path;
  ui->FileListCB->addItem(file_path);
  ui->FileListCB->setCurrentIndex(ui->FileListCB->findText(file_path));
}

std::vector<std::string> FileImportWidget::PathsToStdVector() {
  std::vector<std::string> new_paths;
  for (const auto& path : file_paths_) new_paths.push_back(path.toStdString());
  return new_paths;
}
}  // namespace s21

#ifndef FILEIMPORTWIDGET_H
#define FILEIMPORTWIDGET_H

#include <QWidget>

namespace Ui {
class FileImportWidget;
}
namespace s21 {
class FileImportWidget : public QWidget {
  Q_OBJECT
 public:
  explicit FileImportWidget(QWidget *parent = nullptr);
  ~FileImportWidget();
  void ImportPaths(QVector<QString> paths);
  void ImportPaths(std::vector<QString> paths);
  void ImportPaths(std::vector<std::string> paths);

  std::vector<std::string> ExportPaths();
  QVector<QString> ExportPathsQ();
  std::string GetSelectedPath();
 private slots:
  void ChooseFile();

 private:
  std::vector<std::string> PathsToStdVector();
  void Setup();
  QVector<QString> file_paths_;
  ::Ui::FileImportWidget *ui;
};
}  // namespace s21
#endif  // FILEIMPORTWIDGET_H

#ifndef FILEMANAGER_H
#define FILEMANAGER_H
#include <QFileDialog>
#include <vector>

#include "EasyBMP.h"
#include "Logger.h"
namespace s21 {
struct CharInfo {
  std::vector<double> pixels;
  int letter;
};
class FileManager {
 public:
  FileManager() = default;
  QImage OpenBMP(QString file_path);
  QString GetFilePath(QString filter = "");
  QString GetSaveFilePath(QString filter = "");
  std::vector<CharInfo> ReadDatasetFromTxt(const std::string path);
  void SaveDatasetToBin(const std::vector<CharInfo>& data,
                        const std::string& path);
  std::vector<CharInfo> ReadDatasetFromBin(const std::string& path);

  std::string ReadProgramFile(const std::string& path);

 private:
  Logger logger_{"FileManager"};
  QImage CreateQImage(BMP bmp_image);
};
}  // namespace s21
#endif  // FILEMANAGER_H

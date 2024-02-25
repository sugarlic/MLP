#include "FileManager.h"

#include <fstream>
#include <sstream>

#include "EasyBMP.h"
namespace s21 {
QImage FileManager::CreateQImage(BMP bmp_image) {
  int width = bmp_image.TellWidth();
  int height = bmp_image.TellHeight();

  QImage q_img(width, height, QImage::Format_RGB32);

  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      RGBApixel pixel = bmp_image.GetPixel(x, y);
      q_img.setPixel(x, y, qRgb(pixel.Red, pixel.Green, pixel.Blue));
    }
  }

  return q_img;
}
QImage FileManager::OpenBMP(QString file_path) {
  std::string img_name = file_path.toStdString();
  std::ifstream file(img_name);
  if (!file.is_open()) {
    logger_.Log("Can't open image", Logger::LogLevel::kError);
    return {};
  };
  BMP img;
  img.ReadFromFile(img_name.c_str());
  return CreateQImage(img);
}
// filter - "(*.txt)" etc.
QString FileManager::GetFilePath(QString filter) {
  QString name = qgetenv("USER");
  if (name.isEmpty()) name = qgetenv("USERNAME");
  QString file_path = QFileDialog::getOpenFileName(nullptr, "Open File",
                                                   "/Users/" + name, filter);
  return file_path;
}
// filter - "(*.txt)" etc.
QString FileManager::GetSaveFilePath(QString filter) {
  QString name = qgetenv("USER");
  if (name.isEmpty()) name = qgetenv("USERNAME");
  QString save_path = QFileDialog::getSaveFileName(nullptr, "Save file",
                                                   "/Users/" + name, filter);
  return save_path;
}

std::vector<s21::CharInfo> FileManager::ReadDatasetFromTxt(
    const std::string path) {
  std::ifstream file{};
  std::vector<s21::CharInfo> data{};
  file.open(path);
  if (!file.is_open()) return data;

  std::string line;
  while (std::getline(file, line)) {
    std::istringstream ss(line);
    s21::CharInfo char_inf;
    std::string cell;

    std::getline(ss, cell, ',');
    char_inf.letter = std::stoi(cell) - 1;

    while (std::getline(ss, cell, ',')) {
      char_inf.pixels.push_back(std::stod(cell) / 255);
    }

    data.push_back(char_inf);
  }
  file.close();
  return data;
}

void FileManager::SaveDatasetToBin(const std::vector<CharInfo> &data,
                                   const std::string &path) {
  std::ofstream file(path, std::ios::binary);
  if (!file) {
    logger_.Log("Bad save path.", Logger::LogLevel::kError);
    return;
  }
  size_t data_size = data.size();
  file.write(reinterpret_cast<char *>(&data_size), sizeof(size_t));
  for (const s21::CharInfo &char_info : data) {
    size_t pixel_vec_size = char_info.pixels.size();
    file.write(reinterpret_cast<char *>(&pixel_vec_size), sizeof(size_t));

    file.write(reinterpret_cast<const char *>(char_info.pixels.data()),
               pixel_vec_size * sizeof(double));

    file.write(reinterpret_cast<const char *>(&char_info.letter), sizeof(int));
  }
  file.close();
}

std::vector<CharInfo> FileManager::ReadDatasetFromBin(const std::string &path) {
  std::vector<CharInfo> data;
  std::ifstream file(path, std::ios::binary);
  if (!file) {
    logger_.Log("Bad load path.", Logger::LogLevel::kError);
    return data;
  }
  size_t data_size;
  file.read(reinterpret_cast<char *>(&data_size), sizeof(size_t));
  data.resize(data_size);

  for (CharInfo &char_info : data) {
    size_t pixel_vec_size;

    file.read(reinterpret_cast<char *>(&pixel_vec_size), sizeof(size_t));

    char_info.pixels.resize(pixel_vec_size);

    file.read(reinterpret_cast<char *>(char_info.pixels.data()),
              pixel_vec_size * sizeof(double));
    file.read(reinterpret_cast<char *>(&char_info.letter), sizeof(int));
  }

  file.close();
  return data;
}
std::string FileManager::ReadProgramFile(const std::string &path) {
  std::ifstream file(path);
  if (!file.is_open()) {
    logger_.Log("Can't open program file.", Logger::LogLevel::kError);
    return {};
  }
  return std::string(std::istreambuf_iterator<char>(file),
                     (std::istreambuf_iterator<char>()));
}
}  // namespace s21

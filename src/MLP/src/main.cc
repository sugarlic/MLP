#include <QApplication>
#include <filesystem>
#include <memory>

#include "GraphPerceptron.h"
#include "Mainwindow.h"

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  std::filesystem::path executablePath = std::filesystem::absolute(argv[0]);
  std::filesystem::path executableDirectory = executablePath.parent_path();
  std::filesystem::current_path(executableDirectory);
  const int img_size = 28;
  const int letters_c = 26;
  const int input_l_size = img_size * img_size;
  const int hidden_l_size = 25 * 4;
  const int output_l_size = letters_c;
  std::vector<int> network_settings{input_l_size, hidden_l_size, hidden_l_size,
                                    output_l_size};
  a.setOrganizationName("MLP_esi");
  std::shared_ptr<s21::Network> n =
      std::make_shared<s21::Network>(network_settings);
  s21::NetworkControler c(n);
  s21::MainWindow w(nullptr, &c);
  w.show();
  return a.exec();
}

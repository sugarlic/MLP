#include <gtest/gtest.h>

#include <filesystem>
#include <memory>
#include <vector>

#include "FileManager.h"
#include "NetworkControler.h"

class NetworkTest : public ::testing::Test {
 protected:
  const int img_size = 28;
  const int letters_c = 26;
  const int input_l_size = img_size * img_size;
  const int hidden_l_size = 25 * 4;
  const int output_l_size = letters_c;
  std::vector<int> network_settings{input_l_size, hidden_l_size, hidden_l_size,
                                    output_l_size};
  std::shared_ptr<s21::Network> n =
      std::make_shared<s21::Network>(network_settings);
  s21::NetworkControler c{n};
};

TEST_F(NetworkTest, precision_test_matrix) {
  c.ReadWeights("res/Weights/M_2_100_92.wt");
  s21::NetworkArgs data{"res/emnist-letters-test.csv", s21::kMatrix,
                        2};  // 2 - hidden layers count
  std::vector<double> res = c.NetworkTest(1, data);

  EXPECT_TRUE(res[0] > 80);  // precision [%]
  EXPECT_TRUE(res[1] < 10);  // time [sec]
}

TEST_F(NetworkTest, precision_test_graph) {
  c.ReadWeights("res/Weights/G_2_100_82.wt");
  s21::NetworkArgs data{"res/emnist-letters-test.csv", s21::kGraph,
                        2};  // 2 - hidden layers count
  std::vector<double> res = c.NetworkTest(1, data);

  EXPECT_TRUE(res[0] > 70);  // precision [%]
  EXPECT_TRUE(res[1] < 10);  // time [sec]
}

TEST_F(NetworkTest, image_analyze_test_matrix) {
  c.ReadWeights("res/Weights/M_3_100_88.wt");
  s21::FileManager fm;
  auto image = fm.OpenBMP("res/Images/C.bmp");
  if (image.isNull()) {
    FAIL() << "No image file";
    return;
  }
  int ans = c.AnalyzeImg(image);
  EXPECT_EQ(ans, 2);  // C is 3nd(- 1) letter
}
TEST_F(NetworkTest, image_analyze_test_graph) {
  c.ReadWeights("res/Weights/G_2_100_82.wt");
  s21::FileManager fm;
  auto image = fm.OpenBMP("res/Images/Z.bmp");
  if (image.isNull()) {
    FAIL() << "No image file";
    return;
  }
  int ans = c.AnalyzeImg(image);
  EXPECT_EQ(ans, 25);  // Z is 26nd(- 1) letter
}

int main(int argc, char **argv) {
  std::filesystem::path currentPath = std::filesystem::absolute(argv[0]);
  std::filesystem::path parentFolder = currentPath.parent_path();
  std::filesystem::current_path(parentFolder);

  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

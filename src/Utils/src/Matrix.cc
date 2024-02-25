#include <Matrix.h>

#include <chrono>
#include <cstdlib>
namespace s21 {

matrix CreateMatrix(int row, int col) {
  matrix matrix;
  matrix.reserve(row);
  for (int i = 0; i < row; ++i) matrix.push_back(std::vector<double>(col));
  return matrix;
};
void RandFillMatrix(matrix& matrix) {
  if (matrix.empty()) return;
  std::srand(std::time(nullptr));
  int rows = matrix.size();
  int cols = matrix[0].size();
  for (int i = 0; i < rows; ++i)
    for (int j = 0; j < cols; ++j)
      matrix[i][j] = static_cast<double>(std::rand()) / RAND_MAX * 2.0 - 1.0;
}
std::vector<double> MulVector(matrix& matrix, const std::vector<double>& v) {
  std::vector<double> res{};
  if (matrix.empty()) return res;
  if (matrix[0].size() == v.size()) {
    int rows = matrix.size();
    res.resize(rows);
    for (int i = 0; i < rows; ++i) {
      for (size_t j = 0; j < v.size(); ++j) {
        res[i] += matrix[i][j] * v[j];
      }
    }
  }
  return res;
}
void SumVectors(std::vector<double>& v1, const std::vector<double>& v2) {
  if (v1.size() == v2.size()) {
    for (size_t i = 0; i < v1.size(); ++i) v1[i] += v2[i];
  }
}
}  // namespace s21

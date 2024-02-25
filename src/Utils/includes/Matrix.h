#ifndef MATRIX_H
#define MATRIX_H

#include <vector>
namespace s21 {
using matrix = std::vector<std::vector<double>>;
matrix CreateMatrix(int row, int col);
void RandFillMatrix(matrix& matrix);
std::vector<double> MulVector(matrix& matrix, const std::vector<double>& v);
void SumVectors(std::vector<double>& v1, const std::vector<double>& v2);
}  // namespace s21

#endif  // MATRIX_H
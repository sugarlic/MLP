#ifndef ACTIVATIONFUNCTION_H
#define ACTIVATIONFUNCTION_H

#include <cmath>
#include <vector>

namespace s21 {
class ActivationFunction {
 public:
  void Use(std::vector<double> &values);
  double Use(double v);
  double UseDerivative(double v);
};
}  // namespace s21

#endif  // ACTIVATIONFUNCTION_H
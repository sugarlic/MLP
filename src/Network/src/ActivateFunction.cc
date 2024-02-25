#include <cmath>
#include <iostream>
#include <vector>

#include "ActivationFunction.h"
void s21::ActivationFunction::Use(std::vector<double>& values) {
  for (auto& v : values) v = 1 / (1 + exp(-v));
}

double s21::ActivationFunction::UseDerivative(double v) {
  return Use(v) / (1 - Use(v));
}

double s21::ActivationFunction::Use(double v) { return 1 / (1 + exp(-v)); }
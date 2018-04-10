#include <iostream>
#include "matrix.hpp"

using namespace thundercat;

Matrix::Matrix(unsigned int N, unsigned int M, unsigned int NZ):
  N(N), M(M), NZ(NZ) { }

bool Matrix::isSquare() {
  return N == M;
}

Matrix::~Matrix() = default;

std::vector<MatrixStripeInfo> *Matrix::getStripeInfos(unsigned int numPartitions) {
  std::cout << "getStripeInfos is not implemented for this type of matrix." << std::endl;
  exit(1);
}
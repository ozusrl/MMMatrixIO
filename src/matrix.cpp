#include "matrix.hpp"

using namespace thundercat;

Matrix::Matrix(unsigned int N, unsigned int M, unsigned int NZ):
  N(N), M(M), NZ(NZ) { }

bool Matrix::isSquare() {
  return N == M;
}

Matrix::~Matrix() = default;

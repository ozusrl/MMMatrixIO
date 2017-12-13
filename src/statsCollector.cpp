#include <iostream>
#include <stdio.h>
#include "matrix.hpp"
#include "mmmatrix.hpp"
#include <math.h>

using namespace thundercat;
using namespace std;

bool __DEBUG__ = false;

int main(int argc, const char *argv[]) {
  // Usage: ./test <matrixFilePath>
  if (argc < 2) {
    cerr << "You must give me a .mtx filename.\n";
    exit(1);
  }
  string matrixName(argv[1]);
  MMMatrix<double> *mmMatrix = MMMatrix<double>::fromFile(matrixName);
  CSRMatrix<double> *csrMatrix = mmMatrix->toCSR();

  // General info
  int N = csrMatrix->N;
  int M = csrMatrix->M;
  int NZ = csrMatrix->NZ;
  double meanRowLength = NZ / (double)N;
  bool symmetric = mmMatrix->isSymmetric();
  printf("%d %d %d %s %.5f ", N, M, NZ, symmetric ? "sym" : "unsym", meanRowLength);

  // Row length info
  int maxRowLength = 0;
  double disparity = 0;
  double sum = 0;
  double skewness = 0;
  for (int i = 0; i < N; i++) {
    int length = csrMatrix->rowPtr[i+1] - csrMatrix->rowPtr[i];
    if (length > maxRowLength) {
      maxRowLength = length;
    }
    double diff = length - meanRowLength;
    sum += diff * diff;
    skewness += (diff * diff * diff);
    double sumDistances = 0;
    if (length != 0){
      for (int j = csrMatrix->rowPtr[i]; j < csrMatrix->rowPtr[i+1] - 1; j++) {
        sumDistances += csrMatrix->colIndices[j+1] - csrMatrix->colIndices[j];
      }
      disparity += sumDistances / length;
    }
  }
  double variance = sum / N;
  double stdDev = sqrt(variance);
  disparity = disparity / N;
  double variation = stdDev / meanRowLength;
  skewness = (skewness / N) / pow(stdDev, 3.0);
  printf("%d %.5f %.5f %.5f %.5f", maxRowLength, stdDev, variation, skewness, disparity);

  printf("\n");
  return 0;
}


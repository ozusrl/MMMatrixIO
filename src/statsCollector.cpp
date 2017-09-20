#include <iostream>
#include <stdio.h>
#include "matrix.h"
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
  MMMatrix *mmMatrix = MMMatrix::fromFile(matrixName);
  CSRMatrix *csrMatrix = mmMatrix->toCSR();
  delete mmMatrix;

  // General info
  int N = csrMatrix->N;
  int M = csrMatrix->M;
  int NZ = csrMatrix->NZ;
  double meanRowLength = NZ / (double)N;
  printf("%d %d %d %.5f ", N, M, NZ, meanRowLength);

  // Row length info
  int maxRowLength = 0;
  double disparity = 0;
  double sum = 0;
  for (int i = 0; i < N; i++) {
    int length = csrMatrix->rowPtr[i+1] - csrMatrix->rowPtr[i];
    if (length > maxRowLength) {
      maxRowLength = length;
    }
    double diff = length - meanRowLength;
    sum += diff * diff;
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
  printf("%d %.5f %.5f ", maxRowLength, disparity/N, stdDev);

  printf("\n");
  return 0;
}


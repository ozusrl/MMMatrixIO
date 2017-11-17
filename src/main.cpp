#include <iostream>
#include "matrix.h"
#include "mmmatrix.h"

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
  cout << "############### MM  ##############\n";
  MMMatrix *mmMatrix = MMMatrix::fromFile(matrixName);
  mmMatrix->print();
  cout << "############### LD  ##############\n";
  MMMatrix *ldMatrix = mmMatrix->getLD();
  ldMatrix->print();
  cout << "############### UD  ##############\n";
  MMMatrix *udMatrix = mmMatrix->getUD();
  udMatrix->print();
  cout << "############### COO ##############\n";
  COOMatrix *cooMatrix = mmMatrix->toCOO();
  cooMatrix->print();
  cout << "############### CSR ##############\n";
  CSRMatrix *csrMatrix = mmMatrix->toCSR();
  csrMatrix->print();
  cout << "############### CSC ##############\n";
  CSCMatrix *cscMatrix = mmMatrix->toCSC();
  cscMatrix->print();
}


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
  MMMatrix<double> *mmMatrix = MMMatrix<double>::fromFile(matrixName);
  mmMatrix->print();
  cout << "############### LD  ##############\n";
  MMMatrix<double> *ldMatrix = mmMatrix->getLD();
  ldMatrix->print();
  cout << "############### UD  ##############\n";
  MMMatrix<double> *udMatrix = mmMatrix->getUD();
  udMatrix->print();
  cout << "############### COO ##############\n";
  COOMatrix<double> *cooMatrix = mmMatrix->toCOO();
  cooMatrix->print();
  cout << "############### CSR ##############\n";
  CSRMatrix<double> *csrMatrix = mmMatrix->toCSR();
  csrMatrix->print();
  cout << "############### CSC ##############\n";
  CSCMatrix<double> *cscMatrix = mmMatrix->toCSC();
  cscMatrix->print();
}


#include <iostream>
#include "matrix.hpp"
#include "mmmatrix.hpp"
#include "matrixprinter.hpp"

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
  MatrixPrinter::print(*mmMatrix);
  cout << "############### LD  ##############\n";
  MMMatrix<double> *ldMatrix = mmMatrix->getLD();
  MatrixPrinter::print(*ldMatrix);
  cout << "############### UD  ##############\n";
  MMMatrix<double> *udMatrix = mmMatrix->getUD();
  MatrixPrinter::print(*udMatrix);
  cout << "############### COO ##############\n";
  COOMatrix<double> *cooMatrix = mmMatrix->toCOO();
  MatrixPrinter::print(*cooMatrix);
  cout << "############### CSR ##############\n";
  CSRMatrix<double> *csrMatrix = mmMatrix->toCSR();
  MatrixPrinter::print(*csrMatrix);
  cout << "############### CSC ##############\n";
  CSCMatrix<double> *cscMatrix = mmMatrix->toCSC();
  MatrixPrinter::print(*cscMatrix);
}


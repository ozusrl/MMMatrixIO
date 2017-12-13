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
  std::unique_ptr<MMMatrix<double>> mmMatrix = MMMatrix<double>::fromFile(matrixName);
  MatrixPrinter::print(mmMatrix);
  cout << "############### LD  ##############\n";
  std::unique_ptr<MMMatrix<double>> ldMatrix = mmMatrix->getLD();
  MatrixPrinter::print(ldMatrix);
  cout << "############### UD  ##############\n";
  std::unique_ptr<MMMatrix<double>> udMatrix = mmMatrix->getUD();
  MatrixPrinter::print(udMatrix);
  cout << "############### COO ##############\n";
  std::unique_ptr<COOMatrix<double>> cooMatrix = mmMatrix->toCOO();
  MatrixPrinter::print(cooMatrix);
  cout << "############### CSR ##############\n";
  std::unique_ptr<CSRMatrix<double>> csrMatrix = mmMatrix->toCSR();
  MatrixPrinter::print(csrMatrix);
  cout << "############### CSC ##############\n";
  std::unique_ptr<CSCMatrix<double>> cscMatrix = mmMatrix->toCSC();
  MatrixPrinter::print(cscMatrix);
}


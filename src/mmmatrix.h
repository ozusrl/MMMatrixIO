#ifndef _THUNDERCAT_MMMATRIX_H
#define _THUNDERCAT_MMMATRIX_H

#include "matrix.h"
#include <vector>
#include <string>

namespace thundercat{
class MMElement {
 public:
  int rowIndex; int colIndex; double value;

  MMElement(int row, int col, double val);

  static bool compareRowMajor(const MMElement &elt1, const MMElement &elt2);

  static bool compareColumnMajor(const MMElement &elt1, const MMElement &elt2);
};

class MMMatrix : public Matrix {
 private:
  std::vector<MMElement> elements;
  bool symmetric;

 public:
  MMMatrix(unsigned int N, unsigned int M, unsigned int NZ);

  virtual ~MMMatrix();

  void add(int row, int col, double val);

  void print();

  void printAsMTX();

  bool hasFullDiagonal();

  bool isSymmetric();

  void setSymmetric(bool s);

  COOMatrix* toCOO();

  CSRMatrix* toCSR();

  CSCMatrix* toCSC();

  // Return a new matrix that contains the lower triangular part plus the diagonal
  MMMatrix* getLD();

  // Return a new matrix that contains the upper triangular part plus the diagonal
  MMMatrix* getUD();

  static MMMatrix* fromFile(std::string fileName);
};
}

#endif

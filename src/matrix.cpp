#include "matrix.h"
#include <iostream>
#include <algorithm>

#include "mmio.h"

using namespace thundercat;
using namespace std;

///======================
/// Matrix
///======================
Matrix::Matrix(unsigned int N, unsigned int M, unsigned int NZ):
  N(N), M(M), NZ(NZ) {
}

Matrix::~Matrix() {
  // do nothing
}

bool Matrix::isSquare() {
  return N == M;
}

///======================
/// MMElement
///======================
MMElement::MMElement(int row, int col, double val):
rowIndex(row), colIndex(col), value(val) {
  
}

bool MMElement::compareRowMajor(const MMElement &elt1, const MMElement &elt2) {
  if (elt1.rowIndex < elt2.rowIndex) return true;
  else if (elt2.rowIndex < elt1.rowIndex) return false;
  else return elt1.colIndex < elt2.colIndex;
}

bool MMElement::compareColumnMajor(const MMElement &elt1, const MMElement &elt2) {
  if (elt1.colIndex < elt2.colIndex) return true;
  else if (elt2.colIndex < elt1.colIndex) return false;
  else return elt1.rowIndex < elt2.rowIndex;
}

///======================
/// MMMatrix
///======================
MMMatrix::MMMatrix(unsigned int N, unsigned int M, unsigned int NZ):
Matrix(N, M, NZ) {
  symmetric = false;
}

MMMatrix::~MMMatrix() {
  // do nothing
}

void MMMatrix::add(int row, int col, double val) {
  elements.push_back(MMElement(row, col, val));
}

void MMMatrix::print() {
  cout << N << " " << M << " " << elements.size() << "\n";
  for (MMElement &elt : elements) {
    cout << elt.rowIndex << " "
    << elt.colIndex << " "
    << elt.value << "\n";
  }
}

void MMMatrix::printAsMTX() {
  cout << N << " " << M << " " << elements.size() << "\n";
  for (MMElement &elt : elements) {
    cout << (elt.rowIndex + 1) << " "
    << (elt.colIndex + 1) << " "
    << elt.value << "\n";
  }
}

bool MMMatrix::hasFullDiagonal() {
  if (N != M)
    return false;
  
  double *diagVals = new double[N];
  for (int i = 0; i < N; i++)
    diagVals[i] = 0.0;
  
  for (auto &elt : elements) {
    if (elt.rowIndex == elt.colIndex) {
      diagVals[elt.rowIndex] = elt.value;
    }
  }

  for (int i = 0; i < N; i++) {
    if (diagVals[i] == 0.0) {
      return false;
    }
  }
  
  delete[] diagVals;
  return true;
}

bool MMMatrix::isSymmetric() {
  return symmetric;
}

void MMMatrix::setSymmetric(bool s) {
  symmetric = s;
}

COOMatrix* MMMatrix::toCOO() {
  std::sort(elements.begin(), elements.end(), MMElement::compareRowMajor);
  
  long sz = elements.size();
  int *rows = new int[sz];
  int *cols = new int[sz];
  double *vals = new double[sz];
  
  unsigned int eltIndex = 0;
  for (auto &elt : elements) {
    rows[eltIndex] = elt.rowIndex;
    cols[eltIndex] = elt.colIndex;
    vals[eltIndex] = elt.value;
    eltIndex++;
  }
  
  return new COOMatrix(rows, cols, vals, N, M, sz);
}

CSRMatrix* MMMatrix::toCSR() {
  std::sort(elements.begin(), elements.end(), MMElement::compareRowMajor);
  
  long sz = elements.size();
  int *rows = new int[N + 1];
  rows[0] = 0;
  int *cols = new int[sz];
  double *vals = new double[sz];
  
  unsigned int eltIndex = 0;
  unsigned int rowIndex = 0;
  for (auto &elt : elements) {
    cols[eltIndex] = elt.colIndex;
    vals[eltIndex] = elt.value;
    while (rowIndex != elt.rowIndex) {
      rows[rowIndex + 1] = eltIndex;
      rowIndex++;
    }
    eltIndex++;
  }
  while (rowIndex < N) {
    rows[rowIndex + 1] = eltIndex;
    rowIndex++;
  }
  rows[N] = eltIndex;
  
  return new CSRMatrix(rows, cols, vals, N, M, sz);
}

CSCMatrix* MMMatrix::toCSC() {
  std::sort(elements.begin(), elements.end(), MMElement::compareColumnMajor);
  
  long sz = elements.size();
  int *rows = new int[sz];
  int *cols = new int[M + 1];
  cols[0] = 0;
  double *vals = new double[sz];
  
  unsigned int eltIndex = 0;
  unsigned int colIndex = 0;
  for (auto &elt : elements) {
    rows[eltIndex] = elt.rowIndex;
    vals[eltIndex] = elt.value;
    while (colIndex != elt.colIndex) {
      cols[colIndex + 1] = eltIndex;
      colIndex++;
    }
    eltIndex++;
  }
  while (colIndex < M) {
    cols[colIndex + 1] = eltIndex;
    colIndex++;
  }
  cols[M] = eltIndex;
  
  return new CSCMatrix(rows, cols, vals, N, M, sz);
}

MMMatrix* MMMatrix::getLD() {
  MMMatrix *matrix = new MMMatrix(N, M, NZ);
  unsigned int count = 0;
  for (auto &elt : elements) {
    if (elt.rowIndex >= elt.colIndex) {
      matrix->add(elt.rowIndex, elt.colIndex, elt.value);
      count++;
    }
  }
  matrix->NZ = count;
  return matrix;
}

MMMatrix* MMMatrix::getUD() {
  MMMatrix *matrix = new MMMatrix(N, M, NZ);
  unsigned int count = 0;
  for (auto &elt : elements) {
    if (elt.rowIndex <= elt.colIndex) {
      matrix->add(elt.rowIndex, elt.colIndex, elt.value);
      count++;
    }
  }
  matrix->NZ = count;
  return matrix;
}

// Caller of this method is responsible for destructing the
// returned matrix.
MMMatrix* MMMatrix::fromFile(string fileName) {
  FILE *f;
  if ((f = fopen(fileName.c_str(), "r")) == NULL) {
    std::cerr << "Problem opening file " << fileName << ".\n";
    exit(1);
  }
  
  MM_typecode matcode;
  if (mm_read_banner(f, &matcode) != 0) {
    std::cerr << "Could not process Matrix Market banner.\n";
    exit(1);
  }

  if (!mm_is_matrix(matcode) || !mm_is_coordinate(matcode) || !mm_is_sparse(matcode)) {
    std::cerr << "Only sparse matrices in coordinate format are handled.\n";
    exit(1);
  }
  
  if (mm_is_complex(matcode)) {
    std::cerr << "Complex matrices are not handled.\n";
    exit(1);
  }
  
  int N, M, NZ;
  if ((mm_read_mtx_crd_size(f, &N, &M, &NZ)) != 0) {
    std::cerr << "Could not read size information.\n";
    exit(1);
  }

  // Read rows, cols, vals
  MMMatrix *matrix = new MMMatrix(N, M, NZ);
  matrix->setSymmetric(mm_is_symmetric(matcode));
  int row; int col; double val;
  
  string line;
  for (int i = 0; i < NZ; ++i) {
    if (mm_is_pattern(matcode)) {
      // Pattern (i.e. connectivity) matrices do not contain val entry.
      // Such matrices are filled in with 1.0
      fscanf(f, "%d %d\n", &row, &col);
      val = 1.0;
    } else {
      fscanf(f, "%d %d %lg\n", &row, &col, &val);
    }
    // adjust to zero index
    matrix->add(row-1, col-1, val);
    if (mm_is_symmetric(matcode) && row != col) {
      matrix->add(col-1, row-1, val);
      matrix->NZ++;
    }
  }
  if (f !=stdin) {
    fclose(f);
  }
  return matrix;
}

///======================
/// COO
///======================
COOMatrix::COOMatrix(int* __restrict rows, int* __restrict cols, double* __restrict vals,
                     unsigned int N, unsigned int M, unsigned int NZ):
Matrix(N, M, NZ), rowIndices(rows), colIndices(cols), values(vals) {
  
}

COOMatrix::~COOMatrix() {
  delete[] rowIndices;
  delete[] colIndices;
  delete[] values;
}

void COOMatrix::print() {
  cout << "int rowIndices[" << NZ << "] = {\n";
  for(int i = 0; i < NZ; ++i) {
    cout << rowIndices[i] << ",\n";
  }
  cout << "};\n";

  cout << "int colIndices[" << NZ << "] = {\n";
  for(int i = 0; i < NZ; ++i) {
    cout << colIndices[i] << ",\n";
  }
  cout << "};\n";

  cout << "double values[" << NZ << "] = {\n";
  for(int i = 0; i < NZ; ++i) {
    if(values[i] == 0)
      cout << "0.0,\n";
    else
      cout << values[i] << ",\n";
  }
  cout << "};\n";
}

///======================
/// CSR
///======================
CSRMatrix::CSRMatrix(int* __restrict rows, int* __restrict cols, double* __restrict vals,
                     unsigned int N, unsigned int M, unsigned int NZ):
Matrix(N, M, NZ), rowPtr(rows), colIndices(cols), values(vals) {
  
}

CSRMatrix::~CSRMatrix() {
  delete[] rowPtr;
  delete[] colIndices;
  delete[] values;
}

void CSRMatrix::print() {
  cout << "int rowPtr[" << (N + 1) << "] = {\n";
  for(int i = 0; i < N + 1; ++i) {
    cout << rowPtr[i] << ",\n";
  }
  cout << "};\n";
  
  cout << "int colIndices[" << NZ << "] = {\n";
  for(int i = 0; i < NZ; ++i) {
    cout << colIndices[i] << ",\n";
  }
  cout << "};\n";
  
  cout << "double values[" << NZ << "] = {\n";
  for(int i = 0; i < NZ; ++i) {
    if(values[i] == 0)
      cout << "0.0,\n";
    else
      cout << values[i] << ",\n";
  }
  cout << "};\n";
}

///======================
/// CSC
///======================
CSCMatrix::CSCMatrix(int* __restrict rows, int* __restrict cols, double* __restrict vals,
                     unsigned int N, unsigned int M, unsigned int NZ):
Matrix(N, M, NZ), rowIndices(rows), colPtr(cols), values(vals) {
  
}

CSCMatrix::~CSCMatrix() {
  delete[] rowIndices;
  delete[] colPtr;
  delete[] values;
}

void CSCMatrix::print() {
  cout << "int rowIndices[" << NZ << "] = {\n";
  for(int i = 0; i < NZ; ++i) {
    cout << rowIndices[i] << ",\n";
  }
  cout << "};\n";
  
  cout << "int colPtr[" << (M + 1) << "] = {\n";
  for(int i = 0; i < M + 1; ++i) {
    cout << colPtr[i] << ",\n";
  }
  cout << "};\n";
  
  cout << "double values[" << NZ << "] = {\n";
  for(int i = 0; i < NZ; ++i) {
    if(values[i] == 0)
      cout << "0.0,\n";
    else
      cout << values[i] << ",\n";
  }
  cout << "};\n";
}



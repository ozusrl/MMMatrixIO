#include "matrix.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iomanip>

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

// Caller of this method is responsible for destructing the
// returned matrix.
MMMatrix* MMMatrix::fromFile(string fileName) {
  ifstream mmFile(fileName.c_str());
  if (!mmFile.is_open()) {
    std::cerr << "Problem opening file " << fileName << ".\n";
    exit(1);
  }
  string headerLine;
  // consume the comments until we reach the size info
  while (mmFile.good()) {
    getline (mmFile, headerLine);
    if (headerLine[0] != '%') break;
  }
  
  // Read N, M, NZ
  stringstream header(headerLine, ios_base::in);
  int n, m, nz;
  header >> n >> m >> nz;
  
  // Read rows, cols, vals
  MMMatrix *matrix = new MMMatrix(n, m, nz);
  int row; int col; double val;
  
  string line;
  for (int i = 0; i < nz; ++i) {
    getline(mmFile, line);
    stringstream linestream(line, ios_base::in);
    linestream >> row >> col;
    // Pattern (i.e. connectivity) matrices do not contain val entry.
    // Such matrices are filled in with 1.0
    linestream >> val;
    if (linestream.fail())
      val = 1.0;
    // adjust to zero index
    matrix->add(row-1, col-1, val);
  }
  mmFile.close();
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



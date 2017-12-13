#pragma once

#include "matrix.hpp"
#include "mmio.h"
#include <vector>
#include <string>
#include <iostream>

namespace thundercat{
template<typename ValueType>
struct MMElement {
  // Unfortunately, can't make the members const, because sort() does not like this.
  // The workaround is uglier than having non-const members.
  int rowIndex;
  int colIndex;
  ValueType value;

  MMElement(const int row, const int col, const ValueType val):
  rowIndex(row), colIndex(col), value(val) { }
    
  static bool compareRowMajor(const MMElement<ValueType> &elt1, const MMElement<ValueType> &elt2) {
    if (elt1.rowIndex < elt2.rowIndex) return true;
    else if (elt2.rowIndex < elt1.rowIndex) return false;
    else return elt1.colIndex < elt2.colIndex;
  }

  static bool compareColumnMajor(const MMElement<ValueType> &elt1, const MMElement<ValueType> &elt2) {
    if (elt1.colIndex < elt2.colIndex) return true;
    else if (elt2.colIndex < elt1.colIndex) return false;
    else return elt1.rowIndex < elt2.rowIndex;
  }
};

template<typename ValueType>
class MMMatrix {
public:
  const unsigned int N;
  const unsigned int M;
private:
  std::vector< MMElement<ValueType> > elements;
  const bool symmetric;

public:
  MMMatrix(unsigned int N, unsigned int M):
  N(N), M(M), symmetric(false) {
  }
  
  MMMatrix(unsigned int N, unsigned int M, bool symmetric):
  N(N), M(M), symmetric(symmetric) {
  }

  virtual ~MMMatrix() = default;

  const std::vector< MMElement<ValueType> > getElements() {
    return elements;
  }
  
  unsigned int numElements() {
    return elements.size();
  }
  
  void add(int row, int col, ValueType val) {
    elements.push_back(MMElement<ValueType>(row, col, val));
  }

  bool hasFullDiagonal() {
    if (N != M)
      return false;

    int diagValueCount = 0;
    for (auto &elt : elements) {
      if (elt.rowIndex == elt.colIndex) {
        if (elt.value == 0.0)
          return false;
        else
          diagValueCount++;
      }
    }

    return diagValueCount == N;
  }

  bool isSymmetric() {
    return symmetric;
  }

  COOMatrix<ValueType>* toCOO() {
    std::sort(elements.begin(), elements.end(), MMElement<ValueType>::compareRowMajor);
    
    long sz = elements.size();
    int *rows = new int[sz];
    int *cols = new int[sz];
    ValueType *vals = new ValueType[sz];
    
    unsigned int eltIndex = 0;
    for (auto &elt : elements) {
      rows[eltIndex] = elt.rowIndex;
      cols[eltIndex] = elt.colIndex;
      vals[eltIndex] = elt.value;
      eltIndex++;
    }
    
    return new COOMatrix<ValueType>(rows, cols, vals, N, M, sz);
  }

  CSRMatrix<ValueType>* toCSR() {
    std::sort(elements.begin(), elements.end(), MMElement<ValueType>::compareRowMajor);
    
    long sz = elements.size();
    int *rows = new int[N + 1];
    rows[0] = 0;
    int *cols = new int[sz];
    ValueType *vals = new ValueType[sz];
    
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
    
    return new CSRMatrix<ValueType>(rows, cols, vals, N, M, sz);
  }

  CSCMatrix<ValueType>* toCSC() {
    std::sort(elements.begin(), elements.end(), MMElement<ValueType>::compareColumnMajor);
    
    long sz = elements.size();
    int *rows = new int[sz];
    int *cols = new int[M + 1];
    cols[0] = 0;
    ValueType *vals = new ValueType[sz];
    
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
    
    return new CSCMatrix<ValueType>(rows, cols, vals, N, M, sz);
  }

  // Return a new matrix that contains the lower triangular part plus the diagonal
  MMMatrix<ValueType>* getLD() {
    MMMatrix<ValueType> *matrix = new MMMatrix<ValueType>(N, M);
    unsigned int count = 0;
    for (auto &elt : elements) {
      if (elt.rowIndex >= elt.colIndex) {
        matrix->add(elt.rowIndex, elt.colIndex, elt.value);
        count++;
      }
    }
    return matrix;
  }

  // Return a new matrix that contains the upper triangular part plus the diagonal
  MMMatrix<ValueType>* getUD() {
    MMMatrix<ValueType> *matrix = new MMMatrix<ValueType>(N, M);
    unsigned int count = 0;
    for (auto &elt : elements) {
      if (elt.rowIndex <= elt.colIndex) {
        matrix->add(elt.rowIndex, elt.colIndex, elt.value);
        count++;
      }
    }
    return matrix;
  }

  static MMMatrix<ValueType>* fromFile(std::string fileName) {
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
    MMMatrix<ValueType> *matrix = new MMMatrix<ValueType>(N, M, mm_is_symmetric(matcode));
    int row; int col; double val;
    
    std::string line;
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
      matrix->add(row-1, col-1, (ValueType)val);
      if (mm_is_symmetric(matcode) && row != col) {
        matrix->add(col-1, row-1, (ValueType)val);
      }
    }
    if (f !=stdin) {
      fclose(f);
    }
    return matrix;
  }
};
}


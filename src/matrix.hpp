#pragma once

namespace thundercat {
  class Matrix {
  public:
    const unsigned int N; // num rows
    const unsigned int M; // num columns
    const unsigned int NZ;
    
    Matrix(unsigned int N, unsigned int M, unsigned int NZ);
    
    virtual ~Matrix() = 0;
    
    bool isSquare();
  };

  //===============================================
  template<typename ValueType>
  class COOMatrix : public Matrix {
  public:
    int* __restrict rowIndices;
    int* __restrict colIndices;
    ValueType* __restrict values;
    
    COOMatrix(int* __restrict rows, int* __restrict cols, ValueType* __restrict vals,
              unsigned int N, unsigned int M, unsigned int NZ):
    Matrix(N, M, NZ), rowIndices(rows), colIndices(cols), values(vals) {
    }
    
    virtual ~COOMatrix() {
      delete[] rowIndices;
      delete[] colIndices;
      delete[] values;
    }
  };

  //===============================================
  template<typename ValueType>
  class CSRMatrix : public Matrix {
  public:
    int* __restrict rowPtr;
    int* __restrict colIndices;
    ValueType* __restrict values;

    CSRMatrix(int* __restrict rows, int* __restrict cols, ValueType* __restrict vals,
              unsigned int N, unsigned int M, unsigned int NZ):
    Matrix(N, M, NZ), rowPtr(rows), colIndices(cols), values(vals) {
    }

    virtual ~CSRMatrix() {
      delete[] rowPtr;
      delete[] colIndices;
      delete[] values;
    }
  };

  //===============================================
  template<typename ValueType>
  class CSCMatrix : public Matrix {
  public:
    int* __restrict rowIndices;
    int* __restrict colPtr;
    ValueType* __restrict values;

    CSCMatrix(int* __restrict rows, int* __restrict cols, ValueType* __restrict vals,
              unsigned int N, unsigned int M, unsigned int NZ):
    Matrix(N, M, NZ), rowIndices(rows), colPtr(cols), values(vals) {      
    }
    
    virtual ~CSCMatrix() {
      delete[] rowIndices;
      delete[] colPtr;
      delete[] values;
    }
  };
}


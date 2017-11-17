#ifndef _THUNDERCAT_MATRIX_H
#define _THUNDERCAT_MATRIX_H

namespace thundercat {
  class Matrix {
  public:
    unsigned int N; // num rows
    unsigned int M; // num columns
    unsigned int NZ;
    
    Matrix(unsigned int N, unsigned int M, unsigned int NZ);
    
    virtual ~Matrix();

    virtual void print() = 0;
    
    bool isSquare();
  };

  //===============================================
  class COOMatrix : public Matrix {
  public:
    int* __restrict rowIndices;
    int* __restrict colIndices;
    double* __restrict values;
    
    COOMatrix(int* __restrict rows, int* __restrict cols, double* __restrict vals,
              unsigned int N, unsigned int M, unsigned int NZ);
    
    virtual ~COOMatrix();

    virtual void print();
  };
  
  //===============================================
  class CSRMatrix : public Matrix {
  public:
    int* __restrict rowPtr;
    int* __restrict colIndices;
    double* __restrict values;

    CSRMatrix(int* __restrict rows, int* __restrict cols, double* __restrict vals,
              unsigned int N, unsigned int M, unsigned int NZ);

    virtual ~CSRMatrix();
    
    virtual void print();
  };

  //===============================================
  class CSCMatrix : public Matrix {
  public:
    int* __restrict rowIndices;
    int* __restrict colPtr;
    double* __restrict values;

    CSCMatrix(int* __restrict rows, int* __restrict cols, double* __restrict vals,
              unsigned int N, unsigned int M, unsigned int NZ);
    
    virtual ~CSCMatrix();
    
    virtual void print();
  };
}

#endif

#ifndef _THUNDERCAT_MATRIX_H
#define _THUNDERCAT_MATRIX_H

#include <cstdlib> // defines NULL
#include <vector>
#include <map>
#include <set>
#include <utility>

namespace thundercat {
  class Matrix {
  public:
    unsigned int N; // num rows
    unsigned int M; // num columns
    unsigned int NZ;
    
    Matrix(unsigned int N, unsigned int M, unsigned int NZ);
    
    virtual ~Matrix();

    virtual void print() = 0;
  };

  class COOMatrix;
  
  class CSRMatrix;
  
  class CSCMatrix;
  
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
    
  public:
    MMMatrix(unsigned int N, unsigned int M, unsigned int NZ);
    
    virtual ~MMMatrix();
    
    void add(int row, int col, double val);

    void print();
    
    void printAsMTX();
    
    COOMatrix* toCOO();

    CSRMatrix* toCSR();

    CSCMatrix* toCSC();

    // Return a new matrix that contains the lower triangular part plus the diagonal
    MMMatrix* getLD();
    
    // Return a new matrix that contains the upper triangular part plus the diagonal
    MMMatrix* getUD();

    static MMMatrix* fromFile(std::string fileName);
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

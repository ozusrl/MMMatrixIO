#pragma once

#include <vector>
#include <iostream>

namespace thundercat {

  typedef struct {
      unsigned int rowIndexBegin;
      unsigned int rowIndexEnd;
      unsigned long valIndexBegin;
      unsigned long valIndexEnd;
  } MatrixStripeInfo;

  class Matrix {
  public:
    const unsigned int N; // num rows
    const unsigned int M; // num columns
    const unsigned int NZ;
    
    Matrix(unsigned int N, unsigned int M, unsigned int NZ);
    
    virtual ~Matrix() = 0;
    
    bool isSquare();

    virtual std::vector<MatrixStripeInfo> *getStripeInfos(unsigned int numPartitions);


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

    virtual std::vector<MatrixStripeInfo> *getStripeInfos(unsigned int numPartitions) override {
        if (stripeInfos.size() != 0) {
          std::cout << "I was not expecting getStripeInfos to be called multiple times.\n";
        }
        // Split the matrix
        unsigned long chunkSize = this->NZ   / numPartitions;
        unsigned int rowIndex = 0;
        unsigned long valIndex = 0;
        for (int partitionIndex = 0; partitionIndex < numPartitions; ++partitionIndex) {
          unsigned int rowIndexStart = rowIndex;
          unsigned long valIndexStart = valIndex;
          unsigned long numElementsCovered = 0;

          if (partitionIndex == numPartitions - 1) {
            rowIndex = this->N;
            valIndex = this->NZ;
          } else {
            while(numElementsCovered < chunkSize && rowIndex < this->N) {
              numElementsCovered += this->rowPtr[rowIndex+1] - this->rowPtr[rowIndex];
              rowIndex++;
            }
            valIndex += numElementsCovered;
          }

          MatrixStripeInfo stripeInfo;
          stripeInfo.rowIndexBegin = rowIndexStart;
          stripeInfo.rowIndexEnd = rowIndex;
          stripeInfo.valIndexBegin = valIndexStart;
          stripeInfo.valIndexEnd = valIndex;
          stripeInfos.push_back(stripeInfo);
        }
        return &stripeInfos;
      }
  protected:
      std::vector<MatrixStripeInfo> stripeInfos;
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


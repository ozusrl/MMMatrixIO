#pragma once

#include <iostream>

namespace thundercat {
  class Matrix {
  public:
    unsigned int N; // num rows
    unsigned int M; // num columns
    unsigned int NZ;
    
    Matrix(unsigned int N, unsigned int M, unsigned int NZ):
    N(N), M(M), NZ(NZ) {
    }
    
    virtual ~Matrix() {
      // do nothing
    }
    
    virtual void print() = 0;
    
    bool isSquare() {
      return N == M;
    }
  };

  template<typename T>
  struct TypeName
  { static const char* name; };
  
  template<>
  const char* TypeName<double>::name = "double";

  template<>
  const char* TypeName<float>::name = "float";

  template<typename T>
  const char* TypeName<T>::name = "UNSUPPORTED_VALUE_TYPE";

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

    virtual void print() {
      std::cout << "int rowIndices[" << NZ << "] = {\n";
      for(int i = 0; i < NZ; ++i) {
        std::cout << rowIndices[i] << ",\n";
      }
      std::cout << "};\n";
      
      std::cout << "int colIndices[" << NZ << "] = {\n";
      for(int i = 0; i < NZ; ++i) {
        std::cout << colIndices[i] << ",\n";
      }
      std::cout << "};\n";
      
      std::cout << "double values[" << NZ << "] = {\n";
      for(int i = 0; i < NZ; ++i) {
        if(values[i] == 0)
          std::cout << "0.0,\n";
        else
          std::cout << values[i] << ",\n";
      }
      std::cout << "};\n";
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
    
    virtual void print() {
      std::cout << "int rowPtr[" << (N + 1) << "] = {\n";
      for(int i = 0; i < N + 1; ++i) {
        std::cout << rowPtr[i] << ",\n";
      }
      std::cout << "};\n";
      
      std::cout << "int colIndices[" << NZ << "] = {\n";
      for(int i = 0; i < NZ; ++i) {
        std::cout << colIndices[i] << ",\n";
      }
      std::cout << "};\n";
      
      std::cout << "double values[" << NZ << "] = {\n";
      for(int i = 0; i < NZ; ++i) {
        if(values[i] == 0)
          std::cout << "0.0,\n";
        else
          std::cout << values[i] << ",\n";
      }
      std::cout << "};\n";
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
    
    virtual void print() {
      std::cout << "int rowIndices[" << NZ << "] = {\n";
      for(int i = 0; i < NZ; ++i) {
        std::cout << rowIndices[i] << ",\n";
      }
      std::cout << "};\n";
      
      std::cout << "int colPtr[" << (M + 1) << "] = {\n";
      for(int i = 0; i < M + 1; ++i) {
        std::cout << colPtr[i] << ",\n";
      }
      std::cout << "};\n";
      
      std::cout << TypeName<ValueType>::name << " values[" << NZ << "] = {\n";
      for(int i = 0; i < NZ; ++i) {
        if(values[i] == 0)
          std::cout << "0.0,\n";
        else
          std::cout << values[i] << ",\n";
      }
      std::cout << "};\n";
    }
  };
}


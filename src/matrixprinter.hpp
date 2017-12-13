#pragma once

#include "matrix.hpp"
#include "mmmatrix.hpp"
#include <iostream>

namespace thundercat {
  template<typename T>
  struct TypeName
  { static const char* name; };
  
  template<>
  const char* TypeName<double>::name = "double";

  template<>
  const char* TypeName<float>::name = "float";

  template<typename T>
  const char* TypeName<T>::name = "UNSUPPORTED_VALUE_TYPE";

  class MatrixPrinter {
  public:

    template <typename ValueType>
    static void print(MMMatrix<ValueType> &mmMatrix) {
      std::cout << mmMatrix.N << " " << mmMatrix.M << " " << mmMatrix.getElements()->size() << "\n";
      for (MMElement<ValueType> &elt : *(mmMatrix.getElements())) {
        std::cout << elt.rowIndex << " "
        << elt.colIndex << " "
        << elt.value << "\n";
      }
    }
    
    template <typename ValueType>
    static void printAsMTX(MMMatrix<ValueType> &mmMatrix) {
      std::cout << mmMatrix.N << " " << mmMatrix.M << " " << mmMatrix.getElements()->size() << "\n";
      for (MMElement<ValueType> &elt : *(mmMatrix.getElements())) {
        std::cout << (elt.rowIndex + 1) << " "
        << (elt.colIndex + 1) << " "
        << elt.value << "\n";
      }
    }
    
    template <typename ValueType>
    static void print(COOMatrix<ValueType> &cooMatrix) {
      std::cout << "int rowIndices[" << cooMatrix.NZ << "] = {\n";
      for(int i = 0; i < cooMatrix.NZ; ++i) {
        std::cout << cooMatrix.rowIndices[i] << ",\n";
      }
      std::cout << "};\n";
      
      std::cout << "int colIndices[" << cooMatrix.NZ << "] = {\n";
      for(int i = 0; i < cooMatrix.NZ; ++i) {
        std::cout << cooMatrix.colIndices[i] << ",\n";
      }
      std::cout << "};\n";
      
      std::cout << TypeName<ValueType>::name << " values[" << cooMatrix.NZ << "] = {\n";
      for(int i = 0; i < cooMatrix.NZ; ++i) {
        if(cooMatrix.values[i] == 0)
          std::cout << "0.0,\n";
        else
          std::cout << cooMatrix.values[i] << ",\n";
      }
      std::cout << "};\n";
    }

    template <typename ValueType>
    static void print(CSRMatrix<ValueType> &csrMatrix) {
      std::cout << "int rowPtr[" << (csrMatrix.N + 1) << "] = {\n";
      for(int i = 0; i < csrMatrix.N + 1; ++i) {
        std::cout << csrMatrix.rowPtr[i] << ",\n";
      }
      std::cout << "};\n";
      
      std::cout << "int colIndices[" << csrMatrix.NZ << "] = {\n";
      for(int i = 0; i < csrMatrix.NZ; ++i) {
        std::cout << csrMatrix.colIndices[i] << ",\n";
      }
      std::cout << "};\n";
      
      std::cout << TypeName<ValueType>::name << " values[" << csrMatrix.NZ << "] = {\n";
      for(int i = 0; i < csrMatrix.NZ; ++i) {
        if(csrMatrix.values[i] == 0)
          std::cout << "0.0,\n";
        else
          std::cout << csrMatrix.values[i] << ",\n";
      }
      std::cout << "};\n";
    }

    template <typename ValueType>
    static void print(CSCMatrix<ValueType> &cscMatrix) {
      std::cout << "int rowIndices[" << cscMatrix.NZ << "] = {\n";
      for(int i = 0; i < cscMatrix.NZ; ++i) {
        std::cout << cscMatrix.rowIndices[i] << ",\n";
      }
      std::cout << "};\n";
      
      std::cout << "int colPtr[" << (cscMatrix.M + 1) << "] = {\n";
      for(int i = 0; i < cscMatrix.M + 1; ++i) {
        std::cout << cscMatrix.colPtr[i] << ",\n";
      }
      std::cout << "};\n";
      
      std::cout << TypeName<ValueType>::name << " values[" << cscMatrix.NZ << "] = {\n";
      for(int i = 0; i < cscMatrix.NZ; ++i) {
        if(cscMatrix.values[i] == 0)
          std::cout << "0.0,\n";
        else
          std::cout << cscMatrix.values[i] << ",\n";
      }
      std::cout << "};\n";
    }
  };
}


# MatrixMarket IO Library

This is a small library to read matrix data from
MatrixMarket exchange files (`.mtx`).
There are several restrictions/assumptions:

* Matrices with complex values are not handled.
* Pattern matrices are processed as if each value is 1.0.
* Integer-valued matrices are treated as real-valued.
* Symmetry is ignored; the input file is processed as a general matrix.


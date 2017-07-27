#!/bin/bash

MATRICES="simple simple_shuffled simple_emptyrow"

for matrix in $MATRICES
do
    echo "=== " $matrix " ===" 
    ../build/testmatrixio $matrix".mtx" | diff - $matrix"_expected.txt" 
done

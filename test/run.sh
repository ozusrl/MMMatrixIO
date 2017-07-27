#!/bin/bash

MATRICES="simple"

for matrix in $MATRICES
do
    echo "=== " $matrix " ===" 
    ../build/testmatrixio $matrix".mtx" | diff - $matrix"_expected.txt" 
done

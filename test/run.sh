#!/bin/bash

for filename in *.mtx
do
    extension="${filename##*.}"
    basefilename="${filename%.*}"
    
    echo "=== " $basefilename " ===" 
    ../build/testmatrixio $filename > $basefilename"_expected.txt"
    git diff $basefilename"_expected.txt"
done

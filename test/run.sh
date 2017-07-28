#!/bin/bash

for filename in *.mtx
do
    extension="${filename##*.}"
    basefilename="${filename%.*}"
    
    echo "=== " $basefilename " ===" 
    ../build/testmatrixio $filename | diff - $basefilename"_expected.txt" 
done

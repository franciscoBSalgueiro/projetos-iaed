#!/bin/bash

# Author: Francisco Salgueiro

for infile in ./tests/*.in; do
    basename=${infile%.*}
    valgrind ./proj1.out < $infile
done
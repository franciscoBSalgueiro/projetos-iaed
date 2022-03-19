#!/bin/bash

for infile in ./*.in; do
    basename=${infile%.*}
    ../$1 < $infile > $basename.result
    cmp --silent $basename.result $basename.out || echo "$basename TEST FAILED"
    diff $basename.result $basename.out
done
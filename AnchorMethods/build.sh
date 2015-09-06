#!/usr/bin/env bash

clang++ -O1 CircleArea.cpp -c
nm CircleArea.o | c++filt > defined_symbols.txt

clang++ -O1 -DANCHORS CircleArea.cpp -c
nm CircleArea.o | c++filt > defined_symbols_with_anchor.txt

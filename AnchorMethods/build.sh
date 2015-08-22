#!/usr/bin/env bash

clang++ CircleArea.cpp -c
nm CircleArea.o -U | c++filt > defined_symbols.txt

clang++ -DANCHORS CircleArea.cpp -c
nm CircleArea.o -U | c++filt > defined_symbols_with_anchor.txt

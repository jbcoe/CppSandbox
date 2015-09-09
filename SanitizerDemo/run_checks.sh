#!/usr/bin/env bash
clang-check-3.6 -analyze SanitizerDemo.cpp --
rm SanitizerDemo.plist # remove the output file as I do not want it

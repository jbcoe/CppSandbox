#!/bin/bash
clang-format -i $(find . -iname *.cpp)
clang-format -i $(find . -iname *.hpp)
clang-format -i $(find . -iname *.h)


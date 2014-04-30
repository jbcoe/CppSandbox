#!/bin/bash

clang-format -i $(find . -iname "*.cpp" -exec grep -L -e "NO CLANG FORMAT" {} \; )
clang-format -i $(find . -iname "*.hpp" -exec grep -L -e "NO CLANG FORMAT" {} \; )
clang-format -i $(find . -iname "*.h" -exec grep -L -e "NO CLANG FORMAT" {} \; )


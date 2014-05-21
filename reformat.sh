#!/bin/bash

clang-format -i $(find . ! -path "./ConceptsOrigin*" -iname "*.cpp" -exec grep -L -e "NO CLANG FORMAT" {} \; )
clang-format -i $(find . ! -path "./ConceptsOrigin*" -iname "*.h" -exec grep -L -e "NO CLANG FORMAT" {} \; )
clang-format -i $(find . ! -path "./ConceptsOrigin*" -iname "*.hpp" -exec grep -L -e "NO CLANG FORMAT" {} \; )


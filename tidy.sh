#!/bin/bash

for var in $(find . ! -path "./ConceptsOrigin*" -iname "*.cpp" -exec grep -L -e "NO CLANG FORMAT" {} \; ); 
do clang-tidy -fix $var -- -std=c++14 -stdlib=libc++ -I.;
done
 
for var in $(find . ! -path "./ConceptsOrigin*" -iname "*.hpp" -exec grep -L -e "NO CLANG FORMAT" {} \; ); 
do clang-tidy -fix $var -- -std=c++14 -stdlib=libc++ -I.;
done

for var in $(find . ! -path "./ConceptsOrigin*" -iname "*.h" -exec grep -L -e "NO CLANG FORMAT" {} \; ); 
do clang-tidy -fix $var -- -std=c++14 -stdlib=libc++ -I.;
done

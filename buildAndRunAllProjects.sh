#!/bin/sh

for x in $(for var in $(find . -iname Makefile) ; do echo $(basename $(dirname $var)) ; done ); 
do clear ; echo "== $x ==" ; echo ; cd $x ; make clean ; (make && echo [OK]) || echo [FAIL] ;
	echo Press Enter to continue ; read line ; cd .. ; echo ; 
done

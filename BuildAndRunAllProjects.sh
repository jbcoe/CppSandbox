#!/bin/sh

for x in $(for var in $(find . -iname Makefile) ; do echo $(basename $(dirname $var)) ; done ); 
	do echo "== $x ==" ; echo ; cd $x ; make clean ; make ; cd .. ; echo ; 
done

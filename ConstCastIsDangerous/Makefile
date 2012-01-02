include ../compilerSettings.mk

SOURCES=ConstCastIsDangerous.cpp
EXECUTABLE=ConstCastIsDangerous

runAndExpectFail: all
	./${EXECUTABLE} || echo Failure expected

include ../makeCommon.mk

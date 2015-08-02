include ../compilerSettings_clang3.6.mk

SOURCES=ConstCastIsDangerous.cpp
EXECUTABLE=ConstCastIsDangerous

runAndExpectFail: all
	./${EXECUTABLE} || echo Failure expected

include ../makeCommon.mk

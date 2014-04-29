include ../compilerSettings_clang3.5.mk

SOURCES=ConstCastIsDangerous.cpp
EXECUTABLE=ConstCastIsDangerous

runAndExpectFail: all
	./${EXECUTABLE} || echo Failure expected

include ../makeCommon.mk

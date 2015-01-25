CC=clang-mp-3.7
CXX=clang++-mp-3.7
RM=rm -f
OPT=0
CPPFLAGS= -Wall -Winline -Wwrite-strings -Wno-unused -O$(OPT) -std=c++1z -stdlib=libc++ -I../
LDFLAGS= -stdlib=libc++
LDLIBS=


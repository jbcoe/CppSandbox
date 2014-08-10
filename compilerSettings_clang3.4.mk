CC=clang-mp-3.4
CXX=clang++-mp-3.4
RM=rm -f
OPT=0
CPPFLAGS= -Wall -Winline -Wwrite-strings -Wno-unused -O$(OPT) -std=c++1y -stdlib=libc++ -I../
LDFLAGS= -stdlib=libc++
LDLIBS=

CC=clang-mp-3.6
CXX=clang++-mp-3.6
RM=rm -f
OPT=0
CPPFLAGS= -Wall -Winline -Wwrite-strings -Wno-unused -O$(OPT) -std=c++14 -stdlib=libc++ -I../
LDFLAGS= -stdlib=libc++
LDLIBS=


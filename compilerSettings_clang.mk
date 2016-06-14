CC=clang-3.8
CXX=clang++-3.8
RM=rm -f
OPT=0
CPPFLAGS= -Wall -Winline -Wwrite-strings -Wno-unused -O$(OPT) -std=c++14 -stdlib=libc++ -I../
LDFLAGS= -stdlib=libc++
LDLIBS=


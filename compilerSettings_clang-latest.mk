CC=/usr/local/Cellar/llvm/5.0.0/bin/clang
CXX=/usr/local/Cellar/llvm/5.0.0/bin/clang++
RM=rm -f
OPT=0
CPPFLAGS= -Wall -Winline -Wwrite-strings -Wno-unused -O$(OPT) -std=c++14 -stdlib=libc++ -I../
LDFLAGS= -stdlib=libc++
LDLIBS=


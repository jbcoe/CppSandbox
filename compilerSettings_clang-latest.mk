CC=/opt/llvm/bin/clang
CXX=/opt/llvm/bin/clang++
RM=rm -f
OPT=0
CPPFLAGS= -Wall -Winline -Wwrite-strings -Wno-unused -O$(OPT) -std=c++14 -stdlib=libc++ -I../
LDFLAGS= -stdlib=libc++
LDLIBS=


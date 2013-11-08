CC = clang++-svn
OPT=0
CFLAGS = -Wall -Winline -Wwrite-strings -Wno-unused -O${OPT} -std=c++1y -stdlib=libc++ -I../
LDFLAGS = -stdlib=libc++

CC = clang++
OPT=0
CFLAGS = -Wall -Winline -Wwrite-strings -Wno-unused -O${OPT} -std=c++11 -stdlib=libc++ -I../
LDFLAGS = -stdlib=libc++

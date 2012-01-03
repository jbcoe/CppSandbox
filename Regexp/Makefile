include ../compilerSettings.mk

CFLAGS:=$(CFLAGS) -DUSE_BOOST
LDFLAGS:=$(LDFLAGS) -L$(BOOST_LIB) -lboost_regex

SOURCES=Regexp.cpp
EXECUTABLE=Regexp

include ../makeCommon.mk

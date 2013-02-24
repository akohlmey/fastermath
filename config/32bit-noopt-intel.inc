# -*- makefile -*-
# configuration for Linux 32-bit x86 with intel icc using SSE2
ALIGNMENT=16
CC=icc -m32
CPPFLAGS= 
ARCHFLAGS= -mia32 -pc64 -g -fp-speculation=strict -mieee-fp
GENFLAGS=  -falign-functions=$(ALIGNMENT) -fPIC -fno-builtin
OPTFLAGS=  -O0 
WARNFLAGS= -Wall

LD=$(CC) -static-intel
LDFLAGS=   -shared
LDLIBS=
TESTLIBS=  -lrt -lm

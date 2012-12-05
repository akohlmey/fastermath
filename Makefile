# -*- Makefile -*- flags for GNU C compiler to build library
CC=gcc
CFLAGS=-O3 -Wall -W -fPIC -ffast-math -march=core2 -fno-builtin
AR=ar
ARFLAGS=rcsv
LD=$(CC)
CPPFLAGS=
LDFLAGS=-shared
LDLIBS= 

# wrapper sources
SRC=wrapsetup.c exp2.c exp.c exp10.c log.c
OBJ=$(SRC:.c=.o)

default: libfastermath.so libfastermath.a tester testerf

test: tester testerf
	./tester 10000 1000
	./testerf 10000 1000

libfastermath.so: $(OBJ)
	$(LD) $(LDFLAGS) -o $@ $(OBJ) $(LDLIBS)

libfastermath.a: $(OBJ)
	$(AR) $(ARFLAGS) $@ $(OBJ)

tester: tester.c  libfastermath.so
	$(LD) -o $@ $< -L. -lfastermath -Wl,-rpath,. -lm -lrt

testerf: testerf.c libfastermath.so
	$(LD) -o $@ $< -L. -lfastermath -Wl,-rpath,. -lm -lrt

clean:
	rm -f libfastermath.so libfastermath.a $(OBJ) \
	 tester.o tester testerf.o testerf perf.data* gmon.out core.[0-9]*

spotless: clean
	rm -f .depend *~

.depend: $(SRC)
	$(CC) -MM $(SRC) > $@

sinclude .depend


# flags to build library
CC=gcc
CFLAGS=-O3 -Wall -Wextra -fPIC -ffast-math -march=core2
AR=ar
ARFLAGS=rcsv
LD=$(CC)
CPPFLAGS=
LDFLAGS=-shared
LDLIBS= 

# wrapper sources
SRC=wrapsetup.c exp2.c
OBJ=$(SRC:.c=.o)

default: libfastermath.so libfastermath.a tester

libfastermath.so: $(OBJ)
	$(LD) $(LDFLAGS) -o $@ $(OBJ) $(LDLIBS)

libfastermath.a: $(OBJ)
	$(AR) $(ARFLAGS) $@ $(OBJ)

tester: tester.o libfastermath.a
	$(LD) -o $@ $^ -lm -lrt

clean:
	rm -f libfastermath.so libfastermath.a $(OBJ) \
	 tester.o tester

spotless: clean
	rm -f .depend *~

.depend: $(SRC)
	$(CC) -MM $(SRC) > $@

sinclude .depend
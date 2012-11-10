
# flags to build library
CC=gcc
CFLAGS=-O3 -Wall $(DEBUG) -fPIC -fno-builtin
LD=$(CC)
CPPFLAGS=
LDFLAGS=-shared
LDLIBS= -ldl -lc

# wrapper sources
SRC=wrapsetup.c
OBJ=$(SRC:.c=.o)

default: libmathwrap.so libmathwrap.a

libmathwrap.so: $(OBJ)
	$(LD) $(LDFLAGS) -o $@ $(OBJ) $(LDLIBS)

libmathwrap.a: $(OBJ)
	$(AR) $(ARFLAGS) $@ $(OBJ)

clean:
	rm -f libmathwrap.so libmathwrap.a $(OBJ)

spotless: clean
	rm -f .depend *~

.depend: $(SRC)
	$(CC) -MM $(SRC) > $@

sinclude .depend

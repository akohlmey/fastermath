
AR=ar
ARFLAGS=rcsv

ifeq ($(ARCH),)
default:
	@echo
	@echo need to define ARCH
	@echo
	@exit 1
else

sinclude ../config/$(ARCH).inc

CFLAGS= -I../include $(CPPFLAGS) $(ARCHFLAGS) $(GENFLAGS) $(OPTFLAGS) $(WARNFLAGS)
VPATH=../src

# wrapper sources
SRC=exp2.c # exp.c exp10.c
OBJ=$(SRC:.c=.o)


default: all

endif

all: libfastermath.so libfastermath.a tester #testerf

libfastermath.so: $(OBJ)
	$(LD) $(LDFLAGS) -o $@ $(OBJ)

libfastermath.a: $(OBJ)
	$(AR) $(ARFLAGS) $@ $(OBJ)

tester: tester.c  libfastermath.so
	$(LD) -o $@ $< $(LDLIBS) -lrt

testerf: testerf.c libfastermath.so
	$(LD) -o $@ $< -L. -lfastermath -Wl,-rpath,. -lamdlibm -lm -lrt

clean:
	rm -f libfastermath.so libfastermath.a $(OBJ) *.s a.out \
	 tester.o tester testerf.o testerf perf.data* gmon.out core.[0-9]*

spotless: clean
	rm -f .depend *~

.depend: $(SRC)
	$(CC) -MM $(SRC) > $@

.PHONY: all default spotless 

sinclude .depend

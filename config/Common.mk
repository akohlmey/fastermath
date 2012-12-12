
AR=ar
ARFLAGS=rcsv

ifeq ($(ARCH),)
default:
	@echo
	@echo need to define ARCH
	@echo
	@exit 1
else

include ../config/$(ARCH).inc
default: all

endif

CFLAGS= -I../include $(CPPFLAGS) $(ARCHFLAGS) $(GENFLAGS) $(OPTFLAGS) $(WARNFLAGS)
SRC=exp2.c # exp.c exp10.c
OBJ=$(SRC:.c=.o)

vpath %.c ../src
vpath %.h ../include

all: libfastermath.so libfastermath.a tester #testerf

libfastermath.so: $(OBJ)
	$(LD) $(LDFLAGS) -o $@ $(OBJ)

libfastermath.a: $(OBJ)
	$(AR) $(ARFLAGS) $@ $(OBJ)

tester: tester.c  libfastermath.so
	$(LD) -I../include $(CFLAGS) -o $@ $< -L. -lfastermath -Wl,-rpath,$(PWD) $(LDLIBS) $(TESTLIBS)

testerf: testerf.c libfastermath.so
	$(LD) -o $@ $< -L. -lfastermath -Wl,-rpath,. -lamdlibm -lm -lrt

clean:
	rm -f libfastermath.so libfastermath.a $(OBJ) *.s a.out \
	 tester.o tester testerf.o testerf perf.data* gmon.out core.[0-9]*

spotless: clean
	rm -f .depend *~

.depend: $(SRC)
	$(CC) -I../include -MM $^ > $@

.PHONY: all default spotless 

sinclude .depend

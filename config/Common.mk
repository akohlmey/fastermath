
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

DEFS=-I../include -D_FM_INTERNAL
CFLAGS= $(CPPFLAGS) $(DEFS) $(ARCHFLAGS) $(GENFLAGS) $(OPTFLAGS) $(WARNFLAGS)
LIBSRC=exp2.c # exp.c exp10.c
LIBOBJ=$(LIBSRC:.c=.o)
TESTSRC=tester.c # testerf.c
TESTOBJ=$(TESTSRC:.c=.o)

vpath %.c ../src
vpath %.h ../include

all: libfastermath.so libfastermath.a tester #testerf

tester: tester.o libfastermath.a
	$(LD) -o $@ $^ $(LDLIBS) $(TESTLIBS)

testerf: testerf.o libfastermath.a
	$(LD) -o $@ $^ $(LDLIBS) $(TESTLIBS)

libfastermath.so: $(LIBOBJ)
	$(LD) $(LDFLAGS) -o $@ $(LIBOBJ)

libfastermath.a: $(LIBOBJ)
	$(AR) $(ARFLAGS) $@ $(LIBOBJ)

.depend: $(LIBSRC) $(TESTSCR)
	$(CC) $(DEFS) $(CPPFLAGS) -MM $^ > $@

.PHONY: all default
.SUFFIX:
.SUFFIX: .c .o

.c.o:
	$(CC) -o $@ -c $(CFLAGS) $<

sinclude .depend

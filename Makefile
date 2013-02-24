# Top level -*- Makefile -*- for fastermath lib(s)
HOST = $(shell uname -m)
ifeq ($(HOST),x86_64)
ARCHES = $(patsubst config/%.inc,%,$(wildcard config/*.inc))
default: list
else ifeq ($(HOST),i686)
ARCHES = $(patsubst config/%.inc,%,$(wildcard config/32*.inc))
default: list
else ifeq ($(HOST),i386)
ARCHES = $(patsubst config/%.inc,%,$(wildcard config/32*.inc))
default: list
else
default:
	@echo
	@echo unsupported machine $(HOST)
	@echo
	exit 1 
endif

list:
	@echo
	@echo Available configurations:
	@echo $(ARCHES)
	@echo

all:
	for d in $(ARCHES); \
		do dir=Obj_$$d ; test -d $$dir || mkdir -p $$dir ; \
		make -C $$dir -f ../config/Common.mk ARCH=$$d || break ;\
	done

$(ARCHES) :
	mkdir -p Obj_$@
	make -C Obj_$@ -f ../config/Common.mk ARCH=$@

clean:
	rm -rf Obj_* perf.data*

spotless: clean
	rm -f *~ */*~

.SUFFIXES:

.PHONY: default subdirs clean spotless

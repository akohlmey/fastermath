# Top level -*- Makefile -*- for fastermath lib(s)
HOST = $(shell uname -m)
ifeq ($(HOST),x86_64)
ARCHES = $(patsubst config/%.inc,%,$(wildcard config/*.inc))
default: subdirs
else ifeq ($(HOST),i686)
ARCHES = $(patsubst config/%.inc,%,$(wildcard config/32*.inc))
default: subdirs
else
default:
	@echo
	@echo unsupported machine $(HOST)
	@echo
	exit 1 
endif


subdirs:
	for d in $(ARCHES); \
		do dir=Obj_$$d ; test -d $$dir || mkdir -p $$dir ; \
		make -C $$dir -f ../config/Common.mk ARCH=$$d || break ;\
	done

Obj_% : %
	mkdir -p $@

clean:
	rm -rf Obj_* perf.data*

spotless: clean
	rm *~ */*~

.SUFFIX:

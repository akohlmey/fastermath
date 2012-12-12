# Top level -*- Makefile -*- for fastermath lib(s)
ARCHES = $(patsubst config/%.inc,%,$(wildcard config/*.inc))

default: subdirs


subdirs:
	for d in $(ARCHES); \
		do mkdir -p Obj_$$d ; \
		make -C Obj_$$d -f ../config/Common.mk ARCH=$$d || break ;\
	done

Obj_% : %
	mkdir -p $@

clean:
	rm -rf Obj_*

.SUFFIX:

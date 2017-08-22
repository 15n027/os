TOPDIR ?= ../..
include $(TOPDIR)/build/defs.mk

.PHONY: install build clean

all: install

install: build
	$(MAKE) -f Makefile.musl install

build: config.mak
	$(MAKE) -f Makefile.musl

config.mak: Makefile
	CC="$(CC)" \
	CROSS_COMPILE="$(PFX)" \
	ARCH=$(ARCH) \
	CFLAGS="$(CFLAGS)" \
	./configure --target=$(ARCH)-elf --disable-shared --prefix=$(OBJDIRPREFIX) 

include $(TOPDIR)/build/pre-targets.mk

clean:
	$(MAKE) -f Makefile.musl clean
	$(RM) config.mak



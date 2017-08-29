TOPDIR ?= ..
include $(TOPDIR)/build/defs.mk

.PHONY: install build clean

all: install

install: build
	$(MAKE) -f Makefile.musl install

build: $(OBJDIR)/config.mak
	@$(MAKE) -f Makefile.musl

$(OBJDIR)/config.mak: Makefile
	@CC="$(CC)" \
	CROSS_COMPILE="$(PFX)" \
	ARCH=$(ARCH) \
	CFLAGS="$(CFLAGS) -Wno-parentheses -Wno-missing-braces -Werror" \
	./configure --target=$(ARCH)-elf --disable-shared --prefix=$(OBJDIRPREFIX)  \
		--config-mak=$(OBJDIR)/config.mak > /dev/null

include $(TOPDIR)/build/pre-targets.mk

clean:
	$(MAKE) -f Makefile.musl clean
	$(RM) config.mak



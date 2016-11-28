ARCH ?= i386

CC := ~/toolchain/bin/i686-elf-gcc
LD := ~/toolchain/bin/i686-elf-gcc
STRIP := ~/toolchain/bin/i686-elf-strip
AR := ~/toolchain/bin/i686-elf-ar
OBJDIR ?= obj-$(ARCH)
ISODIR ?= iso-$(ARCH)
LIBDIR ?= lib-$(ARCH)

SUBDIRS := iso grub kernel

.PHONY: alldirs $(SUBDIRS)
all: alldirs

alldirs: $(SUBDIRS)

$(SUBDIRS):
	mkdir -p $(OBJDIR)/$@ $(ISODIR) $(LIBDIR)
	$(MAKE) -C $@ LIBDIR=$(abspath $(LIBDIR)) ISODIR=$(abspath $(ISODIR)) OBJDIR=$(abspath $(OBJDIR)/$@)

clean:
	$(RM) -rf $(OBJDIR) $(ISODIR) $(LIBDIR)

kernel: grub

iso: kernel

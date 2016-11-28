ARCH ?= i386

CC := ~/toolchain/bin/i686-elf-gcc
LD := ~/toolchain/bin/i686-elf-gcc
STRIP := ~/toolchain/bin/i686-elf-strip
AR := ~/toolchain/bin/i686-elf-ar
OBJDIR ?= $(abspath obj)
ISODIR ?= $(abspath iso)
LIBDIR ?= $(abspath lib)

export CC
export LD
export STRIP
export AR
export OBJDIR
export ISODIR
export LIBDIR
ISO := $(OBJDIR)/boot.iso

.PHONY: kernel dirs

all: dirs | $(ISO)

dirs:
	mkdir -p $(OBJDIR)
	mkdir -p $(LIBDIR)
	mkdir -p $(ISODIR)

$(ISO): kernel
	@mkdir -p $(ISODIR)/boot/grub
	cp data/grub.cfg $(ISODIR)/boot/grub
	cp $(OBJDIR)/kernel32.elf $(ISODIR)
	grub-mkrescue -o $(ISO) $(ISODIR) || rm -f $(ISO)

kernel:
	$(MAKE) -C kernel
clean:
	$(RM) -rf $(OBJDIR) $(ISODIR) $(LIBDIR)


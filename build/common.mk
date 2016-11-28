ARCH ?= i386

CC := ~/toolchain/bin/i686-elf-gcc
LD := ~/toolchain/bin/i686-elf-gcc
STRIP := ~/toolchain/bin/i686-elf-strip
AR := ~/toolchain/bin/i686-elf-ar
OBJDIR ?= obj-$(ARCH)

CFLAGS += -I$(TOP)
LDFLAGS += -L $(LIBDIR)

NASM := nasm

$(OBJDIR)/%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

$(OBJDIR)/%.o: %.s
	$(NASM) -f elf -o $@ $<


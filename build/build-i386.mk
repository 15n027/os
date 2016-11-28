
SRCS := grub/multiboot.c


KERNEL := data/kernel.elf

CC := ~/toolchain/bin/i686-elf-gcc
LD := ~/toolchain/bin/i686-elf-gcc
STRIP := ~/toolchain/bin/i686-elf-strip
include build/common.mk


$(ISO): $(KERNEL) data/*
	$(GRUB) -o $(ISO) data

$(KERNEL): $(LDSCRIPT) $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $(OBJS) $(LDLIBS) > $(KERNEL32).map

clean:
	$(RM) -rf $(KERNEL32) $(OBJDIR) $(ISO)

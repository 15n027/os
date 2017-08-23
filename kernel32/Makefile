TOPDIR ?= ..
include $(TOPDIR)/build/defs.mk

SRCS := multiboot/multiboot.c \
	main/main.c main/start.s main/earlyconsole.c \
	main/console.c \
	i386/dt.c i386/exception.c i386/exc.s i386/idt-entry.c \
	i386/cpuid.c

CFLAGS += -Werror
CPPFLAGS += -I. -Iinclude
LDSCRIPT := build/ldscript-$(ARCH)
LDFLAGS += -T $(LDSCRIPT) -Wl,--no-gc-sections
LDLIBS := -lc -lgcc

KERNEL := kernel32.elf
include $(TOPDIR)/build/pre-targets.mk

all: $(TARGETDIR)/$(KERNEL)

$(OBJDIR)/$(KERNEL): $(OBJS)
	@$(LD) -o $@ $^ $(LDFLAGS) $(LDLIBS)
	grub-file --is-x86-multiboot $@

$(TARGETDIR)/$(KERNEL): $(OBJDIR)/$(KERNEL)
	@$(INSTALLTARGET) $<

$(KERNEL): $(LDSCRIPT)

$(OBJS): i386/cpuid-entry.h

include $(TOPDIR)/build/post.mk

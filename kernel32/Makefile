TOPDIR ?= ..
include $(TOPDIR)/build/defs.mk

SRCS := multiboot/multiboot.c \
	main/main.c main/start.s main/earlyconsole.c \
	main/console.c main/loader.c main/pagealloc.c \
	i386/dt.c i386/exception.c i386/exc.s i386/idt-entry.c \
	i386/cpuid.c i386/paging.c

CFLAGS += -Werror
CPPFLAGS += -I. -Iinclude
LDSCRIPT := build/ldscript-$(ARCH)
LDFLAGS += -T $(LDSCRIPT)
LDLIBS := -lc $(LIBGCC)

KERNEL := kernel32.elf
include $(TOPDIR)/build/pre-targets.mk

all: $(TARGETDIR)/$(KERNEL)

$(OBJDIR)/$(KERNEL): $(OBJS) $(LDSCRIPT)
	$(LD) -o $@ $(OBJS) $(LDFLAGS) $(LDLIBS)
	grub-file --is-x86-multiboot $@

$(TARGETDIR)/$(KERNEL): $(OBJDIR)/$(KERNEL)
	@$(INSTALLTARGET) $<

$(OBJS): i386/cpuid-entry.h

include $(TOPDIR)/build/post.mk

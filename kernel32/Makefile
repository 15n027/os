TOPDIR ?= ..
include $(TOPDIR)/build/defs.mk

SRCS := multiboot/multiboot.c \
	main/main.c main/start.s  \
	main/console.c main/loader.c main/pagealloc.c \
	i386/dt.c i386/exception.c i386/exc.S  \
	i386/cpuid.c i386/paging.c main/serial.c

CFLAGS += -Werror
CPPFLAGS += -I. -Iinclude -I../include
LDSCRIPT := build/ldscript-$(ARCH)
LDFLAGS += -T $(LDSCRIPT)
LDLIBS := -lc $(LIBGCC)

KERNEL := kernel32.elf
include $(TOPDIR)/build/pre-targets.mk

.EXPORT_ALL_VARIABLES:
.DELETE_ON_ERROR:

all: $(TARGETDIR)/$(KERNEL)

$(OBJDIR)/$(KERNEL).orig: $(OBJS) $(LDSCRIPT)
	@$(LD) -o $@ $(OBJS) $(LDFLAGS) $(LDLIBS)

$(OBJDIR)/$(KERNEL): $(OBJDIR)/$(KERNEL).orig $(TOPDIR)/build/buildIDT
	$(TOPDIR)/build/buildIDT $< $<.idt
	$(OBJCOPY) --update-section IDTData=$<.idt $< $@
	grub-file --is-x86-multiboot $@

$(TARGETDIR)/$(KERNEL): $(OBJDIR)/$(KERNEL)
	@$(INSTALLTARGET) $<

include $(TOPDIR)/build/post.mk

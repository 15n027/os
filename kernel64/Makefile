TOPDIR ?= ..
include $(TOPDIR)/build/defs.mk

ifneq ($(ARCH), x86_64)
$(error "arch is wrong")
endif

SRCS := main.c start.s console.c x86/init.c x86/idt.c x86/exc.s \
	mm/memmap.c libc/lock.c mm/vmm.c libc/malloc.c \
	acpi/os.c legacypic.c acpi/init.c ioapic.c apic.c cpuid.c

CFLAGS += -Werror
CPPFLAGS += -I. -Iinclude -I../include
LDSCRIPT := build/ldscript-$(ARCH)
LDFLAGS += -T $(LDSCRIPT) --gc-sections -static
LDLIBS := -lacpica -lc

KERNEL := kernel64.elf
include $(TOPDIR)/build/pre-targets.mk

all: $(TARGETDIR)/$(KERNEL)

$(OBJDIR)/$(KERNEL): $(OBJS) $(LDSCRIPT)
	@$(LD) -o $@ $(OBJS) $(LDFLAGS) $(LDLIBS) -Map $(OBJDIR)/$(KERNEL).map

$(TARGETDIR)/$(KERNEL): $(OBJDIR)/$(KERNEL)
	@$(INSTALLTARGET) $<

include $(TOPDIR)/build/post.mk

TOPDIR ?= ..
include $(TOPDIR)/build/defs.mk

ifneq ($(ARCH), x86_64)
$(error "arch is wrong")
endif

SRCS := main.c start.S console.c x86/init.c x86/idt.c x86/exc.S \
	mm/memmap.c libc/lock.c mm/vmm.c libc/malloc.c \
	acpi/os.c legacypic.c acpi/init.c ioapic.c x86/apic.c x86/cpuid.c \
	serial.c smp/smp.c smp/bootAP.S percpu.c

CFLAGS += -Werror
CPPFLAGS += -I. -Iinclude -I../include
LDSCRIPT := build/ldscript-$(ARCH)
LDFLAGS += -T $(LDSCRIPT)
LDLIBS := -lacpica -lc

KERNEL := kernel64.elf
include $(TOPDIR)/build/pre-targets.mk

.EXPORT_ALL_VARIABLES:
.DELETE_ON_ERROR:

all: $(TARGETDIR)/$(KERNEL)

$(OBJDIR)/$(KERNEL).orig: $(OBJS) $(LDSCRIPT)
	$(LD) -o $@ $(OBJS) $(LDFLAGS) $(LDLIBS) -Map $(OBJDIR)/$(KERNEL).map --gc-sections

$(OBJDIR)/$(KERNEL): $(OBJDIR)/$(KERNEL).orig $(TOPDIR)/build/buildIDT
	$(TOPDIR)/build/buildIDT $(ARCH) $< $<.idt
	$(OBJCOPY) --update-section IDTData=$<.idt $< $@

$(TARGETDIR)/$(KERNEL): $(OBJDIR)/$(KERNEL)
	@$(INSTALLTARGET) $<

include $(TOPDIR)/build/post.mk

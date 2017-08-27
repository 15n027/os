TOPDIR ?= ..
include $(TOPDIR)/build/defs.mk

ifneq ($(ARCH), x86_64)
$(error "arch is wrong")
endif

SRCS := main.c start.s console.c

CFLAGS += -Werror
CPPFLAGS += -I. -Iinclude
LDSCRIPT := build/ldscript-$(ARCH)
LDFLAGS += -T $(LDSCRIPT) --gc-sections -static
LDLIBS := -lc

KERNEL := kernel64.elf
include $(TOPDIR)/build/pre-targets.mk

all: $(TARGETDIR)/$(KERNEL)

$(OBJDIR)/$(KERNEL): $(OBJS) $(LDSCRIPT)
	$(LD) -o $@ $(OBJS) $(LDFLAGS) $(LDLIBS) -Map $(OBJDIR)/$(KERNEL).map

$(TARGETDIR)/$(KERNEL): $(OBJDIR)/$(KERNEL)
	@$(INSTALLTARGET) $<

include $(TOPDIR)/build/post.mk
TOPDIR ?= ..
include $(TOPDIR)/build/defs.mk

SUBDIRS := dietlibc-0.33

SRCS := multiboot/multiboot.c \
	main/main.c main/start.s

ifeq ($(ARCH), i686)
	SRCS += i386/dt.c i386/exception.c i386/exc.s
endif

CPPFLAGS += -I.
LDSCRIPT := build/ldscript-$(ARCH)
LDFLAGS += -T $(LDSCRIPT)
LDLIBS := -ldietlibc

KERNEL := $(OBJDIR)/kernel.elf

include $(TOPDIR)/build/pre-targets.mk

all: $(KERNEL)

$(KERNEL): $(OBJS)
	@$(CC) -o $@ $^ $(LDFLAGS) $(LDLIBS)
	@$(INSTALLTARGET) $(KERNEL)

$(KERNEL): $(LDSCRIPT)


include $(TOPDIR)/build/post.mk

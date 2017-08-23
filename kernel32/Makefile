TOPDIR ?= ..
include $(TOPDIR)/build/defs.mk

#SUBDIRS := dietlibc-0.33 dev musl-1.1.16
SUBDIRS := musl-1.1.16 dev

SRCS := multiboot/multiboot.c \
	main/main.c main/start.s main/device.c main/earlyconsole.c \
	main/console.c

ifeq ($(ARCH), i686)
	SRCS += i386/dt.c i386/exception.c i386/exc.s i386/idt-entry.c
endif

CFLAGS += -Werror
CPPFLAGS += -I.
LDSCRIPT := build/ldscript-$(ARCH)
LDFLAGS += -T $(LDSCRIPT) -Wl,--no-gc-sections
#LDLIBS := -ldietlibc -lgcc
LDLIBS := -lc -lgcc

KERNEL := $(OBJDIR)/kernel.elf
KERNELTARGET := $(TARGETDIR)/kernel.elf
include $(TOPDIR)/build/pre-targets.mk

all: $(KERNEL) $(KERNELTARGET)

$(KERNEL): $(OBJS)
#	cd musl-1.1.16 && ./build.sh
	$(LD) -o $@ $^ $(LDFLAGS) $(LDLIBS)
	grub-file --is-x86-multiboot $(KERNEL)

$(KERNELTARGET): $(KERNEL)
	@$(INSTALLTARGET) $(KERNEL)

$(KERNEL): $(LDSCRIPT)

include $(TOPDIR)/build/post.mk

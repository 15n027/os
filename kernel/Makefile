TOP := ../
include $(TOP)/build/common.mk

SRCS := main.c start.s
TARGET_EXE := kernel.elf

LDLIBS += -lgcc -lmultiboot
CFLAGS += -ffreestanding -O2 -mno-sse -mno-mmx -m32 -g -ggdb -g3 -nostdlib
LDSCRIPT := ldscript-$(ARCH)

#LDFLAGS := -T $(LDSCRIPT) -Wl,--no-gc-sections -nostdlib -Wl,-M
LDFLAGS += -T $(LDSCRIPT) -Wl,--no-gc-sections -nostdlib

postbuild: $(OBJDIR)/$(TARGET_EXE)
	cp $(OBJDIR)/$(TARGET_EXE) $(ISODIR)

include $(TOP)/build/targets.mk

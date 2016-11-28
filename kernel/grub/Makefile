TOP := ../
include $(TOP)/build/common.mk

SRCS := multiboot.c
TARGET_LIB := libmultiboot.a

postbuild: $(LIBDIR)/$(TARGET_LIB)
	mkdir -p $(ISODIR)/boot/grub
	cp grub.cfg $(ISODIR)/boot/grub

multiboot.o: multiboot.h

include $(TOP)/build/targets.mk

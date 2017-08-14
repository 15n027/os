TOPDIR ?= ../
include $(TOPDIR)/build/defs.mk

ISO := $(OBJDIR)/boot.iso
ISODIR := $(OBJDIR)/iso-staging

$(ISO): $(KERNEL) grub.cfg
	@mkdir -p $(ISODIR)/boot/grub
	cp grub.cfg $(ISODIR)/boot/grub
	cp $(OBJDIR)/kernel.elf $(ISODIR)
	grub-mkrescue -o $(ISO) $(ISODIR) || rm -f $(ISO)

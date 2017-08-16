TOPDIR ?= ../
include $(TOPDIR)/build/defs.mk

ISO := $(TARGETDIR)/boot.iso
ISODIR := $(OBJDIR)/iso-staging

$(ISO): $(KERNEL) grub.cfg
	@$(INSTALL) $(ISODIR)/boot/grub grub.cfg
	@$(INSTALL) $(ISODIR) $(TARGETDIR)/kernel.elf
#	grub-mkrescue -o $(ISO) $(ISODIR) || rm -f $(ISO)
	@genisoimage -o $(ISO) $(ISODIR) || rm -f $(ISO)
	@$(INSTALL) $(OBJDIRPREFIX)/tftp $(ISO)

include $(TOPDIR)/build/post.mk

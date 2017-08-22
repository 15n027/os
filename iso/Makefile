TOPDIR ?= ..
include $(TOPDIR)/build/defs.mk

ISO := $(TARGETDIR)/boot.iso
ISODIR := $(OBJDIR)/iso-staging
KERNEL := $(TARGETDIR)/kernel.elf

$(ISO): $(KERNEL) grub.cfg Makefile
	@$(INSTALL) $(ISODIR)/boot/grub grub.cfg
	@$(INSTALL) $(ISODIR) $(TARGETDIR)/kernel.elf
	@echo "Generating " $(ISO)
	@grub-mkrescue -o $(ISO) $(ISODIR) > /dev/null 2>&1 || rm -f $(ISO)
#	@genisoimage -o $(ISO) $(ISODIR) || rm -f $(ISO)
	@$(INSTALL) $(OBJDIRPREFIX)/tftp $(ISO)

include $(TOPDIR)/build/post.mk

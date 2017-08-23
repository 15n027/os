TOPDIR ?= ..
include $(TOPDIR)/build/defs.mk

ISO := $(TARGETDIR)/boot.iso
ISODIR := $(OBJDIR)/iso-staging
KERNEL32 := kernel32.elf

$(ISO): $(TARGETDIR)/$(KERNEL32) grub.cfg Makefile
	@$(INSTALL) $(ISODIR)/boot/grub grub.cfg
	@$(INSTALL) $(ISODIR) $(TARGETDIR)/$(KERNEL32)
	@echo "Generating " $(ISO)
	@grub-mkrescue -o $(ISO) $(ISODIR) > /dev/null 2>&1 || rm -f $(ISO)
#	@genisoimage -o $(ISO) $(ISODIR) || rm -f $(ISO)
	@$(INSTALL) $(TARGETDIR)/tftp $(ISO)

include $(TOPDIR)/build/post.mk

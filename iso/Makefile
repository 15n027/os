TOPDIR ?= ..
include $(TOPDIR)/build/defs.mk

ISO := $(TARGETDIR)/boot.iso
ISODIR := $(OBJDIR)/iso-staging
KERNEL32 := kernel32.elf
KERNEL64 := kernel64.elf

$(ISO): $(TARGETDIR)/$(KERNEL32) $(TARGETDIR)/$(KERNELL64) grub.cfg Makefile
	@$(INSTALL) $(ISODIR)/boot/grub grub.cfg
	@$(INSTALL) $(ISODIR) $(TARGETDIR)/$(KERNEL32)
	@$(INSTALL) $(ISODIR) $(TARGETDIR)/$(KERNEL64)
	@echo "Generating " $(ISO)
	@grub-mkrescue -o $(ISO) $(ISODIR) > /dev/null 2>&1 || rm -f $(ISO)
#	@genisoimage -o $(ISO) $(ISODIR) || rm -f $(ISO)
	@$(INSTALL) $(TARGETDIR)/tftp $(ISO)

include $(TOPDIR)/build/post.mk

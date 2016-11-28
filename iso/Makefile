TOP := ../
include $(TOP)/build/common.mk

ISO := $(OBJDIR)/boot.iso

$(ISO):
	grub-mkrescue -o $(ISO) $(ISODIR) || rm -f $(ISO)

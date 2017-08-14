TOPDIR := $(abspath .)
export TOPDIR
include $(TOPDIR)/build/defs.mk

DIRS := kernel iso

all:
	for dir in $(DIRS); do \
		if [ -d $$dir ]; then \
			$(MAKE) -C $$dir; \
		fi; \
	done

clean:
	$(RM) -rf $(OBJDIR) $(ISODIR) $(LIBDIR)
	for dir in $(DIRS); do \
		if [ -d $$dir ]; then \
			$(MAKE) -C $$dir clean; \
		fi; \
	done


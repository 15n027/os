include build/defs.mk

SUBDIRS := kernel iso

include build/post.mk

clean:
	$(RM) -rf $(OBJDIRPREFIX)

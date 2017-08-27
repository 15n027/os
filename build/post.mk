all: $(OBJS) | subdirs

.PHONY: subdirs subdirs32 subdirs64

subdirs64:
	@for dir in $(SUBDIRS64); do \
		if [ -d $$dir ]; then \
			$(MAKE) -C $$dir ARCH=x86_64 || exit $$?; \
		fi; \
	done

subdirs32:
	@for dir in $(SUBDIRS32); do \
		if [ -d $$dir ]; then \
			$(MAKE) -C $$dir ARCH=i686 || exit $$?; \
		fi; \
	done

subdirs: subdirs32 subdirs64
	@for dir in $(SUBDIRS); do \
		if [ -d $$dir ]; then \
			$(MAKE) -C $$dir || exit $$?; \
		fi; \
	done

$(OBJS): Makefile $(TOPDIR)/build/defs.mk $(TOPDIR)/build/post.mk $(TOPDIR)/build/pre-targets.mk | subdirs

$(OBJDIR)/%.o: %.c
	@$(CC) -c -o $@ $< $(CFLAGS) $(CPPFLAGS)

$(OBJDIR)/%.o: %.s
	@$(CC) -c -o $@ $< $(CFLAGS) $(CPPFLAGS)

clean:
	$(RM) -rf $(abspath $(OBJTOPDIR)) $(OBJS)
	@for dir in $(SUBDIRS32); do \
		if [ -d $$dir ]; then \
			$(MAKE) -C $$dir ARCH=i686 clean; \
		fi; \
	done
	@for dir in $(SUBDIRS64); do \
		if [ -d $$dir ]; then \
			$(MAKE) -C $$dir ARCH=x86_64 clean; \
		fi; \
	done
	@for dir in $(SUBDIRS); do \
		if [ -d $$dir ]; then \
			$(MAKE) -C $$dir clean; \
		fi; \
	done


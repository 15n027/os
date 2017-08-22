all: $(OBJS) | subdirs

subdirs:
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
	$(RM) -rf $(OBJDIRPREFIX) $(OBJS)
	@for dir in $(SUBDIRS); do \
		if [ -d $$dir ]; then \
			$(MAKE) -C $$dir clean; \
		fi; \
	done


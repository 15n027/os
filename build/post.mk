$(OBJDIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) -c -o $@ $< $(CFLAGS) $(CPPFLAGS)

$(OBJDIR)/%.o: %.s
	@mkdir -p $(dir $@)
	$(CC) -c -o $@ $< $(CFLAGS) $(CPPFLAGS)


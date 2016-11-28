ASMSRCS := $(filter %.s, $(SRCS))
CSRCS := $(filter %.c, $(SRCS))
OBJS := $(addprefix $(OBJDIR)/, $(CSRCS:.c=.o) $(ASMSRCS:.s=.o))

ifdef TARGET_LIB
$(LIBDIR)/$(TARGET_LIB): $(OBJS)
	$(AR) -r $@ $^
	ranlib $@
endif

ifdef TARGET_EXE
$(OBJDIR)/$(TARGET_EXE): $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS) $(LDLIBS)
endif

clean:
	$(RM) -f $(OBJS) $(TARGET_LIB) $(TARGET_EXE)

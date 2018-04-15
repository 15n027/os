CSRCS := $(filter %.c, $(SRCS))
ASMSRCS := $(filter %.s, $(SRCS)) $(filter %.S, $(SRCS))

OBJS := $(addprefix $(OBJDIR)/, $(CSRCS:.c=.o)) \
	$(addprefix $(OBJDIR)/, $(addsuffix .o, $(basename $(ASMSRCS))))

$(OBJDIR)/%.o: %.S
	$(CC) -c -o $@ $< $(CPPFLAGS) $(ASFLAGS)

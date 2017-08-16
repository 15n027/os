CSRCS := $(filter %.c, $(SRCS))
ASMSRCS := $(filter %.s, $(SRCS))

OBJS := $(addprefix $(OBJDIR)/, $(CSRCS:.c=.o)) \
	$(addprefix $(OBJDIR)/, $(ASMSRCS:.s=.o))

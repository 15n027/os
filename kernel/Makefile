TOPDIR ?= ../
include $(TOPDIR)/build/defs.mk

DIRS := i386 dietlibc-0.33

SRCS := multiboot/multiboot.c i386/dt.c i386/exception.c i386/exc.s \
	main/main.c main/start.s

CSRCS := $(filter %.c, $(SRCS))
ASMSRCS := $(filter %.s, $(SRCS))

OBJS := $(addprefix $(OBJDIR)/, $(CSRCS:.c=.o)) \
	$(addprefix $(OBJDIR)/, $(ASMSRCS:.s=.o))

CPPFLAGS += -I.
LDFLAGS := -T build/ldscript-$(ARCH) -nostdlib -Wl,--no-gc-sections -L $(OBJDIR)/libc
LDLIBS := $(OBJDIR)/libc/dietlibc.a


KERNEL := $(OBJDIR)/kernel.elf

all: $(KERNEL)

$(KERNEL): $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS) $(LDLIBS)

$(OBJS): | subdirs

subdirs:
	for dir in $(DIRS); do \
		if [ -d $$dir ]; then \
			$(MAKE) -C $$dir; \
		fi; \
	done

clean:
	$(RM) $(OBJS)
	for dir in $(DIRS); do \
		if [ -d $$dir ]; then \
			$(MAKE) -C $$dir clean; \
		fi; \
	done


include $(TOPDIR)/build/post.mk

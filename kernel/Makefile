MODULES := init i386

include $(patsubst %, %/Subdir.mk, $(MODULES))

SRCS += main.c start.s
CSRC := $(filter %.c, $(SRCS))
SSRC := $(filter %.s, $(SRCS))

OBJS := $(addprefix $(OBJDIR)/,         \
	$(CSRC:.c=.o) 			\
	$(SSRC:.s=.o) 			\
	)

TARGET_EXE := kernel.elf

LDLIBS += -lgcc
CFLAGS += -ffreestanding -O3 -mno-sse -mno-mmx -m32 -g -ggdb -g3 -nostdlib \
	-std=gnu99 -Wall -Werror \
	-I$(abspath .) $(patsubst %, -I%, $(MODULES))

LDSCRIPT := build/ldscript-i386

LDFLAGS += -T $(LDSCRIPT) -Wl,--no-gc-sections -nostdlib

KERNEL32 := $(OBJDIR)/kernel32.elf
KERNEL := $(KERNEL32)

all: $(KERNEL)

$(KERNEL): $(OBJS)
	$(CC) -o $@ $^ $(LDLIBS) $(LDFLAGS)

$(OBJDIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) -c $(CFLAGS) -o $@ $<

$(OBJDIR)/%.o: %.s
	@mkdir -p $(dir $@)
	$(CC) -c $(CFLAGS) -o $@ $<

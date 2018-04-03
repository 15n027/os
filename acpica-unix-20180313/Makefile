
TOPDIR ?= ..
include $(TOPDIR)/build/defs.mk

SRCS := \
	 $(wildcard source/components/dispatcher/*.c) \
	 $(wildcard source/components/executer/*.c) \
	 $(wildcard source/components/namespace/*.c) \
	 $(wildcard source/components/resources/*.c) \
	 $(wildcard source/components/utilities/*.c) \
	 $(wildcard source/components/events/*.c) \
	 $(wildcard source/components/hardware/*.c) \
	 $(wildcard source/components/parser/*.c) \
	 $(wildcard source/components/tables/*.c)

HDRS := $(wildcard source/include/*.h)
PHDRS := $(wildcard source/include/platform/*.h)
ACPICALIB := $(OBJDIR)/libacpica.a

CPPFLAGS += -Isource/include -I$(TOPDIR)/include

include $(TOPDIR)/build/pre-targets.mk

all: headers $(ACPICALIB)

$(ACPICALIB): $(OBJS) headers
	@$(AR) -rc $(ACPICALIB) $(OBJS)
	@$(INSTALLLIB) $(ACPICALIB)

headers:
	$(INSTALL) $(OBJDIRPREFIX)/include/acpica $(HDRS)
	$(INSTALL) $(OBJDIRPREFIX)/include/acpica/platform $(PHDRS)

include $(TOPDIR)/build/post.mk

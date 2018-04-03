
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


HDRS := acapps.h acbuffer.h acclib.h accommon.h acconfig.h acconvert.h acdebug.h \
	acdisasm.h acdispat.h acevents.h acexcep.h acglobal.h achware.h acinterp.h \
	aclocal.h acmacros.h acnames.h acnamesp.h acobject.h acopcode.h acoutput.h \
	acparser.h acpi.h acpiosxf.h acpixf.h acpredef.h acresrc.h acrestyp.h acstruct.h \
	actables.h actbinfo.h actbl1.h actbl2.h actbl3.h actbl.h actypes.h acutils.h acuuid.h \
	amlcode.h amlresrc.h \
	platform/accygwin.h platform/acdragonflyex.h platform/acdragonfly.h platform/acefiex.h \
	platform/acefi.h platform/acenvex.h platform/acenv.h platform/acfreebsd.h platform/acgccex.h \
	platform/acgcc.h platform/achaiku.h platform/acintel.h platform/acjwos.h platform/aclinuxex.h \
	platform/aclinux.h platform/acmacosx.h platform/acmsvcex.h platform/acmsvc.h platform/acnetbsd.h \
	platform/acos2.h platform/acqnx.h platform/acwin64.h platform/acwin.h 

DSTHDRS := $(addprefix $(OBJDIRPREFIX)/include/acpica/,$(HDRS))

SRCHDRS := $(addprefix source/include/,$(HDRS))
ACPICALIB := $(OBJDIR)/libacpica.a

CPPFLAGS += -Isource/include -I$(TOPDIR)/include

include $(TOPDIR)/build/pre-targets.mk

all: $(ACPICALIB) $(DSTHDRS)

$(ACPICALIB): $(OBJS)
	@$(AR) -rc $(ACPICALIB) $(OBJS)
	@$(INSTALLLIB) $(ACPICALIB)

$(OBJDIRPREFIX)/include/acpica/%.h: source/include/%.h
	@mkdir -p $(dir $@)
	cp $< $@

include $(TOPDIR)/build/post.mk

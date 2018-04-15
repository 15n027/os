#pragma once

enum {
    IOAPIC_REG_ID  = 0,
    IOAPIC_REG_VER = 1,
    IOAPIC_REG_ARB = 2,
};
#define IOAPIC_REDTBL(x)  (0x10 + (x) * 2)
#define IOAPIC_REDTBL_MAX (24)

enum {
    IOAPIC_TRIGGER_MASK       = 1 << 15,
    IOAPIC_TRIGGER_LEVEL      = IOAPIC_TRIGGER_MASK,
    IOAPIC_TRIGGER_EDGE       = 0,
    IOAPIC_POLARITY_MASK      = 1 << 13,
    IOAPIC_POLARITY_ACTIVE_HI = 0,
    IOAPIC_POLARITY_ACTIVE_LO = IOAPIC_POLARITY_MASK,
};

typedef struct {
    uint32 trigger;
    uint32 polarity;
    uint8 irq;
} IOApicPin;

bool RegisterIOApic(PA addr, const IOApicPin pins[static IOAPIC_REDTBL_MAX]);

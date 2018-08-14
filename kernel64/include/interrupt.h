#pragma once

#include "basic_types.h"
#include "x86/exception.h"

typedef struct IretFrame {
    VA rip;
    uint64 cs;
    uint64 flags;
    VA rsp;
    uint64 ss;
} IretFrame;

typedef bool (*interrupt_handler)(IretFrame *frame);

bool install_handler(uint8 vector, interrupt_handler fn);

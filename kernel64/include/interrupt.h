#pragma once

#include "basic_types.h"
#include "x86/x86_defs.h"

typedef bool (*interrupt_handler)(IretFrame *frame, void *ctx);

bool install_handler(uint8 vector, interrupt_handler fn, void *ctx);

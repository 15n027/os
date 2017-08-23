#pragma once
#include "linker_set.h"

typedef void (*sys_startup_fn)(void);
SET_DECLARE(sys_startup, sys_startup_fn);

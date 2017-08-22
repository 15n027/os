#pragma once
#include "linker_set.h"

typedef struct Device {
    int (*probe)(struct Device *);
    int (*attach)(struct Device *);
    int (*write)(struct Device *, void *buf, size_t len);
} Device;

typedef void (*device_register_fn)(void);
SET_DECLARE(device_init_set, device_register_fn);

void device_register(void);

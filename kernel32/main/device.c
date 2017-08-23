#include "linker_set.h"
#include "device.h"

SET_DEFINE(device_init_set, device_register_fn);

void
device_register(void)
{
    device_register_fn *fn;
    SET_FOREACH(device_init_set, fn) {
        (*fn)();
    }
}

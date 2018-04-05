#include "basic_types.h"
#include "x86/exception.h"

typedef bool (*interrupt_handler)(IntrFrame64 *frame);

bool install_handler(uint8 vector, interrupt_handler fn);

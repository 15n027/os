#include "dietfeatures.h"
#include <unistd.h>
#include <string.h>

#define _BSD_SOURCE
#undef __attribute_dontuse__
#define __attribute_dontuse__
#include <errno.h>


char*strerror(int errnum) {
    return "STRERROR UNIMPLEMENTED";
}

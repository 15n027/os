#include <unistd.h>
#include <string.h>
#include "dietstdio.h"
#include "dietfeatures.h"
int __stdio_outs(const char *s,size_t len);

int puts(const char *s) {
  return (__stdio_outs(s,strlen(s)) && __stdio_outs("\n",1))?0:-1;
}


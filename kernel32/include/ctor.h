#ifndef CTOR_H
#define CTOR_H

#define CTOR(fn) \
    asm(".section .ctors\n" \
        ".long " #fn "\n")

#endif

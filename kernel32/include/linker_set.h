#ifndef LINKER_SET_H
#define LINKER_SET_H
#include <stddef.h>
#include "basic_defs.h"

#define CONCAT(x, y) x##y
#define CONCAT2(x, y) CONCAT(x, y)

#define LINKER_SET_(set) linker_set_##set
#define SET_PREFIX(set) linker_set_##set
#define SET_TYPE(set) struct CONCAT2(SET_PREFIX(set), _t)
#define SET_HEAD(set) CONCAT2(SET_PREFIX(set), _head)
#define SET_TAIL(set) CONCAT2(SET_PREFIX(set), _tail)

#define SET_DECLARE(set, data_type)                         \
    SET_TYPE(set) { data_type data; SET_TYPE(set) *next; }; \
    extern SET_TYPE(set) SET_PREFIX(set);                   \
    extern SET_TYPE(set) *SET_HEAD(set);                    \
    extern SET_TYPE(set) *SET_TAIL(set)


#define SET_DEFINE(set, data_type) \
    SET_TYPE(set) SET_PREFIX(set) = {0, 0}; \
    SET_TYPE(set) *SET_HEAD(set) = &SET_PREFIX(set);  \
    SET_TYPE(set) *SET_TAIL(set) = &SET_PREFIX(set)


#define TEXT_SET(set, fn) TEXT_SET2(set, fn, __COUNTER__)
#define TEXT_SET2(set, fn, uniq) TEXT_SET3(set, fn, uniq)
#define TEXT_SET3(set, fn, uniq)                                        \
        static void __attribute__((constructor)) do_text_set_ ## set ## uniq (void) { \
        static SET_TYPE(set) obj;                                          \
        obj.data = fn;                                                  \
        obj.next = 0;                                                   \
        SET_TAIL(set)->next = &obj;                                     \
        SET_TAIL(set) = &obj;                                           \
    }
#define SET_LISTITEM(set, var) container_of(var, SET_TYPE(set), data)
#define SET_NEXT(set, var) SET_LISTITEM(set, var)->next
#define SET_FIRSTITEM(set) SET_HEAD(set)->next
#define SET_FOREACH(set, var) \
    for (var = SET_FIRSTITEM(set) ? &SET_FIRSTITEM(set)->data : 0; \
         SET_LISTITEM(set, var) != 0;                        \
         var = SET_NEXT(set, var) ? &SET_NEXT(set, var)->data : 0)

#endif

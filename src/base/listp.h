#ifndef COIN_BASE_LISTP_H
#define COIN_BASE_LISTP_H

#ifndef COIN_INTERNAL
#error this is a private header file
#endif

#include <Inventor/C/base/list.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Append without changing the list if its buffer cannot grow. */
SbBool cc_list_try_append(cc_list * list, void * item);
SbBool cc_list_try_reserve(cc_list * list, int capacity);

#ifdef __cplusplus
}
#endif

#endif

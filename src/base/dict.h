#ifndef CC_DICT_H
#define CC_DICT_H

/**************************************************************************\
 * Copyright (c) Kongsberg Oil & Gas Technologies AS
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 * 
 * Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 * 
 * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 * 
 * Neither the name of the copyright holder nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
\**************************************************************************/

#ifndef COIN_INTERNAL
#error this is a private header file
#endif // COIN_INTERNAL

#include <stdlib.h>
#include <Inventor/C/basic.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

  typedef struct cc_dict cc_dict;
  typedef uintptr_t cc_dict_hash_func(const uintptr_t key);
  typedef void cc_dict_apply_func(uintptr_t key, void * val, void * closure);
  typedef enum cc_dict_put_result {
    CC_DICT_PUT_FAILED = -1,
    CC_DICT_PUT_REPLACED = 0,
    CC_DICT_PUT_INSERTED = 1
  } cc_dict_put_result;

  /* Returns NULL on allocation failure. Non-finite or nonpositive load
     factors use 0.75; finite positive factors above one are accepted. */
  cc_dict * cc_dict_construct(unsigned int size, float loadfactor);
  void cc_dict_destruct(cc_dict * ht);
  void cc_dict_clear(cc_dict * ht);

  /* Distinguishes resource failure, replacement, and new insertion. On
     failure the dictionary and the previous value remain unchanged. */
  cc_dict_put_result cc_dict_try_put(cc_dict * ht, uintptr_t key, void * val);
  /* Legacy return: TRUE for insertion, FALSE for replacement or failure.
     Use cc_dict_try_put() when failure must be distinguished. */
  SbBool cc_dict_put(cc_dict * ht, uintptr_t key, void * val);
  SbBool cc_dict_get(cc_dict * ht, uintptr_t key, void ** val);
  SbBool cc_dict_remove(cc_dict * ht, uintptr_t key);
  /* The callback may remove its current entry. It must not otherwise change
     this dictionary, including insertion, clear, rehash, or destruction. */
  void cc_dict_apply(cc_dict * ht, cc_dict_apply_func * func, void * closure);

  unsigned int cc_dict_get_num_elements(cc_dict * ht);

  /* func must not be NULL. Existing entries are reindexed when the hash
     function is changed. On allocation failure, the old hash stays active. */
  void cc_dict_set_hash_func(cc_dict * ht, cc_dict_hash_func * func);
  void cc_dict_print_stat(cc_dict * ht);

/* ********************************************************************** */

#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */

#endif /* ! CC_DICT_H */

/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2001 by Systems in Motion.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  version 2 as published by the Free Software Foundation.  See the
 *  file LICENSE.GPL at the root directory of this source distribution
 *  for more details.
 *
 *  If you desire to use Coin with software that is incompatible
 *  licensewise with the GPL, and / or you would like to take
 *  advantage of the additional benefits with regard to our support
 *  services, please contact Systems in Motion about acquiring a Coin
 *  Professional Edition License.  See <URL:http://www.coin3d.org> for
 *  more information.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  <URL:http://www.sim.no>, <mailto:support@sim.no>
 *
\**************************************************************************/

/* FIXME: support HP-UX 10.x? (Doesn't have dlopen().) 20010626 mortene. */


#ifdef HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */

#ifdef HAVE_DLFCN_H
#include <dlfcn.h> /* Programming interface to libdl. */
#endif /* HAVE_DLFCN_H */

#include <stddef.h> /* NULL definition. */
#include <malloc.h>
#include <Inventor/C/glue/dl.h>

struct cc_libhandle_struct {
  void * nativehnd;
};

cc_libhandle
cc_dl_open(const char * filename)
{
#ifdef HAVE_DL_LIB
  cc_libhandle h = malloc(sizeof(struct cc_libhandle_struct));
  /* if (!h), FIXME: error handling. 20020906 mortene. */
  h->nativehnd = dlopen(filename, RTLD_LAZY);
  /* if (!h) FIXME: Error handling (kintel 20011121) */
  return h;
#endif
  return NULL;
}

void *
cc_dl_sym(cc_libhandle handle, const char * symbolname)
{
  void * ptr = NULL;
  if ((handle == NULL) || (handle->nativehnd == NULL)) return NULL;

#ifdef HAVE_DL_LIB
  ptr = dlsym(handle->nativehnd, symbolname);
#endif /* HAVE_DL_LIB */

  return ptr;
}

void
cc_dl_close(cc_libhandle handle)
{
#ifdef HAVE_DL_LIB
  (void)dlclose(handle->nativehnd);
#endif /* HAVE_DL_LIB */
  
  free(handle);
}

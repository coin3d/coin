/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2003 by Systems in Motion.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Systems in Motion about acquiring
 *  a Coin Professional Edition License.
 *
 *  See <URL:http://www.coin3d.org> for  more information.
 *
 *  Systems in Motion, Teknobyen, Abels Gate 5, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no>.
 *
\**************************************************************************/

/* FIXME: this is a very early work-in-progress, it seems. 20020915 mortene.

#include <Inventor/C/base/name.h>

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */

/* ********************************************************************** */

cc_name *
cc_name_struct_malloc(void)
{
  cc_name * name_struct;
  name_struct = (cc_name *) malloc(sizeof(cc_name));
  assert(name_struct != NULL);
  return name_struct;  
} /* cc_name_struct_malloc() */

void
cc_name_struct_init(cc_name * name_struct)
{
  assert(name_struct != NULL);
} /* cc_name_struct_init() */

void
cc_name_struct_clean(cc_name * name_struct)
{
  assert(name_struct != NULL);
} /* cc_name_struct_clean() */

void
cc_name_struct_free(cc_name * name_struct)
{
  if ( name_struct ) free(name_struct);
} /* cc_name_struct_free() */

/* ********************************************************************** */

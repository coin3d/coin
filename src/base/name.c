/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2001 by Systems in Motion.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 2 as
 *  published by the Free Software Foundation.  See the file LICENSE.GPL
 *  at the root directory of this source distribution for more details.
 *
 *  If you desire to use Coin with software that is incompatible
 *  licensewise with the GPL, and / or you would like to take
 *  advantage of the additional benefits with regard to our support
 *  services, please contact Systems in Motion about acquiring a Coin
 *  Professional Edition License.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  www.sim.no, support@sim.no, Voice: +47 22114160, Fax: +47 22207097
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

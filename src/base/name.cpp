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

// FIXME: this implements a C ADT for doing unique string
// mappings. It's basically just a quick hack of wrapping the C ADT
// around our SbName C++ class. We should really correct this to be
// the other way around.
//
// Don't make this class public until the C rewrite happens.
//
// 20030606 mortene.

// FIXME: couldn't the API be better implemented as simply:
//
//  const char * cc_namemap_add_string(const char *); // Returns ptr in map.
//  const char * cc_namemap_get_address(const char *); // Return NULL if non-existing.
//
// ?
//
// That actually seems sufficient to me (but I haven't given it much
// thought yet). I believe SbName could have been layered on top of
// those two simple functions.
//
// 20030606 mortene.

#include <Inventor/C/base/name.h>
#include <Inventor/SbName.h>

/* ********************************************************************** */

struct cc_name {
  SbName * cppname;
};

cc_name *
cc_name_new(const char * str)
{
  cc_name * n = new struct cc_name;
  n->cppname = new SbName(str);
  return n;
}

void
cc_name_destruct(cc_name * name)
{
  delete name->cppname;
  delete name;
}

const char *
cc_name_get_string(const cc_name * name)
{
  return name->cppname->getString();
}

unsigned int
cc_name_get_length(const cc_name * name)
{
  return name->cppname->getLength();
}

SbBool
cc_name_equals(const cc_name * name1, const cc_name * name2)
{
  return *(name1->cppname) == *(name2->cppname);
}

SbBool
cc_name_equals_string(const cc_name * name, const char * str)
{
  return *(name->cppname) == str;
}

/* ********************************************************************** */

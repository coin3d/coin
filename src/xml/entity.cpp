/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) by Kongsberg Oil & Gas Technologies.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Kongsberg Oil & Gas Technologies
 *  about acquiring a Coin Professional Edition License.
 *
 *  See http://www.coin3d.org/ for more information.
 *
 *  Kongsberg Oil & Gas Technologies, Bygdoy Alle 5, 0257 Oslo, NORWAY.
 *  http://www.sim.no/  sales@sim.no  coin-support@coin3d.org
 *
\**************************************************************************/

#include <Inventor/C/XML/entity.h>

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H

#include <stdlib.h>
#include <assert.h>

// *************************************************************************

struct cc_xml_ent {
  const char * name;
  const char * value;
};

// *************************************************************************

cc_xml_ent * 
cc_xml_ent_new(void)
{
  cc_xml_ent * ent = new cc_xml_ent;
  ent->name = NULL;
  ent->value = NULL;
  return ent;
}

void
cc_xml_ent_delete(cc_xml_ent * ent)
{
  delete ent;
}

const char *
cc_xml_ent_get_name(cc_xml_ent * ent)
{
  assert(ent);
  return ent->name;
}

const char *
cc_xml_ent_get_value(cc_xml_ent * ent)
{
  assert(ent);
  return ent->value;
}

// *************************************************************************

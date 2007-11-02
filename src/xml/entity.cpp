/**************************************************************************\
 *
 *  This file is part of the SIM Scenery library.
 *  Copyright (C) 2000-2007 by Systems in Motion.  All rights reserved.
 *
 *  This software is proprietary to and embodies the confidential 
 *  technology of Systems in Motion.  Possession, use, or copying of this
 *  software and media is authorized only pursuant to a valid written
 *  license from Systems in Motion or an authorized sublicensor. 
 *
 *  For more information, contact SIM <http://www.sim.no/> by means of:
 *  Mail: Systems in Motion AS, Bygdøy allé 5, N-0257 Oslo, Norway;
 *  Email: <sales@sim.no>; Voice: +47 23 27 25 10; Fax: +47 23 27 25 11.
 *
\**************************************************************************/

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H

#include <stdlib.h>
#include <assert.h>

#include <Inventor/C/XML/entity.h>

// *************************************************************************

struct cc_xml_ent {
  const char * name;
  const char * value;
};

// *************************************************************************

cc_xml_ent * 
cc_xml_ent_new(void)
{
  cc_xml_ent * ent = (cc_xml_ent *) malloc(sizeof(struct cc_xml_ent));
  ent->name = NULL;
  ent->value = NULL;
  return ent;
}

void
cc_xml_ent_delete(cc_xml_ent * ent)
{
  free(ent);
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

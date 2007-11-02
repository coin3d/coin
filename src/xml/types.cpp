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

#include <Inventor/C/XML/types.h>

#include <Inventor/C/XML/document.h>
#include <Inventor/C/XML/element.h>
#include <Inventor/C/XML/attribute.h>
#include <Inventor/C/XML/entity.h>
#include <Inventor/C/XML/path.h>

/* ********************************************************************** */

BuxDocument *
cc_xml_wrap(cc_xml_doc * doc, int pass_ownership)
{
  return new BuxDocument(doc);
}

cc_xml_doc *
cc_xml_unwrap(BuxDocument * doc, int pass_ownership)
{
  return doc->getHandle();
}

BuxElement *
cc_xml_wrap(cc_xml_elt * elt, int pass_ownership)
{
  return BuxElement(elt);
}

cc_xml_elt *
cc_xml_unwrap(BuxElement * elt, int pass_ownership)
{
  return elt->getHandle();
}

BuxAttribute *
cc_xml_wrap(cc_xml_attr * attr, int pass_ownership)
{
  return BuxAttribute(attr);
}

cc_xml_attr *
cc_xml_unwrap(BuxAttribute * attr, int pass_ownership)
{
  return attr->getHandle();
}

BuxEntity *
cc_xml_wrap(cc_xml_ent * ent, int pass_ownership)
{
  return BuxEntity(ent);
}

cc_xml_ent *
cc_xml_unwrap(BuxEntity * ent, int pass_ownership)
{
  return ent->getHandle();
}

BuxPath *
cc_xml_wrap(cc_xml_path * path, int pass_ownership)
{
  return BuxPath(path);
}

cc_xml_path *
cc_xml_unwrap(BuxPath * path, int pass_ownership)
{
  return path->getHandle();
}

/* ********************************************************************** */

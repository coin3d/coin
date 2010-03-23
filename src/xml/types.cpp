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

/*! \file types.h */
#include <Inventor/C/XML/types.h>

#include <Inventor/C/XML/document.h>
#include <Inventor/C/XML/element.h>
#include <Inventor/C/XML/attribute.h>
#include <Inventor/C/XML/entity.h>
#include <Inventor/C/XML/path.h>

/*!
  \var cc_xml_filter_cb

  Called twice for each element while parsing input, if set.  The push
  argument tells you if the element is being pushed (or opened) or popped
  (closed).  The return value is only considered during the pop callback.
*/

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

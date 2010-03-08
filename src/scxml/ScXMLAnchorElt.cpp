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

#include <Inventor/scxml/ScXMLAnchorElt.h>

/*!
  \class ScXMLAnchorElt ScXMLAnchorElt.h Inventor/scxml/ScXMLAnchorElt.h
  \brief implements the &lt;anchor&gt; SCXML element.

  The &lt;anchor&gt; element is not supported yet other than as a
  dummy state.

  \since Coin 3.0
  \ingroup scxml
*/

#include <cassert>
#include <cstring>

#include <Inventor/errors/SoDebugError.h>
#include <Inventor/C/XML/element.h>
#include <Inventor/scxml/ScXML.h>

#include "scxml/ScXMLCommonP.h"
#include "SbBasicP.h"

// *************************************************************************

class ScXMLAnchorEltReader : public ScXMLEltReader {
public:
  ScXMLAnchorEltReader(void);
  virtual ScXMLElt * read(ScXMLElt * container, cc_xml_elt * xmlelt, ScXMLDocument * doc, ScXMLStateMachine * sm);
};

ScXMLAnchorEltReader::ScXMLAnchorEltReader(void)
: ScXMLEltReader("anchor")
{
}

ScXMLElt *
ScXMLAnchorEltReader::read(ScXMLElt * container, cc_xml_elt * xmlelt, ScXMLDocument * COIN_UNUSED_ARG(doc), ScXMLStateMachine * COIN_UNUSED_ARG(sm))
{
  assert(container && xmlelt);
  ScXMLAnchorElt * anchor = new ScXMLAnchorElt;
  anchor->setContainer(container);
  this->setXMLAttributes(anchor, xmlelt);

  // handle XML attributes
  if (unlikely(!anchor->handleXMLAttributes())) {
    delete anchor;
    return NULL;
  }

  const int numchildren = cc_xml_elt_get_num_children(xmlelt);
  for (int c = 0; c < numchildren; ++c) {
    cc_xml_elt * element = cc_xml_elt_get_child(xmlelt, c);
    const char * elementtype = cc_xml_elt_get_type(element);

    if (strcmp(elementtype, COIN_XML_CDATA_TYPE) == 0) {
      // ignore CDATA
      continue;
    }

    SoDebugError::post("ScXMLAnchorEltReader::read",
                       "unexpected XML element '<%s>' found in <anchor>",
                       elementtype);
    delete anchor;
    return NULL;
  }

  return anchor;
}

// *************************************************************************

class ScXMLAnchorElt::PImpl {
public:
};

#define PRIVATE

SCXML_ELEMENT_SOURCE(ScXMLAnchorElt);

void
ScXMLAnchorElt::initClass(void)
{
  SCXML_OBJECT_INIT_CLASS(ScXMLAnchorElt, ScXMLElt, "ScXMLElt");
  SCXML_ELEMENT_REGISTER_READER(ScXMLAnchorElt, "anchor", ScXMLAnchorEltReader);
}

void
ScXMLAnchorElt::cleanClass(void)
{
  SCXML_ELEMENT_UNREGISTER_READER(ScXMLAnchorElt);
  ScXMLAnchorElt::classTypeId = SoType::badType();
}

ScXMLAnchorElt::ScXMLAnchorElt(void)
: type(NULL),
  snapshot(NULL)
{
}

ScXMLAnchorElt::~ScXMLAnchorElt(void)
{
  this->setTypeAttribute(NULL);
  this->setSnapshotAttribute(NULL);
}

void
ScXMLAnchorElt::setTypeAttribute(const char * typestr)
{
  SCXML__SET_ATTRIBUTE_VALUE(this->type, "type", typestr);
}

void
ScXMLAnchorElt::setSnapshotAttribute(const char * snapshotstr)
{
  SCXML__SET_ATTRIBUTE_VALUE(this->snapshot, "snapshot", snapshotstr);
}

SbBool
ScXMLAnchorElt::handleXMLAttributes(void)
{
  if (!inherited::handleXMLAttributes()) return FALSE;

  this->setTypeAttribute(this->getXMLAttribute("type"));
  this->setSnapshotAttribute(this->getXMLAttribute("snapshot"));

  if (this->type == NULL) {
    return FALSE;
  }

  return TRUE;
}

void
ScXMLAnchorElt::copyContents(const ScXMLElt * rhs)
{
  inherited::copyContents(rhs);
  const ScXMLAnchorElt * orig = coin_assert_cast<const ScXMLAnchorElt *>(rhs);
  this->setTypeAttribute(orig->getTypeAttribute());
  this->setSnapshotAttribute(orig->getSnapshotAttribute());
}

const ScXMLElt *
ScXMLAnchorElt::search(const char * attrname, const char * attrvalue) const
{
  const ScXMLElt * hit = inherited::search(attrname, attrvalue);
  if (hit) {
    return hit;
  }
  if (strcmp(attrname, "type") == 0) {
    if (this->type && strcmp(attrvalue, this->type) == 0) {
      return this;
    }
  }
  else if (strcmp(attrname, "snapshot") == 0) {
    if (this->snapshot && strcmp(attrvalue, this->snapshot) == 0) {
      return this;
    }
  }
  return NULL;
}

#undef PRIVATE

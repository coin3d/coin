/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2008 by Kongsberg SIM.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Kongsberg SIM about acquiring
 *  a Coin Professional Edition License.
 *
 *  See http://www.coin3d.org/ for more information.
 *
 *  Kongsberg SIM, Postboks 1283, Pirsenteret, 7462 Trondheim, NORWAY.
 *  http://www.sim.no/  sales@sim.no  coin-support@coin3d.org
 *
\**************************************************************************/

#include <Inventor/scxml/ScXMLAnchor.h>

#include <assert.h>
#include <string.h>

#include <Inventor/scxml/ScXML.h>

// *************************************************************************

/*!
  \class ScXMLAnchor ScXMLAnchor.h Inventor/scxml/ScXMLAnchor.h
  \brief Implementation of the &lt;anchor&gt; SCXML element.

  The &lt;anchor&gt; element is not supported yet other than as a
  dummy state.

  \since Coin 3.0
  \ingroup scxml
*/

class ScXMLAnchorP {
public:
};

SCXML_OBJECT_SOURCE(ScXMLAnchor);

void
ScXMLAnchor::initClass(void)
{
  SCXML_OBJECT_INIT_CLASS(ScXMLAnchor, ScXMLObject, SCXML_DEFAULT_NS, "anchor");
}

ScXMLAnchor::ScXMLAnchor(void)
  : type(NULL), snapshot(NULL)
{
}

ScXMLAnchor::~ScXMLAnchor(void)
{
  this->setTypeAttribute(NULL);
  this->setSnapshotAttribute(NULL);
}

void
ScXMLAnchor::setTypeAttribute(const char * typestr)
{
  if (this->type && this->type != this->getXMLAttribute("type")) {
    delete [] this->type;
  }
  this->type = NULL;
  if (typestr) {
    this->type = new char [ strlen(typestr) + 1 ];
    strcpy(this->type, typestr);
  }
}

void
ScXMLAnchor::setSnapshotAttribute(const char * snapshotstr)
{
  if (this->snapshot && this->snapshot != this->getXMLAttribute("snapshot")) {
    delete [] this->snapshot;
  }
  this->snapshot = NULL;
  if (snapshotstr) {
    this->snapshot = new char [ strlen(snapshotstr) + 1 ];
    strcpy(this->snapshot, snapshotstr);
  }
}

SbBool
ScXMLAnchor::handleXMLAttributes(void)
{
  if (!inherited::handleXMLAttributes()) return FALSE;

  this->type = const_cast<char *>(this->getXMLAttribute("type"));
  this->snapshot = const_cast<char *>(this->getXMLAttribute("snapshot"));

  if (!this->type) {
    return FALSE;
  }

  return TRUE;
}

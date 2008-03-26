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

#include <Inventor/scxml/ScXML.h>

SCXML_OBJECT_SOURCE(ScXMLAnchor);

/*!
  \class ScXMLAnchor Inventor/scxml/ScXMLAnchor.h
  \brief not implemented
*/

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
  this->setTypeXMLAttr(NULL);
  this->setSnapshotXMLAttr(NULL);
}

void
ScXMLAnchor::setTypeXMLAttr(const char * typestr)
{
  if (this->type && this->type != this->getAttribute("type")) {
    delete [] const_cast<char *>(this->type);
  }
  this->type = NULL;
  if (typestr) {
    char * buffer = new char [ strlen(typestr) + 1 ];
    strcpy(buffer, typestr);
    this->type = buffer;
  }
}

void
ScXMLAnchor::setSnapshotXMLAttr(const char * snapshotstr)
{
  if (this->snapshot && this->snapshot != this->getAttribute("snapshot")) {
    delete [] const_cast<char *>(this->snapshot);
  }
  this->snapshot = NULL;
  if (snapshotstr) {
    char * buffer = new char [ strlen(snapshotstr) + 1 ];
    strcpy(buffer, snapshotstr);
    this->snapshot = buffer;
  }
}

SbBool
ScXMLAnchor::handleXMLAttributes(void)
{
  this->type = this->getAttribute("type");
  this->snapshot = this->getAttribute("snapshot");

  if (!this->type) {
    return FALSE;
  }

  return TRUE;
}


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

#include <Inventor/scxml/ScXMLStateBase.h>

#include <assert.h>
#include <string.h>

SCXML_OBJECT_ABSTRACT_SOURCE(ScXMLStateBase);

void
ScXMLStateBase::initClass(void)
{
  SCXML_OBJECT_INIT_ABSTRACT_CLASS(ScXMLStateBase, ScXMLObject);
}

ScXMLStateBase::ScXMLStateBase(void)
  : id(NULL)
{
}

ScXMLStateBase::~ScXMLStateBase(void)
{
  this->setIdXMLAttr(NULL);
}

void
ScXMLStateBase::setIdXMLAttr(const char * idstr)
{
  if (this->id && this->id != this->getAttribute("id")) {
    delete [] const_cast<char *>(this->id);
  }
  this->id = NULL;
  if (idstr) {
    char * buffer = new char [ strlen(idstr) + 1 ];
    strcpy(buffer, idstr);
    this->id = buffer;
  }
}

// const char * ScXMLState::getIdXMLAttr(void) const

SbBool
ScXMLStateBase::handleXMLAttributes(void)
{
  if (!inherited::handleXMLAttributes()) return FALSE;

  this->id = this->getAttribute("id");

  if (!this->id) { return FALSE; }

  return TRUE;
}

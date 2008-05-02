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

#include <Inventor/scxml/ScXMLDocument.h>

#include <assert.h>
#include <algorithm>
#include <vector>

#include <Inventor/scxml/ScXML.h>
#include <Inventor/scxml/ScXMLState.h>
#include <Inventor/scxml/ScXMLFinal.h>

#include "scxml/ScXMLCommonP.h"

// *************************************************************************

/*!
  \class ScXMLDocument ScXMLDocument.h Inventor/scxml/ScXMLDocument.h
  \brief Implementation of the root &lt;scxml&gt; SCXML element.

  \since Coin 3.0
  \ingroup scxml
*/

class ScXMLDocumentP {
public:
  std::vector<ScXMLState *> statelist;
  std::vector<ScXMLState *> parallellist;
  std::vector<ScXMLFinal *> finallist;
  // datamodel

};

#define PRIVATE(obj) ((obj)->pimpl)

SCXML_OBJECT_SOURCE(ScXMLDocument);

void
ScXMLDocument::initClass(void)
{
  SCXML_OBJECT_INIT_CLASS(ScXMLDocument, ScXMLObject, SCXML_DEFAULT_NS, "scxml");
}

ScXMLDocument::ScXMLDocument(void)
  : referenced(FALSE),
    xmlns(NULL), version(NULL), initialstate(NULL)
{
}

ScXMLDocument::~ScXMLDocument(void)
{
  this->setXMLNSAttribute(NULL);
  this->setVersionAttribute(NULL);
  this->setInitialStateAttribute(NULL);

  {
    std::vector<ScXMLState *>::iterator stateit =
      PRIVATE(this)->statelist.begin();
    while (stateit != PRIVATE(this)->statelist.end()) {
      delete *stateit;
      ++stateit;
    }
    PRIVATE(this)->statelist.clear();
  }

  {
    std::vector<ScXMLState *>::iterator parallelit =
      PRIVATE(this)->parallellist.begin();
    while (parallelit != PRIVATE(this)->parallellist.end()) {
      delete *parallelit;
      ++parallelit;
    }
    PRIVATE(this)->parallellist.clear();
  }

  {
    std::vector<ScXMLFinal *>::iterator finalit =
      PRIVATE(this)->finallist.begin();
    while (finalit != PRIVATE(this)->finallist.end()) {
      delete *finalit;
      ++finalit;
    }
    PRIVATE(this)->finallist.clear();
  }
}

// *************************************************************************

void
ScXMLDocument::setIsReferenced(SbBool isreferenced)
{
  this->referenced = isreferenced;
}

SbBool
ScXMLDocument::isReferenced(void) const
{
  return this->referenced;
}


// *************************************************************************

void
ScXMLDocument::setXMLNSAttribute(const char * xmlnsstr)
{
  if (this->xmlns && this->xmlns != this->getXMLAttribute("xmlns")) {
    delete [] this->xmlns;
  }
  this->xmlns = NULL;
  if (xmlnsstr) {
    this->xmlns = new char [ strlen(xmlnsstr) + 1 ];
    strcpy(this->xmlns, xmlnsstr);
  }
}

// const char * ScXMLDocument::getXMLNSAttribute(void) const

void
ScXMLDocument::setVersionAttribute(const char * versionstr)
{
  if (this->version && this->version != this->getXMLAttribute("version")) {
    delete [] this->version;
  }
  this->version = NULL;
  if (versionstr) {
    this->version = new char [ strlen(versionstr) + 1 ];
    strcpy(this->version, versionstr);
  }
}

// const char * ScXMLDocument::getVersionAttribute(void) const
  
void
ScXMLDocument::setInitialStateAttribute(const char * initialstatestr)
{
  if (this->initialstate &&
      (this->initialstate != this->getXMLAttribute("initialstate"))) {
    delete [] this->initialstate;
  }
  this->initialstate = NULL;
  if (initialstatestr) {
    this->initialstate = new char [ strlen(initialstatestr) + 1 ];
    strcpy(this->initialstate, initialstatestr);
  }
}

// const char * ScXMLDocument::getInitialStateAttribute(void) const

SbBool
ScXMLDocument::handleXMLAttributes(void)
{
  if (!inherited::handleXMLAttributes()) return FALSE;

  this->xmlns = const_cast<char *>(this->getXMLAttribute("xmlns"));
  this->version = const_cast<char *>(this->getXMLAttribute("version"));
  this->initialstate = const_cast<char *>(this->getXMLAttribute("initialstate"));

  if (!this->referenced && this->initialstate == NULL) {
    // requirement for root document, but not for referenced documents
    return FALSE;
  }

  return TRUE;
}

// *************************************************************************

SCXML_LIST_OBJECT_API_IMPL(ScXMLDocument, ScXMLState, PRIVATE(this)->statelist, State, States);

SCXML_LIST_OBJECT_API_IMPL(ScXMLDocument, ScXMLState, PRIVATE(this)->parallellist, Parallel, Parallels);

SCXML_LIST_OBJECT_API_IMPL(ScXMLDocument, ScXMLFinal, PRIVATE(this)->finallist, Final, Finals);

// *************************************************************************

#undef PRIVATE

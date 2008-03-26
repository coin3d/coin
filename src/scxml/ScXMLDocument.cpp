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

#include <Inventor/scxml/ScXML.h>
#include <Inventor/scxml/ScXMLState.h>
#include <Inventor/scxml/ScXMLFinal.h>

#include "scxml/ScXMLCommonP.h"

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
  this->setXMLNSXMLAttr(NULL);
  this->setVersionXMLAttr(NULL);
  this->setInitialStateXMLAttr(NULL);

  {
    std::vector<const ScXMLState *>::iterator stateit = this->statelist.begin();
    while (stateit != this->statelist.end()) {
      delete *stateit;
      ++stateit;
    }
    this->statelist.clear();
  }

  {
    std::vector<const ScXMLState *>::iterator parallelit = this->parallellist.begin();
    while (parallelit != this->parallellist.end()) {
      delete *parallelit;
      ++parallelit;
    }
    this->parallellist.clear();
  }

  {
    std::vector<const ScXMLFinal *>::iterator finalit = this->finallist.begin();
    while (finalit != this->finallist.end()) {
      delete *finalit;
      ++finalit;
    }
    this->finallist.clear();
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
ScXMLDocument::setXMLNSXMLAttr(const char * xmlnsstr)
{
  if (this->xmlns && this->xmlns != this->getAttribute("xmlns")) {
    delete [] const_cast<char *>(this->xmlns);
  }
  this->xmlns = NULL;
  if (xmlnsstr) {
    char * buffer = new char [ strlen(xmlnsstr) + 1 ];
    strcpy(buffer, xmlnsstr);
    this->xmlns = buffer;
  }
}

// const char * ScXMLDocument::getXMLNSXMLAttr(void) const

void
ScXMLDocument::setVersionXMLAttr(const char * versionstr)
{
  if (this->version && this->version != this->getAttribute("version")) {
    delete [] const_cast<char *>(this->version);
  }
  this->version = NULL;
  if (versionstr) {
    char * buffer = new char [ strlen(versionstr) + 1 ];
    strcpy(buffer, versionstr);
    this->version = buffer;
  }
}

// const char * ScXMLDocument::getVersionXMLAttr(void) const
  
void
ScXMLDocument::setInitialStateXMLAttr(const char * initialstatestr)
{
  if (this->initialstate &&
      (this->initialstate != this->getAttribute("initialstate"))) {
    delete [] const_cast<char *>(this->initialstate);
  }
  this->initialstate = NULL;
  if (initialstatestr) {
    char * buffer = new char [ strlen(initialstatestr) + 1 ];
    strcpy(buffer, initialstatestr);
    this->initialstate = buffer;
  }
}

// const char * ScXMLDocument::getInitialStateXMLAttr(void) const

SbBool
ScXMLDocument::handleXMLAttributes(void)
{
  if (!inherited::handleXMLAttributes()) return FALSE;

  this->xmlns = this->getAttribute("xmlns");
  this->version = this->getAttribute("version");
  this->initialstate = this->getAttribute("initialstate");

  if (!this->referenced && this->initialstate == NULL) {
    // requirement for root document, but not for referenced documents
    return FALSE;
  }

  return TRUE;
}

// *************************************************************************

SCXML_LIST_OBJECT_API_IMPL(ScXMLDocument, ScXMLState, statelist, State, States);

SCXML_LIST_OBJECT_API_IMPL(ScXMLDocument, ScXMLState, parallellist, Parallel, Parallels);

SCXML_LIST_OBJECT_API_IMPL(ScXMLDocument, ScXMLFinal, finallist, Final, Finals);


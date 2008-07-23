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

#include <Inventor/scxml/ScXMLInvoke.h>

#include <assert.h>
#include <string.h>

// *************************************************************************

/*!
  \class ScXMLInvoke ScXMLInvoke.h Inventor/scxml/ScXMLInvoke.h
  \brief Implementation of the &lt;invoke&gt; SCXML element.

  \since Coin 3.0
  \ingroup scxml
*/

class ScXMLInvoke::PImpl {
public:
};

SCXML_OBJECT_ABSTRACT_SOURCE(ScXMLInvoke);

void
ScXMLInvoke::initClass(void)
{
  SCXML_OBJECT_INIT_ABSTRACT_CLASS(ScXMLInvoke, ScXMLObject);
}

ScXMLInvoke::ScXMLInvoke(void)
  : targettype(NULL), src(NULL), srcexpr(NULL)
{
}

ScXMLInvoke::~ScXMLInvoke(void)
{
  this->setTargetTypeAttribute(NULL);
  this->setSrcAttribute(NULL);
  this->setSrcExprAttribute(NULL);
}

void
ScXMLInvoke::setTargetTypeAttribute(const char * targettypestr)
{
  if (this->targettype &&
      this->targettype != this->getXMLAttribute("targettype")) {
    delete [] this->targettype;
  }
  this->targettype = NULL;
  if (targettypestr) {
    this->targettype = new char [ strlen(targettypestr) + 1 ];
    strcpy(this->targettype, targettypestr);
  }
}

// const char * ScXMLInvoke::getTargetTypeAttribute(void) const

void
ScXMLInvoke::setSrcAttribute(const char * srcstr)
{
  if (this->src && this->src != this->getXMLAttribute("src")) {
    delete [] this->src;
  }
  this->src = NULL;
  if (srcstr) {
    this->src = new char [ strlen(srcstr) + 1 ];
    strcpy(this->src, srcstr);
  }
}

// const char * ScXMLInvoke::getSrcAttribute(void) const

void
ScXMLInvoke::setSrcExprAttribute(const char * srcexprstr)
{
  if (this->srcexpr && this->srcexpr != this->getXMLAttribute("srcexpr")) {
    delete [] this->srcexpr;
  }
  this->srcexpr = NULL;
  if (srcexprstr) {
    this->srcexpr = new char [ strlen(srcexprstr) + 1 ];
    strcpy(this->srcexpr, srcexprstr);
  }
}

// const char * ScXMLInvoke::getSrcExprAttribute(void) const

SbBool
ScXMLInvoke::handleXMLAttributes(void)
{
  if (!inherited::handleXMLAttributes()) return FALSE;

  this->targettype = const_cast<char *>(this->getXMLAttribute("targettype"));
  this->src = const_cast<char *>(this->getXMLAttribute("src"));
  this->srcexpr = const_cast<char *>(this->getXMLAttribute("srcexpr"));

  if (!this->targettype) { return FALSE; }

  return TRUE;
}

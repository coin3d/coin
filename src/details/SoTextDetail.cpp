/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2003 by Systems in Motion.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Systems in Motion about acquiring
 *  a Coin Professional Edition License.
 *
 *  See <URL:http://www.coin3d.org> for  more information.
 *
 *  Systems in Motion, Teknobyen, Abels Gate 5, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no>.
 *
\**************************************************************************/

/*!
  \class SoTextDetail SoTextDetail.h Inventor/details/SoTextDetail.h
  \brief The SoTextDetail stores information about a character in a string.
  \ingroup details

  Instances of this class are used for storing information about hit
  points on textual 2D or 3D geometry after pick operations, and for
  storing information returned to tessellation callbacks.

  \sa SoRayPickAction, SoCallbackAction
  \sa SoText3, SoText2, SoAsciiText
*/
// FIXME: write a full test and usage example to see if this class is
// actually used properly by it's "client" shape nodes. 20011128 mortene.

#include <Inventor/details/SoTextDetail.h>
#include <Inventor/SbName.h>

SO_DETAIL_SOURCE(SoTextDetail);

/*!
  Constructor sets up an "empty" detail; all indices are set to -1 to
  indicate this.
*/
SoTextDetail::SoTextDetail(void)
  : stringindex(-1),
    charindex(-1),
    part(-1)
{
}

/*!
  Destructor. This class does not allocate any extra resources, so no
  actions are taken.
*/
SoTextDetail::~SoTextDetail()
{
}

// Doc in superclass.
void
SoTextDetail::initClass(void)
{
  SO_DETAIL_INIT_CLASS(SoTextDetail, SoDetail);
}

// Doc in superclass.
SoDetail *
SoTextDetail::copy(void) const
{
  SoTextDetail *copy = new SoTextDetail;
  *copy = *this;
  return copy;
}

/*!
  Returns the index of the string where a character was hit /
  generated, from a set of multiple strings.

  \sa SoMFString
*/
int
SoTextDetail::getStringIndex(void) const
{
  return this->stringindex;
}

/*!
  Returns the index of the character in the string which was hit.

  \sa getStringIndex()
*/
int
SoTextDetail::getCharacterIndex(void) const
{
  return this->charindex;
}

/*!
  For SoText3, returns the part id of the text geometry. The id
  numbers matches those specified in the SoText3::Part enumeration.
*/
int
SoTextDetail::getPart(void) const
{
  return this->part;
}

/*!
  Internal method for "client" shape nodes to initialize this
  SoTextDetail instance.
*/
void
SoTextDetail::setStringIndex(const int idx)
{
  this->stringindex = idx;
}

/*!
  Internal method for "client" shape nodes to initialize this
  SoTextDetail instance.
*/
void
SoTextDetail::setCharacterIndex(const int idx)
{
  this->charindex = idx;
}

/*!
  Internal method for "client" shape nodes to initialize this
  SoTextDetail instance.
*/
void
SoTextDetail::setPart(const int part)
{
  this->part = part;
}

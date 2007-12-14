/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2007 by Systems in Motion.  All rights reserved.
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
 *  See http://www.coin3d.org/ for more information.
 *
 *  Systems in Motion, Postboks 1283, Pirsenteret, 7462 Trondheim, NORWAY.
 *  http://www.sim.no/  sales@sim.no  coin-support@coin3d.org
 *
\**************************************************************************/

/*!
  \class SoAnnoText3 SoAnnoText3.h Inventor/nodes/SoAnnoText3.h
  \brief The SoAnnoText3 class is ... FIXME: doc
  \ingroup nodes

  FIXME: doc

  <b>FILE FORMAT/DEFAULTS:</b>
  \code
    AnnoText3 {
        string ""
        justification LEFT
        spacing 1
        alternateRep NULL
    }
  \endcode

  \since Coin 2.4
  \since TGS Inventor v2.6
*/

// *************************************************************************

#include <Inventor/nodes/SoAnnoText3.h>

#include <Inventor/errors/SoDebugError.h>

#include "nodes/SoSubNodeP.h"

// *************************************************************************

#ifndef COIN_BETA_VERSION
#error This must either be implemented properly or taken out before the next official release.
#endif // COIN_BETA_VERSION

// FIXME: for the implementation, it may be possible to grab some of
// the code from the SoAnnotation node? Seem related to the
// functionality of this one.
//
// Also, an implementation of this node *must* first trigger a
// refactoring process, where we clean up a lot of the code now
// _duplicated_ between SoText3, SoAsciiText, SoVRMLText.
//
// I.e. separate out the common parts, design a decent internal
// interface on it, and remove all the ugly copy'n'paste jobs within
// the text-node implementations.
//
// 20060224 mortene.

// *************************************************************************

SO_NODE_SOURCE(SoAnnoText3);

// *************************************************************************

SoAnnoText3::SoAnnoText3(void)
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoAnnoText3);

  SO_NODE_ADD_FIELD(string, (""));

  SO_NODE_DEFINE_ENUM_VALUE(Justification, LEFT);
  SO_NODE_DEFINE_ENUM_VALUE(Justification, RIGHT);
  SO_NODE_DEFINE_ENUM_VALUE(Justification, CENTER);
  SO_NODE_SET_SF_ENUM_TYPE(justification, Justification);
  SO_NODE_ADD_FIELD(justification, (LEFT));

  SO_NODE_ADD_FIELD(spacing, (1.0));

  SO_NODE_ADD_FIELD(alternateRep, (NULL));


  static SbBool first = TRUE;
  if (first) {
    SoDebugError::postWarning("SoAnnoText3::SoAnnoText3",
                              "Node not yet implemented.");
    first = FALSE;
  }
}

SoAnnoText3::~SoAnnoText3()
{
}

// Doc from superclass.
void
SoAnnoText3::initClass(void)
{
  // Note: I couldn't find a way to figure out whether this node was
  // added by TGS for Inventor 2.5 or 2.6, so I set it up for the most
  // conservative choice.  -mortene.
  SO_NODE_INTERNAL_INIT_CLASS(SoAnnoText3, SO_FROM_INVENTOR_2_6|SO_FROM_COIN_2_4);
}

// *************************************************************************

SbBox3f
SoAnnoText3::getCharacterBounds(SoState * state, int stringidx, int charidx)
{
  // FIXME: implement

  return SbBox3f();
}

// *************************************************************************

void
SoAnnoText3::notify(SoNotList * l)
{
  // FIXME: implement

  inherited::notify(l);
}

// *************************************************************************

void
SoAnnoText3::GLRender(SoGLRenderAction * action)
{
  // FIXME: implement
}

void
SoAnnoText3::getPrimitiveCount(SoGetPrimitiveCountAction * action)
{
  // FIXME: implement
}

void
SoAnnoText3::rayPick(SoRayPickAction * action)
{
  // FIXME: implement
}

void
SoAnnoText3::write(SoWriteAction * action)
{
  // FIXME: implement
}

void
SoAnnoText3::computeBBox(SoAction * action, SbBox3f & box, SbVec3f & center)
{
  // FIXME: implement
}

void
SoAnnoText3::generatePrimitives(SoAction * action)
{
  // FIXME: implement
}

// *************************************************************************

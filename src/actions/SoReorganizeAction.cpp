/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2005 by Systems in Motion.  All rights reserved.
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
 *  See <URL:http://www.coin3d.org/> for more information.
 *
 *  Systems in Motion, Postboks 1283, Pirsenteret, 7462 Trondheim, NORWAY.
 *  <URL:http://www.sim.no/>.
 *
\**************************************************************************/

/*!
  \class SoReorganizeAction Inventor/include/SoReorganizeAction.h
  \brief The SoReorganizeAction class ...
*/

#include <Inventor/SbName.h>
#include <Inventor/actions/SoReorganizeAction.h>
#include <coindefs.h> // COIN_STUB()

class SoReorganizeActionP {
 public:
  SoReorganizeActionP(SoReorganizeAction * masterin) 
    : master(masterin),
      gennormals(TRUE),
      gentexcoords(TRUE),
      gentristrips(FALSE),
      genvp(FALSE),
      matchidx(TRUE) { }

  SoReorganizeAction * master;
  SbBool gennormals;
  SbBool gentexcoords;
  SbBool gentristrips;
  SbBool genvp;
  SbBool matchidx;
};

#define PRIVATE(obj) obj->pimpl

SO_ACTION_SOURCE(SoReorganizeAction);

// Override from parent class.
void
SoReorganizeAction::initClass(void)
{
  SO_ACTION_INTERNAL_INIT_CLASS(SoReorganizeAction, SoSimplifyAction);
}


/*!
  A constructor.
*/

SoReorganizeAction::SoReorganizeAction(SoSimplifier * simplifier)
{
  PRIVATE(this) = new SoReorganizeActionP(this);
  SO_ACTION_CONSTRUCTOR(SoReorganizeAction);
}

/*!
  The destructor.
*/

SoReorganizeAction::~SoReorganizeAction(void)
{
  delete PRIVATE(this);
}

SoSeparator * 
SoReorganizeAction::getSimplifiedSceneGraph(void) const
{
  return NULL;
}

void 
SoReorganizeAction::generateNormals(SbBool onoff)
{
  PRIVATE(this)->gennormals = onoff;
}

SbBool 
SoReorganizeAction::areNormalGenerated(void) const
{
  return PRIVATE(this)->gennormals;
}

void 
SoReorganizeAction::generateTriangleStrips(SbBool onoff)
{
  PRIVATE(this)->gentristrips = onoff;
}

SbBool 
SoReorganizeAction::areTriangleStripGenerated(void) const
{
  return PRIVATE(this)->gentristrips;
}

void 
SoReorganizeAction::generateTexCoords(SbBool onoff)
{
  PRIVATE(this)->gentexcoords = onoff;
}

SbBool 
SoReorganizeAction::areTexCoordsGenerated(void) const
{
  return PRIVATE(this)->gentexcoords;
}

void 
SoReorganizeAction::generateVPNodes(SbBool onoff)
{
  PRIVATE(this)->genvp = onoff;
}

SbBool 
SoReorganizeAction::areVPNodesGenerated(void)
{
  return PRIVATE(this)->genvp;
}

void 
SoReorganizeAction::matchIndexArrays(SbBool onoff)
{
  PRIVATE(this)->matchidx = onoff;
}

SbBool 
SoReorganizeAction::areIndexArraysMatched(void) const
{
  return PRIVATE(this)->matchidx;
}

SoSimplifier * 
SoReorganizeAction::getSimplifier(void) const
{
  return NULL;
}

void 
SoReorganizeAction::apply(SoNode * root)
{
  COIN_STUB();
}

void 
SoReorganizeAction::apply(SoPath * path)
{
  COIN_STUB();
}

void 
SoReorganizeAction::apply(const SoPathList & pathlist, SbBool obeysrules)
{
  COIN_STUB();
}

void 
SoReorganizeAction::startReport(const char * msg)
{
  COIN_STUB();
}

void 
SoReorganizeAction::finishReport(void)
{
  COIN_STUB();
}

// Documented in superclass.
void
SoReorganizeAction::beginTraversal(SoNode * /* node */)
{
  COIN_STUB();
}

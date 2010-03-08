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

/*!
  \class SoSimplifyAction Inventor/include/SoSimplifyAction.h
  \brief The SoSimplifyAction class is the base class for the simplify
  action classes.
*/

#include <Inventor/actions/SoSimplifyAction.h>

#include <Inventor/SbName.h>

#include "coindefs.h" // COIN_STUB()
#include "actions/SoSubActionP.h"

class SoSimplifyActionP {
public:
};

SO_ACTION_SOURCE(SoSimplifyAction);

// Overridden from parent class.
void
SoSimplifyAction::initClass(void)
{
  SO_ACTION_INTERNAL_INIT_CLASS(SoSimplifyAction, SoAction);
}


/*!
  A constructor.
*/

SoSimplifyAction::SoSimplifyAction(void)
{
}

/*!
  The destructor.
*/

SoSimplifyAction::~SoSimplifyAction(void)
{
}

// Documented in superclass.
void
SoSimplifyAction::beginTraversal(SoNode * node)
{
  inherited::beginTraversal(node);
}

void 
SoSimplifyAction::apply(SoNode * root)
{
  inherited::apply(root);
}

void 
SoSimplifyAction::apply(SoPath * path)
{
  inherited::apply(path);
}

void 
SoSimplifyAction::apply(const SoPathList & pathlist, SbBool obeysrules)
{
  inherited::apply(pathlist, obeysrules);
}

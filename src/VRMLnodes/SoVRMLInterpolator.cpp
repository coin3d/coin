/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2001 by Systems in Motion. All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public License
 *  version 2.1 as published by the Free Software Foundation. See the
 *  file LICENSE.LGPL at the root directory of the distribution for
 *  more details.
 *
 *  If you want to use Coin for applications not compatible with the
 *  LGPL, please contact SIM to acquire a Professional Edition license.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  http://www.sim.no support@sim.no Voice: +47 22114160 Fax: +47 22207097
 *
\**************************************************************************/

/*!
  \class SoVRMLInterpolator SoVRMLInterpolator.h Inventor/VRMLnodes/SoVRMLInterpolator.h
  \brief The SoVRMLInterpolator class ...
  \ingroup vrml2

  FIXME: write class doc
*/

#include <Inventor/VRMLnodes/SoVRMLInterpolator.h>
#include <Inventor/nodes/SoSubNodeP.h>
#include <coindefs.h> // COIN_STUB()

SO_NODE_ABSTRACT_SOURCE(SoVRMLInterpolator);

/*!
  Constructor.
*/
SoVRMLInterpolator::SoVRMLInterpolator(void)
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoVRMLInterpolator);
}

/*!
  Destructor.
*/
SoVRMLInterpolator::~SoVRMLInterpolator()
{
}

/*!
  Does initialization common for all objects of the
  SoVRMLInterpolator class. This includes setting up the
  type system, among other things.
*/
void
SoVRMLInterpolator::initClass(void)
{
  SO_NODE_INTERNAL_INIT_ABSTRACT_CLASS(SoVRMLInterpolator);
}

/*!
  FIXME: write function documentation
*/
SbBool
SoVRMLInterpolator::getOutputName(const SoVRMLInterpOutput * /* output */,
                                  SbName & /* outputName */) const
{
  COIN_STUB();
  return FALSE;
}

/**************************************************************************\
 * 
 *  Copyright (C) 1998-1999 by Systems in Motion.  All rights reserved.
 *
 *  This file is part of the Coin library.
 *
 *  This file may be distributed under the terms of the Q Public License
 *  as defined by Troll Tech AS of Norway and appearing in the file
 *  LICENSE.QPL included in the packaging of this file.
 *
 *  If you want to use Coin in applications not covered by licenses
 *  compatible with the QPL, you can contact SIM to aquire a
 *  Professional Edition license for Coin.
 *
 *  Systems in Motion AS, Prof. Brochs gate 6, N-7030 Trondheim, NORWAY
 *  http://www.sim.no/ sales@sim.no Voice: +47 22114160 Fax: +47 67172912
 *
\**************************************************************************/

/*!
  \class SoVRMLInterpolator SoVRMLInterpolator.h Inventor/VRMLnodes/SoVRMLInterpolator.h
  \brief The SoVRMLInterpolator class ...
  \ingroup vrml2

  FIXME: write class doc
*/

#include <Inventor/VRMLnodes/SoVRMLInterpolator.h>
#include <Inventor/SbName.h>

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
  assert(0 && "FIXME: not implemented yet");
  return FALSE;
}

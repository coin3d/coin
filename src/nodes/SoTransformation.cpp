/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2001 by Systems in Motion.  All rights reserved.
 *  
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  version 2 as published by the Free Software Foundation.  See the
 *  file LICENSE.GPL at the root directory of this source distribution
 *  for more details.
 *
 *  If you desire to use Coin with software that is incompatible
 *  licensewise with the GPL, and / or you would like to take
 *  advantage of the additional benefits with regard to our support
 *  services, please contact Systems in Motion about acquiring a Coin
 *  Professional Edition License.  See <URL:http://www.coin3d.org> for
 *  more information.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  <URL:http://www.sim.no>, <mailto:support@sim.no>
 *
\**************************************************************************/

/*!
  \class SoTransformation SoTransformation.h Inventor/nodes/SoTransformation.h
  \brief The SoTransformation class is the abstract base class for transformation nodes.
  \ingroup nodes

  To position and orient geometry within the 3D world space, various
  node types for transformations are used. These nodes all inherit the
  SoTransformation base class.
*/


#include <Inventor/actions/SoCallbackAction.h>
#include <Inventor/nodes/SoSubNodeP.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/actions/SoGetBoundingBoxAction.h>
#include <Inventor/actions/SoGetPrimitiveCountAction.h>
#include <Inventor/actions/SoPickAction.h>
#include <Inventor/elements/SoBBoxModelMatrixElement.h>
#include <Inventor/elements/SoGLModelMatrixElement.h>
#include <Inventor/elements/SoLocalBBoxMatrixElement.h>

SO_NODE_ABSTRACT_SOURCE(SoTransformation);

/*!
  Constructor.
*/
SoTransformation::SoTransformation(void)
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoTransformation);
}

/*!
  Destructor.
*/
SoTransformation::~SoTransformation()
{
}

// Doc from superclass.
void
SoTransformation::initClass(void)
{
  SO_NODE_INTERNAL_INIT_ABSTRACT_CLASS(SoTransformation, SO_FROM_INVENTOR_2_1);

  SO_ENABLE(SoGetBoundingBoxAction, SoModelMatrixElement);
  SO_ENABLE(SoGetBoundingBoxAction, SoBBoxModelMatrixElement);
  SO_ENABLE(SoGetBoundingBoxAction, SoLocalBBoxMatrixElement);

  SO_ENABLE(SoGLRenderAction, SoGLModelMatrixElement);

  SO_ENABLE(SoPickAction, SoModelMatrixElement);

  SO_ENABLE(SoCallbackAction, SoModelMatrixElement);

  SO_ENABLE(SoGetPrimitiveCountAction, SoModelMatrixElement);
}

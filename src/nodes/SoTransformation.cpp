/**************************************************************************\
 *
 *  Copyright (C) 1998-2000 by Systems in Motion.  All rights reserved.
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
  \class SoTransformation SoTransformation.h Inventor/nodes/SoTransformation.h
  \brief The SoTransformation class is the abstract base class for transformation nodes.
  \ingroup nodes

  To position and orient geometry within the 3D world space, various
  node types for transformations are used. These nodes all inherit the
  SoTransformation base class.
*/


#include <Inventor/actions/SoCallbackAction.h>
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
  SO_NODE_INTERNAL_INIT_ABSTRACT_CLASS(SoTransformation);

  SO_ENABLE(SoGetBoundingBoxAction, SoModelMatrixElement);
  SO_ENABLE(SoGetBoundingBoxAction, SoBBoxModelMatrixElement);
  SO_ENABLE(SoGetBoundingBoxAction, SoLocalBBoxMatrixElement);

  SO_ENABLE(SoGLRenderAction, SoGLModelMatrixElement);

  SO_ENABLE(SoPickAction, SoModelMatrixElement);

  SO_ENABLE(SoCallbackAction, SoModelMatrixElement);

  SO_ENABLE(SoGetPrimitiveCountAction, SoModelMatrixElement);
}

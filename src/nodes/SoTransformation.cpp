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
  \class SoTransformation SoTransformation.h Inventor/nodes/SoTransformation.h
  \brief The SoTransformation class is the abstract base class for transformation nodes.
  \ingroup nodes

  To position and orient geometry within the 3D world space, various
  node types for transformations are used. These nodes all inherit the
  SoTransformation base class.

  Note that transformations will be accumulated through the scene
  graph, unless used under SoSeparator nodes, where the transformation
  matrix stack will be pushed and popped. Here is a short example
  demonstrating this principle:

  \verbatim
#Inventor V2.1 ascii

Separator {
   BaseColor { rgb 1 0 0 }
   Cone { }

   Translation { translation 3 0 0 }

   Separator {
      Rotation { rotation 1 0 0  1.57 }
      BaseColor { rgb 0 1 0 }
      Cone { }
   } # SoSeparator restores transformation matrix here

   Translation { translation 3 0 0 }

   Rotation { rotation 1 0 0  1.57 }
   BaseColor { rgb 0 0 1 }
   Cone { }

   Translation { translation 3 0 0 }

   # Last SoRotation was not within SoSeparator, so this next
   # SoRotation will accumulate with the previous.
   Rotation { rotation 1 0 0  1.57 }
   BaseColor { rgb 1 1 0 }
   Cone { }
}
  \endverbatim

  Which results in the following scene:

  <center><img src="http://doc.coin3d.org/images/Coin/nodes/transformaccum.png"></center>
*/


#include <Inventor/actions/SoCallbackAction.h>

#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/actions/SoGetBoundingBoxAction.h>
#include <Inventor/actions/SoGetPrimitiveCountAction.h>
#include <Inventor/actions/SoPickAction.h>
#include <Inventor/elements/SoBBoxModelMatrixElement.h>
#include <Inventor/elements/SoGLModelMatrixElement.h>
#include <Inventor/elements/SoLocalBBoxMatrixElement.h>

#include "nodes/SoSubNodeP.h"

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
  SO_NODE_INTERNAL_INIT_ABSTRACT_CLASS(SoTransformation, SO_FROM_INVENTOR_1);

  SO_ENABLE(SoGetBoundingBoxAction, SoModelMatrixElement);
  SO_ENABLE(SoGetBoundingBoxAction, SoBBoxModelMatrixElement);
  SO_ENABLE(SoGetBoundingBoxAction, SoLocalBBoxMatrixElement);

  SO_ENABLE(SoGLRenderAction, SoGLModelMatrixElement);

  SO_ENABLE(SoPickAction, SoModelMatrixElement);

  SO_ENABLE(SoCallbackAction, SoModelMatrixElement);

  SO_ENABLE(SoGetPrimitiveCountAction, SoModelMatrixElement);
}

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
  \class SoMatrixTransform SoMatrixTransform.h Inventor/nodes/SoMatrixTransform.h
  \brief The SoMatrixTransform class ...
  \ingroup nodes

  FIXME: write class doc
*/

#include <Inventor/nodes/SoMatrixTransform.h>

#include <Inventor/actions/SoGetMatrixAction.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/elements/SoGLNormalizeElement.h>
#include <Inventor/elements/SoModelMatrixElement.h>

/*!
  \var SoSFMatrix SoMatrixTransform::matrix
  FIXME: write documentation for field
*/


// *************************************************************************

SO_NODE_SOURCE(SoMatrixTransform);

/*!
  Constructor.
*/
SoMatrixTransform::SoMatrixTransform()
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoMatrixTransform);

  SO_NODE_ADD_FIELD(matrix, (SbMatrix::identity()));
}

/*!
  Destructor.
*/
SoMatrixTransform::~SoMatrixTransform()
{
}

/*!
  Does initialization common for all objects of the
  SoMatrixTransform class. This includes setting up the
  type system, among other things.
*/
void
SoMatrixTransform::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoMatrixTransform);
}

/*!
  FIXME: write function documentation
*/
void
SoMatrixTransform::doAction(SoAction * action)
{
  if (!this->matrix.isIgnored()) {
    SoModelMatrixElement::mult(action->getState(), this,
                               this->matrix.getValue());
  }
}

/*!
  FIXME: write function documentation
*/
void
SoMatrixTransform::GLRender(SoGLRenderAction * action)
{
  // TODO: code to test if matrix is legal. For now we'll
  // just invalidate the flag in the normalize element (pederb).
  SoGLNormalizeElement::setMatrixState(action->getState(), FALSE);

  SoMatrixTransform::doAction((SoAction *)action);
}

/*!
  FIXME: write function documentation
*/
void
SoMatrixTransform::getBoundingBox(SoGetBoundingBoxAction * action)
{
  SoMatrixTransform::doAction((SoAction *)action);
}

/*!
  FIXME: write doc
 */
void
SoMatrixTransform::callback(SoCallbackAction *action)
{
  SoMatrixTransform::doAction((SoAction*)action);
}

/*!
  FIXME: write doc
 */
void
SoMatrixTransform::getMatrix(SoGetMatrixAction *action)
{
  SbMatrix m = this->matrix.getValue();
  action->getMatrix().multLeft(m);
  
  SbMatrix mi = m.inverse();
  action->getInverse().multRight(m);
}

/*!
  FIXME: write doc
 */
void
SoMatrixTransform::pick(SoPickAction *action)
{
  SoMatrixTransform::doAction((SoAction*)action);
}

/*!
  FIXME: write doc
 */
void
SoMatrixTransform::getPrimitiveCount(SoGetPrimitiveCountAction *action)
{
  SoMatrixTransform::doAction((SoAction*)action);
}

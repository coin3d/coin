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
  \class SoScale SoScale.h Inventor/nodes/SoScale.h
  \brief The SoScale class ...
  \ingroup nodes

  FIXME: write class doc
*/

#include <Inventor/nodes/SoScale.h>

#include <Inventor/actions/SoGetMatrixAction.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/elements/SoModelMatrixElement.h>
#include <Inventor/elements/SoGLNormalizeElement.h>

/*!
  \var SoSFVec3f SoScale::scaleFactor
  FIXME: write documentation for field
*/

// *************************************************************************

SO_NODE_SOURCE(SoScale);

/*!
  Constructor.
*/
SoScale::SoScale()
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoScale);

  SO_NODE_ADD_FIELD(scaleFactor, (1.0f, 1.0f, 1.0f));
}

/*!
  Destructor.
*/
SoScale::~SoScale()
{
}

/*!
  Does initialization common for all objects of the
  SoScale class. This includes setting up the
  type system, among other things.
*/
void
SoScale::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoScale);
}

/*!
  FIXME: write function documentation
*/
void
SoScale::doAction(SoAction * action)
{
  SoModelMatrixElement::scaleBy(action->getState(), this,
                                this->scaleFactor.getValue());
}

/*!
  FIXME: write function documentation
*/
void
SoScale::GLRender(SoGLRenderAction * action)
{
  if (scaleFactor.getValue() != SbVec3f(1.0f, 1.0f, 1.0f)) {
    SoGLNormalizeElement::setMatrixState(action->getState(), FALSE);
  }

  SoScale::doAction((SoAction *)action);
}

/*!
  FIXME: write function documentation
*/
void
SoScale::getBoundingBox(SoGetBoundingBoxAction * action)
{
  SoScale::doAction((SoAction *)action);
}

/*!
  FIXME: write doc
 */
void
SoScale::callback(SoCallbackAction *action)
{
  SoScale::doAction((SoAction*)action);
}

/*!
  FIXME: write doc
 */
void
SoScale::getMatrix(SoGetMatrixAction * action)
{
  SbVec3f scalevec = this->scaleFactor.getValue();
  SbMatrix m;

  m.setScale(scalevec);
  action->getMatrix().multLeft(m);
  
  m.setScale(SbVec3f(1.0f / scalevec[0], 1.0f / scalevec[1], 1.0f / scalevec[2]));
  action->getInverse().multRight(m);
}

/*!
  FIXME: write doc
 */
void
SoScale::pick(SoPickAction *action)
{
  SoScale::doAction((SoAction*)action);
}

/*!
  FIXME: write doc
 */
void
SoScale::getPrimitiveCount(SoGetPrimitiveCountAction *action)
{
  SoScale::doAction((SoAction*)action);
}

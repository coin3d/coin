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
  \class SoTranslation SoTranslation.h Inventor/nodes/SoTranslation.h
  \brief The SoTranslation class ...
  \ingroup nodes

  FIXME: write class doc
*/

#include <Inventor/nodes/SoTranslation.h>

#include <Inventor/actions/SoAction.h>
#include <Inventor/actions/SoGetMatrixAction.h>
#include <Inventor/actions/SoGetBoundingBoxAction.h>
#include <Inventor/elements/SoModelMatrixElement.h>

/*!
  \var SoSFVec3f SoTranslation::translation
  FIXME: write documentation for field
*/

// *************************************************************************

SO_NODE_SOURCE(SoTranslation);

/*!
  Constructor.
*/
SoTranslation::SoTranslation()
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoTranslation);

  SO_NODE_ADD_FIELD(translation, (0.0f, 0.0f, 0.0f));
}

/*!
  Destructor.
*/
SoTranslation::~SoTranslation()
{
}

/*!
  Does initialization common for all objects of the
  SoTranslation class. This includes setting up the
  type system, among other things.
*/
void
SoTranslation::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoTranslation);
}

/*!
  FIXME: write function documentation
*/
void
SoTranslation::doAction(SoAction * action)
{
  if (this->translation.getValue() != SbVec3f(0.0f, 0.0f, 0.0f)) {
    SoModelMatrixElement::translateBy(action->getState(), this,
                                      this->translation.getValue());
  }
}

/*!
  FIXME: write function documentation
*/
void
SoTranslation::GLRender(SoGLRenderAction * action)
{
  SoTranslation::doAction((SoAction *)action);
}

/*!
  FIXME: write function documentation
*/
void
SoTranslation::getBoundingBox(SoGetBoundingBoxAction * action)
{
  SoTranslation::doAction(action);
}

/*!
  FIXME: write doc
 */
void
SoTranslation::callback(SoCallbackAction *action)
{
  SoTranslation::doAction((SoAction*)action);
}

/*!
  FIXME: write doc
 */
void
SoTranslation::getMatrix(SoGetMatrixAction *action)
{
  action->translateBy(translation.getValue());
}

/*!
  FIXME: write doc
 */
void
SoTranslation::pick(SoPickAction *action)
{
  SoTranslation::doAction((SoAction*)action);
}

/*!
  FIXME: write doc
 */
void
SoTranslation::getPrimitiveCount(SoGetPrimitiveCountAction *action)
{
  SoTranslation::doAction((SoAction*)action);
}

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
  \class SoTransform SoTransform.h Inventor/nodes/SoTransform.h
  \brief The SoTransform class ...
  \ingroup nodes

  FIXME: write class doc
*/

#include <Inventor/nodes/SoTransform.h>



#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

#include <Inventor/actions/SoAction.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/actions/SoGetBoundingBoxAction.h>
#include <Inventor/actions/SoGetMatrixAction.h>

#include <Inventor/elements/SoModelMatrixElement.h>
#include <Inventor/elements/SoGLNormalizeElement.h>

/*!
  \var SoSFVec3f SoTransform::translation
  FIXME: write documentation for field
*/
/*!
  \var SoSFRotation SoTransform::rotation
  FIXME: write documentation for field
*/
/*!
  \var SoSFVec3f SoTransform::scaleFactor
  FIXME: write documentation for field
*/
/*!
  \var SoSFRotation SoTransform::scaleOrientation
  FIXME: write documentation for field
*/
/*!
  \var SoSFVec3f SoTransform::center
  FIXME: write documentation for field
*/

// *************************************************************************

SO_NODE_SOURCE(SoTransform);

/*!
  Constructor.
*/
SoTransform::SoTransform()
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoTransform);

  SO_NODE_ADD_FIELD(translation, (0.0f, 0.0f, 0.0f));
  SO_NODE_ADD_FIELD(rotation, (SbRotation(SbVec3f(0.0f, 0.0f, 1.0f), 0.0f)));
  SO_NODE_ADD_FIELD(scaleFactor, (1.0f, 1.0f, 1.0f));
  SO_NODE_ADD_FIELD(scaleOrientation, (SbRotation(SbVec3f(0.0f, 0.0f, 1.0f),
                                                  0.0f)));
  SO_NODE_ADD_FIELD(center, (0.0f, 0.0f, 0.0f));
}

/*!
  Destructor.
*/
SoTransform::~SoTransform()
{
}

/*!
  Does initialization common for all objects of the
  SoTransform class. This includes setting up the
  type system, among other things.
*/
void
SoTransform::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoTransform);
}

/*!
  FIXME: write function documentation
*/
void
SoTransform::pointAt(const SbVec3f & /* fromPoint */,
                     const SbVec3f & /* toPoint */)
{
  COIN_STUB();
}

/*!
  FIXME: write function documentation
*/
void
SoTransform::getScaleSpaceMatrix(SbMatrix & /* mat */,
                                 SbMatrix & /* inv */) const
{
  COIN_STUB();
}

/*!
  FIXME: write function documentation
*/
void
SoTransform::getRotationSpaceMatrix(SbMatrix & /* mat */,
                                    SbMatrix & /* inv */) const
{
  COIN_STUB();
}

/*!
  FIXME: write function documentation
*/
void
SoTransform::getTranslationSpaceMatrix(SbMatrix & /* mat */,
                                       SbMatrix & /* inv */) const
{
  COIN_STUB();
}

/*!
  FIXME: write function documentation
*/
void
SoTransform::multLeft(const SbMatrix & /* mat */)
{
  COIN_STUB();
}

/*!
  FIXME: write function documentation
*/
void
SoTransform::multRight(const SbMatrix & /* mat */)
{
  COIN_STUB();
}

/*!
  FIXME: write function documentation
*/
void
SoTransform::combineLeft(SoTransformation * /* nodeOnRight */)
{
  COIN_STUB();
}

/*!
  FIXME: write function documentation
*/
void
SoTransform::combineRight(SoTransformation * /* nodeOnLeft */)
{
  COIN_STUB();
}

/*!
  FIXME: write function documentation
*/
void
SoTransform::setMatrix(const SbMatrix & /* mat */)
{
  COIN_STUB();
}

/*!
  FIXME: write function documentation
*/
void
SoTransform::recenter(const SbVec3f & /* newCenter */)
{
  COIN_STUB();
}

/*!
  FIXME: write function documentation
*/
void
SoTransform::doAction(SoAction * action)
{
#if 0 // debug
  SbVec3f angle;
  float rot;
  rotation.getValue().getValue(angle, rot);
  SbVec3f scaleangle;
  float scalerotation;
  scaleOrientation.getValue().getValue(scaleangle, scalerotation);
  SoDebugError::post("SoTransform::doAction",
                     "\n\ttranslation: <%f, %f, %f>"
                     "\n\trotation: <%f, %f, %f> %f"
                     "\n\tscaleFactor: <%f, %f, %f>"
                     "\n\tscaleOrientation: <%f, %f, %f> %f"
                     "\n\tcenter: <%f, %f, %f>",
                     translation.getValue()[0],
                     translation.getValue()[1],
                     translation.getValue()[2],
                     angle[0], angle[1], angle[2], rot,
                     scaleFactor.getValue()[0],
                     scaleFactor.getValue()[1],
                     scaleFactor.getValue()[2],
                     scaleangle[0], scaleangle[1], scaleangle[2],
                     scalerotation,
                     center.getValue()[0],
                     center.getValue()[1],
                     center.getValue()[2]);
#endif // debug

  SbMatrix matrix;
  matrix.setTransform(translation.getValue(),
                      rotation.getValue(),
                      scaleFactor.getValue(),
                      scaleOrientation.getValue(),
                      center.getValue());

  SoModelMatrixElement::mult(action->getState(), this, matrix);
}

/*!
  FIXME: write function documentation
*/
void
SoTransform::GLRender(SoGLRenderAction * action)
{
  if (scaleFactor.getValue() != SbVec3f(1.0f, 1.0f, 1.0f)) {
    SoGLNormalizeElement::setMatrixState(action->getState(), FALSE);
  }
  SoTransform::doAction((SoAction *)action);
}

/*!
  FIXME: write function documentation
*/
void
SoTransform::getBoundingBox(SoGetBoundingBoxAction * action)
{
  SoTransform::doAction((SoAction *)action);
}

/*!
  FIXME: write function documentation
*/
void
SoTransform::getMatrix(SoGetMatrixAction * action)
{
  SbMatrix matrix;
  matrix.setTransform(translation.getValue(),
                      rotation.getValue(),
                      scaleFactor.getValue(),
                      scaleOrientation.getValue(),
                      center.getValue());
  action->mult(matrix);
}

/*!
  FIXME: write function documentation
*/
void
SoTransform::callback(SoCallbackAction * action)
{
  SoTransform::doAction((SoAction *)action);
}

/*!
  FIXME: write function documentation
*/
void
SoTransform::pick(SoPickAction * action)
{
  SoTransform::doAction((SoAction *)action);
}

/*!
  FIXME: write doc
 */
void
SoTransform::getPrimitiveCount(SoGetPrimitiveCountAction *action)
{
  SoTransform::doAction((SoAction *)action);
}

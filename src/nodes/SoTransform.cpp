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
  \brief The SoTransform class is the "all-purpose" transformation node type.
  \ingroup nodes

  Like SoMatrixTransform, nodes of this type gives the application
  programmer maximum flexibility when specifying geometry
  transformations in a scene graph. If you want to set and keep the
  various components of the transformation matrix in separate
  entities, this node type is preferable, though.

  The order of operations is: first scaling is done, then rotation,
  then translation.
*/

#include <Inventor/nodes/SoTransform.h>
#include <Inventor/nodes/SoSubNodeP.h>

#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/actions/SoGetMatrixAction.h>
#include <Inventor/elements/SoModelMatrixElement.h>
#include <Inventor/elements/SoGLNormalizeElement.h>

#include <coindefs.h> // COIN_STUB()


/*!
  \var SoSFVec3f SoTransform::translation
  The translation part of the transformation.
*/
/*!
  \var SoSFRotation SoTransform::rotation
  The rotation part of the transformation.
*/
/*!
  \var SoSFVec3f SoTransform::scaleFactor
  The scaling part of the transformation.
*/
/*!
  \var SoSFRotation SoTransform::scaleOrientation
  The orientation the object is set to before scaling.
*/
/*!
  \var SoSFVec3f SoTransform::center
  The center point for the rotation.
*/

// *************************************************************************

SO_NODE_SOURCE(SoTransform);

/*!
  Constructor.
*/
SoTransform::SoTransform(void)
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoTransform);

  SO_NODE_ADD_FIELD(translation, (0.0f, 0.0f, 0.0f));
  SO_NODE_ADD_FIELD(rotation, (SbRotation(SbVec3f(0.0f, 0.0f, 1.0f), 0.0f)));
  SO_NODE_ADD_FIELD(scaleFactor, (1.0f, 1.0f, 1.0f));
  SO_NODE_ADD_FIELD(scaleOrientation, (SbRotation(SbVec3f(0.0f, 0.0f, 1.0f), 0.0f)));
  SO_NODE_ADD_FIELD(center, (0.0f, 0.0f, 0.0f));
}

/*!
  Destructor.
*/
SoTransform::~SoTransform()
{
}

// Doc from superclass.
void
SoTransform::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoTransform);
}

/*!
  FIXME: write function documentation
*/
void
SoTransform::pointAt(const SbVec3f & frompoint, const SbVec3f & topoint)
{
  COIN_STUB();
}

/*!
  FIXME: write function documentation
*/
void
SoTransform::getScaleSpaceMatrix(SbMatrix & mat, SbMatrix & inv) const
{
  COIN_STUB();
}

/*!
  FIXME: write function documentation
*/
void
SoTransform::getRotationSpaceMatrix(SbMatrix & mat, SbMatrix & inv) const
{
  COIN_STUB();
}

/*!
  FIXME: write function documentation
*/
void
SoTransform::getTranslationSpaceMatrix(SbMatrix & mat, SbMatrix & inv) const
{
  COIN_STUB();
}

/*!
  FIXME: write function documentation
*/
void
SoTransform::multLeft(const SbMatrix & mat)
{
  COIN_STUB();
}

/*!
  FIXME: write function documentation
*/
void
SoTransform::multRight(const SbMatrix & mat)
{
  COIN_STUB();
}

/*!
  FIXME: write function documentation
*/
void
SoTransform::combineLeft(SoTransformation * nodeonright)
{
  COIN_STUB();
}

/*!
  FIXME: write function documentation
*/
void
SoTransform::combineRight(SoTransformation * nodeonleft)
{
  COIN_STUB();
}

/*!
  FIXME: write function documentation
*/
void
SoTransform::setMatrix(const SbMatrix & mat)
{
  COIN_STUB();
}

/*!
  FIXME: write function documentation
*/
void
SoTransform::recenter(const SbVec3f & newcenter)
{
  COIN_STUB();
}

// Doc from superclass.
void
SoTransform::doAction(SoAction * action)
{
#if COIN_DEBUG && 0 // debug
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

// Doc from superclass.
void
SoTransform::GLRender(SoGLRenderAction * action)
{
  if (scaleFactor.getValue() != SbVec3f(1.0f, 1.0f, 1.0f)) {
    SoGLNormalizeElement::setMatrixState(action->getState(), FALSE);
  }
  SoTransform::doAction((SoAction *)action);
}

// Doc from superclass.
void
SoTransform::getBoundingBox(SoGetBoundingBoxAction * action)
{
  SoTransform::doAction((SoAction *)action);
}

// Doc from superclass.
void
SoTransform::getMatrix(SoGetMatrixAction * action)
{
  SbMatrix m;
  m.setTransform(translation.getValue(),
                 rotation.getValue(),
                 scaleFactor.getValue(),
                 scaleOrientation.getValue(),
                 center.getValue());
  action->getMatrix().multLeft(m);

  SbMatrix mi = m.inverse();
  action->getInverse().multRight(mi);
}

// Doc from superclass.
void
SoTransform::callback(SoCallbackAction * action)
{
  SoTransform::doAction((SoAction *)action);
}

// Doc from superclass.
void
SoTransform::pick(SoPickAction * action)
{
  SoTransform::doAction((SoAction *)action);
}

// Doc from superclass.
void
SoTransform::getPrimitiveCount(SoGetPrimitiveCountAction * action)
{
  SoTransform::doAction((SoAction *)action);
}

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
  \class SoAntiSquish SoAntiSquish.h Inventor/nodes/SoAntiSquish.h
  \brief The SoAntiSquish class is used to make scaling uniform.
  \ingroup nodes

  It is used by draggers and manipulators to retain the shape of their
  geometry, even if the current transformation matrix contains a nonuniform
  scale. When traversed, this node replaces the scale vector of the matrix
  with uniform values, bases on one of the Sizing strategies.
*/

#include <Inventor/nodes/SoAntiSquish.h>
#include <Inventor/actions/SoGetMatrixAction.h>
#include <Inventor/elements/SoModelMatrixElement.h>
#include <Inventor/SbRotation.h>
#include <Inventor/SbVec3f.h>
#include <Inventor/SbBox3f.h>
#include <Inventor/system/kosher.h>

/*!
  \enum SoAntiSquish::Sizing
  FIXME: write documentation for enum
*/
/*!
  \var SoAntiSquish::Sizing SoAntiSquish::X
  FIXME: write documentation for enum definition
*/
/*!
  \var SoAntiSquish::Sizing SoAntiSquish::Y
  FIXME: write documentation for enum definition
*/
/*!
  \var SoAntiSquish::Sizing SoAntiSquish::Z
  FIXME: write documentation for enum definition
*/
/*!
  \var SoAntiSquish::Sizing SoAntiSquish::AVERAGE_DIMENSION
  FIXME: write documentation for enum definition
*/
/*!
  \var SoAntiSquish::Sizing SoAntiSquish::BIGGEST_DIMENSION
  FIXME: write documentation for enum definition
*/
/*!
  \var SoAntiSquish::Sizing SoAntiSquish::SMALLEST_DIMENSION
  FIXME: write documentation for enum definition
*/
/*!
  \var SoAntiSquish::Sizing SoAntiSquish::LONGEST_DIAGONAL
  FIXME: write documentation for enum definition
*/


/*!
  \var SoSFEnum SoAntiSquish::sizing
  FIXME: write documentation for field
*/
/*!
  \var SoSFBool SoAntiSquish::recalcAlways
  FIXME: write documentation for field
*/


// *************************************************************************

SO_NODE_SOURCE(SoAntiSquish);

/*!
  Constructor.
*/
SoAntiSquish::SoAntiSquish()
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoAntiSquish);

  SO_NODE_ADD_FIELD(recalcAlways, (TRUE));
  SO_NODE_ADD_FIELD(sizing, (SoAntiSquish::AVERAGE_DIMENSION));

  SO_NODE_DEFINE_ENUM_VALUE(Sizing, X);
  SO_NODE_DEFINE_ENUM_VALUE(Sizing, Y);
  SO_NODE_DEFINE_ENUM_VALUE(Sizing, Z);
  SO_NODE_DEFINE_ENUM_VALUE(Sizing, AVERAGE_DIMENSION);
  SO_NODE_DEFINE_ENUM_VALUE(Sizing, BIGGEST_DIMENSION);
  SO_NODE_DEFINE_ENUM_VALUE(Sizing, SMALLEST_DIMENSION);
  SO_NODE_DEFINE_ENUM_VALUE(Sizing, LONGEST_DIAGONAL);
  SO_NODE_SET_SF_ENUM_TYPE(sizing, Sizing);

  this->matrixValid = FALSE;
  this->inverseValid = FALSE;
}

/*!
  Destructor.
*/
SoAntiSquish::~SoAntiSquish()
{
}

/*!
  Does initialization common for all objects of the
  SoAntiSquish class. This includes setting up the
  type system, among other things.
*/
void
SoAntiSquish::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoAntiSquish);
}

/*!
  FIXME: write function documentation
*/
void
SoAntiSquish::getBoundingBox(SoGetBoundingBoxAction *action)
{
  SoAntiSquish::doAction((SoAction*) action);
}

/*!
  FIXME: write doc
*/
void
SoAntiSquish::recalc(void)
{
  this->matrixValid = FALSE;
}

/*!
  FIXME: write doc
*/
void
SoAntiSquish::doAction(SoAction *action)
{
  SoState *state = action->getState();
  if (!this->matrixValid || this->recalcAlways.getValue()) {
    this->matrixValid = TRUE;
    this->inverseValid = FALSE;
    this->unsquishedMatrix =
      this->getUnsquishingMatrix(SoModelMatrixElement::get(state),
                                 FALSE, this->inverseMatrix);
  }
  SoModelMatrixElement::set(action->getState(), this, this->unsquishedMatrix);
}
/*!
  FIXME: write doc
*/
void
SoAntiSquish::callback(SoCallbackAction *action)
{
  SoAntiSquish::doAction((SoAction*)action);
}
/*!
  FIXME: write doc
*/
void
SoAntiSquish::GLRender(SoGLRenderAction *action)
{
  SoAntiSquish::doAction((SoAction*) action);
}

/*!
  FIXME: write doc
*/
void
SoAntiSquish::getMatrix(SoGetMatrixAction *action)
{
  if (!this->matrixValid || !this->inverseValid ||
      this->recalcAlways.getValue()) {
    this->matrixValid = TRUE;
    this->inverseValid = TRUE;
    this->unsquishedMatrix =
      this->getUnsquishingMatrix(action->getMatrix(),
                                 TRUE, action->getInverse());

  }
#ifdef SB_MATRIX_WORKAROUND
  action->getMatrix() = this->unsquishedMatrix;
  action->getInverse() = this->inverseMatrix;
#else // normal compilers use this code
  action->getMatrix().setValue(this->unsquishedMatrix);
  action->getInverse().setValue(this->inverseMatrix);
#endif // fix for vc6 compiler
}

/*!
  FIXME: write doc
*/
void
SoAntiSquish::pick(SoPickAction *action)
{
  SoAntiSquish::doAction((SoAction*) action);
}

/*!
  FIXME: write doc
*/
SbMatrix
SoAntiSquish::getUnsquishingMatrix(const SbMatrix &squishedmatrix,
                                   const SbBool calcinverse,
                                   SbMatrix &inversematrix)
{
  SbRotation r, so;
  SbVec3f t, scale;

  float val;

  squishedmatrix.getTransform(t, r, scale, so);
  switch (this->sizing.getValue()) {
  case X:
    val = scale[0];
    break;
  case Y:
    val = scale[1];
    break;
  case Z:
    val = scale[2];
    break;
  case AVERAGE_DIMENSION:
    val = (scale[0] + scale[1] + scale[2]) / 3.0f;
    break;
  case BIGGEST_DIMENSION:
    val = scale[0];
    if (scale[1] > val) val = scale[1];
    if (scale[2] > val) val = scale[2];
    break;
  case SMALLEST_DIMENSION:
    val = scale[0];
    if (scale[1] < val) val = scale[1];
    if (scale[2] < val) val = scale[2];
    break;
  case LONGEST_DIAGONAL:
    {
      SbVec3f unitcube[8];
      for (int i = 0; i < 8; i++) {
        unitcube[i][0] = i & 1 ? 1.0f : -1.0f;
        unitcube[i][1] = i & 2 ? 1.0f : -1.0f;
        unitcube[i][2] = i & 4 ? 1.0f : -1.0f;
        squishedmatrix.multVecMatrix(unitcube[i], unitcube[i]);
      }

      val = (unitcube[1] - unitcube[6]).sqrLength();
      float tmp = (unitcube[5] - unitcube[2]).sqrLength();
      if (tmp > val) val = tmp;
      tmp = (unitcube[3] - unitcube[4]).sqrLength();
      if (tmp > val) val = tmp;
      tmp = (unitcube[0] - unitcube[7]).sqrLength();
      if (tmp > val) val = tmp;

      val = (float) sqrt(val);
      val *= 0.5f;
      break;
    }
  default:
    assert(0 && "unknown sizing parameter");
    val = (scale[0] + scale[1] + scale[2]) / 3.0f; // use avarage
    break;
  }
  scale[0] = scale[1] = scale[2] = val;
  SbMatrix matrix;
  matrix.setTransform(t, r, scale, so);
  if (calcinverse) inversematrix = matrix.inverse();
  return matrix;
}

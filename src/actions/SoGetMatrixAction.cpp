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
  \class SoGetMatrixAction SoGetMatrixAction.h Inventor/actions/SoGetMatrixAction.h
  \brief The SoGetMatrixAction class is an action for accumulating the transformation matrix of a subgraph.
  \ingroup actions

  This action makes it easy to calculate and convert back and from the
  global coordinate system and local coordinates in a hierarchical
  model.
*/

#include <Inventor/actions/SoGetMatrixAction.h>
#include <Inventor/nodes/SoNode.h>
#include <Inventor/elements/SoViewportRegionElement.h>
#include <assert.h>

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG


SO_ACTION_SOURCE(SoGetMatrixAction);


// Overridden from parent.
void
SoGetMatrixAction::initClass(void)
{
  SO_ACTION_INIT_CLASS(SoGetMatrixAction, SoAction);

  SO_ENABLE(SoGetMatrixAction, SoViewportRegionElement);
}


/*!
  Constructor.
*/
SoGetMatrixAction::SoGetMatrixAction(const SbViewportRegion & region)
  : viewportregion(region)
{
  SO_ACTION_CONSTRUCTOR(SoGetMatrixAction);

  static int first = 1;
  if (first) {
    first = 0;
    SO_ACTION_ADD_METHOD(SoNode, SoNode::getMatrixS);
  }
  methods->setUp();
}

/*!
  The destructor.
*/

SoGetMatrixAction::~SoGetMatrixAction()
{
}

/*!
  This method sets the viewport region.
*/

void
SoGetMatrixAction::setViewportRegion(const SbViewportRegion & region)
{
  this->viewportregion = region;
}

/*!
  This method returns the viewport region.
*/

const SbViewportRegion &
SoGetMatrixAction::getViewportRegion(void) const
{
  return this->viewportregion;
}

/*!
  This method returns the transformation matrix.
*/
SbMatrix &
SoGetMatrixAction::getMatrix(void)
{
  return this->matrix;
}

/*!
  This method returns the inverse matrix.
*/
SbMatrix &
SoGetMatrixAction::getInverse(void)
{
  return this->invmatrix;
}

/*!
  This method returns the texture matrix.
*/
SbMatrix &
SoGetMatrixAction::getTextureMatrix(void)
{
  return this->texmatrix;
}

/*!
  This method returns the inverse texture matrix.
*/
SbMatrix &
SoGetMatrixAction::getTextureInverse(void)
{
  return this->invtexmatrix;
}

/*!
  This method makes sure the graph is not traversed like it normally would.
 */
void
SoGetMatrixAction::beginTraversal(SoNode * node)
{
  assert(this->traversalMethods);

  this->matrix.makeIdentity();
  this->invmatrix.makeIdentity();
  this->texmatrix.makeIdentity();
  this->invtexmatrix.makeIdentity();

  this->traverse(node);
}

// the following functions are not in the original Inventor API

/*!
  FIXME: write doc.
*/

void
SoGetMatrixAction::mult(const SbMatrix &matrix)
{
  this->matrix.multLeft(matrix);
  this->invmatrix = this->matrix.inverse();
}

/*!
  FIXME: write doc.
*/

void
SoGetMatrixAction::translateBy(const SbVec3f &vec)
{
  SbMatrix mat;
  mat.setTranslate(vec);
  this->matrix.multLeft(mat);
  mat.setTranslate(-vec);
  this->invmatrix.multRight(mat);
}

/*!
  FIXME: write doc.
*/

void
SoGetMatrixAction::rotateBy(const SbRotation &rot)
{
  SbMatrix mat;
  mat.setRotate(rot);
  this->matrix.multLeft(mat);
  mat.setRotate(rot.inverse());
  this->invmatrix.multRight(mat);
}

/*!
  FIXME: write doc.
*/

void
SoGetMatrixAction::scaleBy(const SbVec3f &scaleFactor)
{
  SbMatrix mat;
  mat.setScale(scaleFactor);
  this->matrix.multLeft(mat);
#if COIN_DEBUG && 1 // debug
  if (scaleFactor[0] == 0.0f || scaleFactor[1] == 0.0f || scaleFactor[2] == 0.0f) {
    SoDebugError::postInfo("SoGetMatrixAction::scaleBy",
                           "Zero scale detected!");
  }
#endif // debug
  SbVec3f invScale;
  invScale[0] = 1.0f / scaleFactor[0];
  invScale[1] = 1.0f / scaleFactor[1];
  invScale[2] = 1.0f / scaleFactor[2];
  mat.setScale(invScale);
  this->invmatrix.multRight(mat);
}

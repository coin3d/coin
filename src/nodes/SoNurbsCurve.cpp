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
  \class SoNurbsCurve SoNurbsCurve.h Inventor/nodes/SoNurbsCurve.h
  \brief The SoNurbsCurve class is a node for representing smooth curves.
  \ingroup nodes

  Explaining NURBS is beyond the scope of this documentation. If you
  are unfamiliar with the principles of representing smooth curves and
  surfaces when doing 3D visualization, we recommend finding a good
  book on the subject.
*/

#include <Inventor/nodes/SoNurbsCurve.h>
#include <Inventor/nodes/SoSubNodeP.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/actions/SoGetBoundingBoxAction.h>
#include <Inventor/bundles/SoMaterialBundle.h>
#include <Inventor/caches/SoBoundingBoxCache.h>
#include <Inventor/elements/SoGLCoordinateElement.h>
#include <coindefs.h> // COIN_STUB()
#ifdef _WIN32
#include <windows.h>
#endif // !_WIN32
#include <GL/gl.h>
#include <GL/glu.h>

/*!
  \var SoSFInt32 SoNurbsCurve::numControlPoints
  FIXME: write documentation for field
*/
/*!
  \var SoMFFloat SoNurbsCurve::knotVector
  FIXME: write documentation for field
*/

// *************************************************************************

SO_NODE_SOURCE(SoNurbsCurve);

/*!
  Constructor.
*/
SoNurbsCurve::SoNurbsCurve(void)
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoNurbsCurve);

  SO_NODE_ADD_FIELD(numControlPoints, (0));
  SO_NODE_ADD_FIELD(knotVector, (0));

  this->nurbsrenderer = NULL;
}

/*!
  Destructor.
*/
SoNurbsCurve::~SoNurbsCurve()
{
  if (this->nurbsrenderer) gluDeleteNurbsRenderer(this->nurbsrenderer);
}

// Doc from parent class.
void
SoNurbsCurve::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoNurbsCurve);
}

// Doc from parent class.
void
SoNurbsCurve::GLRender(SoGLRenderAction * action)
{
  //
  // this is just a quick hack. See comment in SoNurbsSurface::GLRender
  //


  if (!this->shouldGLRender(action)) return;

  if (this->nurbsrenderer == NULL) this->nurbsrenderer = gluNewNurbsRenderer();

  SoState * state = action->getState();

  const SoCoordinateElement * coords =
    SoCoordinateElement::getInstance(state);

//    SoTextureCoordinateBundle tb(action, TRUE, FALSE); //FIXME
//    doTextures = tb.needCoordinates();

  SoMaterialBundle mb(action);
  mb.sendFirst();

  glEnable(GL_AUTO_NORMAL);

  int dim = coords->is3D() ? 3 : 4;

  const SoCoordinateElement * coordelem =
    SoCoordinateElement::getInstance(state);

  GLfloat * ptr = coords->is3D() ?
    (GLfloat *)coordelem->getArrayPtr3() :
    (GLfloat *)coordelem->getArrayPtr4();


  gluBeginCurve(this->nurbsrenderer);
  gluNurbsCurve(this->nurbsrenderer,
                this->knotVector.getNum(),
                (GLfloat *)this->knotVector.getValues(0),
                dim,
                ptr,
                this->knotVector.getNum() - this->numControlPoints.getValue(),
                (GLenum)(dim == 3 ? GL_MAP1_VERTEX_3 : GL_MAP1_VERTEX_4));

  gluEndCurve(this->nurbsrenderer);
  glDisable(GL_AUTO_NORMAL);
}

// Doc from parent class.
void
SoNurbsCurve::computeBBox(SoAction * action, SbBox3f & box, SbVec3f & center)
{
  // FIXME: this is just a quick approximation

  SoState * state = action->getState();
  const SoCoordinateElement * coordelem =
    SoCoordinateElement::getInstance(state);


  int numCoords = coordelem->getNum();
  int num = this->numControlPoints.getValue();

  assert(num <= numCoords);

  if (coordelem->is3D()) {
    const SbVec3f * coords = coordelem->getArrayPtr3();
    assert(coords);
    box.makeEmpty();
    for (int i = 0; i < num; i++) box.extendBy(coords[i]);
  }
  else {
    const SbVec4f * coords = coordelem->getArrayPtr4();
    assert(coords);
    for (int i = 0; i < num; i++) {
      SbVec4f tmp = coords[i];
      // FIXME: shouldn't we "normalize" with the fourth element of
      // each coordinate vector? 20000424 mortene.
      box.extendBy(SbVec3f(tmp[0], tmp[1], tmp[2]));
    }
  }
  center = box.getCenter();
}

// Doc from parent class.
void
SoNurbsCurve::rayPick(SoRayPickAction * action)
{
  COIN_STUB();
}

// Doc from parent class.
void
SoNurbsCurve::getPrimitiveCount(SoGetPrimitiveCountAction * action)
{
  COIN_STUB();
}

// Doc from parent class.
void
SoNurbsCurve::getBoundingBox(SoGetBoundingBoxAction * action)
{
  inherited::getBoundingBox(action);
  SoBoundingBoxCache::setHasLinesOrPoints(action->getState());
}

/*!
  FIXME: write doc
 */
void
SoNurbsCurve::sendPrimitive(SoAction *,  SoPrimitiveVertex *)
{
  COIN_STUB();
}

// Doc from parent class.
void
SoNurbsCurve::generatePrimitives(SoAction * action)
{
  COIN_STUB();
}

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
  \brief The SoNurbsCurve class ...
  \ingroup nodes

  FIXME: write class doc
*/

#include <Inventor/nodes/SoNurbsCurve.h>
#include <coindefs.h> // COIN_STUB()
#include <Inventor/actions/SoGLRenderAction.h>
#ifdef _WIN32
#include <windows.h>
#endif // !_WIN32
#include <GL/gl.h>
#include <GL/glu.h>

#include <Inventor/elements/SoGLCoordinateElement.h>
#include <Inventor/bundles/SoMaterialBundle.h>

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
SoNurbsCurve::SoNurbsCurve()
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoNurbsCurve);

  SO_NODE_ADD_FIELD(numControlPoints, (0));
  SO_NODE_ADD_FIELD(knotVector, (0));

  this->nurbsRenderer = NULL;
}

/*!
  Destructor.
*/
SoNurbsCurve::~SoNurbsCurve()
{
  if (this->nurbsRenderer) {
    gluDeleteNurbsRenderer(this->nurbsRenderer);
  }
}

/*!
  Does initialization common for all objects of the
  SoNurbsCurve class. This includes setting up the
  type system, among other things.
*/
void
SoNurbsCurve::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoNurbsCurve);
}

/*!
  FIXME: write function documentation
*/
void
SoNurbsCurve::GLRender(SoGLRenderAction * action)
{
  //
  // this is just a quick hack. See comment in SoNurbsSurface::GLRender
  //


  SoState * state = action->getState();

  if (!this->shouldGLRender(action)) {
    return;
  }

  if (this->nurbsRenderer == NULL) {
    this->nurbsRenderer = gluNewNurbsRenderer();
  }

  const SoCoordinateElement * coords =
    SoCoordinateElement::getInstance(state);

//    SoTextureCoordinateBundle tb(action, TRUE, FALSE); //FIXME
//    doTextures = tb.needCoordinates();

  SoMaterialBundle mb(action);
  mb.sendFirst();

  gluNurbsProperty(nurbsRenderer, (GLenum) GLU_SAMPLING_TOLERANCE, 50.0);
  gluNurbsProperty(nurbsRenderer, (GLenum) GLU_DISPLAY_MODE, GLU_FILL);

  glEnable(GL_AUTO_NORMAL);

  int dim = coords->is3D() ? 3 : 4;

  const SoCoordinateElement *coordelem =
    SoCoordinateElement::getInstance(state);

  GLfloat * ptr = coords->is3D() ?
    (GLfloat *)coordelem->getArrayPtr3() :
    (GLfloat *)coordelem->getArrayPtr4();

  gluNurbsCurve(this->nurbsRenderer, knotVector.getNum(),
                (GLfloat *)knotVector.getValues(0),
                dim, ptr,
                knotVector.getNum()-numControlPoints.getValue() ,
                (GLenum)GLU_FILL);

  gluEndSurface(this->nurbsRenderer);
  glDisable(GL_AUTO_NORMAL);
}

/*!
  FIXME: write function documentation
*/
void
SoNurbsCurve::computeBBox(SoAction * action, SbBox3f & box, SbVec3f & center)
{
  // FIXME: this is just a quick approximation
  SoState * state = action->getState();
  const SoCoordinateElement *coordelem =
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
    for (int i = 0; i< num; i++) {
      SbVec4f tmp = coords[i];
      box.extendBy(SbVec3f(tmp[0], tmp[1], tmp[2]));
    }
  }
  center = box.getCenter();
}

/*!
  FIXME: write doc
 */
void
SoNurbsCurve::rayPick(SoRayPickAction * /* action */)
{
  COIN_STUB();
}

/*!
  FIXME: write doc
 */
void
SoNurbsCurve::getPrimitiveCount(SoGetPrimitiveCountAction * /* action */)
{
  COIN_STUB();
}

/*!
  FIXME: write doc
 */
void
SoNurbsCurve::getBoundingBox(SoGetBoundingBoxAction * /* action */)
{
  COIN_STUB();
}

/*!
  FIXME: write doc
 */
void
SoNurbsCurve::sendPrimitive(SoAction *,  SoPrimitiveVertex *)
{
  COIN_STUB();
}

/*!
  FIXME: write doc
 */
void
SoNurbsCurve::generatePrimitives(SoAction * /* action */)
{
  COIN_STUB();
}

/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2001 by Systems in Motion. All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public License
 *  version 2.1 as published by the Free Software Foundation. See the
 *  file LICENSE.LGPL at the root directory of the distribution for
 *  more details.
 *
 *  If you want to use Coin for applications not compatible with the
 *  LGPL, please contact SIM to acquire a Professional Edition license.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  http://www.sim.no support@sim.no Voice: +47 22114160 Fax: +47 22207097
 *
\**************************************************************************/

/*!
  \class SoShape SoShape.h Inventor/nodes/SoShape.h
  \brief The SoShape class is the superclass for geometry shapes.
  \ingroup nodes

  The node types which have actual geometry to render inherits this
  class. For convenince, the SoShape class contains various common
  code used by the subclasses.
*/


#include <Inventor/misc/SoState.h>
#include <Inventor/nodes/SoSubNodeP.h>
#include <coindefs.h> // COIN_OBSOLETED()
#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG
#include <Inventor/SoPrimitiveVertex.h>
#include <Inventor/nodes/SoShape.h>
#include <Inventor/actions/SoRayPickAction.h>
#include <Inventor/actions/SoGetBoundingBoxAction.h>
#include <Inventor/actions/SoCallbackAction.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/elements/SoTextureCoordinateElement.h>
#include <Inventor/elements/SoTransparencyElement.h>
#include <Inventor/elements/SoModelMatrixElement.h>
#include <Inventor/elements/SoViewVolumeElement.h>
#include <Inventor/elements/SoViewingMatrixElement.h>
#include <Inventor/elements/SoProjectionMatrixElement.h>
#include <Inventor/elements/SoViewportRegionElement.h>
#include <Inventor/elements/SoDiffuseColorElement.h>
#include <Inventor/elements/SoShapeStyleElement.h>
#include <Inventor/elements/SoGLShapeHintsElement.h>
#include <Inventor/elements/SoGLTextureEnabledElement.h>

#include <Inventor/misc/SoGL.h>
#include <Inventor/bundles/SoMaterialBundle.h>
#include <Inventor/SbTesselator.h>
#include <Inventor/details/SoFaceDetail.h>
#include <Inventor/details/SoLineDetail.h>
#include <Inventor/SoPickedPoint.h>
#include <Inventor/actions/SoGetPrimitiveCountAction.h>
#include <Inventor/elements/SoGLTextureImageElement.h>
#include <Inventor/elements/SoComplexityElement.h>
#include <Inventor/SbPlane.h>
#include <Inventor/SbBox2f.h>

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H

#ifdef HAVE_WINDOWS_H
#include <windows.h>
#endif // HAVE_WINDOWS_H
#include <GL/gl.h>
#include <string.h>
#include <stdlib.h>

/*!
  \enum SoShape::TriangleShape
  \internal
*/

/*!
  \fn void SoShape::computeBBox(SoAction * action, SbBox3f & box, SbVec3f & center)
  \internal
*/
/*!
  \fn void SoShape::generatePrimitives(SoAction * action)
  \internal
*/

// *************************************************************************

// Private class.

class shapePrimitiveData {
public:
  shapePrimitiveData() {
    this->counter = 0;
    this->action = NULL;
    this->shape = NULL;
    this->faceCounter = 0;
    this->arraySize = 4;
    this->vertsArray = new SoPrimitiveVertex[this->arraySize];
    this->pointDetails = new SoPointDetail[this->arraySize];
    this->tess = new SbTesselator();
    this->faceDetail = NULL;
    this->lineDetail = NULL;
  }
  ~shapePrimitiveData() {
    delete [] this->vertsArray;
    delete [] this->pointDetails;
    delete this->tess;
  }

  void beginShape(SoShape * shape, SoAction * action,
                  SoShape::TriangleShape shapetype,
                  SoDetail * detail) {
    this->shape = shape;
    this->action = action;
    this->shapetype = shapetype;
    // this is a hack. Only one of these will be used, and the
    // other one is an illegal cast.
    this->faceDetail = (SoFaceDetail *)detail;
    this->lineDetail = (SoLineDetail *)detail;
    this->counter = 0;
  }
  void endShape() {
    if (this->shapetype == SoShape::POLYGON) {
      this->handleFaceDetail(this->counter);

      if (SoShapeHintsElement::getFaceType(action->getState()) ==
          SoShapeHintsElement::CONVEX) {
        for (int i = 1; i < this->counter-1; i++) {
          this->shape->invokeTriangleCallbacks(this->action,
                                               &vertsArray[0],
                                               &vertsArray[i],
                                               &vertsArray[i+1]);
        }
      }
      else {
        this->tess->setCallback(shapePrimitiveData::tess_callback, this);
        this->tess->beginPolygon(TRUE);
        for (int i = 0; i < counter; i++) {
          this->tess->addVertex(vertsArray[i].getPoint(), &vertsArray[i]);
        }
        this->tess->endPolygon();
      }
    }
  }

  void shapeVertex(const SoPrimitiveVertex * const v) {
    switch (shapetype) {
    case SoShape::TRIANGLE_STRIP:
      if (this->counter >= 3) {
        if (this->counter & 1) this->copyVertex(2, 0);
        else this->copyVertex(2, 1);
      }
      this->setVertex(SbMin(this->counter, 2), v);
      this->counter++;
      if (this->counter >= 3) {
        this->handleFaceDetail(3);
        this->shape->invokeTriangleCallbacks(this->action,
                                             &vertsArray[0],
                                             &vertsArray[1],
                                             &vertsArray[2]);
      }
      break;
    case SoShape::TRIANGLE_FAN:
      if (this->counter == 3) {
        this->copyVertex(2, 1);
        this->setVertex(2, v);
      }
      else {
        this->setVertex(this->counter++, v);
      }
      if (this->counter == 3) {
        this->handleFaceDetail(3);
        this->shape->invokeTriangleCallbacks(this->action,
                                             &vertsArray[0],
                                             &vertsArray[1],
                                             &vertsArray[2]);
      }
      break;
    case SoShape::TRIANGLES:
      this->setVertex(counter++, v);
      if (this->counter == 3) {
        this->handleFaceDetail(3);
        this->shape->invokeTriangleCallbacks(this->action,
                                             &vertsArray[0],
                                             &vertsArray[1],
                                             &vertsArray[2]);
        this->counter = 0;
      }
      break;
    case SoShape::POLYGON:
      if (this->counter >= this->arraySize) {
        this->arraySize <<= 1;
        SoPrimitiveVertex * newArray = new SoPrimitiveVertex[this->arraySize];
        memcpy(newArray, this->vertsArray,
               sizeof(SoPrimitiveVertex)* this->counter);
        delete [] this->vertsArray;
        this->vertsArray = newArray;

        SoPointDetail * newparray = new SoPointDetail[this->arraySize];
        memcpy(newparray, this->pointDetails,
               sizeof(SoPointDetail)* this->counter);
        delete [] this->pointDetails;
        this->pointDetails = newparray;

        if (this->faceDetail) {
          for (int i = 0; i < this->counter; i++) {
            this->vertsArray[i].setDetail(&this->pointDetails[i]);
          }
        }
      }
      this->setVertex(this->counter++, v);
      break;
    case SoShape::QUADS:
      this->setVertex(this->counter++, v);
      if (this->counter == 4) {
        this->handleFaceDetail(4);
        this->shape->invokeTriangleCallbacks(this->action,
                                             &vertsArray[0],
                                             &vertsArray[1],
                                             &vertsArray[2]);
        this->shape->invokeTriangleCallbacks(this->action,
                                             &vertsArray[0],
                                             &vertsArray[2],
                                             &vertsArray[3]);
        this->counter = 0;
      }
      break;
    case SoShape::QUAD_STRIP:
      this->setVertex(this->counter++, v);
      if (counter == 4) {
        // can't use handleFaceDetail(), because of the vertex
        // order.
        if (this->faceDetail) {
          this->faceDetail->setNumPoints(4);
          this->faceDetail->setPoint(0, &this->pointDetails[0]);
          this->vertsArray[0].setDetail(this->faceDetail);
          this->faceDetail->setPoint(1, &this->pointDetails[1]);
          this->vertsArray[1].setDetail(this->faceDetail);
          this->faceDetail->setPoint(2, &this->pointDetails[3]);
          this->vertsArray[2].setDetail(this->faceDetail);
          this->faceDetail->setPoint(3, &this->pointDetails[2]);
          this->vertsArray[3].setDetail(this->faceDetail);
        }
        this->shape->invokeTriangleCallbacks(this->action,
                                             &vertsArray[0],
                                             &vertsArray[1],
                                             &vertsArray[3]);
        this->shape->invokeTriangleCallbacks(this->action,
                                             &vertsArray[0],
                                             &vertsArray[3],
                                             &vertsArray[2]);
        this->copyVertex(2, 0);
        this->copyVertex(3, 1);
        this->counter = 2;
      }
      break;
    case SoShape::POINTS:
      this->shape->invokePointCallbacks(this->action, v);
      break;
    case SoShape::LINES:
      this->setVertex(this->counter++, v);
      if (this->counter == 2) {
        this->handleLineDetail();
        this->shape->invokeLineSegmentCallbacks(this->action,
                                                &vertsArray[0],
                                                &vertsArray[1]);
        this->counter = 0;
      }
      break;
    case SoShape::LINE_STRIP:
      this->setVertex(this->counter++, v);
      if (this->counter == 2) {
        this->handleLineDetail();
        this->shape->invokeLineSegmentCallbacks(this->action,
                                                &vertsArray[0],
                                                &vertsArray[1]);
        this->copyVertex(1, 0);
        this->counter = 1;
      }
      break;
    default:
      assert(0 && "Unknown shape type");
    }
  }

public:
  SoShape::TriangleShape shapetype;
  SoAction * action;
  SoShape * shape;
  SoPrimitiveVertex * vertsArray;
  SoPointDetail * pointDetails;
  SoFaceDetail * faceDetail;
  SoLineDetail * lineDetail;
  int arraySize;
  int counter;
  SbTesselator * tess;
  int faceCounter;

  void copyVertex(const int src, const int dest) {
    this->vertsArray[dest] = this->vertsArray[src];
    if (this->faceDetail) {
      this->pointDetails[dest] = this->pointDetails[src];
      this->vertsArray[dest].setDetail(&this->pointDetails[dest]);
    }
  }
  void setVertex(const int idx, const SoPrimitiveVertex * const v) {
    this->vertsArray[idx] = *v;
    if (this->faceDetail) {
      SoPointDetail * pd = (SoPointDetail *)v->getDetail();
      assert(pd);
      this->pointDetails[idx] = * pd;
      this->vertsArray[idx].setDetail(&this->pointDetails[idx]);
    }
  }

  void handleFaceDetail(const int numv) {
    if (this->faceDetail) {
      this->faceDetail->setNumPoints(numv);
      for (int i = 0; i < numv; i++) {
        this->faceDetail->setPoint(i, &this->pointDetails[i]);
        this->vertsArray[i].setDetail(this->faceDetail);
      }
    }
  }
  void handleLineDetail(void) {
    if (this->lineDetail) {
      this->lineDetail->setPoint0(&this->pointDetails[0]);
      this->lineDetail->setPoint1(&this->pointDetails[1]);
    }
  }

  SoDetail * createPickDetail() {
    switch (this->shapetype) {
    case SoShape::TRIANGLE_STRIP:
    case SoShape::TRIANGLE_FAN:
    case SoShape::TRIANGLES:
      {
        SoFaceDetail * detail = (SoFaceDetail *)this->faceDetail->copy();
        detail->setNumPoints(3);
        detail->setPoint(0, &this->pointDetails[0]);
        detail->setPoint(1, &this->pointDetails[1]);
        detail->setPoint(2, &this->pointDetails[2]);
        return detail;
      }
    case SoShape::POLYGON:
      {
        SoFaceDetail * detail = (SoFaceDetail *)this->faceDetail->copy();
        detail->setNumPoints(this->counter);
        for (int i = 0; i < this->counter; i++) {
          detail->setPoint(i, &this->pointDetails[i]);
        }
        return detail;
      }
    case SoShape::QUADS:
    case SoShape::QUAD_STRIP:
      {
        SoFaceDetail * detail = (SoFaceDetail *)this->faceDetail->copy();
        detail->setNumPoints(4);
        detail->setPoint(0, &this->pointDetails[0]);
        detail->setPoint(1, &this->pointDetails[1]);
        detail->setPoint(2, &this->pointDetails[2]);
        detail->setPoint(3, &this->pointDetails[3]);
        return detail;
      }
    case SoShape::POINTS:
      {
        assert(0 && "should not get here");
        return NULL;
      }
    case SoShape::LINES:
    case SoShape::LINE_STRIP:
      {
        SoLineDetail * detail = (SoLineDetail *)this->lineDetail->copy();
        detail->setPoint0(&this->pointDetails[0]);
        detail->setPoint1(&this->pointDetails[1]);
        return detail;
      }
    default:
      assert(0 && "unknown shape type");
      return NULL;
    }
  }

  static void tess_callback(void * v0, void * v1, void * v2, void * data) {
    shapePrimitiveData * thisp = (shapePrimitiveData *) data;
    thisp->shape->invokeTriangleCallbacks(thisp->action,
                                          (SoPrimitiveVertex *)v0,
                                          (SoPrimitiveVertex *)v1,
                                          (SoPrimitiveVertex *)v2);
  }
};

// *************************************************************************

static shapePrimitiveData * primData = NULL;

SO_NODE_ABSTRACT_SOURCE(SoShape);


/*!
  Constructor.
*/
SoShape::SoShape(void)
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoShape);
}

/*!
  Destructor.
*/
SoShape::~SoShape()
{
}

// Doc in parent.
void
SoShape::initClass(void)
{
  SO_NODE_INTERNAL_INIT_ABSTRACT_CLASS(SoShape);
}

// Doc in parent.
void
SoShape::getBoundingBox(SoGetBoundingBoxAction * action)
{
  SbBox3f box;
  SbVec3f center;
  this->computeBBox(action, box, center);

  if (!box.isEmpty()) {
    action->extendBy(box);
    action->setCenter(center, TRUE);
  }
}

// used in generatePrimitives() callbacks to set correct material
static SoMaterialBundle * currentBundle = NULL;

// Doc in parent.
void
SoShape::GLRender(SoGLRenderAction * action)
{
  // if we get here, the shape do not have a render method and
  // generatePrimitives should therefore be used to render the
  // shape. This is probably painfully slow, so if you want speed,
  // implement the GLRender() method.  pederb, 20000612

  if (!this->shouldGLRender(action)) return;
  SoMaterialBundle mb(action);
  mb.sendFirst();
  currentBundle = &mb;  // needed in the primitive callbacks
  this->generatePrimitives(action);
}

// Doc in parent.
void
SoShape::callback(SoCallbackAction * action)
{
  if (action->shouldGeneratePrimitives(this)) {
    if (primData) primData->faceCounter = 0;
    this->generatePrimitives(action);
  }
}

/*!
  Calculates picked point based on primitives generated by subclasses.
*/
void
SoShape::rayPick(SoRayPickAction * action)
{
  if (this->shouldRayPick(action)) {
    this->computeObjectSpaceRay(action);
    this->generatePrimitives(action);
  }
}

/*!
  A convenience function that returns the size of a \a boundingbox
  projected onto the screen. Useful for \c SCREEN_SPACE complexity
  geometry.
*/
void
SoShape::getScreenSize(SoState * const state, const SbBox3f & boundingbox,
                       SbVec2s & rectsize)
{
  SbMatrix projmatrix;
  projmatrix = (SoModelMatrixElement::get(state) *
                SoViewingMatrixElement::get(state) *
                SoProjectionMatrixElement::get(state));
  
  SbVec2s vpsize = SoViewportRegionElement::get(state).getViewportSizePixels();
  SbVec3f bmin, bmax;
  boundingbox.getBounds(bmin, bmax);
  
  SbVec3f v;
  SbBox2f normbox;
  normbox.makeEmpty();
  for (int i = 0; i < 8; i++) {
    v.setValue(i&1 ? bmin[0] : bmax[0],
               i&2 ? bmin[1] : bmax[1],
               i&4 ? bmin[2] : bmax[2]);
    projmatrix.multVecMatrix(v, v);
    normbox.extendBy(SbVec2f(v[0], v[1]));
  }
  float nx, ny;
  normbox.getSize(nx, ny);

  // restrict size of projection. It is often way off when object
  // intersects the near plane. We should probably do clipping against
  // the view volume do be 100% correct, but that would be too slow.
  // pederb, 2001-05-20
  if (nx > 10.0f) nx = 10.0f;
  if (ny > 10.0f) ny = 10.0f;
  
  rectsize[0] = (short) SbMin(32767.0f, float(vpsize[0])*0.5f*nx);
  rectsize[1] = (short) SbMin(32767.0f, float(vpsize[1])*0.5f*ny);
}

/*!
  Returns the complexity value to be used by subclasses. Considers
  complexity type. For \c OBJECT_SPACE complexity this will be a
  number between 0 and 1. For \c SCREEN_SPACE complexity it is a
  number from 0 and up.
*/
float
SoShape::getComplexityValue(SoAction * action)
{
  SoState * state = action->getState();
  switch (SoComplexityTypeElement::get(state)) {
  case SoComplexityTypeElement::SCREEN_SPACE:
    {
      SbBox3f box;
      SbVec3f center;
      this->computeBBox(action, box, center);
      SbVec2s size;
      SoShape::getScreenSize(state, box, size);
      // FIXME: probably needs calibration.

#if 1 // testing new complexity code
      // The cast within the sqrt() is done to avoid ambigouity error
      // from HPUX aCC, as sqrt() can be either "long double sqrt(long
      // double)" or "float sqrt(float)". mortene.
      return float(sqrt((float)SbMax(size[0], size[1]))) * 0.4f *
        SoComplexityElement::get(state);
#else // first version
      float numPixels = float(size[0])* float(size[1]);
      return numPixels * 0.0001f * SoComplexityElement::get(state);
#endif
    }
  case SoComplexityTypeElement::OBJECT_SPACE:
    return SoComplexityElement::get(state);
  case SoComplexityTypeElement::BOUNDING_BOX:
    assert(0 && "should never get here");
    return 0.5f;
  default:
    assert(0 && "unknown complexity type");
    return 0.5f;
  }
}

typedef struct {
  int idx : 31;
  unsigned int backface : 1;
  float dist;
} sorted_triangle;

static SbList <SoPrimitiveVertex> * transparencybuffer = NULL;
static SbList <sorted_triangle> * sorted_triangle_list = NULL;
static SbBool is_doing_sorted_rendering; // need this in invokeTriangleCallbacks()

// qsort callback
static int
compare_triangles(const void * ptr1, const void * ptr2)
{
  sorted_triangle * tri1 = (sorted_triangle*) ptr1;
  sorted_triangle * tri2 = (sorted_triangle*) ptr2;

  if (tri1->dist > tri2->dist) return -1;
  if (tri1->dist == tri2->dist) return tri2->backface - tri1->backface;
  return 1;
}

// atexit callback
static void
soshape_cleanup_transparencybuffer(void)
{
  delete transparencybuffer;
  delete sorted_triangle_list;
}

/*!
  \internal
*/
SbBool
SoShape::shouldGLRender(SoGLRenderAction * action)
{
  if (action->getCurPathCode() == SoAction::OFF_PATH &&
      !this->affectsState()) return FALSE;

  SoState * state = action->getState();

  SbBool needNormals =
    SoLightModelElement::get(state) == SoLightModelElement::PHONG;

  SbBool transparent = SoTextureImageElement::containsTransparency(state);
  if (!transparent) {
    const SoDiffuseColorElement * diffelt =
      SoDiffuseColorElement::getInstance(state);
    if (diffelt->isPacked()) transparent = diffelt->hasPackedTransparency();
    else {
      const SoTransparencyElement * trans =
        SoTransparencyElement::getInstance(state);
      transparent = trans->getNum() > 1 ||
        trans->get(0) > 0.0f;
    }
  }

  if (action->handleTransparency(transparent))
    return FALSE;

  if (SoDrawStyleElement::get(state) == SoDrawStyleElement::INVISIBLE)
    return FALSE;

  // make sure lazy elements are up to date
  // all material lazy elements are handled by SoMaterialBundle
  state->lazyEvaluate();

  // SoGLTextureImageElement is lazy, but needs some arguments
  // update manually
  const SoGLTextureImageElement * ti = (SoGLTextureImageElement *)
    state->getConstElement(SoGLTextureImageElement::getClassStackIndex());
  ti->evaluate(SoGLTextureEnabledElement::get(state),
               transparent && !SoShapeStyleElement::isScreenDoor(state));

  if (SoComplexityTypeElement::get(state) ==
      SoComplexityTypeElement::BOUNDING_BOX) {

    SbBox3f box;
    SbVec3f center;
    this->computeBBox(action, box, center);
    center = (box.getMin() + box.getMax()) * 0.5f;
    SbVec3f size = box.getMax()  - box.getMin();

    SoMaterialBundle mb(action);
    mb.sendFirst();

    {
      const SoGLShapeHintsElement * sh = (SoGLShapeHintsElement *)
        state->getConstElement(SoGLShapeHintsElement::getClassStackIndex());
      sh->forceSend(TRUE, FALSE, FALSE);
    }

    glPushMatrix();
    glTranslatef(center[0], center[1], center[2]);
    sogl_render_cube(size[0], size[1], size[2], &mb,
                     SOGL_NEED_NORMALS | SOGL_NEED_TEXCOORDS);
    glPopMatrix();
    return FALSE;
  }

  // test if we should sort triangles before rendering
  if (transparent &&
      ((action->getTransparencyType() ==
        SoGLRenderAction::SORTED_OBJECT_SORTED_TRIANGLE_BLEND) ||
       (action->getTransparencyType() ==
        SoGLRenderAction::SORTED_OBJECT_SORTED_TRIANGLE_ADD))) {

    if (transparencybuffer == NULL) {
      transparencybuffer = new SbList <SoPrimitiveVertex>;
      sorted_triangle_list = new SbList <sorted_triangle>;
      atexit(soshape_cleanup_transparencybuffer);
    }

    // do this before generating triangles to get correct
    // material for lines and point (only triangles are sorted).
    SoMaterialBundle mb(action);
    mb.sendFirst();
    currentBundle = &mb;

    // this will render lines and points, and copy triangle vertices
    // into transparencybuffer.
    transparencybuffer->truncate(0);
    is_doing_sorted_rendering = TRUE;
    this->generatePrimitives(action);
    is_doing_sorted_rendering = FALSE;
    int i, n = transparencybuffer->getLength() / 3;
    if (n == 0) return FALSE; // finished

    const SoPrimitiveVertex * varray = transparencybuffer->getArrayPtr();

    sorted_triangle_list->truncate(0);
    sorted_triangle tri;

    const SoPrimitiveVertex * v;
    const SbMatrix & mm = SoModelMatrixElement::get(state);

    SoShapeHintsElement::VertexOrdering vo;
    SoShapeHintsElement::ShapeType st;
    SoShapeHintsElement::FaceType ft;
    SoShapeHintsElement::get(state, vo, st, ft);

    SbBool bfcull = 
      (vo != SoShapeHintsElement::UNKNOWN_ORDERING) &&
      (st == SoShapeHintsElement::SOLID);

    if (bfcull || vo == SoShapeHintsElement::UNKNOWN_ORDERING) {
      SbPlane nearp = SoViewVolumeElement::get(state).getPlane(0.0f);
      nearp = SbPlane(-nearp.getNormal(), -nearp.getDistanceFromOrigin());
      // if back face culling is enabled, we can do less work
      SbVec3f center;
      for (i = 0; i < n; i++) {
        int idx = i*3;
        center.setValue(0.0f, 0.0f, 0.0f);
        tri.idx = idx;
        for (int j = 0; j < 3; j++) {
          tri.backface = 0;
          v = varray + idx + j;
          center += v->getPoint();
        }
        center /= 3.0f;
        mm.multVecMatrix(center, center);
        tri.dist = nearp.getDistance(center);
        sorted_triangle_list->append(tri);
      }
    }
    else {
      // project each point onto screen to find the vertex
      // ordering of the triangle. Sort on vertex closest
      // to the near plane.
      SbMatrix obj2vp = 
        mm * SoViewingMatrixElement::get(state) *
        SoProjectionMatrixElement::get(state);
      
      int clockwise = (vo == SoShapeHintsElement::CLOCKWISE) ? 1 : 0;
      SbVec3f c[3];
      for (i = 0; i < n; i++) {
        int idx = i*3;
        tri.idx = idx;
        // projected coordinates are between -1 and 1
        float smalldist = 10.0f;
        for (int j = 0; j < 3; j++) {
          v = varray + idx + j;
          c[j] = v->getPoint();
          obj2vp.multVecMatrix(c[j], c[j]);
          float dist = c[j][2];
          if (dist < smalldist) smalldist = dist;
        }
        SbVec3f v0 = c[2]-c[0];
        SbVec3f v1 = c[1]-c[0];
        // we need only the z-component of the cross product
        // to determine if triangle is cw or ccw
        float cz = v0[0]*v1[1] - v0[1]*v1[0];
        tri.backface = clockwise; 
        if (cz < 0.0f) tri.backface = 1 - clockwise;
        tri.dist = smalldist;
        sorted_triangle_list->append(tri);
      }
    }

    const sorted_triangle * tarray = sorted_triangle_list->getArrayPtr();
    qsort((void*)tarray, n, sizeof(sorted_triangle), compare_triangles);

    int idx;
    
    // this rendering loop can be optimized a lot, of course, but speed
    // is not so important here, since it's slow to generate, copy and
    // sort the triangles anyway.
    glBegin(GL_TRIANGLES);
    for (i = 0; i < n; i++) {
      idx = tarray[i].idx;
      v = varray + idx;
      glTexCoord4fv(v->getTextureCoords().getValue());
      glNormal3fv(v->getNormal().getValue());
      currentBundle->send(v->getMaterialIndex(), TRUE);
      glVertex3fv(v->getPoint().getValue());

      v = varray + idx+1;
      glTexCoord4fv(v->getTextureCoords().getValue());
      glNormal3fv(v->getNormal().getValue());
      currentBundle->send(v->getMaterialIndex(), TRUE);
      glVertex3fv(v->getPoint().getValue());

      v = varray + idx+2;
      glTexCoord4fv(v->getTextureCoords().getValue());
      glNormal3fv(v->getNormal().getValue());
      currentBundle->send(v->getMaterialIndex(), TRUE);
      glVertex3fv(v->getPoint().getValue());
    }
    glEnd();
    return FALSE; // tell shape _not_ to render
  }
  return TRUE;
}

/*!
  \internal
*/
SbBool
SoShape::shouldRayPick(SoRayPickAction * const action)
{
  switch (SoPickStyleElement::get(action->getState())) {
  case SoPickStyleElement::SHAPE:
    return TRUE;
  case SoPickStyleElement::BOUNDING_BOX:
    this->rayPickBoundingBox(action);
    return FALSE;
  case SoPickStyleElement::UNPICKABLE:
    return FALSE;
  default:
    assert(0 && "unknown pick style");
    return TRUE;
  }
}

/*!
  \internal
*/
void
SoShape::beginSolidShape(SoGLRenderAction * action)
{
  SoState * state = action->getState();
  state->push();

  SoShapeHintsElement::set(state,
                           SoShapeHintsElement::COUNTERCLOCKWISE,
                           SoShapeHintsElement::SOLID,
                           SoShapeHintsElement::FACE_TYPE_AS_IS);
}

/*!
  \internal
*/
void
SoShape::endSolidShape(SoGLRenderAction * action)
{
  action->getState()->pop();
}

/*!
  \internal
*/
void
SoShape::computeObjectSpaceRay(SoRayPickAction * const action)
{
  action->setObjectSpace();
}

/*!
  \internal
*/
void
SoShape::computeObjectSpaceRay(SoRayPickAction * const action,
                               const SbMatrix & matrix)
{
  action->setObjectSpace(matrix);
}

/*!
  Will create triangle detail for a SoPickedPoint. This method will
  only be called internally, when generatePrimitives() is used for
  picking (SoShape::rayPick() is not overloaded).

  This method returns \c NULL in Open Inventor, and subclasses will
  need to overload this method to create details for a SoPickedPoint.

  This is not necessary with Coin. Of course, if you choose to
  overload it, it will work in the same way as Open Inventor.

  For this to work, you must supply a face or line detail when
  generating primitives. If you supply \c NULL for the detail argument in
  SoShape::beginShape(), you'll have to overload this method.
*/
SoDetail *
SoShape::createTriangleDetail(SoRayPickAction * action,
                              const SoPrimitiveVertex * /*v1*/,
                              const SoPrimitiveVertex * /*v2*/,
                              const SoPrimitiveVertex * /*v3*/,
                              SoPickedPoint * pp)
{
  if (primData && primData->faceDetail) {
    return primData->createPickDetail();
  }
#if COIN_DEBUG
  SoDebugError::postInfo("SoShape::createTriangleDetail",
                         "Unable to create triangle detail.");
#endif // COIN_DEBUG
  return NULL;
}

/*!
  Will create line detail for a SoPickedPoint. This method will only
  be called internally, when generatePrimitives() is used for picking
  (SoShape::rayPick() is not overloaded).

  This method returns \c NULL in Open Inventor, and subclasses will
  need to overload this method to create details for a SoPickedPoint.

  This is not necessary with Coin. Of course, if you choose to
  overload it, it will work in the same way as Open Inventor.

  For this to work, you must supply a face or line detail when
  generating primitives. If you supply \c NULL for the detail argument in
  SoShape::beginShape(), you'll have to overload this method.
*/
SoDetail *
SoShape::createLineSegmentDetail(SoRayPickAction * action,
                                 const SoPrimitiveVertex * /* v1 */,
                                 const SoPrimitiveVertex * /* v2 */,
                                 SoPickedPoint * pp)
{
  if (primData && primData->lineDetail) {
    return primData->createPickDetail();
  }
#if COIN_DEBUG
  SoDebugError::postInfo("SoShape::createLineSegmentDetail",
                         "Unable to create line segment detail.");
#endif // COIN_DEBUG
  return NULL;
}

/*!
  Will create point detail for a SoPickedPoint. This method will only
  be called internally, when generatePrimitives() is used for picking
  (SoShape::rayPick() is not overloaded).

  This method returns \c NULL in Open Inventor, and subclasses will
  need to overload this method to create details for a SoPickedPoint.

  This is not necessary with Coin. Of course, if you choose to
  overload it, it will work in the same way as Open Inventor.

  For this to work, you must supply a point detail in the
  SoPrimitiveVertex in generatePrimitives().
*/
SoDetail *
SoShape::createPointDetail(SoRayPickAction * /* action */,
                           const SoPrimitiveVertex * v,
                           SoPickedPoint * /* pp */)
{
  if (v->getDetail()) return v->getDetail()->copy();
  return NULL;
}

/*!
  \internal
*/
void
SoShape::invokeTriangleCallbacks(SoAction * const action,
                                 const SoPrimitiveVertex * const v1,
                                 const SoPrimitiveVertex * const v2,
                                 const SoPrimitiveVertex * const v3)
{
  if (action->getTypeId().isDerivedFrom(SoRayPickAction::getClassTypeId())) {
    SoRayPickAction * ra = (SoRayPickAction *) action;

    SbVec3f intersection;
    SbVec3f barycentric;
    SbBool front;

    if (ra->intersect(v1->getPoint(), v2->getPoint(), v3->getPoint(),
                      intersection, barycentric, front)) {

      if (ra->isBetweenPlanes(intersection)) {
        SoPickedPoint * pp = ra->addIntersection(intersection);
        if (pp) {
          pp->setDetail(this->createTriangleDetail(ra, v1, v2, v3, pp), this);
          // calculate normal at picked point
          SbVec3f n =
            v1->getNormal() * barycentric[0] +
            v2->getNormal() * barycentric[1] +
            v3->getNormal() * barycentric[2];
          n.normalize();
          pp->setObjectNormal(n);

          // calculate texture coordinate at picked point
          SbVec4f tc =
            v1->getTextureCoords() * barycentric[0] +
            v2->getTextureCoords() * barycentric[1] +
            v3->getTextureCoords() * barycentric[2];
          pp->setObjectTextureCoords(tc);

          // material index need to be approximated, since there is no
          // way to average material indices :( This makes it
          // impossible to fully support color per vertex. An
          // extension to the OIV API would perhaps be a good idea
          // here? Maybe calculate the rgba value for diffuse and
          // transparency and set it in SoPickedPoint?
          float maxval = barycentric[0];
          const SoPrimitiveVertex * maxv = v1;
          if (barycentric[1] > maxval) {
            maxv = v2;
            maxval = barycentric[1];
          }
          if (barycentric[2] > maxval) {
            maxv = v3;
          }
          pp->setMaterialIndex(maxv->getMaterialIndex());
        }
      }
    }
  }
  else if (action->getTypeId().isDerivedFrom(SoCallbackAction::getClassTypeId())) {
    SoCallbackAction * ca = (SoCallbackAction *) action;
    ca->invokeTriangleCallbacks(this, v1, v2, v3);
  }
  else if (action->getTypeId().isDerivedFrom(SoGetPrimitiveCountAction::getClassTypeId())) {
    SoGetPrimitiveCountAction * ga = (SoGetPrimitiveCountAction *) action;
    ga->incNumTriangles();
  }
  else if (action->getTypeId().isDerivedFrom(SoGLRenderAction::getClassTypeId())) {
    if (is_doing_sorted_rendering) {
      transparencybuffer->append(*v1);
      transparencybuffer->append(*v2);
      transparencybuffer->append(*v3);
    }
    else {
      glBegin(GL_TRIANGLES);
      glTexCoord4fv(v1->getTextureCoords().getValue());
      glNormal3fv(v1->getNormal().getValue());
      currentBundle->send(v1->getMaterialIndex(), TRUE);
      glVertex3fv(v1->getPoint().getValue());

      glTexCoord4fv(v2->getTextureCoords().getValue());
      glNormal3fv(v2->getNormal().getValue());
      currentBundle->send(v2->getMaterialIndex(), TRUE);
      glVertex3fv(v2->getPoint().getValue());

      glTexCoord4fv(v3->getTextureCoords().getValue());
      glNormal3fv(v3->getNormal().getValue());
      currentBundle->send(v3->getMaterialIndex(), TRUE);
      glVertex3fv(v3->getPoint().getValue());
      glEnd();
    }
  }
}

/*!
  \internal
*/
void
SoShape::invokeLineSegmentCallbacks(SoAction * const action,
                                    const SoPrimitiveVertex * const v1,
                                    const SoPrimitiveVertex * const v2)
{
  if (action->getTypeId().isDerivedFrom(SoRayPickAction::getClassTypeId())) {
    SoRayPickAction * ra = (SoRayPickAction *) action;

    SbVec3f intersection;
    if (ra->intersect(v1->getPoint(), v2->getPoint(), intersection)) {
      if (ra->isBetweenPlanes(intersection)) {
        SoPickedPoint * pp = ra->addIntersection(intersection);
        if (pp) {
          pp->setDetail(this->createLineSegmentDetail(ra, v1, v2, pp), this);
          float total = (v2->getPoint()-v1->getPoint()).length();
          float len1 = (intersection-v1->getPoint()).length();
          float len2 = (intersection-v2->getPoint()).length();
          if (total > 0.0f) {
            len1 /= total;
            len2 /= total;
          }
          SbVec3f n =
            v1->getNormal() * len1 +
            v2->getNormal() * len2;
          n.normalize();
          pp->setObjectNormal(n);

          SbVec4f tc =
            v1->getTextureCoords() * len1 +
            v2->getTextureCoords() * len2;
          pp->setObjectTextureCoords(tc);
          pp->setMaterialIndex(len1 >= len2 ?
                               v1->getMaterialIndex() :
                               v2->getMaterialIndex());

        }
      }
    }
  }
  else if (action->getTypeId().isDerivedFrom(SoCallbackAction::getClassTypeId())) {
    SoCallbackAction * ca = (SoCallbackAction *) action;
    ca->invokeLineSegmentCallbacks(this, v1, v2);
  }
  else if (action->getTypeId().isDerivedFrom(SoGetPrimitiveCountAction::getClassTypeId())) {
    SoGetPrimitiveCountAction * ga = (SoGetPrimitiveCountAction *) action;
    ga->incNumLines();
  }
  else if (action->getTypeId().isDerivedFrom(SoGLRenderAction::getClassTypeId())) {
    glBegin(GL_LINES);
    glTexCoord4fv(v1->getTextureCoords().getValue());
    glNormal3fv(v1->getNormal().getValue());
    currentBundle->send(v1->getMaterialIndex(), TRUE);
    glVertex3fv(v1->getPoint().getValue());

    glTexCoord4fv(v2->getTextureCoords().getValue());
    glNormal3fv(v2->getNormal().getValue());
    currentBundle->send(v2->getMaterialIndex(), TRUE);
    glVertex3fv(v2->getPoint().getValue());
    glEnd();
  }
}

/*!
  \internal
*/
void
SoShape::invokePointCallbacks(SoAction * const action,
                              const SoPrimitiveVertex * const v)
{
  if (action->getTypeId().isDerivedFrom(SoRayPickAction::getClassTypeId())) {
    SoRayPickAction * ra = (SoRayPickAction *) action;

    SbVec3f intersection = v->getPoint();
    if (ra->intersect(intersection)) {
      if (ra->isBetweenPlanes(intersection)) {
        SoPickedPoint * pp = ra->addIntersection(intersection);
        if (pp) {
          pp->setDetail(this->createPointDetail(ra, v, pp), this);
          pp->setObjectNormal(v->getNormal());
          pp->setObjectTextureCoords(v->getTextureCoords());
          pp->setMaterialIndex(v->getMaterialIndex());
        }
      }
    }
  }
  else if (action->getTypeId().isDerivedFrom(SoCallbackAction::getClassTypeId())) {
    SoCallbackAction * ca = (SoCallbackAction *) action;
    ca->invokePointCallbacks(this, v);
  }
  else if (action->getTypeId().isDerivedFrom(SoGetPrimitiveCountAction::getClassTypeId())) {
    SoGetPrimitiveCountAction * ga = (SoGetPrimitiveCountAction *) action;
    ga->incNumPoints();
  }
  else if (action->getTypeId().isDerivedFrom(SoGLRenderAction::getClassTypeId())) {
    glBegin(GL_POINTS);
    glTexCoord4fv(v->getTextureCoords().getValue());
    glNormal3fv(v->getNormal().getValue());
    currentBundle->send(v->getMaterialIndex(), TRUE);
    glVertex3fv(v->getPoint().getValue());
    glEnd();
  }
}

/*!
  This method is slightly different from its counterpart from the
  original Open Inventor library, as this method has an SoDetail as
  the last argument, and not an SoFaceDetail. This is because we
  accept more TriangleShape types, and the detail might be a
  SoFaceDetail or a SoLineDetail. There is no use sending in a
  SoPointDetail, as nothing will be done with it.
*/
void
SoShape::beginShape(SoAction * const action, const TriangleShape shapetype,
                    SoDetail * const detail)
{
  if (primData == NULL) primData = new shapePrimitiveData();
  primData->beginShape(this, action, shapetype, detail);
}


/*!
  \internal
*/
void
SoShape::shapeVertex(const SoPrimitiveVertex * const v)
{
  assert(primData);
  primData->shapeVertex(v);
}

/*!
  \internal
*/
void
SoShape::endShape(void)
{
  assert(primData);
  primData->endShape();
}

/*!
  Convenience function which sets up an SoPrimitiveVertex, and sends
  it using the SoShape::shapeVertex() function.
*/
void
SoShape::generateVertex(SoPrimitiveVertex * const pv,
                        const SbVec3f & point,
                        const SbBool usetexfunc,
                        const SoTextureCoordinateElement * const tce,
                        const float s,
                        const float t,
                        const SbVec3f & normal)
{
  SbVec4f texCoord;
  if (usetexfunc)
    texCoord = tce->get(point, normal);
  else
    texCoord.setValue(s, t, 0.0f, 1.0f);
  pv->setPoint(point);
  pv->setNormal(normal);
  pv->setTextureCoords(texCoord);
  shapeVertex(pv);
}

/*!
  Overloaded from default setting in SoNode, as we know for certain
  that no node classes derived from SoShape will affect the rendering
  state.
 */
SbBool
SoShape::affectsState(void) const
{
  // a standard shape node should not affect the state.
  return FALSE;
}

// Doc in parent.
void
SoShape::getPrimitiveCount(SoGetPrimitiveCountAction * action)
{
  if (this->shouldPrimitiveCount(action)) this->generatePrimitives(action);
}

/*!
  Not implemented in Coin. Should probably have been private in TGS Inventor.
 */
float
SoShape::getDecimatedComplexity(SoState * state, float complexity)
{
  COIN_OBSOLETED();
  return 1.0f * complexity;
}

/*!
  Not implemented in Coin. Should probably have been private in OIV.
*/
void
SoShape::GLRenderBoundingBox(SoGLRenderAction * action)
{
  COIN_OBSOLETED();
}

/*!
  \internal
 */
SbBool
SoShape::shouldPrimitiveCount(SoGetPrimitiveCountAction * action)
{
  return TRUE; // FIXME: what to do here? pederb 1999-11-25
}

//
// used when pickStyle == BOUNDING_BOX
//
void
SoShape::rayPickBoundingBox(SoRayPickAction * action)
{
  SbBox3f box;
  SbVec3f center;
  this->computeBBox(action, box, center);
  if (box.isEmpty()) return;
  this->computeObjectSpaceRay(action);
  SbVec3f isect;
  if (action->intersect(box, isect, FALSE)) {
    if (action->isBetweenPlanes(isect)) {
      action->addIntersection(isect);
    }
  }
}

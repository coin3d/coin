/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2001 by Systems in Motion.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  version 2 as published by the Free Software Foundation.  See the
 *  file LICENSE.GPL at the root directory of this source distribution
 *  for more details.
 *
 *  If you desire to use Coin with software that is incompatible
 *  licensewise with the GPL, and / or you would like to take
 *  advantage of the additional benefits with regard to our support
 *  services, please contact Systems in Motion about acquiring a Coin
 *  Professional Edition License.  See <URL:http://www.coin3d.org> for
 *  more information.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  <URL:http://www.sim.no>, <mailto:support@sim.no>
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
#include <Inventor/nodes/SoVertexShape.h>
#include <Inventor/actions/SoRayPickAction.h>
#include <Inventor/actions/SoGetBoundingBoxAction.h>
#include <Inventor/actions/SoCallbackAction.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/caches/SoBoundingBoxCache.h>
#include <Inventor/elements/SoCacheElement.h>
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
#include <Inventor/elements/SoGLTexture3EnabledElement.h>
#include <Inventor/elements/SoTextureQualityElement.h>
#include <Inventor/elements/SoCullElement.h>

#include <Inventor/misc/SoGL.h>
#include <Inventor/misc/SoGLBigImage.h>
#include <Inventor/bundles/SoMaterialBundle.h>
#include <Inventor/details/SoFaceDetail.h>
#include <Inventor/details/SoLineDetail.h>
#include <Inventor/SoPickedPoint.h>
#include <Inventor/actions/SoGetPrimitiveCountAction.h>
#include <Inventor/elements/SoGLTextureImageElement.h>
#include <Inventor/elements/SoComplexityElement.h>
#include <Inventor/SbPlane.h>
#include <Inventor/SbBox2f.h>
#include <Inventor/SbClip.h>

// SoShape.cpp grew too big, so I had to move some code into
// three new files. pederb, 2001-07-18
#include "soshape_primdata.h"
#include "soshape_trianglesort.h"
#include "soshape_bigtexture.h"

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H

#include <Inventor/system/gl.h>
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

// FIXME: should be documented properly. See also FIXME above
// beingShape() doc. 20020523 mortene.
/*!
  \fn void SoShape::generatePrimitives(SoAction * action)
  \internal
*/

// *************************************************************************

#ifndef DOXYGEN_SKIP_THIS

class SoShapeP {
public:
  SoShapeP() {
    this->bboxcache = NULL;
    this->invalidcounter = 0;
    this->readcounter = 0;
  }
  
  ~SoShapeP() {
    if (this->bboxcache) { this->bboxcache->unref(); }
  }
  SoBoundingBoxCache * bboxcache;
  int invalidcounter;
  int readcounter;
};

#endif // DOXYGEN_SKIP_THIS

#undef THIS
#define THIS this->pimpl

// *************************************************************************


static shapePrimitiveData * primData = NULL;

SO_NODE_ABSTRACT_SOURCE(SoShape);


/*!
  Constructor.
*/
SoShape::SoShape(void)
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoShape);
  
  // don't allocate private data until we need it
  THIS = NULL;
}

/*!
  Destructor.
*/
SoShape::~SoShape()
{
  delete THIS;
}

// Doc in parent.
void
SoShape::initClass(void)
{
  SO_NODE_INTERNAL_INIT_ABSTRACT_CLASS(SoShape, SO_FROM_INVENTOR_1);
}

// Doc in parent.
void
SoShape::getBoundingBox(SoGetBoundingBoxAction * action)
{
  SbBox3f box;
  SbVec3f center;
  this->getBBox(action, box, center);
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

// test bbox intersection
static SbBool
soshape_ray_intersect(SoRayPickAction * action, const SbBox3f & box)
{
  if (box.isEmpty()) return FALSE;
  return action->intersect(box, TRUE);
}


/*!
  Calculates picked point based on primitives generated by subclasses.
*/
void
SoShape::rayPick(SoRayPickAction * action)
{
  if (this->shouldRayPick(action)) {
    this->computeObjectSpaceRay(action);
    
    // if we have a valid bbox cache, test bbox/ray intersection
    // before testing all triangles.
    if (!THIS || 
        !THIS->bboxcache || 
        !THIS->bboxcache->isValid(action->getState()) ||
        soshape_ray_intersect(action, THIS->bboxcache->getProjectedBox())) {
      this->generatePrimitives(action);
    }
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
      this->getBBox(action, box, center);
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
    // return default value. We might get here when generating
    // primitives, not when rendering.
    return 0.5f;
  default:
    assert(0 && "unknown complexity type");
    return 0.5f;
  }
}

static SbBool is_doing_sorted_rendering;     // need this in invokeTriangleCallbacks()
static SbBool is_doing_bigtexture_rendering;


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
      // Earlier we had the following code here, which might have been
      // a bit faster than the current version:
      //
      //   transparent = trans->getNum() > 1 || trans->get(0) > 0.0f;
      //
      // The assumption was that if more than one transparency value
      // was supplied, at least one of them should be != 0.0,
      // otherwise there wouldn't be any point in having more than one
      // value. But, since we'll break out of the loop below when the
      // first transparent value is found, it should be just as fast
      // in most cases.               pederb, 2001-10-23

      // NB: this transparency test is not guaranteed to be correct,
      // since there's no way that we can figure out if the
      // transparent values are actually used by the shape. Maybe a
      // new virtual function SoShape::hasTransparency(SoState*) would
      // be a good idea?             pederb, 2001-10-23

      const int n = trans->getNum();
      const float * tarr = trans->getArrayPtr();
      for (int i = 0; i < n; i++) {
        if (tarr[i] > 0.0f) {
          transparent = TRUE;
          break;
        }
      }
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
  ti->evaluate(SoGLTextureEnabledElement::get(state) ||
               SoGLTexture3EnabledElement::get(state),
               transparent && !SoShapeStyleElement::isScreenDoor(state));

  if (SoComplexityTypeElement::get(state) ==
      SoComplexityTypeElement::BOUNDING_BOX) {


    this->GLRenderBoundingBox(action);
    return FALSE;
  }

  // test if we should sort triangles before rendering
  if (transparent &&
      ((action->getTransparencyType() ==
        SoGLRenderAction::SORTED_OBJECT_SORTED_TRIANGLE_BLEND) ||
       (action->getTransparencyType() ==
        SoGLRenderAction::SORTED_OBJECT_SORTED_TRIANGLE_ADD))) {

    // do this before generating triangles to get correct
    // material for lines and point (only triangles are sorted).
    SoMaterialBundle mb(action);
    mb.sendFirst();
    currentBundle = &mb;

    trisort_begin_shape(state);

    // this will render lines and points, and copy triangle vertices
    // into transparencybuffer.
    is_doing_sorted_rendering = TRUE;
    this->generatePrimitives(action);
    is_doing_sorted_rendering = FALSE;

    trisort_end_shape(state, mb); // this will render the triangles
    return FALSE; // tell shape _not_ to render
  }

  SoGLTextureImageElement::Model model;
  SbColor blendcolor;
  SoGLImage * glimage = SoGLTextureImageElement::get(state, model, blendcolor);
  if (glimage &&
      glimage->isOfType(SoGLBigImage::getClassTypeId()) &&
      SoGLTextureEnabledElement::get(state)) {
    // do this before generating triangles to get correct
    // material for lines and point (only triangles are handled for now).
    SoMaterialBundle mb(action);
    mb.sendFirst();
    currentBundle = &mb;

    SoGLBigImage * big = (SoGLBigImage*) glimage;

    is_doing_bigtexture_rendering = TRUE;


    bigtexture_begin_shape(state, big, SoTextureQualityElement::get(state));
    this->generatePrimitives(action);
    bigtexture_end_shape(state, this, mb);
    is_doing_bigtexture_rendering = FALSE;

    return FALSE;
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
  picking (SoShape::rayPick() is not overridden).

  This method returns \c NULL in Open Inventor, and subclasses will
  need to override this method to create details for a SoPickedPoint.

  This is not necessary with Coin. Of course, if you choose to
  override it, it will work in the same way as Open Inventor.

  For this to work, you must supply a face or line detail when
  generating primitives. If you supply \c NULL for the detail argument in
  SoShape::beginShape(), you'll have to override this method.
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
  (SoShape::rayPick() is not overridden).

  This method returns \c NULL in Open Inventor, and subclasses will
  need to override this method to create details for a SoPickedPoint.

  This is not necessary with Coin. Of course, if you choose to
  override it, it will work in the same way as Open Inventor.

  For this to work, you must supply a face or line detail when
  generating primitives. If you supply \c NULL for the detail argument in
  SoShape::beginShape(), you'll have to override this method.
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
  (SoShape::rayPick() is not overridden).

  This method returns \c NULL in Open Inventor, and subclasses will
  need to override this method to create details for a SoPickedPoint.

  This is not necessary with Coin. Of course, if you choose to
  override it, it will work in the same way as Open Inventor.

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
      trisort_triangle(action->getState(), v1, v2, v3);
    }
    else if (is_doing_bigtexture_rendering) {
      bigtexture_triangle(action->getState(), v1, v2, v3);
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
          float len1 = 1.0f;
          float len2 = 0.0f;
          if (total >= 0.0f) {
            len1 = (intersection-v1->getPoint()).length();
            len2 = (intersection-v2->getPoint()).length();
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

// FIXME: document this method properly. It's a must for app
// programmers extending the library with new SoShape-derived
// nodes. One item for the doc when it's written: mention that
// beginShape() and endShape() can be called several times during
// generatePrimitives() (seems to be a FAQ). 20020523 mortene.
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

// FIXME: document this method properly. It's a must for app
// programmers extending the library with new SoShape-derived
// nodes. 20020523 mortene.
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
  it using the SoShape::shapeVertex() function. 2D version
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
  this->generateVertex(pv, point, usetexfunc, tce, s, t, 0.0f, normal);
}

/*!
  Convenience function which sets up an SoPrimitiveVertex, and sends
  it using the SoShape::shapeVertex() function. 3D version.

  \since 2001-12-04
*/
void
SoShape::generateVertex(SoPrimitiveVertex * const pv,
                        const SbVec3f & point,
                        const SbBool usetexfunc,
                        const SoTextureCoordinateElement * const tce,
                        const float s,
                        const float t,
                        const float r,
                        const SbVec3f & normal)
{
  SbVec4f texCoord;
  if (usetexfunc)
    texCoord = tce->get(point, normal);
  else
    texCoord.setValue(s, t, r, 1.0f);
  pv->setPoint(point);
  pv->setNormal(normal);
  pv->setTextureCoords(texCoord);
  shapeVertex(pv);
}

// Doc in superclass.
SbBool
SoShape::affectsState(void) const
{
  // Overridden from default setting in SoNode to return FALSE instead
  // of TRUE, as we know for certain that no node classes derived from
  // SoShape will affect the rendering state.
  return FALSE;
}

// Doc in superclass.
void
SoShape::getPrimitiveCount(SoGetPrimitiveCountAction * action)
{
  if (this->shouldPrimitiveCount(action)) this->generatePrimitives(action);
}

/*!
  Not implemented in Coin. Should probably have been private in TGS
  Inventor API.
 */
float
SoShape::getDecimatedComplexity(SoState * state, float complexity)
{
  COIN_OBSOLETED();
  return 1.0f * complexity;
}

/*!
  Render a bounding box.
*/
void
SoShape::GLRenderBoundingBox(SoGLRenderAction * action)
{
  SbBox3f box;
  SbVec3f center;
  this->getBBox(action, box, center);
  center = (box.getMin() + box.getMax()) * 0.5f;
  SbVec3f size = box.getMax()  - box.getMin();
  
  SoMaterialBundle mb(action);
  mb.sendFirst();
  
  {
    SoGLShapeHintsElement::forceSend(action->getState(), TRUE, FALSE, FALSE);
  }
  
  glPushMatrix();
  glTranslatef(center[0], center[1], center[2]);
  sogl_render_cube(size[0], size[1], size[2], &mb,
                   SOGL_NEED_NORMALS | SOGL_NEED_TEXCOORDS);
  glPopMatrix();
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
  this->getBBox(action, box, center);
  if (box.isEmpty()) return;
  this->computeObjectSpaceRay(action);
  SbVec3f isect;
  if (action->intersect(box, isect, FALSE)) {
    if (action->isBetweenPlanes(isect)) {
      action->addIntersection(isect);
    }
  }
}

// Doc from superclass.
void
SoShape::notify(SoNotList * nl)
{
  inherited::notify(nl);
  if (THIS && THIS->bboxcache) { 
    THIS->bboxcache->invalidate(); 
  }
}

// return the bbox for this shape, using the cache if valid,
// calculating it if not.
void 
SoShape::getBBox(SoAction * action, SbBox3f & box, SbVec3f & center)
{
  SbBool isvalid = FALSE;
  SbBool shouldcache = FALSE;

  if (this->isOfType(SoVertexShape::getClassTypeId())) {
    if (THIS && THIS->bboxcache && THIS->bboxcache->isValid(action->getState())) {
      isvalid = TRUE;
    }
    else {
      if (!THIS) THIS = new SoShapeP;
      shouldcache = TRUE;
    }
  }

  if (isvalid) {
    box = THIS->bboxcache->getProjectedBox();
    // we know center will be set, so just fetch it from the cache
    center = THIS->bboxcache->getCenter();
  }
  else {
    SoState * state = action->getState();
    SbBool storedinvalid = FALSE;
    if (shouldcache) {
      // must push state to make cache dependencies work
      state->push();
      storedinvalid = SoCacheElement::setInvalid(FALSE);
      if (THIS->bboxcache) THIS->bboxcache->unref();
      THIS->bboxcache = new SoBoundingBoxCache(state);
      THIS->bboxcache->ref();
      SoCacheElement::set(state, THIS->bboxcache);
    }
    this->computeBBox(action, box, center);
    if (shouldcache) {
      THIS->bboxcache->set(box, TRUE, center);
      // pop state since we pushed it
      state->pop();
      SoCacheElement::setInvalid(storedinvalid);
    }
  }
}


#undef THIS

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
  \class SoShape SoShape.h Inventor/nodes/SoShape.h
  \brief The SoShape class is the superclass for geometry shapes.
  \ingroup nodes

  The node types which have actual geometry to render inherits this
  class. For convenince, the SoShape class contains various common
  code used by the subclasses.
*/

// Metadon doc:
/*¡
  FIXME: there're a bunch of FIXMEs scattered around in the source for
  the SoShape class. They seem harmless, but they should be checked
  and exterminated. 20000312 mortene.
 */

#include <Inventor/misc/SoState.h>
#include <Inventor/nodes/SoSubNodeP.h>
#include <coindefs.h> // COIN_STUB()
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
#include <Inventor/elements/SoViewportRegionElement.h>
#include <Inventor/elements/SoDiffuseColorElement.h>

// Lazy GL-elements not handled by SoMaterialBundle
#include <Inventor/elements/SoGLPointSizeElement.h>
#include <Inventor/elements/SoGLLineWidthElement.h>
#include <Inventor/elements/SoGLPolygonOffsetElement.h>
#include <Inventor/elements/SoGLShapeHintsElement.h>
#include <Inventor/elements/SoGLShadeModelElement.h>
#include <Inventor/elements/SoGLLightModelElement.h>
#include <Inventor/elements/SoGLNormalizeElement.h>

#include <Inventor/misc/SoGL.h>
#include <Inventor/bundles/SoMaterialBundle.h>
#include <Inventor/SbTesselator.h>
#include <Inventor/details/SoFaceDetail.h>
#include <Inventor/details/SoLineDetail.h>
#include <Inventor/SoPickedPoint.h>
#include <Inventor/actions/SoGetPrimitiveCountAction.h>
#include <Inventor/elements/SoGLTextureEnabledElement.h>
#include <Inventor/elements/SoGLTextureImageElement.h>

#ifdef _WIN32
#include <windows.h>
#endif // _WIN32
#include <GL/gl.h>
#include <string.h>

#include <Inventor/elements/SoComplexityElement.h>

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
    // this is a trick. Only one of these will be used, and the
    // other one is an illegal cast.
    this->faceDetail = (SoFaceDetail *)detail;
    this->lineDetail = (SoLineDetail *)detail;
    this->counter = 0;
  }
  void endShape() {
    if (this->shapetype == SoShape::POLYGON) {
      if (SoShapeHintsElement::getFaceType(action->getState()) ==
          SoShapeHintsElement::CONVEX) {
        for (int i = 1; i < counter-1; i++) {
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
        this->shape->invokeTriangleCallbacks(this->action,
                                             &vertsArray[0],
                                             &vertsArray[1],
                                             &vertsArray[2]);
      }
      break;
    case SoShape::TRIANGLES:
      this->setVertex(counter++, v);
      if (this->counter == 3) {
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
        this->shape->invokeLineSegmentCallbacks(this->action,
                                                &vertsArray[0],
                                                &vertsArray[1]);
        this->counter = 0;
      }
      break;
    case SoShape::LINE_STRIP:
      this->setVertex(this->counter++, v);
      if (this->counter == 2) {
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

  void doTriangleCB(const int i0, const int i1, const int i2) {
    if (this->faceDetail) {
      this->faceDetail->setNumPoints(3);
      this->faceDetail->setPoint(0, &this->pointDetails[i0]);
      this->faceDetail->setPoint(1, &this->pointDetails[i0]);
      this->faceDetail->setPoint(2, &this->pointDetails[i0]);
      this->vertsArray[i0].setDetail(this->faceDetail);
      this->vertsArray[i1].setDetail(this->faceDetail);
      this->vertsArray[i2].setDetail(this->faceDetail);
    }
    this->shape->invokeTriangleCallbacks(this->action,
                                         &this->vertsArray[i0],
                                         &this->vertsArray[i1],
                                         &this->vertsArray[i2]);
  }
  void doLineSegmentCB(const int i0, const int i1) {
    if (this->lineDetail) {
      this->lineDetail->setPoint0(&this->pointDetails[i0]);
      this->lineDetail->setPoint1(&this->pointDetails[i1]);
      this->vertsArray[i0].setDetail(this->lineDetail);
      this->vertsArray[i1].setDetail(this->lineDetail);
    }
    this->shape->invokeLineSegmentCallbacks(this->action,
                                            &this->vertsArray[i0],
                                            &this->vertsArray[i1]);
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

  // FIXME: detect and heed SoClipPlane's element settings.
  // 19990421 mortene.

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
  const SbViewVolume & vv = SoViewVolumeElement::get(state);
  SbBox3f worldBox(boundingbox);
  worldBox.transform(SoModelMatrixElement::get(state));
  SbVec2f normSize = vv.projectBox(worldBox);
  const SbViewportRegion & vr = SoViewportRegionElement::get(state);
  SbVec2s pixelSize = vr.getViewportSizePixels();

  // make sure short doesn't overflow
  rectsize[0] = (short) SbMin(32767.0f, (float(pixelSize[0])* normSize[0]));
  rectsize[1] = (short) SbMin(32767.0f, (float(pixelSize[1])* normSize[1]));
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
      // FIXME: needs calibration.

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

/*!
  \internal
*/
SbBool
SoShape::shouldGLRender(SoGLRenderAction * action)
{
  if (action->getCurPathCode() == SoAction::OFF_PATH &&
      !this->affectsState()) return FALSE;

  // make sure lazy elements are up to date
  // all material lazy elements are handled by SoMaterialBundle

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

  {
    const SoGLPointSizeElement * ps = (SoGLPointSizeElement *)
      state->getConstElement(SoGLPointSizeElement::getClassStackIndex());
    ps->evaluate();
  }

  {
    const SoGLLineWidthElement * lw = (SoGLLineWidthElement *)
      state->getConstElement(SoGLLineWidthElement::getClassStackIndex());
    lw->evaluate();
  }

  {
    const SoGLPolygonOffsetElement * off = (SoGLPolygonOffsetElement *)
      state->getConstElement(SoGLPolygonOffsetElement::getClassStackIndex());
    off->evaluate();
  }

  if (!this->willSetShadeModel()) {
    const SoGLShadeModelElement * sm = (SoGLShadeModelElement *)
      state->getConstElement(SoGLShadeModelElement::getClassStackIndex());
    sm->evaluate();
  }

  {
    const SoGLLightModelElement * lm = (SoGLLightModelElement *)
      state->getConstElement(SoGLLightModelElement::getClassStackIndex());
    lm->evaluate();
  }

  // only evaluate texture image if texturing is enabled
  if (SoGLTextureEnabledElement::get(state)) {
    const SoGLTextureImageElement * ti = (SoGLTextureImageElement *)
      state->getConstElement(SoGLTextureImageElement::getClassStackIndex());
    ti->evaluate();
  }

  {
    const SoGLTextureEnabledElement * te = (SoGLTextureEnabledElement *)
      state->getConstElement(SoGLTextureEnabledElement::getClassStackIndex());
    te->evaluate();
  }

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

    if (needNormals) {
      const SoGLNormalizeElement * ne = (SoGLNormalizeElement *)
        state->getConstElement(SoGLNormalizeElement::getClassStackIndex());
      ne->forceSend(TRUE); // cube will provide unit normals
    }

    glPushMatrix();
    glTranslatef(center[0], center[1], center[2]);
    sogl_render_cube(size[0], size[1], size[2], &mb,
                     SOGL_NEED_NORMALS | SOGL_NEED_TEXCOORDS);
    glPopMatrix();
    return FALSE;
  }

  if (!willSetShapeHints()) {
    const SoGLShapeHintsElement * sh = (SoGLShapeHintsElement *)
      state->getConstElement(SoGLShapeHintsElement::getClassStackIndex());
    sh->evaluate();
  }

  if (needNormals && !this->willUpdateNormalizeElement(state)) {
    const SoGLNormalizeElement * ne = (SoGLNormalizeElement *)
      state->getConstElement(SoGLNormalizeElement::getClassStackIndex());
    ne->evaluate();
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
  // FIXME: turn on backface culling
}

/*!
  \internal
*/
void
SoShape::endSolidShape(SoGLRenderAction * action)
{
  // FIXME: disable backface culling
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
  Should be overloaded by shapes that will set the ShadeModel before
  rendering. SoGLShadeModelElement is a lazy GL element, and it helps
  performance if nodes that will set the shade model before rendering
  returns \c TRUE here.  Ordinary shape nodes will not have worry
  about this method as the default method returns \c FALSE.

  This method is not a part of the original Open Inventor API.  Don't
  overload it if you intend to make a node that will work with both
  Coin and Open Inventor.
*/
SbBool
SoShape::willSetShadeModel(void) const
{
  return FALSE;
}

/*!
  Should be overloaded by subclasses that will set shape hints before
  rendering. The SoGLShapeHintsElement is a lazy element, and it will
  help performance if all nodes which will set the shape hints before
  rendering returns \c TRUE here.

  This method is not a part of the original Open Inventor API. Don't
  overload it if you intend to make a shape node that will work with
  both Coin and Open Inventor.
*/
SbBool
SoShape::willSetShapeHints(void) const
{
  return FALSE;
}

/*!
  Should be overloaded by subclasses that handles normals in a
  non-default way (not using the SoNormal node). The node is then
  responsible of handling this element before rendering.  Default
  method returns \c FALSE.

  This method is not a part of the original Open Inventor API. Don't
  overload it if you intend to make a shape node that will work with
  both Coin and Open Inventor.
*/
SbBool
SoShape::willUpdateNormalizeElement(SoState *) const
{
  return FALSE;
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
  \internal
 */
float
SoShape::getDecimatedComplexity(SoState * state, float complexity)
{
  COIN_STUB();
  // FIXME: get real value from decimation state element. 20000312 mortene.
  return 1.0f * complexity;
}

/*!
  \internal
 */
void
SoShape::GLRenderBoundingBox(SoGLRenderAction * action)
{
  COIN_STUB(); // FIXME
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


/**************************************************************************\
 *
 *  Copyright (C) 1998-1999 by Systems in Motion.  All rights reserved.
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
  \brief The SoShape class ...
  \ingroup nodes

  FIXME: write class doc
*/



#include <Inventor/misc/SoState.h>
#include <Inventor/SbBox2f.h>
#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG
#include <Inventor/SoPrimitiveVertex.h>
#include <Inventor/nodes/SoShape.h>

#if !defined(COIN_EXCLUDE_SORAYPICKACTION)
#include <Inventor/actions/SoRayPickAction.h>
#endif // ! COIN_EXCLUDE_SORAYPICKACTION
#if !defined(COIN_EXCLUDE_SOGETBOUNDINGBOXACTION)
#include <Inventor/actions/SoGetBoundingBoxAction.h>
#endif // !COIN_EXCLUDE_SOGETBOUNDINGBOXACTION
#if !defined(COIN_EXCLUDE_SOCALLBACKACTION)
#include <Inventor/actions/SoCallbackAction.h>
#endif // !COIN_EXCLUDE_SOCALLBACKACTION
#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
#include <Inventor/actions/SoGLRenderAction.h>
#endif // !COIN_EXCLUDE_SOGLRENDERACTION
#if !defined(COIN_EXCLUDE_SOTEXTURECOORDINATEELEMENT)
#include <Inventor/elements/SoTextureCoordinateElement.h>
#endif // !COIN_EXCLUDE_SOTEXTURECOORDINATEELEMENT
#if !defined(COIN_EXCLUDE_SOTRANSPARENCYELEMENT)
#include <Inventor/elements/SoTransparencyElement.h>
#endif // ! COIN_EXCLUDE_SOTRANSPARENCYELEMENT
#if !defined(COIN_EXCLUDE_SODRAWSTYLEELEMENT)
#include <Inventor/elements/SoDrawStyleElement.h>
#endif // ! COIN_EXCLUDE_SODRAWSTYLEELEMENT
#if !defined(COIN_EXCLUDE_SOMODELMATRIXELEMENT)
#include <Inventor/elements/SoModelMatrixElement.h>
#endif // ! COIN_EXCLUDE_SOMODELMATRIXELEMENT
#if !defined(COIN_EXCLUDE_SOVIEWVOLUMEELEMENT)
#include <Inventor/elements/SoViewVolumeElement.h>
#endif // !COIN_EXCLUDE_SOVIEWVOLUMEELEMENT
#if !defined(COIN_EXCLUDE_SOVIEWCOLUMEELEMENT)
#include <Inventor/elements/SoViewportRegionElement.h>
#endif // !COIN_EXCLUDE_SOVIEWPORTREGIONELEMENT
#if !defined(COIN_EXCLUDE_SOLIGHTMODELELEMENT)
#include <Inventor/elements/SoLightModelElement.h>
#endif // ! COIN_EXCLUDE_SOLIGHTMODELELEMENT

// Lazy GL-elements not handled by SoMaterialBundle
#if !defined(COIN_EXCLUDE_SOGLPOINTSIZEELEMENT)
#include <Inventor/elements/SoGLPointSizeElement.h>
#endif // !COIN_EXCLUDE_SOGLPOINTSIZEELEMENT
#if !defined(COIN_EXCLUDE_SOGLLINEWIDTHELEMENT)
#include <Inventor/elements/SoGLLineWidthElement.h>
#endif // !COIN_EXCLUDE_SOGLLINEWIDTHELEMENT
#if !defined(COIN_EXCLUDE_SOGLPOLYGONOFFSETELEMENT)
#include <Inventor/elements/SoGLPolygonOffsetElement.h>
#endif // !COIN_EXCLUDE_SOGLPOLYGONOFFSETELEMENT
#if !defined(COIN_EXCLUDE_SOGLSHAPEHINTSELEMENT)
#include <Inventor/elements/SoGLShapeHintsElement.h>
#endif // !COIN_EXCLUDE_SOGLSHAPEHINTSELEMENT
#if !defined(COIN_EXCLUDE_SOGLSHADEMODELELEMENT)
#include <Inventor/elements/SoGLShadeModelElement.h>
#endif // !COIN_EXCLUDE_SOGLSHADEMODELELEMENT
#if !defined(COIN_EXCLUDE_SOGLLIGHTMODELELEMENT)
#include <Inventor/elements/SoGLLightModelElement.h>
#endif // !COIN_EXCLUDE_SOGLLIGHTMODELELEMENT
#if !defined(COIN_EXCLUDE_SOGLNORMALIZEELELEMENT)
#include <Inventor/elements/SoGLNormalizeElement.h>
#endif // !COIN_EXCLUDE_SOGLNORMALIZEELEMENT


#include <Inventor/misc/SoGL.h>
#include <Inventor/bundles/SoMaterialBundle.h>
#include <Inventor/SbTesselator.h>
#include <Inventor/details/SoFaceDetail.h>
#include <Inventor/details/SoPointDetail.h>
#include <Inventor/details/SoLineDetail.h>
#include <Inventor/SoPickedPoint.h>
#include <Inventor/actions/SoGetPrimitiveCountAction.h>
#include <Inventor/elements/SoGLTextureEnabledElement.h>


#ifdef _WIN32
#include <windows.h>
#endif // !_WIN32

#include <GL/gl.h>
#include <string.h>
#include <math.h>

#if !defined(COIN_EXCLUDE_SOCOMPLEXITYTYPEELEMENT)
#include <Inventor/elements/SoComplexityTypeElement.h>
#endif // !COIN_EXCLUDE_SOCOMPLEXITYTYPEELEMENT
#if !defined(COIN_EXCLUDE_SOCOMPLEXITYELEMENT)
#include <Inventor/elements/SoComplexityElement.h>
#endif // !COIN_EXCLUDE_SOCOMPLEXITYTYPEELEMENT

/*!
  \enum SoShape::TriangleShape
  FIXME: write documentation for enum
*/
/*!
  \var SoShape::TriangleShape SoShape::TRIANGLE_STRIP
  FIXME: write documentation for enum definition
*/
/*!
  \var SoShape::TriangleShape SoShape::TRIANGLE_FAN
  FIXME: write documentation for enum definition
*/
/*!
  \var SoShape::TriangleShape SoShape::TRIANGLES
  FIXME: write documentation for enum definition
*/
/*!
  \var SoShape::TriangleShape SoShape::POLYGON
  FIXME: write documentation for enum definition
*/


/*!
  \fn void SoShape::computeBBox(SoAction * action, SbBox3f & box, SbVec3f & center)
  FIXME: write doc
*/
/*!
  \fn void SoShape::generatePrimitives(SoAction * action)
  FIXME: write doc
*/

// *************************************************************************
SO_NODE_ABSTRACT_SOURCE(SoShape);

/*!
  Constructor.
*/
SoShape::SoShape()
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoShape);
}

/*!
  Destructor.
*/
SoShape::~SoShape()
{
}

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

  void beginShape(SoShape *shape, SoAction *action,
                  SoShape::TriangleShape shapeType,
                  SoDetail *detail) {
    this->shape = shape;
    this->action = action;
    this->shapeType = shapeType;
    // this is a trick. Only onw of these will be used, and the
    // other one is an illegal cast.
    this->faceDetail = (SoFaceDetail*)detail;
    this->lineDetail = (SoLineDetail*)detail;
    this->counter = 0;
  }
  void endShape() {
    if (this->shapeType == SoShape::POLYGON) {
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
    switch (shapeType) {
    case SoShape::TRIANGLE_STRIP:
      if (this->counter >= 3) {
        if (this->counter & 1) this->copyVertex(2, 0);
        else this->copyVertex(2, 1);
      }
      this->setVertex(this->counter%3, v);
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
        SoPrimitiveVertex *newArray = new SoPrimitiveVertex[this->arraySize];
        memcpy(newArray, this->vertsArray,
               sizeof(SoPrimitiveVertex)*this->counter);
        delete [] this->vertsArray;
        this->vertsArray = newArray;

        SoPointDetail *newparray = new SoPointDetail[this->arraySize];
        memcpy(newparray, this->pointDetails,
               sizeof(SoPointDetail)*this->counter);
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
  SoShape::TriangleShape shapeType;
  SoAction *action;
  SoShape *shape;
  SoPrimitiveVertex *vertsArray;
  SoPointDetail *pointDetails;
  SoFaceDetail *faceDetail;
  SoLineDetail *lineDetail;
  int arraySize;
  int counter;
  SbTesselator *tess;
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
      SoPointDetail *pd = (SoPointDetail*)v->getDetail();
      assert(pd);
      this->pointDetails[idx] = *pd;
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

  SoDetail *createPickDetail() {
    switch(this->shapeType) {
    case SoShape::TRIANGLE_STRIP:
    case SoShape::TRIANGLE_FAN:
    case SoShape::TRIANGLES:
      {
        SoFaceDetail *detail = (SoFaceDetail*)this->faceDetail->copy();
        detail->setNumPoints(3);
        detail->setPoint(0, &this->pointDetails[0]);
        detail->setPoint(1, &this->pointDetails[1]);
        detail->setPoint(2, &this->pointDetails[2]);
        return detail;
      }
    case SoShape::POLYGON:
      {
        SoFaceDetail *detail = (SoFaceDetail*)this->faceDetail->copy();
        detail->setNumPoints(this->counter);
        for (int i = 0; i < this->counter; i++) {
          detail->setPoint(i, &this->pointDetails[i]);
        }
        return detail;
      }
    case SoShape::QUADS:
    case SoShape::QUAD_STRIP:
      {
        SoFaceDetail *detail = (SoFaceDetail*)this->faceDetail->copy();
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
        SoLineDetail *detail = (SoLineDetail*)this->lineDetail->copy();
        detail->setPoint0(&this->pointDetails[0]);
        detail->setPoint1(&this->pointDetails[1]);
        return detail;
      }
    default:
      assert(0 && "unknown shape type");
      return NULL;
    }
  }

  static void tess_callback(void *v0, void *v1, void *v2, void *data) {
    shapePrimitiveData *thisp = (shapePrimitiveData*) data;
    thisp->shape->invokeTriangleCallbacks(thisp->action,
                                          (SoPrimitiveVertex*)v0,
                                          (SoPrimitiveVertex*)v1,
                                          (SoPrimitiveVertex*)v2);
  }
};

static shapePrimitiveData *primData = NULL;

/*!
  Does initialization common for all objects of the
  SoShape class. This includes setting up the
  type system, among other things.
*/
void
SoShape::initClass(void)
{
  SO_NODE_INTERNAL_INIT_ABSTRACT_CLASS(SoShape);
}


#if !defined(COIN_EXCLUDE_SOGETBOUNDINGBOXACTION)
/*!
  Calls the virtual function SoShape::computeBBox() to handle the
  bounding box action. A SoShape subclass should usually reimplement
  computeBBox().
*/
void
SoShape::getBoundingBox(SoGetBoundingBoxAction * action)
{
  SbBox3f box;
  SbVec3f center;
  this->computeBBox(action, box, center);

  // FIXME: detect and heed SoClipPlane's element settings.
  // 19990421 mortene.

  action->extendBy(box);
  action->setCenter(center, TRUE);
}
#endif // !COIN_EXCLUDE_SOGETBOUNDINGBOXACTION

#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
/*!
  Renders the primitives generated by subclass. Should be
  overloaded.
*/
void
SoShape::GLRender(SoGLRenderAction * /* action */)
{
#if COIN_DEBUG
  SoDebugError::postInfo("SoShape::GLRender",
                         "method has not been overloaded in subclass '%s'",
                         this->getTypeId().getName().getString());
#endif // COIN_DEBUG
}
#endif // !COIN_EXCLUDE_SOGLRENDERACTION

#if !defined(COIN_EXCLUDE_SOCALLBACKACTION)
/*!
  Calls generatePrimitives().
*/
void
SoShape::callback(SoCallbackAction * action)
{
  if (action->shouldGeneratePrimitives(this)) {
    if (primData) primData->faceCounter = 0;
    this->generatePrimitives(action);
  }
}
#endif // !COIN_EXCLUDE_SOCALLBACKACTION

#if !defined(COIN_EXCLUDE_SORAYPICKACTION)
/*!
  Calculates picked point based on primitives generated by subclasses.
  May be implemented by subclass.
*/
void
SoShape::rayPick(SoRayPickAction *action)
{
  if (this->shouldRayPick(action)) {
    this->computeObjectSpaceRay(action);
    this->generatePrimitives(action);
  }
}
#endif // !COIN_EXCLUDE_SORAYPICKACTION

/*!
  A convenience function that returns the size of a bounding box
  projected onto the screen. Useful for SCREEN_SPACE complexity
  geometry.
*/
void
SoShape::getScreenSize(SoState * const state,
                       const SbBox3f &boundingBox,
                       SbVec2s &rectSize)
{
  const SbViewVolume &vv = SoViewVolumeElement::get(state);
  SbBox3f worldBox(boundingBox);
  worldBox.transform(SoModelMatrixElement::get(state));
  SbVec2f normSize = vv.projectBox(worldBox);
  const SbViewportRegion &vr = SoViewportRegionElement::get(state);
  SbVec2s pixelSize = vr.getViewportSizePixels();

  // make sure short doesn't overflow
  rectSize[0] = (short) SbMin(32767.0f, (float(pixelSize[0])*normSize[0]));
  rectSize[1] = (short) SbMin(32767.0f, (float(pixelSize[1])*normSize[1]));
}

/*!
  Returns the complexity value to be used by subclasses. Considers
  complexity type. For OBJECT_SPACE complexity this will be a number
  between 0 and 1. For SCREEN_SPACE complexity it is a number from 0
  and up.
*/
float
SoShape::getComplexityValue(SoAction *action)
{
  SoState *state = action->getState();
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
      return sqrt(SbMax(size[0], size[1])) * 0.4f *
        SoComplexityElement::get(state);
#else // first version
      float numPixels = float(size[0])*float(size[1]);
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


#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
/*!
  FIXME: write function documentation
*/
SbBool
SoShape::shouldGLRender(SoGLRenderAction * action)
{
  if (action->getCurPathCode() == SoAction::OFF_PATH &&
      !this->affectsState()) return FALSE;

  // make sure lazy elements are up to date
  // all material lazy elements are handled by SoMaterialBundle

  SoState * state = action->getState();

  SbBool needNormals = TRUE;
#if !defined(COIN_EXCLUDE_SOLIGHTMODELELEMENT)
  needNormals =
    SoLightModelElement::get(state) == SoLightModelElement::PHONG;
#endif // ! COIN_EXCLUDE_SOLIGHTMODELELEMENT

#if !defined(COIN_EXCLUDE_SOTRANSPARENCYELEMENT)
  const SoTransparencyElement * trans =
    SoTransparencyElement::getInstance(state);
  SbBool t = trans->getNum() && trans->get(0) > 0.0f;

  if (action->handleTransparency(t)) return FALSE;
#endif // ! COIN_EXCLUDE_SOTRANSPARENCYELEMENT

#if !defined(COIN_EXCLUDE_SODRAWSTYLEELEMENT)
  if (SoDrawStyleElement::get(state) == SoDrawStyleElement::INVISIBLE)
    return FALSE;
#endif

#if !defined(COIN_EXCLUDE_SOGLPOINTSIZEELEMENT)
  {
    const SoGLPointSizeElement * ps = (SoGLPointSizeElement *)
      state->getConstElement(SoGLPointSizeElement::getClassStackIndex());
    ps->evaluate();
  }
#endif // !COIN_EXCLUDE_SOGLPOINTSIZEELEMENT

#if !defined(COIN_EXCLUDE_SOGLLINEWIDTHELEMENT)
  {
    const SoGLLineWidthElement * lw = (SoGLLineWidthElement *)
      state->getConstElement(SoGLLineWidthElement::getClassStackIndex());
    lw->evaluate();
  }
#endif // !COIN_EXCLUDE_SOGLLINEWIDTHELEMENT

#if !defined(COIN_EXCLUDE_SOGLPOLYGONOFFSETELEMENT)
  {
    const SoGLPolygonOffsetElement * off = (SoGLPolygonOffsetElement *)
      state->getConstElement(SoGLPolygonOffsetElement::getClassStackIndex());
    off->evaluate();
  }
#endif // !COIN_EXCLUDE_SOGLPOLYGONOFFSETELEMENT

#if !defined(COIN_EXCLUDE_SOGLSHADEMODELELEMENT)
  if (!this->willSetShadeModel()) {
    const SoGLShadeModelElement * sm = (SoGLShadeModelElement *)
      state->getConstElement(SoGLShadeModelElement::getClassStackIndex());
    sm->evaluate();
  }
#endif

#if !defined(COIN_EXCLUDE_SOGLLIGHTMODELELEMENT)
  {
    const SoGLLightModelElement * lm = (SoGLLightModelElement *)
      state->getConstElement(SoGLLightModelElement::getClassStackIndex());
    lm->evaluate();
  }
#endif // !COIN_EXCLUDE_SOGLLIGHTMODELELEMENT

#if !defined(COIN_EXCLUDE_SOGLTEXTUREENABLESELEMENT)
  {
    const SoGLTextureEnabledElement * te = (SoGLTextureEnabledElement *)
      state->getConstElement(SoGLTextureEnabledElement::getClassStackIndex());
    te->evaluate();
  }
#endif // !COIN_EXCLUDE_SOGLTEXTUREENABLEDELEMENT

#if !defined(COIN_EXCLUDE_SOCOMPLEXITYTYPEELEMENT)

  if (SoComplexityTypeElement::get(state) ==
      SoComplexityTypeElement::BOUNDING_BOX) {

    SbBox3f box;
    SbVec3f center;
    this->computeBBox(action, box, center);
    center = (box.getMin() + box.getMax()) * 0.5f;
    SbVec3f size = box.getMax()  - box.getMin();

    SoMaterialBundle mb(action);
    mb.sendFirst();

#if !defined(COIN_EXCLUDE_SOGLSHAPEHINTSELEMENT)
    {
      const SoGLShapeHintsElement * sh = (SoGLShapeHintsElement *)
        state->getConstElement(SoGLShapeHintsElement::getClassStackIndex());
      sh->forceSend(TRUE, FALSE, FALSE);
    }
#endif // ! COIN_EXCLUDE_SOGLSHAPEHINTSELEMENT

#if !defined(COIN_EXCLUDE_SOGLNORMALIZEELEMENT)
    if (needNormals) {
      const SoGLNormalizeElement * ne = (SoGLNormalizeElement *)
        state->getConstElement(SoGLNormalizeElement::getClassStackIndex());
      ne->forceSend(TRUE); // cube will provide unit normals
    }
#endif // !COIN_EXCLUDE_SOGLLIGHTMODELELEMENT

    glPushMatrix();
    glTranslatef(center[0], center[1], center[2]);
    sogl_render_cube(size[0], size[1], size[2], &mb,
                     SOGL_NEED_NORMALS | SOGL_NEED_TEXCOORDS);
    glPopMatrix();
    return FALSE;
  }

#endif // ! COIN_EXCLUDE_SOCOMPLEXITYTYPEELEMENT


#if !defined(COIN_EXCLUDE_SOGLSHAPEHINTSELEMENT)
  if (!willSetShapeHints()) {
    const SoGLShapeHintsElement * sh = (SoGLShapeHintsElement *)
      state->getConstElement(SoGLShapeHintsElement::getClassStackIndex());
    sh->evaluate();
  }
#endif

#if !defined(COIN_EXCLUDE_SOGLNORMALIZEELEMENT)
  if (needNormals && !this->willUpdateNormalizeElement(state)) {
    const SoGLNormalizeElement * ne = (SoGLNormalizeElement *)
      state->getConstElement(SoGLNormalizeElement::getClassStackIndex());
    ne->evaluate();
  }
#endif // !COIN_EXCLUDE_SOGLLIGHTMODELELEMENT

  return TRUE;
}
#endif // !COIN_EXCLUDE_SOGLRENDERACTION

#if !defined(COIN_EXCLUDE_SORAYPICKACTION)
/*!
  FIXME: write function documentation
*/
SbBool
SoShape::shouldRayPick(SoRayPickAction * const /* action */)
{
  // FIXME: test bbox, create rayPick cache, or something?
  return TRUE;
}
#endif // !COIN_EXCLUDE_SORAYPICKACTION

#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
/*!
  FIXME: write function documentation
*/
void
SoShape::beginSolidShape(SoGLRenderAction * /* action */)
{
  // FIXME: turn on backface culling
}

/*!
  FIXME: write function documentation
*/
void
SoShape::endSolidShape(SoGLRenderAction * /* action */)
{
  // FIXME: disable backface culling
}
#endif // !COIN_EXCLUDE_SOGLRENDERACTION

#if !defined(COIN_EXCLUDE_SORAYPICKACTION)
/*!
  FIXME: write function documentation
*/
void
SoShape::computeObjectSpaceRay(SoRayPickAction * const action)
{
  action->setObjectSpace();
}

/*!
  FIXME: write function documentation
*/
void
SoShape::computeObjectSpaceRay(SoRayPickAction * const action,
                               const SbMatrix &matrix)
{
  action->setObjectSpace(matrix);
}

/*!
  Will create triangle detail for a SoPickedPoint. This method will only be
  called internally, when generatePrimitives() is used for picking
  (SoShape::rayPick() is not overloaded).

  This method returns NULL in OIV, and subclasses will need to overload
  this method to create details for a SoPickedPoint.

  This is not necessary with Coin. Of course, if you choose to overload
  it, it will work in the same way as OIV.

  For this to work, you must supply a face or line detail when generating
  primitives. If you supply NULL for the detail argument in SoShape::beginShape(),
  you'll have to overload this method.
*/
SoDetail *
SoShape::createTriangleDetail(SoRayPickAction *action,
                              const SoPrimitiveVertex */*v1*/,
                              const SoPrimitiveVertex */*v2*/,
                              const SoPrimitiveVertex */*v3*/,
                              SoPickedPoint *pp)
{
  assert(primData);
  assert(primData->faceDetail);
  assert(primData->shape == this);
  assert(primData->action == (SoAction*) action);

  return primData->createPickDetail();
}

/*!
  Will create line detail for a SoPickedPoint. This method will only be
  called internally, when generatePrimitives() is used for picking
  (SoShape::rayPick() is not overloaded).

  This method returns NULL in OIV, and OIV shape subclasses will need to overload
  this method to create details for a SoPickedPoint.

  This is not necessary with Coin. Of course, if you choose to overload
  it, it will work in the same way as OIV..

  For this to work, you must supply a face or line detail when generating
  primitives. If you supply NULL for the detail argument in SoShape::beginShape(),
  you'll have to overload this method.
*/
SoDetail *
SoShape::createLineSegmentDetail(SoRayPickAction *action,
                                 const SoPrimitiveVertex * /* v1 */,
                                 const SoPrimitiveVertex * /* v2 */,
                                 SoPickedPoint *pp)
{
  assert(primData);
  assert(primData->lineDetail);
  assert(primData->shape == this);
  assert(primData->action == (SoAction*) action);

  return primData->createPickDetail();
}

/*!
  Will create point detail for a SoPickedPoint. This method will only be
  called internally, when generatePrimitives() is used for picking
  (SoShape::rayPick() is not overloaded).

  This method returns NULL in OIV, and OIV shape subclasses will need to overload
  this method to create details for a SoPickedPoint.

  This is not necessary with Coin. Of course, if you choose to overload
  it, it will work in the same way as OIV.

  For this to work, you must supply a point detail in the SoPrimitiveVertex
  in generatePrimitives().
*/
SoDetail *
SoShape::createPointDetail(SoRayPickAction * /* action */,
                           const SoPrimitiveVertex *v,
                           SoPickedPoint * /* pp */)
{
  if (v->getDetail()) return v->getDetail()->copy();
  return NULL;
}
#endif // !COIN_EXCLUDE_SORAYPICKACTION

#if !defined(COIN_EXCLUDE_SOACTION)
/*!
  FIXME: write function documentation
*/
void
SoShape::invokeTriangleCallbacks(SoAction * const action,
                                 const SoPrimitiveVertex * const v1,
                                 const SoPrimitiveVertex * const v2,
                                 const SoPrimitiveVertex * const v3)
{
  if (action->getTypeId().isDerivedFrom(SoRayPickAction::getClassTypeId())) {
    SoRayPickAction *ra = (SoRayPickAction*) action;

    SbVec3f intersection;
    SbVec3f barycentric;
    SbBool front;

    if (ra->intersect(v1->getPoint(), v2->getPoint(), v3->getPoint(),
                      intersection, barycentric, front)) {

      if (ra->isBetweenPlanes(intersection)) {
        SoPickedPoint * pp = ra->addIntersection(intersection);
        if (pp) {
          pp->setDetail(this->createTriangleDetail(ra, v1, v2, v3, pp), this);
        }
      }
    }
  }
  else if (action->getTypeId().isDerivedFrom(SoCallbackAction::getClassTypeId())) {
    SoCallbackAction *ca = (SoCallbackAction*) action;
    ca->invokeTriangleCallbacks(this, v1, v2, v3);
  }
  else if (action->getTypeId().isDerivedFrom(SoGetPrimitiveCountAction::getClassTypeId())) {
    SoGetPrimitiveCountAction *ga = (SoGetPrimitiveCountAction*) action;
    ga->incNumTriangles();
  }
  else {
    assert(0 && "FIXME: not implemented");
  }
}

/*!
  FIXME: write function documentation
*/
void
SoShape::invokeLineSegmentCallbacks(SoAction * const action,
                                    const SoPrimitiveVertex * const v1,
                                    const SoPrimitiveVertex * const v2)
{
  if (action->getTypeId().isDerivedFrom(SoRayPickAction::getClassTypeId())) {
    SoRayPickAction *ra = (SoRayPickAction*) action;

    SbVec3f intersection;
    if (ra->intersect(v1->getPoint(), v2->getPoint(), intersection)) {
      if (ra->isBetweenPlanes(intersection)) {
        SoPickedPoint * pp = ra->addIntersection(intersection);
        if (pp) {
          pp->setDetail(this->createLineSegmentDetail(ra, v1, v2, pp), this);
        }
      }
    }
  }
  else if (action->getTypeId().isDerivedFrom(SoCallbackAction::getClassTypeId())) {
    SoCallbackAction *ca = (SoCallbackAction*) action;
    ca->invokeLineSegmentCallbacks(this, v1, v2);
  }
  else if (action->getTypeId().isDerivedFrom(SoGetPrimitiveCountAction::getClassTypeId())) {
    SoGetPrimitiveCountAction *ga = (SoGetPrimitiveCountAction*) action;
    ga->incNumLines();
  }
  else {
    assert(0 && "FIXME: not implemented");
  }

}

/*!
  FIXME: write function documentation
*/
void
SoShape::invokePointCallbacks(SoAction * const action,
                              const SoPrimitiveVertex * const v)
{
  if (action->getTypeId().isDerivedFrom(SoRayPickAction::getClassTypeId())) {
    SoRayPickAction *ra = (SoRayPickAction*) action;

    SbVec3f intersection = v->getPoint();
    if (ra->intersect(intersection)) {
      if (ra->isBetweenPlanes(intersection)) {
        SoPickedPoint * pp = ra->addIntersection(intersection);
        if (pp) {
          pp->setDetail(this->createPointDetail(ra, v, pp), this);
        }
      }
    }
  }
  else if (action->getTypeId().isDerivedFrom(SoCallbackAction::getClassTypeId())) {
    SoCallbackAction *ca = (SoCallbackAction*) action;
    ca->invokePointCallbacks(this, v);
  }
  else if (action->getTypeId().isDerivedFrom(SoGetPrimitiveCountAction::getClassTypeId())) {
    SoGetPrimitiveCountAction *ga = (SoGetPrimitiveCountAction*) action;
    ga->incNumPoints();
  }
  else {
    assert(0 && "FIXME: not implemented");
  }
}

/*!
  This method is slightly different from its OIV counterpart, as this method
  has an SoDetail as the last argument, and not an SoFaceDetail. This is
  because we accept more TriangleShape types, and the detail might be
  a SoFaceDetail or a SoLineDetail. There is no use sending in a
  SoPointDetail, as nothing will be done with it.
*/
void
SoShape::beginShape(SoAction * const action, const TriangleShape shapeType,
                    SoDetail * const detail)
{
  if (primData == NULL) {
    primData = new shapePrimitiveData();
  }
  primData->beginShape(this, action, shapeType, detail);
}


/*!
  FIXME: write function documentation
*/
void
SoShape::shapeVertex(const SoPrimitiveVertex * const v)
{
  assert(primData);
  primData->shapeVertex(v);
}

/*!
  FIXME: write function documentation
*/
void
SoShape::endShape()
{
  assert(primData);
  primData->endShape();
}
#endif // !COIN_EXCLUDE_SOACTION

#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
/*!
  Convenience function which sets up a SoPrimitiveVertex, and sends
  it using the SoShape::shapeVertex() function.
*/
void
SoShape::generateVertex(SoPrimitiveVertex * const pv,
                        const SbVec3f & point,
                        const SbBool useTexFunc,
                        const SoTextureCoordinateElement * const tce,
                        const float s,
                        const float t,
                        const SbVec3f & normal)
{
  SbVec4f texCoord;
#if !defined(COIN_EXCLUDE_SOTEXTURECOORDINATEELEMENT)
  if (useTexFunc)
    texCoord = tce->get(point, normal);
  else
#endif // !COIN_EXCLUDE_SOTEXTURECOORDINATEELEMENT
    texCoord.setValue(s, t, 0.0f, 1.0f);
  pv->setPoint(point);
  pv->setNormal(normal);
  pv->setTextureCoords(texCoord);
  shapeVertex(pv);
}
#endif // !COIN_EXCLUDE_SOGLRENDERACTION


/*!
  Should be overloaded by shapes that will set the ShadeModel
  before rendering. SoGLShadeModelElement is a lazy GL element,
  and it helps performance if nodes that will set the
  shade model before rendering returns \e TRUE here.
  Ordinary shape nodes will not have worry about this method
  as the default method returns \e FALSE.

  This method is not a part of the original OIV API.
  Don't overload it if you intend to make a node that will work
  on both Coin and Open Inventor.
*/
SbBool
SoShape::willSetShadeModel() const
{
  return FALSE;
}

/*!
  Should be overloaded by subclasses that will set
  shape hints before rendering. The SoGLShapeHintsElement
  is a lazy element, and it will help performance if all
  nodes which will set the shape hints before rendering
  returns \e TRUE here.

  This method is not a part of the original OIV API. Don't
  overload it if you intend to make a shape node that will work
  on both Coin and Open Inventor.
*/
SbBool
SoShape::willSetShapeHints() const
{
  return FALSE;
}

/*!
  Should be overloaded by subclasses that handles normals in
  a non-default way (not using the SoNormal node). The node is
  then responsible of handling this element before rendering.
  Default method returns FALSE.

  This method is not a part of the original OIV API. Don't
  overload it if you intend to make a shape node that will work
  on both Coin and Open Inventor.
*/
SbBool
SoShape::willUpdateNormalizeElement(SoState *) const
{
  return FALSE;
}


/*!
  FIXME: write doc
 */
SbBool
SoShape::affectsState(void) const
{
  // a standard shape node should not affect the state.
  return FALSE;
}

#if !defined(COIN_EXCLUDE_SOGETPRIMITIVECOUNTACTION)
/*!
  FIXME: write doc
 */
void
SoShape::getPrimitiveCount(SoGetPrimitiveCountAction *action)
{
  if (this->shouldPrimitiveCount(action)) {
    this->generatePrimitives(action);
  }
}
#endif // !COIN_EXCLUDE_SOGETPRIMITIVECOUNTACTION

/*!
  FIXME: write doc
 */
float
SoShape::getDecimatedComplexity(SoState * /* state */, float /* complexity */)
{
  assert(0 && "FIXME: not implemented");
  return 0.5f;
}

#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
/*!
  FIXME: write doc
 */
void
SoShape::GLRenderBoundingBox(SoGLRenderAction * /* action */)
{
  assert(0 && "FIXME: not implemented");
}
#endif // !COIN_EXCLUDE_SOGLRENDERACTION

#if !defined(COIN_EXCLUDE_SOGETPRIMITIVECOUNTACTION)
/*!
  FIXME: write doc
 */
SbBool
SoShape::shouldPrimitiveCount(SoGetPrimitiveCountAction * /* action */)
{
  return TRUE; // FIXME: what to do here? pederb 1999-11-25
}
#endif // !COIN_EXCLUDE_SOGETPRIMITIVECOUNTACTION

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
  \class SoGLRenderAction Inventor/actions/SoGLRenderAction.h
  \brief The SoGLRenderAction class is an action for rendering an Open Inventor
  scene graph with OpenGL.
*/

#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/lists/SoEnabledElementsList.h>
#include <Inventor/nodes/SoNode.h>
#include <Inventor/actions/SoSubActionP.h>
#include <Inventor/SbColor.h>
#include <Inventor/misc/SoState.h>
#include <Inventor/misc/SoGL.h>
#include <Inventor/SbPlane.h>
#include <Inventor/actions/SoGetBoundingBoxAction.h>
#include <coindefs.h>


#include <Inventor/elements/SoGLLightIdElement.h>
#include <Inventor/elements/SoGLRenderPassElement.h>
#include <Inventor/elements/SoGLUpdateAreaElement.h>
#include <Inventor/elements/SoDecimationTypeElement.h>
#include <Inventor/elements/SoDecimationPercentageElement.h>
#include <Inventor/elements/SoWindowElement.h>
#include <Inventor/elements/SoGLShadeModelElement.h>
#include <Inventor/elements/SoGLNormalizeElement.h>
#include <Inventor/elements/SoShapeStyleElement.h>
#include <Inventor/elements/SoOverrideElement.h>
#include <Inventor/elements/SoTextureOverrideElement.h>
#include <Inventor/elements/SoLazyElement.h>

#include <Inventor/elements/SoViewVolumeElement.h>
#include <Inventor/elements/SoModelMatrixElement.h>

// FIXME: see comment on SoGLViewportRegionElement::set in
// beginTraversel() below. 19990228 mortene.
#include <Inventor/elements/SoGLViewportRegionElement.h>
// FIXME: these should not be necessary here. tmp hack. 19990228 mortene.
#include <Inventor/elements/SoTextureCoordinateElement.h>

#ifdef _WIN32
#include <windows.h>
#endif // _WIN32

#include <GL/gl.h>

// *************************************************************************

/*!
  \fn SoGLRenderAction::TransparencyType

  FIXME: write doc.
*/

/*!
  \fn SoGLRenderAction::AbortCode

  FIXME: write doc.
*/

/*!
  \fn typedef SoGLRenderAction::SoGLRenderAbortCB(void * userData)

  FIXME: write doc.
*/

/*!
  \fn typedef SoGLRenderAction::SoGLRenderPassCB(void * userData)

  FIXME: write doc.
*/


SO_ACTION_SOURCE(SoGLRenderAction);

// Override from parent class.
void
SoGLRenderAction::initClass(void)
{
  SO_ACTION_INIT_CLASS(SoGLRenderAction, SoAction);

  SO_ENABLE(SoGLRenderAction, SoViewportRegionElement);
  SO_ENABLE(SoGLRenderAction, SoGLRenderPassElement);
  SO_ENABLE(SoGLRenderAction, SoDecimationTypeElement);
  SO_ENABLE(SoGLRenderAction, SoDecimationPercentageElement);
  SO_ENABLE(SoGLRenderAction, SoWindowElement);
  SO_ENABLE(SoGLRenderAction, SoGLLightIdElement);
  SO_ENABLE(SoGLRenderAction, SoGLUpdateAreaElement);

  SO_ENABLE(SoGLRenderAction, SoGLShadeModelElement);
  SO_ENABLE(SoGLRenderAction, SoGLNormalizeElement);

  SO_ENABLE(SoGLRenderAction, SoOverrideElement);
  SO_ENABLE(SoGLRenderAction, SoTextureOverrideElement);
  SO_ENABLE(SoGLRenderAction, SoLazyElement);

  // FIXME: see comment on SoGLViewportRegionElement::set in
  // beginTraversel() below. 19990228 mortene.
  SO_ENABLE(SoGLRenderAction, SoGLViewportRegionElement);
}

// *************************************************************************

/*!
  A constructor.
*/

SoGLRenderAction::SoGLRenderAction(const SbViewportRegion & viewportRegion)
{
  SO_ACTION_CONSTRUCTOR(SoGLRenderAction);

  SO_ACTION_ADD_METHOD_INTERNAL(SoNode, SoNode::GLRenderS);

  methods->setUp(); // FIXME: not sure if this should be called here...

  // Can't just push this on the SoViewportRegionElement stack, as the
  // state hasn't been made yet.
  this->viewport = viewportRegion;

  this->passCB = NULL;
  this->passCBdata = NULL;
  this->smoothing = FALSE;
  this->numPasses = 1;

  this->transType = SoGLRenderAction::DELAYED_BLEND;
  this->firstRender = TRUE;
  this->delayedRender = FALSE;
  this->sortRender = FALSE;
  this->isBlendEnabled = FALSE;
  this->bboxAction = NULL;
}

/*!
  The destructor.
*/

SoGLRenderAction::~SoGLRenderAction()
{
  delete this->bboxAction;
}

/*!
  This method sets the viewport region.
*/

void
SoGLRenderAction::setViewportRegion(const SbViewportRegion & newRegion)
{
  if (this->getState()) {
#if 0 // debug
    SoDebugError::postInfo("SoGLRenderAction::setViewportRegion",
                           "aspectratio: %f",
                           newRegion.getViewportAspectRatio());
#endif // debug
    SoViewportRegionElement::set(this->getState(), newRegion);
  }
  this->viewport = newRegion;
}

/*!
  This method gets the viewport region.
*/

const SbViewportRegion &
SoGLRenderAction::getViewportRegion(void) const
{
  return this->viewport;
}

/*!
  This methor sets the area to be updated.
*/

void
SoGLRenderAction::setUpdateArea(const SbVec2f & /* origin */,
                                const SbVec2f & /* size */)
{
  COIN_STUB();
}

/*!
  This method gets the area to be updated.
*/

void
SoGLRenderAction::getUpdateArea(SbVec2f & /* origin */,
                                SbVec2f & /* size */) const
{
  COIN_STUB();
}

/*!
  This method sets the abort callback.  The abort callback is called during
  traversal to check for abort conditions.
*/

void
SoGLRenderAction::setAbortCallback(SoGLRenderAbortCB * const func,
                                   void * const userData)
{
  this->abortCB = func;
  this->abortCBdata = userData;
}

/*!
  This method sets the transparency type.
*/

void
SoGLRenderAction::setTransparencyType(const TransparencyType type)
{
  this->transType = type;
}

/*!
  This method returns the transparency type.
*/

SoGLRenderAction::TransparencyType
SoGLRenderAction::getTransparencyType() const
{
  return this->transType;
}

/*!
  This method sets (or unsets) smoothing.
*/

void
SoGLRenderAction::setSmoothing(const SbBool smooth)
{
  this->smoothing = smooth;
}

/*!
  This method returns wether smoothing is set or not.
*/

SbBool
SoGLRenderAction::isSmoothing() const
{
  return this->smoothing;
}

/*!
  This method sets the number of passes.  Default is 1, anything greater will
  enable antialiasing.
*/

void
SoGLRenderAction::setNumPasses(const int num)
{
  this->numPasses = num;
}

/*!
  This method returns the number of passes set for rendering.
*/

int
SoGLRenderAction::getNumPasses() const
{
  return this->numPasses;
}

/*!
  This method sets wether intermediate results should be displayed during
  multipass antialiasing rendering.  Not in use.
*/

void
SoGLRenderAction::setPassUpdate(const SbBool flag)
{
  this->passUpdate = flag;
}

/*!
  This method returns wether the passUpdate flag is set or not.

  \sa void SoGLRenderAction::setPassUpdate(const SbBool flag)
*/

SbBool
SoGLRenderAction::isPassUpdate() const
{
  return this->passUpdate;
}

/*!
  This method sets the pass callback.  The callback is called between each
  pass.
*/

void
SoGLRenderAction::setPassCallback(SoGLRenderPassCB * const func,
                                  void * const userData)
{
  this->passCB = func;
  this->passCBdata = userData;
}

/*!
  This methos sets the OpenGL cache context key.
*/

void
SoGLRenderAction::setCacheContext(const uint32_t context)
{
  this->cacheContext = context;
}

/*!
  This method returns the cache context key.
*/

uint32_t
SoGLRenderAction::getCacheContext() const
{
  return this->cacheContext;
}

/*!
  This method is called before the action is applied.
*/

void
SoGLRenderAction::beginTraversal(SoNode * node)
{
  if (this->sortRender || this->delayedRender) {
    inherited::beginTraversal(node);
    return;
  }

  static int first = 1;
  if (first) {
    first = 0;
    // FIXME: this should eventually be unnecessary (move GL
    // implementation checking code to SoGL*Element classes). 19990314
    // mortene.
    sogl_global_init(); // find GL implementation dependant limits
  }

  // FIXME: shouldn't the GL initialization rather be done in
  // SoSceneManager::render()? Or perhaps in the individual elements
  // (partly)? 19990215 mortene.

  //
  // Actually, this code should be here in case elements are disabled.
  // This code should then set the GL state to default values...
  // 990630, pederb
  //
  if (this->firstRender) {
    this->firstRender = FALSE;
    glDisable(GL_FOG);
    glDisable(GL_TEXTURE_1D);
    glDisable(GL_LINE_STIPPLE);
    glDisable(GL_POLYGON_STIPPLE);
    glDisable(GL_CULL_FACE);
    glDisable(GL_ALPHA_TEST);
    glDisable(GL_BLEND);
    glDisable(GL_LOGIC_OP);
    glDisable(GL_STENCIL_TEST);
    glDisable(GL_CLIP_PLANE0);
    glDisable(GL_CLIP_PLANE1);
    glDisable(GL_CLIP_PLANE2);
    glDisable(GL_CLIP_PLANE3);
    glDisable(GL_CLIP_PLANE4);
    glDisable(GL_CLIP_PLANE5);
    glDisable(GL_LIGHT0);
    glDisable(GL_LIGHT1);
    glDisable(GL_LIGHT2);
    glDisable(GL_LIGHT3);
    glDisable(GL_LIGHT4);
    glDisable(GL_LIGHT5);
    glDisable(GL_LIGHT6);
    glDisable(GL_LIGHT7);
    glDisable(GL_TEXTURE_GEN_S);
    glDisable(GL_TEXTURE_GEN_T);
    glDisable(GL_TEXTURE_GEN_R);
    glDisable(GL_TEXTURE_GEN_Q);
    glDisable(GL_MAP1_VERTEX_3);
    glDisable(GL_MAP1_VERTEX_4);
    glDisable(GL_MAP1_COLOR_4);
    glDisable(GL_MAP1_INDEX);
    glDisable(GL_MAP1_NORMAL);
    glDisable(GL_MAP1_TEXTURE_COORD_1);
    glDisable(GL_MAP1_TEXTURE_COORD_2);
    glDisable(GL_MAP1_TEXTURE_COORD_3);
    glDisable(GL_MAP1_TEXTURE_COORD_4);
    glDisable(GL_MAP2_VERTEX_3);
    glDisable(GL_MAP2_VERTEX_4);
    glDisable(GL_MAP2_COLOR_4);
    glDisable(GL_MAP2_INDEX);
    glDisable(GL_MAP2_NORMAL);
    glDisable(GL_MAP2_TEXTURE_COORD_1);
    glDisable(GL_MAP2_TEXTURE_COORD_2);
    glDisable(GL_MAP2_TEXTURE_COORD_3);
    glDisable(GL_MAP2_TEXTURE_COORD_4);
    glDisable(GL_POINT_SMOOTH);
    glDisable(GL_LINE_SMOOTH);
    glDisable(GL_SCISSOR_TEST);
    glDisable(GL_COLOR_MATERIAL);
    glDisable(GL_AUTO_NORMAL);

    // FIXME (pederb)
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    // light model
    float lgt[4] = {0.2f, 0.2f, 0.2f, 1.0f};
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lgt);
    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);

    // no texturing unless textures in model
    glDisable(GL_TEXTURE_2D);

    glEnable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glEnable(GL_NORMALIZE);

    glDisable(GL_CULL_FACE);
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
    glFrontFace(GL_CCW);
    glShadeModel(GL_SMOOTH);

    // only useful while using GL_COLOR_MATERIAL
    glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    this->disableBlend(TRUE);
  }

  // FIXME: should we really push() and pop() here? Perhaps in the
  // SoAction class instead? 19990303 mortene.

  this->currentPass = 0;
  this->didHaveTransparent = FALSE;
  this->disableBlend(); // just in case

  if (this->transType == BLEND || this->transType == DELAYED_BLEND ||
      this->transType == SORTED_OBJECT_BLEND) {
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  }
  else if (this->transType == ADD || this->transType == DELAYED_ADD ||
           this->transType == SORTED_OBJECT_ADD) {
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
  }
  this->getState()->push();

  SoShapeStyleElement::setTransparencyType(this->getState(), this->transType);

  SoGLViewportRegionElement::set(this->getState(), this->viewport);
  SoLazyElement::setTransparencyType(this->getState(), (int32_t)this->transType);
  SoLazyElement::setBlending(this->getState(), FALSE);
  SoLazyElement::setColorMaterial(this->getState(), TRUE);

  // FIXME: is this the correct place to set these elements? 19990314 mortene.
  SoDecimationPercentageElement::set(this->getState(), 1.0f);
  SoDecimationTypeElement::set(this->getState(),
                               SoDecimationTypeElement::AUTOMATIC);

  // FIXME: use these as they're supposed to be used. 19990314 mortene.
  SoGLRenderPassElement::set(this->getState(), 0);
  SoGLUpdateAreaElement::set(this->getState(),
                             SbVec2f(0.0f, 0.0f), SbVec2f(1.0f, 1.0f));

  inherited::beginTraversal(node);

  if (this->didHaveTransparent && !this->sortRender) {
    if (this->transType == DELAYED_BLEND ||
        this->transType == DELAYED_ADD) {
      this->currentPass = 1;
      SoGLRenderPassElement::set(this->getState(), 1);
      this->enableBlend();
      inherited::beginTraversal(node);
      this->disableBlend();
    }
    else if (this->transType == SORTED_OBJECT_BLEND ||
             this->transType == SORTED_OBJECT_ADD) {
      this->sortRender = TRUE;
      this->doPathSort();
      this->enableBlend();
      this->apply(this->transpObjPaths, TRUE);
      this->disableBlend();
    }
  }

  this->disableBlend();

  if (this->delayedPaths.getLength()) {
    if (!this->delayedRender) {
      SbBool usedepthbuffer = glIsEnabled(GL_DEPTH_TEST);
      if (usedepthbuffer) glDisable(GL_DEPTH_TEST);
      this->delayedRender = TRUE;
      this->apply(this->delayedPaths, TRUE);
      if (usedepthbuffer) glEnable(GL_DEPTH_TEST);
    }
  }
  this->getState()->pop();
}

void
SoGLRenderAction::endTraversal(SoNode *)
{
  if (this->delayedPaths.getLength() && this->delayedRender) {
    this->delayedRender = FALSE;
    this->delayedPaths.truncate(0);
  }
  if (this->transpObjPaths.getLength() && this->sortRender) {
    this->sortRender = FALSE;
    this->transpObjPaths.truncate(0);
    this->transpObjDistances.truncate(0);
  }
}

/*!
  FIXME: write doc.
*/

SbBool
SoGLRenderAction::handleTransparency(SbBool isTransparent)
{
  if (this->delayedRender) { // special case when rendering delayed paths
    if (!isTransparent) {
      this->disableBlend();
    }
    else {
      if (this->transType != SCREEN_DOOR) this->enableBlend();
      this->disableBlend();
    }
    return FALSE; // always render
  }

  // normal case
  if (isTransparent) this->didHaveTransparent = TRUE;
  if (this->transType == DELAYED_ADD || this->transType == DELAYED_BLEND) {
    if (this->currentPass == 0) return isTransparent;
    else return !isTransparent;
  }
  else if (this->transType == SORTED_OBJECT_ADD ||
           this->transType == SORTED_OBJECT_BLEND) {
    if (this->sortRender || !isTransparent) return FALSE;
    this->addTransPath(this->getCurPath()->copy());
    return TRUE;
  }
  else if (this->transType == ADD || this->transType == BLEND) {
    if (isTransparent) this->enableBlend();
    else this->disableBlend();
    return FALSE; // node should always render
  }
  else
    return FALSE; // polygon stipple used to render
}

/*!
  This method returns the number of the current pass.
*/

int
SoGLRenderAction::getCurPass() const
{
  return this->currentPass;
}

/*!
  This method returns TRUE if the render action should abort now.
  It always returns FALSE at this moment.
*/

SbBool
SoGLRenderAction::abortNow()
{
  // FIXME: not implemented
  return FALSE;
}

void
SoGLRenderAction::addDelayedPath(SoPath * path)
{
  assert(!this->delayedRender);
  SoPath * copy = path->copy();
  this->delayedPaths.append(copy);
}

void
SoGLRenderAction::addTransPath(SoPath *path)
{
  if (this->bboxAction == NULL) {
    this->bboxAction =
      new SoGetBoundingBoxAction(SoViewportRegionElement::get(this->state));
  }
  this->bboxAction->setViewportRegion(SoViewportRegionElement::get(this->state));
  this->bboxAction->apply(path);
  SbVec3f center = this->bboxAction->getBoundingBox().getCenter();
  SoModelMatrixElement::get(this->state).multVecMatrix(center, center);
  float dist = SoViewVolumeElement::get(this->state).getPlane(0.0f).getDistance(center);
  this->transpObjPaths.append(path);
  this->transpObjDistances.append(dist);
}

SbBool
SoGLRenderAction::isRenderingDelayedPaths() const
{
  return this->delayedRender;
}

void
SoGLRenderAction::disableBlend(const SbBool force)
{
  if (force || this->isBlendEnabled) {
    glDisable(GL_BLEND);
    glDepthMask(GL_TRUE);
    this->isBlendEnabled = FALSE;
  }
}

void
SoGLRenderAction::enableBlend(const SbBool force)
{
  if (force || !this->isBlendEnabled) {
    glEnable(GL_BLEND);
    glDepthMask(GL_FALSE);
    this->isBlendEnabled = TRUE;
  }
}

void
SoGLRenderAction::doPathSort()
{
  // need to cast to SbPList to avoid ref/unref problems
  SbPList *plist = (SbPList*) &this->transpObjPaths;
  float * darray = this->transpObjDistances;

  int i, j, distance, n = this->transpObjDistances.getLength();
  void *ptmp;
  float dtmp;

  // shell sort algorithm (O(nlog(n))
  for (distance = 1; distance <= n/9; distance = 3*distance + 1);
  for (; distance > 0; distance /= 3) {
    for (i = distance; i < n; i++) {
      dtmp = darray[i];
      ptmp = plist->get(i);
      j = i;
      while (j >= distance && darray[j-distance] > dtmp) {
        darray[j] = darray[j-distance];
        plist->set(j, plist->get(j-distance));
        j -= distance;
      }
      darray[j] = dtmp;
      plist->set(j, ptmp);
    }
  }
}

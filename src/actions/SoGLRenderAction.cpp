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
 *  http://www.sim.no/ sales@sim.no Voice: +47 73540378 Fax: +47 73943861
 *
\**************************************************************************/

/*!
  \class SoGLRenderAction Inventor/actions/SoGLRenderAction.h
  \brief The SoGLRenderAction class is an action for rendering an Open Inventor
  scene graph with OpenGL.
*/

#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/SbName.h>
#include <Inventor/lists/SoEnabledElementsList.h>
#include <Inventor/nodes/SoNode.h>
#include <Inventor/actions/SoSubAction.h>
#include <Inventor/SbColor.h>
#include <Inventor/misc/SoState.h>
#include <Inventor/misc/SoGL.h>

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

#if !defined(COIN_EXCLUDE_SOGLLIGHTIDELEMENT)
#include <Inventor/elements/SoGLLightIdElement.h>
#endif // !COIN_EXCLUDE_SOGLLIGHTIDELEMENT
#if !defined(COIN_EXCLUDE_SOVIEWPORTREGIONELEMENT)
#include <Inventor/elements/SoViewportRegionElement.h>
#endif // !COIN_EXCLUDE_SOVIEWPORTREGIONELEMENT
#if !defined(COIN_EXCLUDE_SOGLRENDERPASSELEMENT)
#include <Inventor/elements/SoGLRenderPassElement.h>
#endif // !COIN_EXCLUDE_SOGLRENDERPASSELEMENT
#if !defined(COIN_EXCLUDE_SOGLUPDATEAREAELEMENT)
#include <Inventor/elements/SoGLUpdateAreaElement.h>
#endif // !COIN_EXCLUDE_SOGLUPDATEAREAELEMENT
#if !defined(COIN_EXCLUDE_SODECIMATIONTYPEELEMENT)
#include <Inventor/elements/SoDecimationTypeElement.h>
#endif // !COIN_EXCLUDE_SODECIMATIONTYPEELEMENT
#if !defined(COIN_EXCLUDE_SODECIMATIONPERCENTAGEELEMENT)
#include <Inventor/elements/SoDecimationPercentageElement.h>
#endif // !COIN_EXCLUDE_SODECIMATIONPERCENTAGEELEMENT
#if !defined(COIN_EXCLUDE_SOWINDOWELEMENT)
#include <Inventor/elements/SoWindowElement.h>
#endif // !COIN_EXCLUDE_SOWINDOWELEMENT
#if !defined(COIN_EXCLUDE_SOGLSHADEMODELELEMENT)
#include <Inventor/elements/SoGLShadeModelElement.h>
#endif // ! COIN_EXCLUDE_SOGLSHADEMODELELEMENT
#if !defined(COIN_EXCLUDE_SOGLNORMALIZEELEMENT)
#include <Inventor/elements/SoGLNormalizeElement.h>
#endif // ! COIN_EXCLUDE_SOGLNORMALIZEELEMENT
#if !defined(COIN_EXCLUDE_SOSHAPESTYLEELEMENT)
#include <Inventor/elements/SoShapeStyleElement.h>
#endif // ! COIN_EXCLUDE_SOSHAPESTYLEELEMENT

// FIXME: see comment on SoGLViewportRegionElement::set in
// beginTraversel() below. 19990228 mortene.
#if !defined(COIN_EXCLUDE_SOGLVIEWPORTREGIONELEMENT)
#include <Inventor/elements/SoGLViewportRegionElement.h>
#endif // !COIN_EXCLUDE_SOGLVIEWPORTREGIONELEMENT
// FIXME: these should not be necessary here. tmp hack. 19990228 mortene.
#if !defined(COIN_EXCLUDE_SOTEXTURECOORDINATEELEMENT)
#include <Inventor/elements/SoTextureCoordinateElement.h>
#endif // !COIN_EXCLUDE_SOTEXTURECOORDINATEELEMENT

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
  \fn typedef SoGLRenderAction::SoGLRenderAbortCB(void *userData)

  FIXME: write doc.
*/

/*!
  \fn typedef SoGLRenderAction::SoGLRenderPassCB(void *userData)

  FIXME: write doc.
*/
// *************************************************************************

//$ BEGIN TEMPLATE ActionSource( SoGLRenderAction )

SoType SoGLRenderAction::classTypeId = SoType::badType();

/*!
  Returns the unique type identifier for the classname class.
*/
SoType
SoGLRenderAction::getClassTypeId(void)
{
  return classTypeId;
}

/*!
  Returns type identifier for an object.
*/
SoType
SoGLRenderAction::getTypeId(void) const
{
  return classTypeId;
}

// static variables
SoEnabledElementsList * SoGLRenderAction::enabledElements;
SoActionMethodList * SoGLRenderAction::methods;

/*!
  \fn SoGLRenderAction::enabledElements
  FIXME: write doc.
*/

/*!
  \fn SoGLRenderAction::methods
  FIXME: write doc.
*/

/*!
  This method returns the list of enabled elements for the given action class.
*/
const SoEnabledElementsList &
SoGLRenderAction::getEnabledElements(void) const
{
  return *enabledElements;
}

/*!
  This method adds a method to be perfomed by the action class on the given
  node type.
*/
void 
SoGLRenderAction::addMethod(const SoType type, SoActionMethod method)
{
  methods->addMethod(type, method);
}

/*!
  This method enables an element in the state stack for the action class.
*/
void 
SoGLRenderAction::enableElement(const SoType type, const int stackIndex)
{
  enabledElements->enable(type, stackIndex);
}
//$ END TEMPLATE ActionSource

// *************************************************************************

/*!
  This static method initializes the static data for the SoGLRenderAction
  class.
*/

void
SoGLRenderAction::initClass(
    void )
{
//$ BEGIN TEMPLATE InitActionSource( SoGLRenderAction )
  assert(SoGLRenderAction::getClassTypeId() == SoType::badType());
  assert(inherited::getClassTypeId() != SoType::badType());

  SoGLRenderAction::classTypeId = 
      SoType::createType(inherited::getClassTypeId(),
                         "SoGLRenderAction");
  enabledElements = new SoEnabledElementsList(inherited::enabledElements);
  methods = new SoActionMethodList(inherited::methods);
//$ END TEMPLATE InitActionSource

  
  methods->setDefault((void *)SoNode::GLRenderS);

  ENABLE_ELEMENT(SoViewportRegionElement);
  ENABLE_ELEMENT(SoGLRenderPassElement);
  ENABLE_ELEMENT(SoDecimationTypeElement);
  ENABLE_ELEMENT(SoDecimationPercentageElement);
  ENABLE_ELEMENT(SoWindowElement);
  ENABLE_ELEMENT(SoGLLightIdElement);
  ENABLE_ELEMENT(SoGLUpdateAreaElement);

#if !defined(COIN_EXCLUDE_SOGLSHADEMODELELEMENT)
  ENABLE_ELEMENT(SoGLShadeModelElement);
#endif
#if !defined(COIN_EXCLUDE_SOGLNORMALIZEELEMENT)
  ENABLE_ELEMENT(SoGLNormalizeElement);
#endif

  // FIXME: see comment on SoGLViewportRegionElement::set in
  // beginTraversel() below. 19990228 mortene.
  ENABLE_ELEMENT(SoGLViewportRegionElement);
}

/*!
  This static method cleans up the static data for the SoGLRenderAction
  class.
*/

void
SoGLRenderAction::cleanClass(
    void )
{
}

// *************************************************************************

/*!
  A constructor.
*/

SoGLRenderAction::SoGLRenderAction(const SbViewportRegion &viewportRegion)
{
  SO_ACTION_CONSTRUCTOR(SoGLRenderAction);  

  // Can't just push this on the SoViewportRegionElement stack, as the
  // state hasn't been made yet.
  this->viewport = viewportRegion;

  this->passCB = NULL;
  this->passCBdata = NULL;
  this->smoothing = FALSE;
  this->numPasses = 1;

  this->transType = SoGLRenderAction::SCREEN_DOOR;
  this->firstRender = TRUE;
  this->delayedRender = FALSE;
}

/*!
  The destructor.
*/

SoGLRenderAction::~SoGLRenderAction()
{
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
SoGLRenderAction::setUpdateArea(const SbVec2f &origin, const SbVec2f &size)
{
  assert(0 && "FIXME: not implemented" );
}

/*!
  This method gets the area to be updated.
*/

void 
SoGLRenderAction::getUpdateArea(SbVec2f &origin, SbVec2f &size) const
{
  assert(0 && "FIXME: not implemented");
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
				  void *const userData)
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
    glDisable( GL_FOG );
    glDisable( GL_TEXTURE_1D );
    glDisable( GL_LINE_STIPPLE );
    glDisable( GL_POLYGON_STIPPLE );
    glDisable( GL_CULL_FACE );
    glDisable( GL_ALPHA_TEST );
    glDisable( GL_BLEND );
    glDisable( GL_LOGIC_OP );
    glDisable( GL_STENCIL_TEST );
    glDisable( GL_CLIP_PLANE0 );
    glDisable( GL_CLIP_PLANE1 );
    glDisable( GL_CLIP_PLANE2 );
    glDisable( GL_CLIP_PLANE3 );
    glDisable( GL_CLIP_PLANE4 );
    glDisable( GL_CLIP_PLANE5 );
    glDisable( GL_LIGHT0 );
    glDisable( GL_LIGHT1 );
    glDisable( GL_LIGHT2 );
    glDisable( GL_LIGHT3 );
    glDisable( GL_LIGHT4 );
    glDisable( GL_LIGHT5 );
    glDisable( GL_LIGHT6 );
    glDisable( GL_LIGHT7 );
    glDisable( GL_TEXTURE_GEN_S );
    glDisable( GL_TEXTURE_GEN_T );
    glDisable( GL_TEXTURE_GEN_R );
    glDisable( GL_TEXTURE_GEN_Q );
    glDisable( GL_MAP1_VERTEX_3 );
    glDisable( GL_MAP1_VERTEX_4 );
    glDisable( GL_MAP1_COLOR_4 );
    glDisable( GL_MAP1_INDEX );
    glDisable( GL_MAP1_NORMAL );
    glDisable( GL_MAP1_TEXTURE_COORD_1 );
    glDisable( GL_MAP1_TEXTURE_COORD_2 );
    glDisable( GL_MAP1_TEXTURE_COORD_3 );
    glDisable( GL_MAP1_TEXTURE_COORD_4 );
    glDisable( GL_MAP2_VERTEX_3 );
    glDisable( GL_MAP2_VERTEX_4 );
    glDisable( GL_MAP2_COLOR_4 );
    glDisable( GL_MAP2_INDEX );
    glDisable( GL_MAP2_NORMAL );
    glDisable( GL_MAP2_TEXTURE_COORD_1 );
    glDisable( GL_MAP2_TEXTURE_COORD_2 );
    glDisable( GL_MAP2_TEXTURE_COORD_3 );
    glDisable( GL_MAP2_TEXTURE_COORD_4 );
    glDisable( GL_POINT_SMOOTH );
    glDisable( GL_LINE_SMOOTH );
    glDisable( GL_SCISSOR_TEST );
    glDisable( GL_COLOR_MATERIAL );
    glDisable( GL_AUTO_NORMAL );

    // FIXME (pederb)
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    // light model
    float lgt[4] = {0.2, 0.2, 0.2, 1.0};
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
  }
  
  // FIXME: should we really push() and pop() here? Perhaps in the
  // SoAction class instead? 19990303 mortene.

  this->transType = DELAYED_BLEND; // FIXME, just testing

  this->currentPass = 0;
  this->didHaveTransparent = FALSE;
  this->getState()->push();

#if !defined(COIN_EXCLUDE_SOSHAPESTYLEELEMENT)
  SoShapeStyleElement::setTransparencyType(this->getState(), this->transType);
#endif // ! COIN_EXCLUDE_SOSHAPESTYLEELEMENT

  SoGLViewportRegionElement::set(this->getState(), this->viewport);

  // FIXME: is this the correct place to set these elements? 19990314 mortene.
  SoDecimationPercentageElement::set(this->getState(), 1.0f);
  SoDecimationTypeElement::set(this->getState(),
			       SoDecimationTypeElement::AUTOMATIC);

  // FIXME: use these as they're supposed to be used. 19990314 mortene.
  SoGLRenderPassElement::set(this->getState(), 0);
  SoGLUpdateAreaElement::set(this->getState(),
			     SbVec2f(0.0f, 0.0f), SbVec2f(1.0f, 1.0f));

  inherited::beginTraversal(node);
  this->getState()->pop();

  // FIXME: temporary code
  if (transType == DELAYED_BLEND && didHaveTransparent) {
    this->currentPass = 1;
    this->getState()->push();

    SoGLViewportRegionElement::set(this->getState(), this->viewport);
    
    // FIXME: is this the correct place to set these elements? 19990314 mortene.
    SoDecimationPercentageElement::set(this->getState(), 1.0f);
    SoDecimationTypeElement::set(this->getState(),
				 SoDecimationTypeElement::AUTOMATIC);
  
    // FIXME: use these as they're supposed to be used. 19990314 mortene.
    SoGLRenderPassElement::set(this->getState(), 1);
    SoGLUpdateAreaElement::set(this->getState(),
			       SbVec2f(0.0f, 0.0f), SbVec2f(1.0f, 1.0f));

    
    glBlendFunc(GL_ONE_MINUS_SRC_ALPHA, GL_SRC_ALPHA);
    glEnable(GL_BLEND);
    glDepthMask(GL_FALSE);
    
    inherited::beginTraversal(node);
    
    glDisable(GL_BLEND);
    glDepthMask(GL_TRUE);
    
    this->getState()->pop();
  }

  if (this->delayedPaths.getLength()) {
    if (!this->delayedRender) {
      SbBool usedepthbuffer = glIsEnabled(GL_DEPTH_TEST);
      if (usedepthbuffer) glDisable(GL_DEPTH_TEST);
      this->delayedRender = TRUE;
#if 0 // debug
      fprintf(stderr,"rendering delayed paths: %d\n", 
	      this->delayedPaths.getLength());
#endif
      this->apply(this->delayedPaths, TRUE);
      if (usedepthbuffer) glEnable(GL_DEPTH_TEST);
    }
  }
}

void
SoGLRenderAction::endTraversal(SoNode *)
{
  if (this->delayedPaths.getLength() && this->delayedRender) {
    this->delayedRender = FALSE;
    int n = this->delayedPaths.getLength();
    for (int i = 0; i < n; i++) {
      this->delayedPaths[i]->unref();
    }
    this->delayedPaths.truncate(0);
  }
}

/*!
  FIXME: write doc.
*/

SbBool 
SoGLRenderAction::handleTransparency(SbBool isTransparent)
{
  if (isTransparent) this->didHaveTransparent = TRUE;
  return isTransparent && currentPass == 0 &&
    (transType == DELAYED_BLEND || transType == DELAYED_ADD);
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

#if !defined(COIN_EXCLUDE_SOPATH)
void 
SoGLRenderAction::addDelayedPath(SoPath *path)
{
  assert(!this->delayedRender);
  SoPath *copy = path->copy();
  copy->ref();
  this->delayedPaths.append(copy);
}
#endif // ! COIN_EXCLUDE_SOPATH

SbBool 
SoGLRenderAction::isRenderingDelayedPaths() const
{
  return this->delayedRender;
}


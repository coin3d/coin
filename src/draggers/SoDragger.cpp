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
  \class SoDragger SoDragger.h Inventor/draggers/SoDragger.h
  \brief The SoDragger class is the base class for all draggers.
  \ingroup draggers

  Draggers is a mechanism used for letting the end-users of your
  application code interact with elements in 3D, by scaling, rotating
  or translating geometry or other instances in the scene (like
  cameras or lightsources).

  For a very thorough introduction and tutorial to the dragger classes
  and general concepts, we advise you to consult «The Inventor
  Mentor», ISBN 0-201-62495-8, chapter 15.

  This is the common superclass for all dragger classes.

  It holds the current motion matrix, and offers lots of convenience
  methods to build from for it's subclasses -- that is, the
  non-abstract dragger classes to use as nodes in your scenegraph.

  The motion matrix is used to modify the model matrix during
  traversal, and this is a common dragger mechanism -- all draggers
  should update this during dragging.

  A number of the Coin dragger classes have built-in convenience
  wrapper classes, called \e manipulators. See for instance the
  SoTrackballDragger / SoTrackballManip pair.

  The matching manipulator class for any dragger class has basically
  two convenient additions to the functionality of the stand-alone
  dragger: 1) it makes swapping the dragger in and out of the
  scenegraph very straightforward (something which is often done for
  draggers in 3D user interfaces), 2) it wraps up the dragger with
  SoSurroundScale and SoAntiSquish nodes where applicable, so the
  dragger geometry automatically scales up or down to match the
  geometry it influences.

  This last functionality can also be duplicated in a rather
  straightforward manner outside of the context of a manipulator, as
  can be seen from the usage example in the SoSurroundScale class
  documentation.

  One common way of using only \e parts of a dragger is to replace /
  disable the geometry that you don't want the end-user to interact
  with. The following code example shows how to remove the translation
  functionality of the SoTransformBoxDragger:

  \code
  #include <Inventor/Qt/SoQt.h>
  #include <Inventor/Qt/viewers/SoQtExaminerViewer.h>
  #include <Inventor/draggers/SoTransformBoxDragger.h>
  #include <Inventor/nodes/SoSeparator.h>


  int
  main(int argc, char ** argv)
  {
    QWidget * window = SoQt::init(argv[0]);

    SoTransformBoxDragger * dragger = new SoTransformBoxDragger;

    SbString str;
    for (int i = 1; i <= 6; i++) {
      str.sprintf("translator%d.translator", i);
      dragger->setPart(str.getString(), new SoSeparator);
    }

    SoQtExaminerViewer * viewer = new SoQtExaminerViewer(window);
    viewer->setSceneGraph(dragger);
    viewer->show();
    SoQt::show(window);

    SoQt::mainLoop();

    delete viewer;
    return 0;
  }
  \endcode
}

*/
//   FIXME: more class doc! The general concept of draggers should be
//   explained in more detail here -- just refering to the Inventor
//   Mentor is a cop-out. And include at least one general usage example
//   and some screenshots.  20011219 mortene.


/*!
  \var SoSFBool SoDragger::isActive
  Is \c TRUE whenever the user is interacting with the dragger.
*/

/*!
  \enum SoDragger::ProjectorFrontSetting

  Holds various settings for projectors, which might affect
  cylindrical and spherical based draggers.  Specifies whether
  dragging should be based on the front or back of the sphere /
  cylinder, or if the picked point should be used to decide this.
*/

/*!
  \var SoDragger::ProjectorFrontSetting SoDragger::FRONT
  Always use front of projector.
*/

/*!
  \var SoDragger::ProjectorFrontSetting SoDragger::BACK
  Always use back of projector.
*/

/*!
  \var SoDragger::ProjectorFrontSetting SoDragger::USE_PICK
  Use picked point to decide front or back of projector.
*/

// FIXME: document DraggerCB typedef? 20010909 mortene.


#include <Inventor/draggers/SoDragger.h>
#include <Inventor/nodekits/SoSubKitP.h>
#include <Inventor/draggers/SoCenterballDragger.h>
#include <Inventor/draggers/SoDirectionalLightDragger.h>
#include <Inventor/draggers/SoDragPointDragger.h>
#include <Inventor/draggers/SoHandleBoxDragger.h>
#include <Inventor/draggers/SoJackDragger.h>
#include <Inventor/draggers/SoPointLightDragger.h>
#include <Inventor/draggers/SoRotateCylindricalDragger.h>
#include <Inventor/draggers/SoRotateDiscDragger.h>
#include <Inventor/draggers/SoRotateSphericalDragger.h>
#include <Inventor/draggers/SoScale1Dragger.h>
#include <Inventor/draggers/SoScale2Dragger.h>
#include <Inventor/draggers/SoScale2UniformDragger.h>
#include <Inventor/draggers/SoScaleUniformDragger.h>
#include <Inventor/draggers/SoSpotLightDragger.h>
#include <Inventor/draggers/SoTabBoxDragger.h>
#include <Inventor/draggers/SoTabPlaneDragger.h>
#include <Inventor/draggers/SoTrackballDragger.h>
#include <Inventor/draggers/SoTransformBoxDragger.h>
#include <Inventor/draggers/SoTransformerDragger.h>
#include <Inventor/draggers/SoTranslate1Dragger.h>
#include <Inventor/draggers/SoTranslate2Dragger.h>
#include <Inventor/actions/SoHandleEventAction.h>
#include <Inventor/actions/SoRayPickAction.h>
#include <Inventor/actions/SoGetMatrixAction.h>
#include <Inventor/events/SoMouseButtonEvent.h>
#include <Inventor/events/SoLocation2Event.h>
#include <Inventor/events/SoKeyboardEvent.h>
#include <Inventor/elements/SoViewVolumeElement.h>
#include <Inventor/elements/SoViewportRegionElement.h>

#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/actions/SoCallbackAction.h>
#include <Inventor/actions/SoRayPickAction.h>
#include <Inventor/actions/SoGetPrimitiveCountAction.h>

#include <Inventor/elements/SoGLTextureImageElement.h>
#include <Inventor/elements/SoGLTextureEnabledElement.h>
#include <Inventor/elements/SoGLTexture3EnabledElement.h>
#include <Inventor/elements/SoShapeHintsElement.h>
#include <Inventor/elements/SoNormalBindingElement.h>
#include <Inventor/elements/SoMaterialBindingElement.h>
#include <Inventor/elements/SoLightModelElement.h>
#include <Inventor/elements/SoNormalElement.h>
#include <Inventor/elements/SoLineWidthElement.h>
#include <Inventor/elements/SoLinePatternElement.h>
#include <Inventor/elements/SoCreaseAngleElement.h>
#include <Inventor/elements/SoComplexityElement.h>
#include <Inventor/elements/SoComplexityTypeElement.h>

#include <Inventor/SbViewportRegion.h>
#include <Inventor/nodes/SoMatrixTransform.h>
#include <Inventor/SoPickedPoint.h>

#include <Inventor/errors/SoDebugError.h>
#include <coindefs.h> // COIN_OBSOLETED

#ifndef DOXYGEN_SKIP_THIS // Don't document internal classes.

class SoDraggerCache {
public:
  SoDraggerCache(SoDragger * parent) :
    path((SoFullPath*)new SoPath(4)),
    dragger(parent),
    matrixAction(new SoGetMatrixAction(dragger->getViewportRegion())),
    draggerToWorld(SbMatrix::identity()),
    worldToDragger(SbMatrix::identity())
  {
    this->path->ref();
  }

  ~SoDraggerCache() {
    delete this->matrixAction;
    this->path->unref();
  }

  void updateMatrix(void) {
    this->matrixAction->setViewportRegion(this->dragger->getViewportRegion());
    this->matrixAction->apply(this->path);
    this->draggerToWorld = this->matrixAction->getMatrix();
    this->worldToDragger = this->matrixAction->getInverse();
  }

  void update(const SoFullPath * newpath, const int draggeridx) {
    this->path->setHead(newpath->getHead());
    for (int i = 1; i <= draggeridx; i++) {
      this->path->append(newpath->getIndex(i));
    }
    this->updateMatrix();
  }

  void truncatePath(void) {
    this->path->truncate(0);
  }

  SoFullPath * path; // use temp path to avoid auditor overhead
  SoDragger * dragger; // pointer to cache owner
  SoGetMatrixAction * matrixAction; // avoid reallocating this action each frame
  SbMatrix draggerToWorld;
  SbMatrix worldToDragger;
};

class SoDraggerP {
public:
  int mingesture;
  SoHandleEventAction * eventaction;
  SoDragger::ProjectorFrontSetting frontonprojector;
  SbBool valuechangedcbenabled;
  SbBool ignoreinbbox;
  const SoEvent * currentevent;
  SoPath * pickedpath;
  class SoDraggerCache * draggercache;

  SoCallbackList startCB;
  SoCallbackList motionCB;
  SoCallbackList finishCB;
  SoCallbackList valueChangedCB;
  SoCallbackList otherEventCB;
  SbMatrix startmotionmatrix;
  SbVec3f startingpoint;
  SbViewVolume viewvolume;
  SbViewportRegion viewport;
  SbVec2s startlocaterpos;
  SoDragger * activechilddragger;
  SbBool isgrabbing;

  SbName surrogatename;
  SoPath * surrogateownerpath;
  SoPath * surrogatepath;
};

#endif // DOXYGEN_SKIP_THIS

SO_KIT_SOURCE(SoDragger);

float SoDragger::minscale = 0.001f;

#define THIS this->pimpl
#define THISP thisp->pimpl

/*!
  A protected constructor for this abstract superclass for all Coin
  draggers.
*/
SoDragger::SoDragger(void)
  : pimpl(new SoDraggerP)
{
  SO_KIT_INTERNAL_CONSTRUCTOR(SoDragger);

  SO_KIT_ADD_CATALOG_ENTRY(motionMatrix, SoMatrixTransform, FALSE, topSeparator, geomSeparator, FALSE);

  SO_KIT_ADD_FIELD(isActive, (FALSE));

  SO_KIT_INIT_INSTANCE();

  THIS->mingesture = 8;
  THIS->eventaction = NULL;
  THIS->frontonprojector = USE_PICK;
  THIS->valuechangedcbenabled = TRUE;
  THIS->ignoreinbbox = FALSE;
  THIS->currentevent = NULL;
  THIS->pickedpath = NULL;
  THIS->draggercache = NULL;
  THIS->isgrabbing = FALSE;
  THIS->activechilddragger = NULL;
  THIS->surrogateownerpath = NULL;
  THIS->surrogatepath = NULL;
}

/*!
  Virtual protected destructor.
*/
SoDragger::~SoDragger()
{
  if (THIS->pickedpath) THIS->pickedpath->unref();
  if (THIS->surrogateownerpath) THIS->surrogateownerpath->unref();
  if (THIS->surrogatepath) THIS->surrogatepath->unref();
  delete THIS->draggercache;
  delete this->pimpl;
}

// Note: the following documentation for initClass() will also be used
// for dragger subclasses, so keep it general.
/*!
  Initializes type system for this dragger class.

  Application programmers should usually not have to invoke this
  method, see documentation of SoInteraction::initClass().
*/
void
SoDragger::initClass(void)
{
  SO_KIT_INTERNAL_INIT_CLASS(SoDragger, SO_FROM_INVENTOR_1);

  SoDragger::initClasses();

  SoType type = SoDragger::getClassTypeId();
  SoRayPickAction::addMethod(type, SoNode::rayPickS);
}

/*!
  Initializes all built-in draggers.
*/
void
SoDragger::initClasses(void)
{
  SoCenterballDragger::initClass();
  SoDirectionalLightDragger::initClass();
  SoDragPointDragger::initClass();
  SoHandleBoxDragger::initClass();
  SoJackDragger::initClass();
  SoPointLightDragger::initClass();
  SoRotateCylindricalDragger::initClass();
  SoRotateDiscDragger::initClass();
  SoRotateSphericalDragger::initClass();
  SoScale1Dragger::initClass();
  SoScale2Dragger::initClass();
  SoScale2UniformDragger::initClass();
  SoScaleUniformDragger::initClass();
  SoSpotLightDragger::initClass();
  SoTabBoxDragger::initClass();
  SoTabPlaneDragger::initClass();
  SoTrackballDragger::initClass();
  SoTransformBoxDragger::initClass();
  SoTransformerDragger::initClass();
  SoTranslate1Dragger::initClass();
  SoTranslate2Dragger::initClass();
}

// Private method that sets some elements to default (for our
// draggers) values.
void
SoDragger::updateElements(SoState * state)
{
  if (state->isElementEnabled(SoShapeHintsElement::getClassStackIndex())) {
    // turn on backface culling for draggers
    SoShapeHintsElement::set(state, this,
                             SoShapeHintsElement::COUNTERCLOCKWISE,
                             SoShapeHintsElement::SOLID,
                             SoShapeHintsElement::CONVEX);
  }
  if (state->isElementEnabled(SoTextureImageElement::getClassStackIndex())) {
    // clear texture data
    SoTextureImageElement::setDefault(state, this);
  }
  if (state->isElementEnabled(SoGLTextureEnabledElement::getClassStackIndex())) {
    // disable 2D texture
    SoGLTextureEnabledElement::set(state, this, FALSE);
  }
  if (state->isElementEnabled(SoGLTexture3EnabledElement::getClassStackIndex())) {
    // disable 3D texture
    SoGLTexture3EnabledElement::set(state, this, FALSE);
  }
  if (state->isElementEnabled(SoNormalBindingElement::getClassStackIndex())) {
    // make default
    SoNormalBindingElement::set(state, SoNormalBindingElement::DEFAULT);
  }
  if (state->isElementEnabled(SoMaterialBindingElement::getClassStackIndex())) {
    // make default
    SoMaterialBindingElement::set(state, SoMaterialBindingElement::DEFAULT);
  }
  if (state->isElementEnabled(SoLightModelElement::getClassStackIndex())) {
    // we need phong shading for our geometry
    SoLightModelElement::set(state, this, SoLightModelElement::PHONG);
  }

  if (state->isElementEnabled(SoNormalElement::getClassStackIndex())) {
    // lines/shapes will use normals if on state. We don't want that.
    SoNormalElement::set(state, this, 0, NULL);
  }
  if (state->isElementEnabled(SoLineWidthElement::getClassStackIndex())) {
    // make default
    SoLineWidthElement::set(state, this, SoLineWidthElement::getDefault());
  }
  if (state->isElementEnabled(SoLinePatternElement::getClassStackIndex())) {
    // make default
    SoLinePatternElement::set(state, this, SoLinePatternElement::getDefault());
  }
  if (state->isElementEnabled(SoCreaseAngleElement::getClassStackIndex())) {
    // set to 0.5, which is the value we like
    SoCreaseAngleElement::set(state, this, 0.5f);
  }
  if (state->isElementEnabled(SoComplexityElement::getClassStackIndex())) {
    // make default
    SoComplexityElement::set(state, this, SoComplexityElement::getDefault());
  }
  if (state->isElementEnabled(SoComplexityElement::getClassStackIndex())) {
    // make default
    SoComplexityTypeElement::set(state, this, SoComplexityTypeElement::getDefault());
  }
  // hopefully we didn't forget something...
}

// The action methods are overridden in case we decide to do some
// extra work before passing the control on the SoBaseKit.

// Doc in superclass. Overridden to initialize some elements before
// traversing children.
void
SoDragger::callback(SoCallbackAction * action)
{
  SoState * state = action->getState();
  state->push();
  inherited::callback(action);
  state->pop();
}

// Doc in superclass. Overridden to initialize some elements before
// traversing children.
void
SoDragger::GLRender(SoGLRenderAction * action)
{
  SoState * state = action->getState();
  state->push();
  this->updateElements(state);
  inherited::GLRender(action);
  state->pop();
}

// Doc in superclass. Overridden to initialize some elements before
// traversing children.
void
SoDragger::getMatrix(SoGetMatrixAction * action)
{
  // no need to update any elements here
  inherited::getMatrix(action);
}

// Doc in superclass. Overridden to initialize some elements before
// traversing children.
void
SoDragger::rayPick(SoRayPickAction * action)
{
  SoState * state = action->getState();
  state->push();
  this->updateElements(state);
  inherited::rayPick(action);
  state->pop();
}

// Doc in superclass. Overridden to initialize some elements before
// traversing children.
void
SoDragger::search(SoSearchAction * action)
{
  // no need to update any elements here
  inherited::search(action);
}

// Doc in superclass. Overridden to initialize some elements before
// traversing children.
void
SoDragger::write(SoWriteAction * action)
{
  // no need to update any elements here
  inherited::write(action);
}

// Doc in superclass. Overridden to initialize some elements before
// traversing children.
void
SoDragger::getPrimitiveCount(SoGetPrimitiveCountAction * action)
{
  SoState * state = action->getState();
  state->push();
  this->updateElements(state);
  inherited::getPrimitiveCount(action);
  state->pop();
}

/*!
  Adds a callback which is called at the start of a drag, after the
  mouse button 1 is pressed, and dragger is picked.
*/
void
SoDragger::addStartCallback(SoDraggerCB * func, void * data)
{
  THIS->startCB.addCallback((SoCallbackListCB *)func, data);
}

/*!
  Removes a previously registered start callback.

  \sa addStartCallback()
*/
void
SoDragger::removeStartCallback(SoDraggerCB * func, void * data)
{
  THIS->startCB.removeCallback((SoCallbackListCB *)func, data);
}

/*!
  Adds a callback which is called for each mouse movement during
  dragging.
*/
void
SoDragger::addMotionCallback(SoDraggerCB * func, void * data)
{
  THIS->motionCB.addCallback((SoCallbackListCB *)func, data);
}

/*!
  Removes a previously registered motion callback.

  \sa addMotionCallback()
*/
void
SoDragger::removeMotionCallback(SoDraggerCB * func, void * data)
{
  THIS->motionCB.removeCallback((SoCallbackListCB *)func, data);
}

/*!
  Adds a callback which is called after dragging is finished.
*/
void
SoDragger::addFinishCallback(SoDraggerCB * func, void * data)
{
  THIS->finishCB.addCallback((SoCallbackListCB *)func, data);
}

/*!
  Removes a finish callback.

  \sa addFinishCallback()
*/
void
SoDragger::removeFinishCallback(SoDraggerCB * func, void * data)
{
  THIS->finishCB.removeCallback((SoCallbackListCB *)func, data);
}

/*!
  Adds a callback which is called after a dragger has changed a field.
  It is not called if the SoDragger::isActive field is changed.

  \sa enableValueChangedCallback()
*/
void
SoDragger::addValueChangedCallback(SoDraggerCB * func, void * data)
{
  THIS->valueChangedCB.addCallback((SoCallbackListCB *)func, data);
}

/*!
  Removes a value changed callback.

  \sa addValueChangedCallback()
*/
void
SoDragger::removeValueChangedCallback(SoDraggerCB * func, void * data)
{
  THIS->valueChangedCB.removeCallback((SoCallbackListCB *)func, data);
}

/*!
  Sets the number of pixel movement needed to trigger a constraint
  gesture.  Default is 8 pixels.
*/
void
SoDragger::setMinGesture(int pixels)
{
  THIS->mingesture = pixels;
}

/*!
  Returns the gesture pixels threshold value.

  \sa setMinGesture()
*/
int
SoDragger::getMinGesture(void) const
{
  return THIS->mingesture;
}

/*!
  Enable or disable "value changed" callbacks.

  \sa addValueChangedCallback()
*/
SbBool
SoDragger::enableValueChangedCallbacks(SbBool val)
{
  SbBool oldval = THIS->valuechangedcbenabled;
  THIS->valuechangedcbenabled = val;
  return oldval;
}

/*!
  Returns the motion matrix for this dragger.
*/
const SbMatrix &
SoDragger::getMotionMatrix(void)
{
  SoMatrixTransform * node = SO_GET_ANY_PART(this, "motionMatrix", SoMatrixTransform);
  assert(node);
  return node->matrix.getValue();
}

/*!
  Adds an event callback for events other then drag events.  As soon
  as dragging starts, the dragger grabs all events (until mouse button
  is released). This method can be used to handle other events during
  dragging.
*/
void
SoDragger::addOtherEventCallback(SoDraggerCB * func, void * data)
{
  THIS->otherEventCB.addCallback((SoCallbackListCB *)func, data);
}

/*!
  Removes a other event callback.

  \sa addOtherEventCallback()
*/
void
SoDragger::removeOtherEventCallback(SoDraggerCB * func, void * data)
{
  THIS->otherEventCB.removeCallback((SoCallbackListCB *)func, data);
}

/*!
  Should be called by compound draggers to register child draggers.
*/
void
SoDragger::registerChildDragger(SoDragger * child)
{
  child->addStartCallback(SoDragger::childStartCB, this);
  child->addMotionCallback(SoDragger::childMotionCB, this);
  child->addFinishCallback(SoDragger::childFinishCB, this);
  child->addOtherEventCallback(SoDragger::childOtherEventCB, this);
  child->addValueChangedCallback(SoDragger::childTransferMotionAndValueChangedCB, this);
}

/*!
  Should be called by compound draggers to unregister child draggers.
*/
void
SoDragger::unregisterChildDragger(SoDragger * child)
{
  child->removeStartCallback(SoDragger::childStartCB, this);
  child->removeMotionCallback(SoDragger::childMotionCB, this);
  child->removeFinishCallback(SoDragger::childFinishCB, this);
  child->removeOtherEventCallback(SoDragger::childOtherEventCB, this);
  child->removeValueChangedCallback(SoDragger::childTransferMotionAndValueChangedCB, this);
}

/*!
  Should be called by compund draggers to register child draggers that
  should move independently of their parent.
*/
void
SoDragger::registerChildDraggerMovingIndependently(SoDragger * child)
{
  child->addStartCallback(SoDragger::childStartCB, this);
  child->addMotionCallback(SoDragger::childMotionCB, this);
  child->addFinishCallback(SoDragger::childFinishCB, this);
  child->addOtherEventCallback(SoDragger::childOtherEventCB, this);
  child->addValueChangedCallback(SoDragger::childValueChangedCB, this);
}

/*!
  Should be called by compund draggers to unregister child draggers.
  \sa registerChildDraggerMovingIndependently()
*/
void
SoDragger::unregisterChildDraggerMovingIndependently(SoDragger * child)
{
  child->removeStartCallback(SoDragger::childStartCB, this);
  child->removeMotionCallback(SoDragger::childMotionCB, this);
  child->removeFinishCallback(SoDragger::childFinishCB, this);
  child->removeOtherEventCallback(SoDragger::childOtherEventCB, this);
  child->removeValueChangedCallback(SoDragger::childValueChangedCB, this);
}

/*!
  Returns a matrix that converts from local to world space.
*/
SbMatrix
SoDragger::getLocalToWorldMatrix(void)
{
  assert(THIS->draggercache);
  SbMatrix m = THIS->draggercache->draggerToWorld;
  m.multLeft(this->getMotionMatrix());
  return m;
}


/*!
  Returns a matrix that converts from world to local space.
*/
SbMatrix
SoDragger::getWorldToLocalMatrix(void)
{
  assert(THIS->draggercache);
  SbMatrix m = THIS->draggercache->worldToDragger;
  m.multRight(this->getMotionMatrix().inverse());
  return m;
}

/*!
  Returns the drag starting point in the local coordinate system.
*/
SbVec3f
SoDragger::getLocalStartingPoint(void)
{
  SbVec3f res;
  this->getWorldToLocalMatrix().multVecMatrix(THIS->startingpoint, res);
  return res;
}

/*!
  Returns the drag starting point in the world coordinate system.
*/
SbVec3f
SoDragger::getWorldStartingPoint(void)
{
  return THIS->startingpoint;
}

/*!
  Returns matrices that will convert between local space and the space in
  which \a partname lies in.
*/
void
SoDragger::getPartToLocalMatrix(const SbName & partname, SbMatrix & parttolocalmatrix, SbMatrix & localtopartmatrix)
{
  // ref, in case somebody is operating on a zero-ref instance to the
  // dragger.
  this->ref();
  // we need to create a path from the root node, since
  // SoSurroundScale nodes need the entire path to calculate the
  // surround parameters correctly.
  SoPath * pathtothis = this->createPathToThis();
  assert(pathtothis);
  pathtothis->ref();
  SoPath * path = (SoPath *)this->createPathToAnyPart(partname, FALSE, FALSE, FALSE, pathtothis);
  assert(path);
  pathtothis->unref();

  path->ref();
  SoGetMatrixAction action(THIS->viewport);
  action.apply(path);
  SbMatrix p2w = action.getMatrix();
  SbMatrix w2p = action.getInverse();
  path->unref();

  // premultiply with matrix to/from this dragger to remove
  // contributions before the dragger.
  parttolocalmatrix = p2w;
  parttolocalmatrix.multRight(this->getWorldToLocalMatrix());

  localtopartmatrix = this->getLocalToWorldMatrix();
  localtopartmatrix.multRight(w2p);

  // we ref'ed at the beginning of the function
  this->unrefNoDelete();
}

/*!
  Convenience method that transforms the local \a frommatrix to a world
  coordinate systems matrix.
*/
void
SoDragger::transformMatrixLocalToWorld(const SbMatrix & frommatrix, SbMatrix & tomatrix)
{
  if (&tomatrix != &frommatrix) tomatrix = frommatrix;
  tomatrix.multRight(this->getLocalToWorldMatrix());
  tomatrix.multLeft(this->getWorldToLocalMatrix());
}

/*!
  Convenience method that transforms the world \a frommatrix to a local
  coordinate systems matrix.
*/
void
SoDragger::transformMatrixWorldToLocal(const SbMatrix & frommatrix, SbMatrix & tomatrix)
{
  if (&tomatrix != &frommatrix) tomatrix = frommatrix;
  tomatrix.multRight(this->getWorldToLocalMatrix());
  tomatrix.multLeft(this->getLocalToWorldMatrix());
}

/*!
  Transforms a matrix that lies in the \a frompartname coordinate system into
  the local coordinate system.
*/
void
SoDragger::transformMatrixToLocalSpace(const SbMatrix & frommatrix, SbMatrix & tomatrix, const SbName & fromspacepartname)
{
  if (&tomatrix != &frommatrix) tomatrix = frommatrix;
  SbMatrix parttolocal, localtopart;
  this->getPartToLocalMatrix(fromspacepartname, parttolocal, localtopart);
  tomatrix.multRight(parttolocal);
  tomatrix.multLeft(localtopart);
}

/*!
  Sets a new current motion matrix for the dragger geometry.

  Triggers value changed callbacks if \a matrix is unequal to the
  previous motion matrix.
*/
void
SoDragger::setMotionMatrix(const SbMatrix & matrix)
{
  SoMatrixTransform * node = SO_GET_ANY_PART(this, "motionMatrix", SoMatrixTransform);
  if (matrix != node->matrix.getValue()) {
    node->matrix = matrix;
    this->valueChanged();
  }
}

/*!
  Can be called by subclasses to trigger value changed callbacks. This might
  be needed if a field is changed without changing the motion matrix.
*/
void
SoDragger::valueChanged(void)
{
  if (THIS->valuechangedcbenabled) {
    THIS->valueChangedCB.invokeCallbacks(this);
  }
}

/*!
  Returns the motion matrix as it was when saveStartParameters() was called.
*/
const SbMatrix &
SoDragger::getStartMotionMatrix(void)
{
  return THIS->startmotionmatrix;
}

/*!
  This is invoked to save start parameters, to enable draggers to
  calculate relative motion.

  Default method in superclass SoDragger just saves the motion matrix,
  but subclasses should overload this method if other data needs to be
  saved.
*/
void
SoDragger::saveStartParameters(void)
{
  THIS->startmotionmatrix = this->getMotionMatrix();
}

/*!
  Returns the picked path.
*/
const SoPath *
SoDragger::getPickPath(void) const
{
  return THIS->pickedpath;
}

/*!
  Returns the current event.
*/
const SoEvent *
SoDragger::getEvent(void) const
{
  return THIS->currentevent;
}

/*!
  Creates a new path to this dragger. Don't forget to ref() and unref() since this
  method creates a fresh copy for you.
*/
SoPath *
SoDragger::createPathToThis(void)
{
  assert(THIS->draggercache);
  if (THIS->draggercache == NULL) return NULL; // should not happen
  assert(THIS->draggercache->path);
  SoPath * orgpath = (SoPath *) THIS->draggercache->path;
  return new SoPath(*orgpath);
}

/*!
  Returns the path to the SoInteractionKit that holds the current surrogate
  path.
*/
const SoPath *
SoDragger::getSurrogatePartPickedOwner(void) const
{
  return THIS->surrogateownerpath;
}

/*!
  Returns the name of the path in the SoInteractionKit that holds the current
  surrogate path.
*/
const SbName &
SoDragger::getSurrogatePartPickedName(void) const
{
  return THIS->surrogatename;
}

/*!
  Returns the current surrogate path.
*/
const SoPath *
SoDragger::getSurrogatePartPickedPath(void) const
{
  return THIS->surrogatepath;
}

/*!
  Sets the staring point for the drag. \a point is usually a
  picked point from a SoRayPickAction.
*/
void
SoDragger::setStartingPoint(const SoPickedPoint * point)
{
  THIS->startingpoint = point->getPoint();
}

/*!
  Sets the starting point for a drag.
*/
void
SoDragger::setStartingPoint(const SbVec3f & point)
{
  THIS->startingpoint = point;
}

/*!
  Return the current view volume.
*/
const SbViewVolume &
SoDragger::getViewVolume(void)
{
  return THIS->viewvolume;
}

/*!
  Sets the current view volume.
*/
void
SoDragger::setViewVolume(const SbViewVolume & vv)
{
  THIS->viewvolume = vv;
}

/*!
  Returns the current viewport region.
*/
const SbViewportRegion &
SoDragger::getViewportRegion(void)
{
  return THIS->viewport;
}

/*!
  Sets the current viewport region.
*/
void
SoDragger::setViewportRegion(const SbViewportRegion & vp)
{
  THIS->viewport = vp;
}

/*!
  Return the current (most recent) SoHandleEventAction.
*/
SoHandleEventAction *
SoDragger::getHandleEventAction(void) const
{
  return THIS->eventaction;
}

/*!
  Stores a handle event action.
*/
void
SoDragger::setHandleEventAction(SoHandleEventAction * action)
{
  THIS->eventaction = action;
}

/*!
  This function is part of the original SGI Inventor 2.1 API, but has
  not been implemented in Coin as it looks like a function which
  should probably have been private in Open Inventor.
*/
void
SoDragger::setTempPathToThis(const SoPath *)
{
  COIN_OBSOLETED();
}

/*!
  Called when dragger starts grabbing events (mouse button down).
  Overload if you need to do something extra in your dragger.
  \sa grabEventCleanup()
*/
void
SoDragger::grabEventsSetup(void)
{
  assert(THIS->eventaction);
  THIS->eventaction->setGrabber(this);
}

/*!
  Called when dragger stops grabbing events (mouse button up).
  \sa grabEventSetup()
*/
void
SoDragger::grabEventsCleanup(void)
{
  assert(THIS->eventaction);
  THIS->eventaction->releaseGrabber();
}

/*!
  Examines the fields of the dragger, changes the matrix according to
  those fields and leaves the rest of the matrix as it was.  The
  following field names are supported: translation, scaleFactor,
  rotation and scaleOrientation.
*/
void
SoDragger::workFieldsIntoTransform(SbMatrix & matrix)
{
  SoSFVec3f * vecfield;
  SoSFRotation * rotfield;
  const SbVec3f * translation = NULL;
  const SbVec3f * scaleFactor = NULL;
  const SbRotation * rotation = NULL;
  const SbRotation * scaleOrientation = NULL;
  const SbVec3f * center = NULL;

  vecfield = (SoSFVec3f *)this->getField("translation");
  if (vecfield) translation = &vecfield->getValue();

  vecfield = (SoSFVec3f *)this->getField("scaleFactor");
  if (vecfield) scaleFactor = &vecfield->getValue();

  vecfield = (SoSFVec3f *)this->getField("center");
  if (vecfield) center = &vecfield->getValue();

  rotfield = (SoSFRotation *)this->getField("rotation");
  if (rotfield) rotation = &rotfield->getValue();

  rotfield = (SoSFRotation *)this->getField("scaleOrientation");
  if (rotfield) scaleOrientation = &rotfield->getValue();

  this->workValuesIntoTransform(matrix, translation, rotation,
                                scaleFactor, scaleOrientation, center);
}

/*!
  Controls the behaviour of the SbProjector.
*/
void
SoDragger::setFrontOnProjector(ProjectorFrontSetting val)
{
  THIS->frontonprojector = val;
}

/*!
  Returns the behaviour of the SbProjector.
*/
SoDragger::ProjectorFrontSetting
SoDragger::getFrontOnProjector(void) const
{
  return THIS->frontonprojector;
}

/*!
  Sets the minimum scale value all scale factors are clamped against.
  This is used in workFieldsIntoTransform(). The default value is 0.01
*/
void
SoDragger::setMinScale(float minscale)
{
  SoDragger::minscale = minscale;
}

/*!
  Returns the minimum scale value.
  \sa setMinScale()
*/
float
SoDragger::getMinScale(void)
{
  return SoDragger::minscale;
}

/*!
  Same as above, but pointers to values are supplied. If a pointer is
  \c NULL, the matrix value for that argument is used when
  reconstructing the matrix.
*/
void
SoDragger::workValuesIntoTransform(SbMatrix & matrix, const SbVec3f * translationptr, const SbRotation * rotationptr, const SbVec3f * scalefactorptr, const SbRotation * scaleorientationptr, const SbVec3f * centerptr)
{
  SbVec3f t, s;
  SbRotation r, so;
  if (centerptr) matrix.getTransform(t, r, s, so, *centerptr);
  else matrix.getTransform(t, r, s, so);

  if (translationptr) t = *translationptr;
  if (rotationptr) r = *rotationptr;
  if (scalefactorptr) s = *scalefactorptr;
  if (scaleorientationptr) so = *scaleorientationptr;

  if (centerptr) matrix.setTransform(t, r, s, so, *centerptr);
  else matrix.setTransform(t, r, s, so);
}

/*!
  Can be used when there is no scaleorientation. Faster than
  SoDragger::workValuesIntoTransform().
*/
void
SoDragger::getTransformFast(SbMatrix & matrix, SbVec3f & translation,
                            SbRotation & rotation, SbVec3f & scalefactor,
                            SbRotation & scaleorientation,
                            const SbVec3f & center)
{
  // FIXME: faster code not implemented, we just forward the call to
  // workValuesIntoTransform() anyway. 20011219 mortene.

  SoDragger::workValuesIntoTransform(matrix, &translation, &rotation,
                                     &scalefactor,
                                     &scaleorientation, &center);
}

/*!
  \overload
*/
void
SoDragger::getTransformFast(SbMatrix & matrix, SbVec3f & translation,
                            SbRotation & rotation, SbVec3f & scalefactor,
                            SbRotation & scaleorientation)
{
  // FIXME: faster code not implemented, we just forward the call to
  // workValuesIntoTransform() anyway. 20011219 mortene.

  SoDragger::workValuesIntoTransform(matrix, &translation, &rotation,
                                     &scalefactor,
                                     &scaleorientation, NULL);
}

/*!
  Returns \a matrix after \a translation has been appended.  If \a
  conversion != \c NULL it is used to transform \a translation into
  the space \a matrix is defined.
*/
SbMatrix
SoDragger::appendTranslation(const SbMatrix & matrix, const SbVec3f & translation, const SbMatrix * conversion)
{
  SbMatrix transform;
  transform.setTranslate(translation);
  if (conversion) {
    transform.multRight(*conversion);
    transform.multLeft(conversion->inverse());
  }
  SbMatrix res = matrix;
  return res.multLeft(transform);
}

/*!
  Returns \a matrix after \a scale and \a scalecenter has been
  appended.  If \a conversion != \c NULL it is used to transform scale
  into the space \a matrix is defined.
*/

SbMatrix
SoDragger::appendScale(const SbMatrix & matrix, const SbVec3f & scale, const SbVec3f & scalecenter, const SbMatrix * conversion)
{
  SbVec3f clampedscale;
  // The explicit casts are done to humour the HPUX aCC compiler,
  // which will otherwise say ``Template deduction failed to find a
  // match for the call to 'SbMax'''.  mortene.
  clampedscale[0] = SbMax((float)scale[0], SoDragger::minscale);
  clampedscale[1] = SbMax((float)scale[1], SoDragger::minscale);
  clampedscale[2] = SbMax((float)scale[2], SoDragger::minscale);

  SbMatrix transform, tmp;
  transform.setTranslate(-scalecenter);
  tmp.setScale(clampedscale);
  transform.multRight(tmp);
  tmp.setTranslate(scalecenter);
  transform.multRight(tmp);

  if (conversion) {
    transform.multRight(*conversion);
    transform.multLeft(conversion->inverse());
  }
  SbMatrix res = matrix;
  return res.multLeft(transform);
}

/*!
  Appends \a rot, around \a rotcenter, to \a matrix. If \a conversion
  is != \c NULL, this is used to move the rotation into that
  coordinate systems before appending the rotation.
*/
SbMatrix
SoDragger::appendRotation(const SbMatrix & matrix, const SbRotation & rot, const SbVec3f & rotcenter, const SbMatrix * conversion)
{
  SbMatrix transform, tmp;
  transform.setTranslate(-rotcenter);
  tmp.setRotate(rot);
  transform.multRight(tmp);
  tmp.setTranslate(rotcenter);
  transform.multRight(tmp);
  if (conversion) {
    transform.multRight(*conversion);
    transform.multLeft(conversion->inverse());
  }
  transform.multRight(matrix);
  return transform;
}

/*!
  Returns the position of the locater.
*/
SbVec2f
SoDragger::getNormalizedLocaterPosition(void)
{
  if (THIS->currentevent) {
    return THIS->currentevent->getNormalizedPosition(THIS->viewport);
  }
#if COIN_DEBUG && 1 // debug
  SoDebugError::postInfo("SoDragger::getLocaterPosition",
                         "current event is not set");
#endif // debug
  return SbVec2f(0, 0);
}

/*!
  \overload
*/
SbVec2s
SoDragger::getLocaterPosition(void)
{
  if (THIS->currentevent) {
    return THIS->currentevent->getPosition();
  }
#if COIN_DEBUG && 1 // debug
  SoDebugError::postInfo("SoDragger::getLocaterPosition",
                         "current event is not set");
#endif // debug
  return SbVec2s(0, 0);
}

/*!
  Returns the position when mouse button 1 was pressed.
*/
SbVec2s
SoDragger::getStartLocaterPosition(void) const
{
  return THIS->startlocaterpos;
}

/*!
  The start locater position is automatically set when mouse button 1 goes
  down, but subclasses can use this method to reset the value.
*/
void
SoDragger::setStartLocaterPosition(SbVec2s pos)
{
  THIS->startlocaterpos = pos;
}

/*!
  Checks if the mouse pointer has been moved enough after the end-user
  hit a constraint mode key (which is typically \c SHIFT, sometimes
  also \c CTRL for the built-in draggers) that we should act upon and
  decide which direction the constraint should be set to.
*/
SbBool
SoDragger::isAdequateConstraintMotion(void)
{
  SbVec2s delta =
    this->getStartLocaterPosition() -
    this->getLocaterPosition();

  // The cast is done to avoid HPUX aCC failing with an "ambiguity
  // error", as sqrt() can be either "long double sqrt(long double)"
  // or "float sqrt(float)".  mortene.
  double len = sqrt(double(delta[0]*delta[0] + delta[1]*delta[1]));

  if (len >= (double) THIS->mingesture) return TRUE;
  return FALSE;
}

/*!
  Checks if \a pickpath contains \a surrogatepath and returns \c TRUE
  if the tail of \a surrogatepath is before any dragger in \a
  pickpath.
*/
SbBool
SoDragger::shouldGrabBasedOnSurrogate(const SoPath * pickpath, const SoPath * surrogatepath)
{
  if (!pickpath->containsPath(surrogatepath)) return FALSE;

  SoFullPath * pick = (SoFullPath*) pickpath;
  SoFullPath * surr = (SoFullPath*) surrogatepath;

  SoNode * tail = surr->getTail();
  SoType draggertype = SoDragger::getClassTypeId();

  for (int i = pick->getLength()-1; i >= 0; i--) {
    SoNode * node = pick->getNode(i);
    if (node == tail) return TRUE;
    if (node->isOfType(draggertype))
      return FALSE;
  }
  return FALSE;
}

/*!
  Store data about the current camera in the given action.
*/
void
SoDragger::setCameraInfo(SoAction * action)
{
  SoState * state = action->getState();
  THIS->viewvolume = SoViewVolumeElement::get(state);
  THIS->viewport = SoViewportRegionElement::get(state);;
}

// Documented in superclass. Overridden to detect picks on dragger.
void
SoDragger::handleEvent(SoHandleEventAction * action)
{
  const SoEvent * event = action->getEvent();

  if (this->isActive.getValue() || this->getActiveChildDragger()) {
    if (!action->getGrabber())
      this->updateDraggerCache(action->getCurPath());
    else
      this->updateDraggerCache(NULL);
  }

  // this is a special case, to be able to detect when somebody
  // clicks ctrl over a dragger. This has a special meaning for
  // some draggers, and it's the only time the otherEvent callbacks
  // are called when the dragger is not active.
  //
  if (!this->isActive.getValue() &&
      (SO_KEY_PRESS_EVENT(event, LEFT_CONTROL) ||
       SO_KEY_PRESS_EVENT(event, RIGHT_CONTROL))) {
    const SoPickedPoint * pp = action->getPickedPoint();
    if (pp && this->isPicked(pp->getPath())) {
      this->eventHandled(event, action);
      THIS->otherEventCB.invokeCallbacks(this);
    }
  }
  else if (SO_MOUSE_PRESS_EVENT(event, BUTTON1)) {
    const SoPickedPoint * pp = action->getPickedPoint();

    SbBool didpick = FALSE;

    if (pp && this->isPicked(pp->getPath())) didpick = TRUE;
    else if (pp) { // check surrogate paths
      SoPath * owner, * path;
      SbName name;
      if (this->isPathSurrogateInMySubgraph(pp->getPath(), owner, name, path)) {
        owner->ref();
        path->ref();
        if (this->shouldGrabBasedOnSurrogate(pp->getPath(), path)) {
          if (THIS->surrogateownerpath) THIS->surrogateownerpath->unref();
          THIS->surrogateownerpath = owner;
          THIS->surrogateownerpath->ref();
          if (THIS->surrogatepath) THIS->surrogatepath->unref();
          THIS->surrogatepath = path;
          THIS->surrogatepath->ref();
          THIS->surrogatename = name;
          didpick = TRUE;
        }
        owner->unref();
        path->unref();
      }
    }
    if (didpick) {
      if (!action->getGrabber())
        this->updateDraggerCache(action->getCurPath());
      else
        this->updateDraggerCache(NULL);

      this->isActive = TRUE;
      this->setCameraInfo(action);
      this->setStartingPoint(pp);
      this->eventHandled(event, action);
      if (THIS->pickedpath) THIS->pickedpath->unref();
      THIS->pickedpath = pp->getPath();
      THIS->pickedpath->ref();

      THIS->startlocaterpos = event->getPosition();
      THIS->isgrabbing = FALSE;
      this->saveStartParameters();
      THIS->startCB.invokeCallbacks(this);
    }
  }
  else if (this->isActive.getValue() && SO_MOUSE_RELEASE_EVENT(event, BUTTON1)) {
    this->isActive = FALSE;
    this->eventHandled(event, action);
    if (THIS->isgrabbing) this->grabEventsCleanup();
    if (THIS->pickedpath) {
      THIS->pickedpath->unref();
      THIS->pickedpath = NULL;
    }
    THIS->finishCB.invokeCallbacks(this);
    THIS->draggercache->truncatePath();
  }
  else if (this->isActive.getValue() && event->isOfType(SoLocation2Event::getClassTypeId())) {
    this->eventHandled(event, action);
    THIS->motionCB.invokeCallbacks(this);
    if (!THIS->isgrabbing) {
      this->grabEventsSetup();
      THIS->isgrabbing = TRUE;
    }
  }
  else if (this->isActive.getValue()) {
    THIS->eventaction = action;
    THIS->currentevent = event;
    THIS->otherEventCB.invokeCallbacks(this);
  }
  if (!action->isHandled())
    inherited::handleEvent(action);
}

/*!
  \COININTERNAL
*/
void
SoDragger::transferMotion(SoDragger * child)
{
  SbMatrix childmatrix = child->getMotionMatrix();
  child->setMotionMatrix(SbMatrix::identity());
  child->transformMatrixLocalToWorld(childmatrix, childmatrix);
  this->transformMatrixWorldToLocal(childmatrix, childmatrix);

  SbMatrix matrix = this->getStartMotionMatrix();
  matrix.multLeft(childmatrix);
  this->setMotionMatrix(matrix);
}

/*!
  Sets whether dragger geometry should be ignored when calculating bbox.
*/
void
SoDragger::setIgnoreInBbox(SbBool val)
{
  THIS->ignoreinbbox = val;
}

/*!
  Returns whether dragger geometry should be ignored when calculating bbox.
*/
SbBool
SoDragger::isIgnoreInBbox(void)
{
  return THIS->ignoreinbbox;
}

// Documented in superclass. Overridden to ignore dragger bounding box
// if SoDragger::isIgnoreInBbox() is \c TRUE.
void
SoDragger::getBoundingBox(SoGetBoundingBoxAction * action)
{
  if (!THIS->ignoreinbbox) inherited::getBoundingBox(action);

}

/*!
  \COININTERNAL
*/
void
SoDragger::setActiveChildDragger(SoDragger * childdragger)
{
  THIS->activechilddragger = childdragger;
}

/*!
  \COININTERNAL
*/
SoDragger *
SoDragger::getActiveChildDragger(void) const
{
  return THIS->activechilddragger;
}

// Documented in superclass. Overridden to set default on
// SoDragger::isActive, SoDragger::motionMatrix and on common
// subdragger fields: translation, center, scaleFactor and rotation.
void
SoDragger::setDefaultOnNonWritingFields(void)
{
#define CHECK_DEFAULT(name, type, val) \
  f = (SoField*) this->getField(name); \
  if (f && !(f->isConnectionEnabled() && f->isConnected())) { \
    if (((type*)f)->getValue() == val) f->setDefault(TRUE); \
  } \
  f = NULL

  // check common subdragger fields
  SoField * f;
  CHECK_DEFAULT("translation", SoSFVec3f, SbVec3f(0.0f, 0.0f, 0.0f));
  CHECK_DEFAULT("center", SoSFVec3f, SbVec3f(0.0f, 0.0f, 0.0f));
  CHECK_DEFAULT("scaleFactor", SoSFVec3f, SbVec3f(1.0f, 1.0f, 1.0f));
  CHECK_DEFAULT("rotation", SoSFRotation, SbRotation::identity());

  // check isActive
  CHECK_DEFAULT("isActive", SoSFBool, FALSE);
#undef CHECK_DEFAULT

  this->motionMatrix.setDefault(TRUE);

  inherited::setDefaultOnNonWritingFields();
}

/*!
  \COININTERNAL
*/
void
SoDragger::childTransferMotionAndValueChangedCB(void * data, SoDragger * child)
{
  SoDragger * thisp = (SoDragger *)data;
  child->removeValueChangedCallback(SoDragger::childTransferMotionAndValueChangedCB, thisp);
  thisp->transferMotion(child);
  child->addValueChangedCallback(SoDragger::childTransferMotionAndValueChangedCB, thisp);
}

/*!
  \COININTERNAL
*/
void
SoDragger::childValueChangedCB(void * data, SoDragger * child)
{
  SoDragger * thisp = (SoDragger *)data;
  thisp->valueChanged();
}

/*!
  \COININTERNAL
*/
void
SoDragger::childStartCB(void * data, SoDragger * child)
{
  SoDragger * thisp = (SoDragger *)data;
  thisp->saveStartParameters();
  thisp->setActiveChildDragger(child);
  THISP->startCB.invokeCallbacks(thisp);
}

/*!
  \COININTERNAL
*/
void
SoDragger::childMotionCB(void * data, SoDragger * child)
{
  SoDragger * thisp = (SoDragger *)data;
  THISP->motionCB.invokeCallbacks(thisp);
}

/*!
  \COININTERNAL
*/
void
SoDragger::childFinishCB(void * data, SoDragger * child)
{
  SoDragger * thisp = (SoDragger *)data;
  THISP->finishCB.invokeCallbacks(thisp);
  thisp->setActiveChildDragger(NULL);
}

/*!
  \COININTERNAL
*/
void
SoDragger::childOtherEventCB(void * data, SoDragger * child)
{
  SoDragger * thisp = (SoDragger *)data;
  THISP->currentevent = child->pimpl->currentevent;
  THISP->eventaction = child->pimpl->eventaction;
  THISP->otherEventCB.invokeCallbacks(thisp);
}

// Returns whether path goes through this node (dragger is picked).
SbBool
SoDragger::isPicked(SoPath * path)
{
  // last dragger in path must be this one
  SoFullPath * fullpath = (SoFullPath *)path;

  int i = fullpath->findNode(this);
  if (i < 0) return FALSE;

  int n = fullpath->getLength();
  for (++i; i < n; i++) {
    SoNode * node = fullpath->getNode(i);
    if (node->isOfType(SoDragger::getClassTypeId())) return FALSE;
  }
  return TRUE;
}

void
SoDragger::eventHandled(const SoEvent * event, SoHandleEventAction * action)
{
  action->setHandled();
  THIS->currentevent = event;
  THIS->eventaction = action;
}

/*!
  \COININTERNAL
*/
void
SoDragger::updateDraggerCache(const SoPath * path)
{
  if (THIS->draggercache == NULL) THIS->draggercache = new SoDraggerCache(this);
  if (path) THIS->draggercache->update((const SoFullPath *)path, path->findNode(this));
  else THIS->draggercache->updateMatrix();
}

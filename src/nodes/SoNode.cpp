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
  \class SoNode SoNode.h Inventor/nodes/SoNode.h
  \brief The SoNode class is the base class for nodes used in scene graphs.
  \ingroup nodes

  Coin is a \e retained \e mode 3D visualization library (built on top
  of the \e immediate \e mode OpenGL library). "Retained mode" means
  that instead of passing commands to draw graphics primitives
  directly to the renderer, you build up data structures which are
  rendered by the library \e on \e demand.

  The node classes are the main "primitive" for building these data
  structures. In Coin, you build tree hierarchies made up of different
  node types: group nodes (for the tree structure layout of the other
  nodes), appearance nodes (for setting up materials, textures, etc),
  shape nodes (for the actual geometry), and nodes for lighting and
  camera positioning.

  One common issue with newcomers to the API is that you should not
  and can not use the C++ delete operator on nodes -- the destructor
  is protected. This is because node instances are using a common
  technique for memory resource handling called "reference
  counting". Nodes are deleted (actually, they delete themselves) when
  their unref() method is called and the reference count goes to zero.

  Usually application programmers won't manually ref() and unref()
  nodes a lot, because you pass the nodes directly to
  SoGroup::addChild() or So*Viewer::setSceneGraph() or something
  similar.  These functions will ref() the nodes they are passed, and
  unref() them when they are finished with them.

  Make sure you do ref() nodes that you keep pointers to so they
  aren't accidentally deleted prematurely due to an unref() call from
  within the library itself.  If you haven't manually called ref() on
  a top-level root node, it will then be deleted automatically. This
  code shows how to do it:

  \code
  SoSeparator * root = new SoSeparator; // root's refcount starts out at zero
  root->addChild(foo_node); // foo_node refcount is increased by 1
  root->addChild(bar_node); // bar_node refcount +1

  // increase refcount before passing it to setScenegraph(), to avoid
  // premature destruction
  root->ref();

  myviewer->setSceneGraph(root); // root's refcount +1, is now 2

  // [misc visualization and processing]

  // myviewer will let go of it's reference to the root node, thereby
  // decreasing it's referencecount by 1
  myviewer->setSceneGraph(NULL);

  // root's refcount goes from +1 to 0, and it will self-destruct controllably
  root->unref();
  // avoid dangling pointer, in case "root" is attempted used again
  // (not really necessary, but good for smoking out bugs early)
  root = NULL;
  \endcode

  For full information and tutorial-style introductions to all API
  issues, see the "Inventor Mentor: Programming Object-Oriented 3D
  Graphics with Open Inventor" (ISBN 0-201-62495-8). It has detailed
  explanations on all the basic principles involved.

  See specifically the section "References and Deletion" in Chapter 3
  to learn about the reference counting techniques.
*/

#include <Inventor/SoOutput.h>
#include <Inventor/actions/SoActions.h>
#include <Inventor/errors/SoDebugError.h>
#include <Inventor/misc/SoChildList.h>
#include <Inventor/nodes/SoNodes.h>
#include <Inventor/nodes/SoUnknownNode.h>
#include <Inventor/nodes/SoTextureScalePolicy.h> // possible part of public API in the future
#include <Inventor/elements/SoCacheElement.h>
#include <Inventor/SoInput.h>
#include <assert.h>

#if HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H

#include <Inventor/system/gl.h> // glGetError


/*!
  \var uint32_t SoNode::uniqueId
  \internal
*/
/*!
  \var uint32_t SoNode::nextUniqueId
  \internal
*/
/*!
  \var int SoNode::nextActionMethodIndex
  \internal
*/

/*!
  \enum SoNode::NodeType
  Used to store node type.
*/

/*!
  \var SoNode::NodeType SoNode::INVENTOR
  Specifies Inventor node type.
*/

/*!
  \var SoNode::NodeType SoNode::VRML1
  Spcifies VRML V1.0 node type.
*/

/*!
  \var SoNode::NodeType SoNode::VRML2
  Spcifies VRML V2.0 node type.
*/

/*!
  \var SoNode::NodeType SoNode::PROTO_INSTANCE_ROOT
  Spcifies a Proto instance root node.
*/

uint32_t SoNode::nextUniqueId = 0;
int SoNode::nextActionMethodIndex = 0;
// Don't set value explicitly to SoType::badType(), to avoid a bug in
// Sun CC v4.0. (Bitpattern 0x0000 equals SoType::badType()).
SoType SoNode::classTypeId;

static void
init_action_methods(void);

// Overridden from parent.
SoType
SoNode::getClassTypeId(void)
{
  return SoNode::classTypeId;
}

// defines for node state flags

#define FLAG_TYPEMASK 0x07 // max  8 types for now. Must be in LSBs
#define FLAG_OVERRIDE 0x08

// private methods. Inlined inside this file only.

// clear bits in stateflags
inline void
SoNode::clearStateFlags(const unsigned int bits)
{
  this->stateflags &= ~bits;
}

// sets bits in stateflags
inline void
SoNode::setStateFlags(const unsigned int bits)
{
  this->stateflags |= bits;
}

// return TRUE if any of bits are set
inline SbBool
SoNode::getState(const unsigned int bits) const
{
  return (this->stateflags & bits) != 0;
}

/*!
  Default constructor, initializes node instance.
*/
SoNode::SoNode(void)
{
  this->uniqueId = SoNode::nextUniqueId++;
  this->stateflags = 0; // clear all flags

  // set node type to Inventor by default.
  this->setNodeType(SoNode::INVENTOR);
}

/*!
  Destructor.
*/
SoNode::~SoNode()
{
#if COIN_DEBUG && 0 // debug
  SoDebugError::postInfo("SoNode::~SoNode", "%p", this);
#endif // debug
}

/*!
  Make a duplicate of this node and return a pointer to the duplicate.

  If this node is a group node, children are also copied and we return
  a pointer to the root of a full copy of the subgraph rooted here.

  If \a copyconnections is \c TRUE, we also copy the connections to
  fields within this node (and ditto for any children and children's
  children etc).


  Note that this function has been made virtual in Coin, which is not
  the case in the original Open Inventor API. We may change this
  method back into being non-virtual again for Coin version 2, as it
  was made virtual more or less by mistake. So please don't write
  application code that depends on SoNode::copy() being virtual.

  The reason this method should not be virtual is because this is \e
  not the function the application programmer should override if she
  needs some special behavior during a copy operation (like copying
  the value of internal data not exposed as fields).

  For that purpose, override the copyContents() method. Your
  overridden copyContents() method should then \e both copy internal
  data aswell as calling the parent superclass' copyContents() method
  for automatically handling of fields and other common data.
*/
SoNode *
SoNode::copy(SbBool copyconnections) const
{
  // FIXME: "de-virtualize" this method for Coin 2? See method
  // documentation above. 20011220 mortene.


  SoFieldContainer::initCopyDict();
  (void)this->addToCopyDict();
  // Call findCopy() to have copyContents() run only once.
  SoNode * cp = (SoNode *)SoFieldContainer::findCopy(this, copyconnections);
  SoFieldContainer::copyDone();
  return cp;
}


// Overridden from parent.
void
SoNode::startNotify(void)
{
  inherited::startNotify();
}

// Overridden from parent.
void
SoNode::notify(SoNotList * l)
{
#if COIN_DEBUG && 0 // debug
  SoDebugError::postInfo("SoNode::notify", "node %p (%s \"%s\"), list %p",
                         this, this->getTypeId().getName().getString(),
                         this->getName().getString(), l);
#endif // debug

  // only continue if node hasn't already been notified.
  // The time stamp is set in the SoNotList constructor.
  if (l->getTimeStamp() > this->uniqueId) {
    this->uniqueId = SoNode::nextUniqueId++;
    inherited::notify(l);
  }
}

/*!
  \internal
*/
int
SoNode::getActionMethodIndex(const SoType type)
{
  return type.getData();
}

/*!
  \internal

  Only in TGS Inventor on Win32 -- to avoid needing to export the
  nextActionMethodIndex member, see SoNode.h for more info.
 */
void
SoNode::setNextActionMethodIndex(int index)
{
  SoNode::nextActionMethodIndex = index;
}

/*!
  \internal

  Only in TGS Inventor on Win32 -- to avoid needing to export the
  nextActionMethodIndex member, see SoNode.h for more info.
*/
int
SoNode::getNextActionMethodIndex(void)
{
  return SoNode::nextActionMethodIndex;
}

/*!
  \internal

  Only in TGS Inventor on Win32 -- to avoid needing to export the
  nextActionMethodIndex member, see SoNode.h for more info.
 */
void
SoNode::incNextActionMethodIndex(void)
{
  SoNode::nextActionMethodIndex++;
}

// doc in super
void
SoNode::initClass(void)
{
  // Make sure we only initialize once.
  assert(SoNode::classTypeId == SoType::badType());
  // Make sure parent class has been initialized.
  assert(inherited::getClassTypeId() != SoType::badType());

  SoNode::classTypeId =
    SoType::createType(inherited::getClassTypeId(), "Node", NULL,
                       SoNode::nextActionMethodIndex++);

  SoNode::initClasses();

  // action methods must be initialized here, since both nodes and
  // actions must be initialized before we can use
  // SO_ACTION_ADD_METHOD
  init_action_methods();
}

/*!
  Initialize all the node classes of Coin.
*/
void
SoNode::initClasses(void)
{
  SoCamera::initClass();
  SoPerspectiveCamera::initClass();
  SoOrthographicCamera::initClass();
  SoShape::initClass();
  SoAsciiText::initClass();
  SoCone::initClass();
  SoCube::initClass();
  SoCylinder::initClass();
  SoVertexShape::initClass();
  SoNonIndexedShape::initClass();
  SoFaceSet::initClass();
  SoLineSet::initClass();
  SoPointSet::initClass();
  SoMarkerSet::initClass();
  SoQuadMesh::initClass();
  SoTriangleStripSet::initClass();
  SoIndexedShape::initClass();
  SoIndexedFaceSet::initClass();
  SoIndexedLineSet::initClass();
  SoIndexedTriangleStripSet::initClass();
  SoImage::initClass();
  SoIndexedNurbsCurve::initClass();
  SoIndexedNurbsSurface::initClass();
  SoNurbsCurve::initClass();
  SoNurbsSurface::initClass();
  SoSphere::initClass();
  SoText2::initClass();
  SoText3::initClass();
  SoGroup::initClass();
  SoSeparator::initClass();
  SoAnnotation::initClass();
  SoLocateHighlight::initClass();
  SoWWWAnchor::initClass();
  SoArray::initClass();
  SoSwitch::initClass();
  SoBlinker::initClass();
  SoLOD::initClass();
  SoLevelOfDetail::initClass();
  SoMultipleCopy::initClass();
  SoPathSwitch::initClass();
  SoTransformSeparator::initClass();
  SoTransformation::initClass();
  SoMatrixTransform::initClass();
  SoRotation::initClass();
  SoPendulum::initClass();
  SoRotor::initClass();
  SoResetTransform::initClass();
  SoRotationXYZ::initClass();
  SoScale::initClass();
  SoTranslation::initClass();
  SoShuttle::initClass();
  SoTransform::initClass();
  SoUnits::initClass();
  SoBaseColor::initClass();
  SoCallback::initClass();
  SoClipPlane::initClass();
  SoColorIndex::initClass();
  SoComplexity::initClass();
  SoCoordinate3::initClass();
  SoCoordinate4::initClass();
  SoLight::initClass();
  SoDirectionalLight::initClass();
  SoSpotLight::initClass();
  SoPointLight::initClass();
  SoDrawStyle::initClass();
  SoEnvironment::initClass();
  SoEventCallback::initClass();
  SoFile::initClass();
  SoFont::initClass();
  SoFontStyle::initClass();
  SoInfo::initClass();
  SoLabel::initClass();
  SoLightModel::initClass();
  SoProfile::initClass();
  SoLinearProfile::initClass();
  SoNurbsProfile::initClass();
  SoMaterial::initClass();
  SoMaterialBinding::initClass();
  SoNormal::initClass();
  SoNormalBinding::initClass();
  SoPackedColor::initClass();
  SoPickStyle::initClass();
  SoPolygonOffset::initClass();
  SoProfileCoordinate2::initClass();
  SoProfileCoordinate3::initClass();
  SoShapeHints::initClass();
  SoTexture2::initClass();
  SoTexture3::initClass();
  SoTexture2Transform::initClass();
  SoTexture3Transform::initClass();
  SoTextureCoordinate2::initClass();
  SoTextureCoordinate3::initClass();
  SoTextureCoordinateBinding::initClass();
  SoTextureCoordinateFunction::initClass();
  SoTextureCoordinateDefault::initClass();
  SoTextureCoordinateEnvironment::initClass();
  SoTextureCoordinatePlane::initClass();
  SoUnknownNode::initClass();
  SoVertexProperty::initClass();
  SoWWWInline::initClass();

  SoTransparencyType::initClass();

  // this node are not part of the public API yet.
  SoTextureScalePolicy::initClass();
}

/*!
  Set the override flag.

  If this flag is \c TRUE, the field values of this node will override
  the field values of other nodes of the same type during scene graph
  traversal.

  A common applicaton for "override nodes" is to place them at the top
  of the tree as a convenient way to force e.g. a common drawstyle on
  the complete tree.
*/
void
SoNode::setOverride(const SbBool state)
{
  if (state != this->getState(FLAG_OVERRIDE)) {
    // This change affects caches in the tree, so we must change our id
    // setting, so the caches are regenerated.
    this->uniqueId = SoNode::nextUniqueId++;

    if (state) this->setStateFlags(FLAG_OVERRIDE);
    else this->clearStateFlags(FLAG_OVERRIDE);
  }
}

/*!
  Return status of override flag.

  \sa setOverride()
*/
SbBool
SoNode::isOverride(void) const
{
  return this->getState(FLAG_OVERRIDE);
}

/*!
  Sets the node type for this node to \a type. Since some nodes
  should be handled differently in VRML1 vs. Inventor, this
  should be used to get correct behavior for those cases.
  The default node type is INVENTOR.

  This method is an extension versus the Open Inventor API.

  \sa getNodeType()
*/
void
SoNode::setNodeType(const NodeType type)
{
  // make sure we have enogh bits to store this type
  assert((uint32_t) type <= FLAG_TYPEMASK);
  // clear old type
  this->clearStateFlags(FLAG_TYPEMASK);
  // set new type
  this->setStateFlags((uint32_t) type);
}

/*!
  Returns the node type set for this node.

  This method is an extension versus the Open Inventor API.

  \sa setNodeType()
*/
SoNode::NodeType
SoNode::getNodeType(void) const
{
  uint32_t type = this->stateflags & FLAG_TYPEMASK;
  return (NodeType) type;
}

/*!
  Returns the last node that was registered under \a name.
*/
SoNode *
SoNode::getByName(const SbName & name)
{
  SoBase * b = SoBase::getNamedBase(name, SoNode::getClassTypeId());
  if (!b) return NULL;
  return (SoNode *)b;
}

/*!
  Finds all nodes with \a name and appends them to the \a l nodelist.
  Returns the number of nodes with the specified name.
*/
int
SoNode::getByName(const SbName & name, SoNodeList & l)
{
  SoBaseList bl;
  int nr = SoBase::getNamedBases(name, bl, SoNode::getClassTypeId());
  for (int i=0; i < nr; i++) l.append((SoNode *)bl[i]);
  return nr;
}


// *************************************************************************
// * ACTION STUFF
// *************************************************************************

/*!
  This function performs the typical operation of a node for any
  action.
*/
void
SoNode::doAction(SoAction * action)
{
}

// Note that this documentation will also be used for all subclasses
// which reimplements the method, so keep the doc "generic enough".
/*!
  Returns \c TRUE if the node could have any effect on the state
  during traversal.

  If it returns \c FALSE, no data in the traversal-state will change
  from the pre-traversal state to the post-traversal state. The
  SoSeparator node will for instance return \c FALSE, as it pushes and
  pops the state before and after traversal of its children. All
  SoShape nodes will also return \c FALSE, as just pushing out
  geometry data to the rendering engine won't affect the actual
  rendering state.

  The default method returns \c TRUE, on a "better safe than sorry"
  philosophy.
*/
SbBool
SoNode::affectsState(void) const
{
  return TRUE;
}

/*!
  This is a static "helper" method registered with the action, and
  used for calling the SoNode::getBoundingBox() virtual method which
  does the \e real work.
*/
void
SoNode::getBoundingBoxS(SoAction * action, SoNode * node)
{
  assert(action && node);
  SoGetBoundingBoxAction * bboxaction = (SoGetBoundingBoxAction *)action;
  bboxaction->checkResetBefore();
  node->getBoundingBox(bboxaction);
  bboxaction->checkResetAfter();
}

// Note that this documentation will also be used for all subclasses
// which reimplements the method, so keep the doc "generic enough".
/*!
  Action method for the SoGetBoundingBoxAction.

  Calculates bounding box and center coordinates for node and modifies
  the values of the \a action to encompass the bounding box for this
  node and to shift the center point for the scene more towards the
  one for this node.

  Nodes influencing how geometry nodes calculates their bounding box
  also overrides this method to change the relevant state variables.
*/
void
SoNode::getBoundingBox(SoGetBoundingBoxAction * action)
{
}

/*!
  This is a static "helper" method registered with the action, and
  used for calling the SoNode::getPrimitiveCount() virtual method
  which does the \e real work.
*/
void
SoNode::getPrimitiveCountS(SoAction * action, SoNode * node)
{
  assert(action && node);
  node->getPrimitiveCount((SoGetPrimitiveCountAction *)action);
}

// Note that this documentation will also be used for all subclasses
// which reimplements the method, so keep the doc "generic enough".
/*!
  Action method for the SoGetPrimitiveCountAction.

  Calculates the number of triangle, line segment and point primitives
  for the node and adds these to the counters of the \a action.

  Nodes influencing how geometry nodes calculates their primitive
  count also overrides this method to change the relevant state
  variables.
*/
void
SoNode::getPrimitiveCount(SoGetPrimitiveCountAction * action)
{
}

/*!
  This is a static "helper" method registered with the action, and
  used for calling the SoNode::GLRender() virtual method which does
  the \e real work.
*/
void
SoNode::GLRenderS(SoAction * action, SoNode * node)
{
  if ((action->getCurPathCode() != SoAction::OFF_PATH) ||
      node->affectsState()) {
    if (((SoGLRenderAction*)action)->abortNow()) {
      SoCacheElement::invalidate(action->getState());
    }
    else {
      node->GLRender((SoGLRenderAction*)action);
    }
  }

  if (COIN_DEBUG) {
    // Note: debugging code like this is also present in
    // SoSeparator::GLRenderBelowPath() and SoState::lazyEvaluate(),
    // but they are default disabled -- even when COIN_DEBUG=1 (due to
    // performance reasons).
    //
    // If you're seeing notifications about GL-errors from here, the
    // first thing to do is to enable those debugging checks too.  You
    // might get lucky and have it pinpointed for you right away. Then
    // again, you might not...

    // FIXME: factorize the code here which is common for all the
    // spots where we test for OpenGL errors into a SoGL
    // function. 20011115 mortene.
    int err = glGetError();
    if (err != GL_NO_ERROR) {
      const char * errorstring;
      switch (err) {
      case GL_INVALID_VALUE:
        errorstring = "GL_INVALID_VALUE";
        break;
      case GL_INVALID_ENUM:
        errorstring = "GL_INVALID_ENUM";
        break;
      case GL_INVALID_OPERATION:
        errorstring = "GL_INVALID_OPERATION";
        break;
      case GL_STACK_OVERFLOW:
        errorstring = "GL_STACK_OVERFLOW";
        break;
      case GL_STACK_UNDERFLOW:
        errorstring = "GL_STACK_UNDERFLOW";
        break;
      case GL_OUT_OF_MEMORY:
        errorstring = "GL_OUT_OF_MEMORY";
        break;
      default:
        errorstring = "Unknown GL error";
        break;
      }
      SoDebugError::postWarning("SoNode::GLRenderS",
                                "GL error: %s, nodetype: %s",
                                errorstring,
                                node->getTypeId().getName().getString());
    }
  }
}

// Note that this documentation will also be used for all subclasses
// which reimplements the method, so keep the doc "generic enough".
/*!
  Action method for the SoGLRenderAction.

  This is called during rendering traversals. Nodes influencing the
  rendering state in any way or who wants to throw geometry primitives
  at OpenGL overrides this method.
*/
void
SoNode::GLRender(SoGLRenderAction * action)
{
}

// Note that this documentation will also be used for all subclasses
// which reimplements the method, so keep the doc "generic enough".
/*!
  Implements the SoAction::BELOW_PATH traversal method for the
  rendering action.
*/
void
SoNode::GLRenderBelowPath(SoGLRenderAction * action)
{
  this->GLRender(action);
}

// Note that this documentation will also be used for all subclasses
// which reimplements the method, so keep the doc "generic enough".
/*!
  Implements the SoAction::IN_PATH traversal method for the rendering
  action.
*/
void
SoNode::GLRenderInPath(SoGLRenderAction * action)
{
  this->GLRender(action);
}

// Note that this documentation will also be used for all subclasses
// which reimplements the method, so keep the doc "generic enough".
/*!
  Implements the SoAction::OFF_PATH traversal method for the rendering
  action.
*/
void
SoNode::GLRenderOffPath(SoGLRenderAction * action)
{
  this->GLRender(action);
}

/*!
  This is a static "helper" method registered with the action, and
  used for calling the SoNode::callback() virtual method which does
  the \e real work.
*/
void
SoNode::callbackS(SoAction * action, SoNode * node)
{
  assert(action && node);
  SoCallbackAction * const cbAction = (SoCallbackAction *)(action);
  if (cbAction->hasTerminated()) return;
  cbAction->setCurrentNode(node);

  cbAction->invokePreCallbacks(node);
  if (cbAction->getCurrentResponse() == SoCallbackAction::CONTINUE) {
    node->callback(cbAction);
  }
  cbAction->invokePostCallbacks(node);
}

// Note that this documentation will also be used for all subclasses
// which reimplements the method, so keep the doc "generic enough".
/*!
  Action method for SoCallbackAction.

  Simply updates the state according to how the node behaves for the
  render action, so the application programmer can use the
  SoCallbackAction for extracting information about the scene graph.
*/
void
SoNode::callback(SoCallbackAction * action)
{
}

/*!
  This is a static "helper" method registered with the action, and
  used for calling the SoNode::getMatrix() virtual method which does
  the \e real work.
*/
void
SoNode::getMatrixS(SoAction * action, SoNode * node)
{
  assert(action && node);
  assert(action->getTypeId() == SoGetMatrixAction::getClassTypeId());
  SoGetMatrixAction * const getMatrixAction = (SoGetMatrixAction *)(action);
  node->getMatrix(getMatrixAction);
}

// Note that this documentation will also be used for all subclasses
// which reimplements the method, so keep the doc "generic enough".
/*!
  Action method for SoGetMatrixAction.

  Updates \a action by accumulating with the transformation matrix of
  this node (if any).
*/
void
SoNode::getMatrix(SoGetMatrixAction * action)
{
}

/*!
  This is a static "helper" method registered with the action, and
  used for calling the SoNode::handleEvent() virtual method which does
  the \e real work.
*/
void
SoNode::handleEventS(SoAction * action, SoNode * node)
{
  assert(action && node);
  assert(action->getTypeId().isDerivedFrom(SoHandleEventAction::getClassTypeId()));
  SoHandleEventAction * handleEventAction = (SoHandleEventAction *)(action);
  node->handleEvent(handleEventAction);
}

// Note that this documentation will also be used for all subclasses
// which reimplements the method, so keep the doc "generic enough".
/*!
  Action method for SoHandleEventAction.

  Inspects the event data from \a action, and processes it if it is
  something which this node should react to.

  Nodes influencing relevant state variables for how event handling is
  done also overrides this method.
*/
void
SoNode::handleEvent(SoHandleEventAction * action)
{
}

/*!
  This is a static "helper" method registered with the action, and
  used for calling the SoNode::pick() virtual method which does the \e
  real work.
*/
void
SoNode::pickS(SoAction * action, SoNode * node)
{
  assert(action && node);
  assert(action->getTypeId().isDerivedFrom(SoPickAction::getClassTypeId()));
  SoPickAction * const pickAction = (SoPickAction *)(action);
  node->pick(pickAction);
}

// Note that this documentation will also be used for all subclasses
// which reimplements the method, so keep the doc "generic enough".
/*!
  Action method for SoPickAction.

  Does common processing for SoPickAction \a action instances.
*/
void
SoNode::pick(SoPickAction * action)
{
}

/*!
  This is a static "helper" method registered with the action, and
  used for calling the SoNode::rayPick() virtual method which does the
  \e real work.
*/
void
SoNode::rayPickS(SoAction * action, SoNode * node)
{
  assert(action && node);
  assert(action->getTypeId().isDerivedFrom(SoRayPickAction::getClassTypeId()));
  SoRayPickAction * const rayPickAction = (SoRayPickAction *)(action);
  node->rayPick(rayPickAction);
}

// Note that this documentation will also be used for all subclasses
// which reimplements the method, so keep the doc "generic enough".
/*!
  Action method for SoRayPickAction.

  Checks the ray specification of the \a action and tests for
  intersection with the data of the node.

  Nodes influencing relevant state variables for how picking is done
  also overrides this method.
*/
void
SoNode::rayPick(SoRayPickAction * action)
{
  // if node has no defined rayPick(), try the pick method
  this->pick(action);
}

/*!
  This is a static "helper" method registered with the action, and
  used for calling the SoNode::search() virtual method which does the
  \e real work.
*/
void
SoNode::searchS(SoAction * action, SoNode * node)
{
  assert(action && node);
  assert(action->getTypeId().isDerivedFrom(SoSearchAction::getClassTypeId()));
  SoSearchAction * const searchAction = (SoSearchAction *)(action);
  node->search(searchAction);
}

// Note that this documentation will also be used for all subclasses
// which reimplements the method, so keep the doc "generic enough".
/*!
  Action method for SoSearchAction.

  Compares the search criteria from the \a action to see if this node
  is a match. Searching is done by matching up \e all criteria set up
  in the SoSearchAction -- if \e any of the requested criteria is a
  miss, the search is not deemed successful for the node.

  \sa SoSearchAction
*/
void
SoNode::search(SoSearchAction * action)
{
  if (action->isFound()) { return; }

  int lookfor = action->getFind();
  SbBool hit = FALSE;

  // Coin v1.0.0 was released with a bug where just one hit out of the
  // criteria would make the search operation on the node successful.
  // Since this doesn't match neither the behavior of SGI Inventor nor
  // the documentation for SoSearchAction, we corrected the behavior
  // for Coin v1.0.1 even though this is on the borderline of what is
  // acceptable for fixing in a minor patch-release update.

  if (lookfor & SoSearchAction::NODE) {
    hit = this == action->getNode();
    if (!hit) { return; }
  }

  if (lookfor & SoSearchAction::NAME) {
    hit = this->getName() == action->getName();
    if (!hit) { return; }
  }

  if (lookfor & SoSearchAction::TYPE) {
    SbBool chkderived;
    SoType searchtype = action->getType(chkderived);
    hit = (this->getTypeId() == searchtype) ||
      (chkderived && this->getTypeId().isDerivedFrom(searchtype));
    if (!hit) { return; }
  }

  if (hit) { action->addPath(action->getCurPath()->copy()); }
}

/*!
  This is a static "helper" method registered with the action, and
  used for calling the SoNode::write() virtual method which does the
  \e real work.
*/
void
SoNode::writeS(SoAction * action, SoNode * node)
{
  assert(action && node);
  assert(action->getTypeId().isDerivedFrom(SoWriteAction::getClassTypeId()));
  SoWriteAction * const writeAction = (SoWriteAction *)(action);

  if (node->getNodeType() == PROTO_INSTANCE_ROOT) {
    assert(0 && "FIXME: PROTO export not supported.");
  }
  node->write(writeAction);
}

// Note that this documentation will also be used for all subclasses
// which reimplements the method, so keep the doc "generic enough".
/*!
  Action method for SoWriteAction.

  Writes out a node object, and any connected nodes, engines etc, if
  necessary.
*/
void
SoNode::write(SoWriteAction * action)
{
  SoOutput * out = action->getOutput();
  if (out->getStage() == SoOutput::COUNT_REFS) {
    this->addWriteReference(out, FALSE);
  }
  else if (out->getStage() == SoOutput::WRITE) {
    if (this->writeHeader(out, FALSE, FALSE)) return;
    this->getFieldData()->write(out, this);
    this->writeFooter(out);
  }
  else assert(0 && "unknown stage");
}

// Note that this documentation will also be used for all subclasses
// which reimplements the method, so keep the doc "generic enough".
/*!
  Returns list of children for this node.
*/
SoChildList *
SoNode::getChildren(void) const
{
  return NULL;
}

/*!
  Called from SoHandleEventAction::setGrabber() to notify a node when
  it becomes the node where all events are sent.
*/
void
SoNode::grabEventsSetup(void)
{
}

/*!
  Called from SoHandleEventAction to notify a node when it looses
  status as the node where events are sent.
*/
void
SoNode::grabEventsCleanup(void)
{
}

/*!
  This returns the node's current unique identification number. It is
  unlikely that application programmers will ever need use this method
  fom client application code, unless working with extensions to the
  core library (and probably not even then).

  The id number is only valid for as long as the node is kept
  unchanged -- upon \e any kind of change the internal id will be
  updated (in the notify() method), and the old id number forgotten.

  The technique described above plays an important role in the way
  internal scenegraph caches are set up and invalidated.

  \sa SoNode::getNextNodeId()
*/
uint32_t
SoNode::getNodeId(void) const
{
  return this->uniqueId;
}

// Note that this documentation will also be used for all subclasses
// which reimplements the method, so keep the doc "generic enough".
/*!
  This method is called from write() if the actual writing pass of the
  write action is taking place. It dumps the node to the given \a out
  output stream.
*/
void
SoNode::writeInstance(SoOutput * out)
{
  SoWriteAction wa(out);
  wa.continueToApply(this);
}

/*!
  Add a copy of this node and (recursively) all children to the copy
  dictionary of SoFieldContainer if this has not already been done.

  Used internally during copy operations.
*/
SoNode *
SoNode::addToCopyDict(void) const
{
#if COIN_DEBUG && 0 // debug
  SoDebugError::postInfo("SoNode::addToCopyDict",
                         "%s node", this->getTypeId().getName().getString());
#endif // debug

  SoNode * cp = (SoNode *)SoFieldContainer::checkCopy(this);
  if (!cp) {
    cp = (SoNode *)this->getTypeId().createInstance();
    assert(cp);
    SoFieldContainer::addCopy(this, cp);

    SoChildList * l = this->getChildren();
    for (int i=0; l && (i < l->getLength()); i++)
      (void)(*l)[i]->addToCopyDict();
  }

  return cp;
}

// Doc in superclass.
void
SoNode::copyContents(const SoFieldContainer * from, SbBool copyconnections)
{
  inherited::copyContents(from, copyconnections);

  SoNode * src = (SoNode *)from;
  this->stateflags = src->stateflags;
}

// Overridden from parent class.
SoFieldContainer *
SoNode::copyThroughConnection(void) const
{
  // Important note: _don't_ switch checkCopy() here with findCopy(),
  // as we're not supposed to create copies of containers "outside"
  // the part of the scene graph which is involved in the copy
  // operation.
  SoFieldContainer * connfc = SoFieldContainer::checkCopy(this);
  return connfc ? connfc : (SoFieldContainer *)this;
}

/*!
  Return the next unique identification number to be assigned upon
  node construction or change.  It is unlikely that application
  programmers will ever need use this method fom client application
  code, unless working with extensions to the core library (and
  probably not even then).

  \sa SoNode::getNodeId
*/
uint32_t
SoNode::getNextNodeId(void)
{
  return SoNode::nextUniqueId;
}

/*!
  \internal
 */
const SoFieldData **
SoNode::getFieldDataPtr(void)
{
  return NULL;
}

// Doc in super.
SbBool
SoNode::readInstance(SoInput * in, unsigned short flags)
{
  // Overridden to set node type.

  SbBool ret = inherited::readInstance(in, flags);
  if (ret) {
    if (in->isFileVRML1()) this->setNodeType(SoNode::VRML1);
    else if (in->isFileVRML2()) this->setNodeType(SoNode::VRML2);
  }
  return ret;
}

// just undef flags here

#undef FLAG_TYPEMASK
#undef FLAG_OVERRIDE

// The following function should probably eventually be renamed/moved
// to SoAction::initActionMethods(). We cannot initialize action
// methods in SoAction::initClass() since nodes must be initialized
// before we can set up action methods, and we cannot initialize nodes
// before actions, since elements (which also depend on actions) are
// enabled in nodes.
static void
init_action_methods(void)
{
  SoCallbackAction::addMethod(SoNode::getClassTypeId(), SoNode::callbackS);
  SoGLRenderAction::addMethod(SoNode::getClassTypeId(), SoNode::GLRenderS);
  SoGetBoundingBoxAction::addMethod(SoNode::getClassTypeId(), SoNode::getBoundingBoxS);
  SoGetMatrixAction::addMethod(SoNode::getClassTypeId(), SoNode::getMatrixS);
  SoGetPrimitiveCountAction::addMethod(SoNode::getClassTypeId(), SoNode::getPrimitiveCountS);
  SoHandleEventAction::addMethod(SoNode::getClassTypeId(), SoNode::handleEventS);
  SoPickAction::addMethod(SoNode::getClassTypeId(), SoNode::pickS);

  // most methods for SoRayPickAction are inherited from SoPickAction
  SoRayPickAction::addMethod(SoCamera::getClassTypeId(), SoNode::rayPickS);
  SoRayPickAction::addMethod(SoSeparator::getClassTypeId(), SoNode::rayPickS);
  SoRayPickAction::addMethod(SoLOD::getClassTypeId(), SoNode::rayPickS);
  SoRayPickAction::addMethod(SoLevelOfDetail::getClassTypeId(), SoNode::rayPickS);
  SoRayPickAction::addMethod(SoShape::getClassTypeId(), SoNode::rayPickS);

  SoSearchAction::addMethod(SoNode::getClassTypeId(), SoNode::searchS);
  SoWriteAction::addMethod(SoNode::getClassTypeId(), SoNode::writeS);
}

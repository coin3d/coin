/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2000 by Systems in Motion. All rights reserved.
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
  node types: group nodes (for the layout of the other nodes),
  appearance nodes (for setting up materials, textures, etc), shape
  nodes (for the actual geometry), and nodes for lighting, camera
  positioning etc etc.

  For more information, see the "Inventor Mentor: Programming
  Object-Oriented 3D Graphics with Open Inventor" (ISBN 0-201-62495-8)
  for detailed explanations on the basic principles involved.
*/

#include <Inventor/SoOutput.h>
#include <Inventor/actions/SoActions.h>
#include <Inventor/errors/SoDebugError.h>
#include <Inventor/misc/SoChildList.h>
#include <Inventor/nodes/SoNodes.h>
#include <Inventor/nodes/SoUnknownNode.h>
#include <assert.h>

#if HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H

#if COIN_DEBUG
#ifdef HAVE_WINDOWS_H
#include <windows.h> // gl.h needs this
#endif // HAVE_WINDOWS_H
#include <GL/gl.h> // glGetError
#endif // COIN_DEBUG

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


uint32_t SoNode::nextUniqueId = 0;
int SoNode::nextActionMethodIndex = 0;
// Don't set value explicitly to SoType::badType(), to avoid a bug in
// Sun CC v4.0. (Bitpattern 0x0000 equals SoType::badType()).
SoType SoNode::classTypeId;


// Overridden from parent.
SoType
SoNode::getClassTypeId(void)
{
  return SoNode::classTypeId;
}

/*!
  Default constructor, initializes node instance.
*/
SoNode::SoNode(void)
{
  this->uniqueId = SoNode::nextUniqueId++;
  this->stateflags.override = FALSE;
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
  the case in the original Open Inventor API.
*/
SoNode *
SoNode::copy(SbBool copyconnections) const
{
  SoFieldContainer::initCopyDict();
  (void)this->addToCopyDict();
  // Call findCopy() to have copyContents() run only once.
  SoNode * cp = (SoNode *)SoFieldContainer::findCopy(this, copyconnections);
  SoFieldContainer::copyDone();
  return cp;
}


// Overloaded from parent.
void
SoNode::startNotify(void)
{
  inherited::startNotify();
}

// Overloaded from parent.
void
SoNode::notify(SoNotList * l)
{
#if COIN_DEBUG && 0 // debug
  SoDebugError::postInfo("SoNode::notify", "node %p (%s \"%s\"), list %p",
                         this, this->getTypeId().getName().getString(),
                         this->getName().getString(), l);
#endif // debug
  this->uniqueId = SoNode::nextUniqueId++;
  inherited::notify(l);
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

// Overridden from parent class.
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
  SoAnnoText3::initClass();
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
  SoAnnoText3Property::initClass();
  SoGroup::initClass();
  SoSeparator::initClass();
  SoAnnotation::initClass();
  SoLocateHighlight::initClass();
  SoWWWAnchor::initClass();
  SoArray::initClass();
  SoSwitch::initClass();
  SoBlinker::initClass();
  SoLOD::initClass();
  SoLevelOfSimplification::initClass();
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
  SoTexture2Transform::initClass();
  SoTextureCoordinate2::initClass();
  SoTextureCoordinateBinding::initClass();
  SoTextureCoordinateFunction::initClass();
  SoTextureCoordinateDefault::initClass();
  SoTextureCoordinateEnvironment::initClass();
  SoTextureCoordinatePlane::initClass();
  SoUnknownNode::initClass();
  SoVertexProperty::initClass();
  SoWWWInline::initClass();
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
  if ((state && !this->isOverride()) || (!state && this->isOverride())){
    // This change affects caches in the tree, so we must change our id
    // setting, so the caches are regenerated.
    this->uniqueId = SoNode::nextUniqueId++;
  }

  this->stateflags.override = state;
}

/*!
  Return status of override flag.

  \sa setOverride()
*/
SbBool
SoNode::isOverride(void) const
{
  return this->stateflags.override;
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

/*!
  Returns \c TRUE if the node could have any effect on the state
  during traversal.

  If it returns \c FALSE, no data in the traversal-state will change
  from the pre-traversal state to the post-traversal state. The
  SoSeparator node will for instance return \c FALSE, as it pushes and
  pops the state before and after traversal of its children. All shape
  nodes will also return \c FALSE, as just pushing out geometry data
  to the rendering engine won't affect the actual rendering state.

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

/*!
  Action method for the SoGetBoundingBoxAction.

  Calculates bounding box and center coordinates for node and modifies
  the values of the \a action to encompass the bounding box for this
  node and to shift the center point for the scene more towards the
  one for this node.

  Nodes influencing how geometry nodes calculates their bounding box
  also overloads this method to change the relevant state variables.
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

/*!
  Action method for the SoGetPrimitiveCountAction.

  Calculates the number of triangle, line segment and point primitives
  for the node and adds these to the counters of the \a action.

  Nodes influencing how geometry nodes calculates their primitive
  count also overloads this method to change the relevant state
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
    node->GLRender((SoGLRenderAction*)action);
  }
#if COIN_DEBUG
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
    SoDebugError::postInfo("SoNode::GLRenderS",
                           "GL error: %s, nodetype: %s",
                           errorstring,
                           node->getTypeId().getName().getString());
  }
#endif // COIN_DEBUG
}

/*!
  Action method for the SoGLRenderAction.

  This is called during rendering traversals. Nodes influencing the
  rendering state in any way or who wants to throw geometry primitives
  at OpenGL overloads this method.
*/
void
SoNode::GLRender(SoGLRenderAction * action)
{
}

/*!
  Implements the SoAction::BELOW_PATH traversal method for the
  rendering action.
*/
void
SoNode::GLRenderBelowPath(SoGLRenderAction * action)
{
  this->GLRender(action);
}

/*!
  Implements the SoAction::IN_PATH traversal method for the rendering
  action.
*/
void
SoNode::GLRenderInPath(SoGLRenderAction * action)
{
  this->GLRender(action);
}

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

/*!
  Action method for SoHandleEventAction.

  Inspects the event data from \a action, and processes it if it is
  something which this node should react to.

  Nodes influencing relevant state variables for how event handling is
  done also overloads this method.
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

/*!
  Action method for SoRayPickAction.

  Checks the ray specification of the \a action and tests for
  intersection with the data of the node.

  Nodes influencing relevant state variables for how picking is done
  also overloads this method.
*/
void
SoNode::rayPick(SoRayPickAction * action)
{
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

/*!
  Action method for SoSearchAction.

  Compares the search criteria from the \a action to see if this node
  is a match.
*/
void
SoNode::search(SoSearchAction * action)
{
  if (action->isFound())
    return;

  int lookFor = action->getFind();
  SbBool flag;

  if ((lookFor & SoSearchAction::NODE) &&
       (this == action->getNode())) {
    action->addPath(action->getCurPath()->copy());
#if COIN_DEBUG && 0 // debug
    SoDebugError::postInfo("SoNode::search", "NODE match found.\n");
#endif // debug
  } else if ((lookFor & SoSearchAction::NAME) &&
      (this->getName() == action->getName())) {
    action->addPath(action->getCurPath()->copy());
#if COIN_DEBUG && 0 // debug
    SoDebugError::postInfo("SoNode::search", "NAME match found.\n");
#endif // debug
  } else if ((lookFor & SoSearchAction::TYPE) &&
      ((this->getTypeId() == action->getType(flag)) ||
      (flag && this->getTypeId().isDerivedFrom(action->getType(flag))))) {
    action->addPath(action->getCurPath()->copy());
#if COIN_DEBUG && 0 // debug
    SoDebugError::postInfo("SoNode::search", "TYPE match found.\n");
#endif // debug
  }
#if COIN_DEBUG && 0 // debug
  else {
    SoDebugError::postInfo("SoNode::search", "no match for %p (type '%s').",
                           this, this->getTypeId().getName().getString());
  }
#endif // debug
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
  node->write(writeAction);
}

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
    this->writeInstance(out);
    this->writeFooter(out);
  }
  else assert(0 && "unknown stage");
}

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
  \internal
*/
uint32_t
SoNode::getNodeId(void) const
{
  return this->uniqueId;
}

/*!
  This method is called from write() if the actual writing pass of the
  write action is taking place. It dumps the node to the given \a out
  output stream.
*/
void
SoNode::writeInstance(SoOutput * out)
{
  // Fields are written from SoFieldContainer.
  inherited::writeInstance(out);
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

// Overloaded from parent class.
void
SoNode::copyContents(const SoFieldContainer * from, SbBool copyconnections)
{
  inherited::copyContents(from, copyconnections);

  SoNode * src = (SoNode *)from;
  this->stateflags.override = src->isOverride();
}

// Overloaded from parent class.
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
  \internal
  Return the next unique identification number to be assigned.
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

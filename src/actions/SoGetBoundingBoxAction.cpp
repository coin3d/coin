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
  \class SoGetBoundingBoxAction Inventor/actions/SoGetBoundingBoxAction.h
  \brief The SoGetBoundingBoxAction class is an action for getting
  bounding boxes.
*/

#include <Inventor/actions/SoGetBoundingBoxAction.h>
#include <Inventor/actions/SoSubAction.h>
#include <Inventor/lists/SoEnabledElementsList.h>

#if !defined(COIN_EXCLUDE_SOLOCALBBOXMATRIXELEMENT)
#include <Inventor/elements/SoLocalBBoxMatrixElement.h>
#endif // !COIN_EXCLUDE_SOLOCALBBOXMATRIXELEMENT
#if !defined(COIN_EXCLUDE_SOVIEWINGMATRIXELEMENT)
#include <Inventor/elements/SoViewingMatrixElement.h>
#endif // !COIN_EXCLUDE_SOVIEWINGMATRIXELEMENT
#if !defined(COIN_EXCLUDE_SOVIEWPORTREGIONELEMENT)
#include <Inventor/elements/SoViewportRegionElement.h>
#endif // !COIN_EXCLUDE_SOVIEWPORTREGIONELEMENT

#include <Inventor/errors/SoDebugError.h>

#include <Inventor/nodes/SoNode.h>

#define FLAG_CENTER_SET    0x0001
#define FLAG_CAMERA_SPACE  0x0002
#define FLAG_RESET_BEFORE  0x0004

// *************************************************************************

/*!
  \fn SoGetBoundingBoxAction::ResetType

  FIXME: write doc.
*/

// *************************************************************************

//$ BEGIN TEMPLATE ActionSource( SoGetBoundingBoxAction )
//$ BEGIN TEMPLATE ActionClassTypeSource( SoGetBoundingBoxAction )

SoType SoGetBoundingBoxAction::classTypeId = SoType::badType();

/*!
  Returns the unique type identifier for the classname class.
*/
SoType
SoGetBoundingBoxAction::getClassTypeId(void)
{
  return classTypeId;
}

/*!
  Returns type identifier for an object.
*/
SoType
SoGetBoundingBoxAction::getTypeId(void) const
{
  return classTypeId;
}
//$ END TEMPLATE ActionClassTypeSource

#include <assert.h>

// static variables
SoEnabledElementsList * SoGetBoundingBoxAction::enabledElements;
SoActionMethodList * SoGetBoundingBoxAction::methods;

/*!
  \fn SoGetBoundingBoxAction::enabledElements
  FIXME: write doc.
*/

/*!
  \fn SoGetBoundingBoxAction::methods
  FIXME: write doc.
*/

/*!
  This method returns the list of enabled elements for the given action class.
*/
const SoEnabledElementsList &
SoGetBoundingBoxAction::getEnabledElements(void) const
{
  assert(enabledElements);
  return *enabledElements;
}

/*!
  This method adds a method to be perfomed by the action class on the given
  node type.
*/
void 
SoGetBoundingBoxAction::addMethod(const SoType type, SoActionMethod method)
{
  assert(methods);
  methods->addMethod(type, method);
}

/*!
  This method enables an element in the state stack for the action class.
*/
void 
SoGetBoundingBoxAction::enableElement(const SoType type, const int stackIndex)
{
  assert(enabledElements);
  enabledElements->enable(type, stackIndex);
}
//$ END TEMPLATE ActionSource

/*!
  A constructor.
*/

SoGetBoundingBoxAction::SoGetBoundingBoxAction(const SbViewportRegion &
					       viewportRegion)
  : center(0,0,0),
    vpRegion(viewportRegion), 
    resetPath(NULL),
    flags(FLAG_RESET_BEFORE)
{
  SO_ACTION_CONSTRUCTOR(SoGetBoundingBoxAction);  

  static int first = 1;
  if (first) {
    // FIXME: only enable action methods for nodes that can affect the
    // bouding box calculations... No big deal, though.
    SO_ACTION_ADD_METHOD(SoNode, SoNode::getBoundingBoxS);
    first = 0;
  }
  
  methods->setUp(); // FIXME: not sure if this should be called here...
}

/*!
  The destructor.
*/

SoGetBoundingBoxAction::~SoGetBoundingBoxAction()
{
}

/*!
  This static method initializes all the static data for the
  SoGetBoundingBoxAction class.
*/

void
SoGetBoundingBoxAction::initClass(void)
{
//$ BEGIN TEMPLATE InitActionSource( SoGetBoundingBoxAction )
  assert(SoGetBoundingBoxAction::getClassTypeId() == SoType::badType());
  assert(inherited::getClassTypeId() != SoType::badType());

  SoGetBoundingBoxAction::classTypeId = 
      SoType::createType(inherited::getClassTypeId(),
                         "SoGetBoundingBoxAction");
  enabledElements = new SoEnabledElementsList(inherited::enabledElements);
  methods = new SoActionMethodList(inherited::methods);
//$ END TEMPLATE InitActionSource

  ENABLE_ELEMENT(SoViewportRegionElement);
}

/*!
  This static method cleans up all the static data for the
  SoGetBoundingBoxAction class.
*/

void
SoGetBoundingBoxAction::cleanClass(void)
{
  delete methods;
  delete SoGetBoundingBoxAction::enabledElements;
  methods = NULL;
  SoGetBoundingBoxAction::enabledElements = NULL;  
}

// *************************************************************************

/*!
  This method sets the viewport region.
*/

void 
SoGetBoundingBoxAction::setViewportRegion(const SbViewportRegion &newRegion)
{
  this->vpRegion = newRegion;
}

/*!
  This method returns the viewport region.
*/

const SbViewportRegion &
SoGetBoundingBoxAction::getViewportRegion(void) const
{
  return this->vpRegion;
}

/*!
  This method gets the bounding box.
*/

SbBox3f 
SoGetBoundingBoxAction::getBoundingBox(void) const
{
  return this->bbox.project();
}

/*!
  FIXME: write doc.
*/

SbXfBox3f &
SoGetBoundingBoxAction::getXfBoundingBox(void)
{
  return this->bbox;
}

/*!
  This method returns the center.
*/

const SbVec3f &
SoGetBoundingBoxAction::getCenter(void) const
{
  if (!this->isCenterSet()) {
    // Cast away constness and set.
    SoGetBoundingBoxAction * action = (SoGetBoundingBoxAction *)this;
    action->center.setValue(0.0f, 0.0f, 0.0f);
  }
  // FIXME: probably needs to transform center..? 19990316
  // mortene. Update: if so, remember to check the code in
  // SoSeparator::getBoundingBox() (the state->pop() probably needs to
  // be moved further down in the code). 19990422 mortene.
  return this->center;
}

/*!
  This method sets wether the returned bounding box should be in the
  camera space or not.
*/

void 
SoGetBoundingBoxAction::setInCameraSpace(const SbBool on)
{
  if (on) {
    this->flags |= FLAG_CAMERA_SPACE;
  }
  else {
    this->flags &= ~FLAG_CAMERA_SPACE;
  }
}

/*!
  This method returns wether the bounding box returned is to be in the
  camera space or not.
*/

SbBool 
SoGetBoundingBoxAction::isInCameraSpace(void) const
{
  return (this->flags & FLAG_CAMERA_SPACE) != 0;
}

/*!
  This method forces the computed bounding box to be reset and the
  transformation to be identity before or after the tail node of
  \a path, depending on the \a resetBefore argument.  NULL can be
  specified as the \a path argument to disable this behaviour.

  \sa const SoPath * SoGetBoundingBoxAction::getResetPath() const
  \sa SbBool SoGetBoundingBoxAction::isResetPath() const
  \sa SbBool SoGetBoundingBoxAction::isResetBefore() const
  \sa SoGetBoundingBoxAction::ResetType SoGetBoundingBoxAction::getWhatReset() const
*/

void 
SoGetBoundingBoxAction::setResetPath(const SoPath *path, 
				     const SbBool resetBefore, 
				     const ResetType what)
{
  this->resetPath = (SoPath*)path; // force this cast :-(
  this->resetType = what;
  if (resetBefore)
    this->flags |= FLAG_RESET_BEFORE;
  else
    this->flags &= ~FLAG_RESET_BEFORE;
}

/*!
  This method returns the reset path (or NULL).

  \sa void SoGetBoundingBoxAction::setResetPath(const SoPath *path, const SbBool resetBefore, const ResetType what)
  \sa SbBool SoGetBoundingBoxAction::isResetPath() const
  \sa SbBool SoGetBoundingBoxAction::isResetBefore() const
  \sa SoGetBoundingBoxAction::ResetType SoGetBoundingBoxAction::getWhatReset() const
*/

const SoPath *
SoGetBoundingBoxAction::getResetPath() const
{
  return this->resetPath;
}

/*!
  This method returns wether a reset path is set or not.

  \sa void SoGetBoundingBoxAction::setResetPath(const SoPath *path, const SbBool resetBefore, const ResetType what)
  \sa const SoPath * SoGetBoundingBoxAction::getResetPath() const
  \sa SbBool SoGetBoundingBoxAction::isResetBefore() const
  \sa SoGetBoundingBoxAction::ResetType SoGetBoundingBoxAction::getWhatReset() const
*/

SbBool 
SoGetBoundingBoxAction::isResetPath() const
{
  return this->resetPath != NULL;
}

/*!
  This method returns wether the bounding box and transformation is reset before
  or after the tail node of the reset path.

  \sa void SoGetBoundingBoxAction::setResetPath(const SoPath *path, const SbBool resetBefore, const ResetType what)
  \sa const SoPath * SoGetBoundingBoxAction::getResetPath() const
  \sa SbBool SoGetBoundingBoxAction::isResetPath() const
  \sa SoGetBoundingBoxAction::ResetType SoGetBoundingBoxAction::getWhatReset() const
*/

SbBool 
SoGetBoundingBoxAction::isResetBefore() const
{
  return (this->flags & FLAG_RESET_BEFORE) != 0;
}

/*!
  This method returns what type of reset has been specified for the reset
  path.

  \sa void SoGetBoundingBoxAction::setResetPath(const SoPath *path, const SbBool resetBefore, const ResetType what)
  \sa const SoPath * SoGetBoundingBoxAction::getResetPath() const
  \sa SbBool SoGetBoundingBoxAction::isResetPath() const
  \sa SbBool SoGetBoundingBoxAction::isResetBefore() const
*/

SoGetBoundingBoxAction::ResetType 
SoGetBoundingBoxAction::getWhatReset() const
{
  return this->resetType;
}

/* EXTENDER methods ***************************************************/

/*!
  FIXME: write doc.
*/

void 
SoGetBoundingBoxAction::checkResetBefore()
{
  if (this->resetPath && this->isResetBefore()) {
    // FIXME: do something
  }
}

/*!
  FIXME: write doc.
*/

void 
SoGetBoundingBoxAction::checkResetAfter()
{
  if (this->resetPath && !this->isResetBefore()) {
    // FIXME: do something
  }
}

/*!
  FIXME: write doc.
*/

void 
SoGetBoundingBoxAction::extendBy(const SbBox3f &box)
{
  // FIXME: maybe we should convert to an SbXfBox3f? 19990320 mortene.
  SbBox3f tbox = box;
#if !defined(COIN_EXCLUDE_SOLOCALBBOXMATRIXELEMENT)
  tbox.transform(SoLocalBBoxMatrixElement::get(this->state));
#endif // !COIN_EXCLUDE_SOLOCALBBOXMATRIXELEMENT
#if !defined(COIN_EXCLUDE_SOVIEWINGMATRIXELEMENT)
  if (this->isInCameraSpace()) {
    const SbMatrix & mat = SoViewingMatrixElement::get(this->state);
#if 0 // debug
    SoDebugError::postInfo("SoGetBoundingBoxAction::extendBy",
			   "in camera space, sbbox3f");
#endif // debug
    tbox.transform(mat);
  }
#endif // !COIN_EXCLUDE_SOVIEWINGMATRIXELEMENT

  this->bbox.extendBy(tbox);
}

/*!
  FIXME: write doc.
*/

void 
SoGetBoundingBoxAction::extendBy(const SbXfBox3f &box)
{
  SbXfBox3f lbox = box;
#if !defined(COIN_EXCLUDE_SOLOCALBBOXMATRIXELEMENT)
  lbox.transform(SoLocalBBoxMatrixElement::get(this->state));
#endif // !COIN_EXCLUDE_SOLOCALBBOXMATRIXELEMENT
#if !defined(COIN_EXCLUDE_SOVIEWINGMATRIXELEMENT)
  if (this->isInCameraSpace()) {
    const SbMatrix & mat = SoViewingMatrixElement::get(this->state);
#if 0 // debug
    SoDebugError::postInfo("SoGetBoundingBoxAction::extendBy",
			   "in camera space, sbxfbox3f");
#endif // debug
    lbox.transform(mat);
  }
#endif // !COIN_EXCLUDE_SOVIEWINGMATRIXELEMENT
  this->bbox.extendBy(lbox);
}
  
/*!
  FIXME: write doc.
*/

void 
SoGetBoundingBoxAction::setCenter(const SbVec3f &center,
				  const SbBool transformCenter)
{
  assert(!this->isCenterSet());
  this->flags |= FLAG_CENTER_SET;

  if (transformCenter) {
    this->center = center;
#if !defined(COIN_EXCLUDE_SOLOCALBBOXMATRIXELEMENT)
    const SbMatrix & lmat = SoLocalBBoxMatrixElement::get(this->state);
    lmat.multVecMatrix(this->center, this->center);
#endif // COIN_EXCLUDE_SOLOCALBBOXMATRIXELEMENT
#if !defined(COIN_EXCLUDE_SOVIEWINGMATRIXELEMENT)
    if (this->isInCameraSpace()) {
      const SbMatrix & vmat = SoViewingMatrixElement::get(this->state);
      vmat.multVecMatrix(this->center, this->center);
    }
#endif // COIN_EXCLUDE_SOVIEWINGMATRIXELEMENT
  }
  else {
    this->center = center;
  }

#if 0 // debug
  SoDebugError::post("SoGetBoundingBoxAction::setCenter",
		     "center: <%f, %f, %f>, transformCenter: %s, "
		     "this->center: <%f, %f, %f>",
		     center[0], center[1], center[2],
		     transformCenter ? "TRUE" : "FALSE",
		     this->center[0], this->center[1], this->center[2]);
#endif // debug
}

/*!
  FIXME: write doc.
*/

SbBool 
SoGetBoundingBoxAction::isCenterSet() const
{
  return (this->flags & FLAG_CENTER_SET) != 0;
}

/*!
  FIXME: write doc.
*/

void 
SoGetBoundingBoxAction::resetCenter()
{
  this->flags &= ~FLAG_CENTER_SET;
  // FIXME: check this->isInCameraSpace() and modify? Probably not, but
  // investigate. 19990513 mortene.
  this->center.setValue(0.0f, 0.0f, 0.0f);
}

/* protected methods ****************************************************/

#include <Inventor/misc/SoState.h>

/*!
  FIXME: write doc.
*/

void 
SoGetBoundingBoxAction::beginTraversal(SoNode * const node)
{
  this->resetCenter();
  this->bbox.makeEmpty();

  assert(this->getState());

  // FIXME: should we really push() and pop() here? Perhaps in the
  // SoAction class instead? 19990303 mortene.

  this->getState()->push();
  SoViewportRegionElement::set(this->getState(), this->vpRegion);
  inherited::beginTraversal(node);
  this->getState()->pop();
}

#undef FLAG_CENTER_SET
#undef FLAG_CAMERA_SPACE
#undef FLAG_RESET_BEFORE



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
  \class SoSelection SoSelection.h Inventor/nodes/SoSelection.h
  \brief The SoSelection class ...
  \ingroup nodes

  FIXME: write class doc
*/


#include <Inventor/nodes/SoSelection.h>


/*!
  \enum SoSelection::Policy
  FIXME: write documentation for enum
*/
/*!
  \var SoSelection::Policy SoSelection::SINGLE
  FIXME: write documentation for enum definition
*/
/*!
  \var SoSelection::Policy SoSelection::TOGGLE
  FIXME: write documentation for enum definition
*/
/*!
  \var SoSelection::Policy SoSelection::SHIFT
  FIXME: write documentation for enum definition
*/


/*!
  \var SoSFEnum SoSelection::policy
  FIXME: write documentation for field
*/


// *************************************************************************

SO_NODE_SOURCE(SoSelection);

/*!
  Constructor.
*/
SoSelection::SoSelection()
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoSelection);

  SO_NODE_ADD_FIELD(policy, (SoSelection::SHIFT));

  SO_NODE_DEFINE_ENUM_VALUE(Policy, SINGLE);
  SO_NODE_DEFINE_ENUM_VALUE(Policy, TOGGLE);
  SO_NODE_DEFINE_ENUM_VALUE(Policy, SHIFT);
  SO_NODE_SET_SF_ENUM_TYPE(policy, Policy);
}

/*!
  Destructor.
*/
SoSelection::~SoSelection()
{
}

/*!
  Does initialization common for all objects of the
  SoSelection class. This includes setting up the
  type system, among other things.
*/
void
SoSelection::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoSelection);
}



/*!
  FIXME: write doc
 */
SoSelection::SoSelection(int /* nChildren */)
{
  assert(0 && "FIXME: not implemented");
}

/*!
  FIXME: write doc
 */
void
SoSelection::select(const SoPath * /* path */)
{
  assert(0 && "FIXME: not implemented");
}

/*!
  FIXME: write doc
 */
void
SoSelection::select(SoNode * /* node */)
{
  assert(0 && "FIXME: not implemented");
}

/*!
  FIXME: write doc
 */
void
SoSelection::deselect(const SoPath * /* path */)
{
  assert(0 && "FIXME: not implemented");
}

/*!
  FIXME: write doc
 */
void
SoSelection::deselect(int /* which */)
{
  assert(0 && "FIXME: not implemented");
}

/*!
  FIXME: write doc
 */
void
SoSelection::deselect(SoNode * /* node */)
{
  assert(0 && "FIXME: not implemented");
}

/*!
  FIXME: write doc
 */
void
SoSelection::toggle(const SoPath * /* path */)
{
  assert(0 && "FIXME: not implemented");
}

/*!
  FIXME: write doc
 */
void
SoSelection::toggle(SoNode * /* node */)
{
  assert(0 && "FIXME: not implemented");
}

/*!
  FIXME: write doc
 */
SbBool
SoSelection::isSelected(const SoPath * /* path */) const
{
  assert(0 && "FIXME: not implemented");
  return FALSE;
}

/*!
  FIXME: write doc
 */
SbBool
SoSelection::isSelected(SoNode * /* node */) const
{
  assert(0 && "FIXME: not implemented");
  return FALSE;
}

/*!
  FIXME: write doc
 */
void
SoSelection::deselectAll(void)
{
  assert(0 && "FIXME: not implemented");
}

/*!
  FIXME: write doc
 */
int
SoSelection::getNumSelected(void) const
{
  assert(0 && "FIXME: not implemented");
  return 0;
}

/*!
  FIXME: write doc
 */
const SoPathList *
SoSelection::getList(void) const
{
  assert(0 && "FIXME: not implemented");
  return NULL;
}

/*!
  FIXME: write doc
 */
SoPath *
SoSelection::getPath(int /* index */) const
{
  assert(0 && "FIXME: not implemented");
  return NULL;
}

/*!
  FIXME: write doc
 */
SoPath *
SoSelection::operator[](int /* i */) const
{
  assert(0 && "FIXME: not implemented");
  return NULL;
}

/*!
  FIXME: write doc
 */
void
SoSelection::addSelectionCallback(SoSelectionPathCB * /* f */, void * /* userData */)
{
  assert(0 && "FIXME: not implemented");
}

/*!
  FIXME: write doc
 */
void
SoSelection::removeSelectionCallback(SoSelectionPathCB * /* f */, void * /* userData */)
{
  assert(0 && "FIXME: not implemented");
}

/*!
  FIXME: write doc
 */
void
SoSelection::addDeselectionCallback(SoSelectionPathCB * /* f */, void * /* userData */)
{
  assert(0 && "FIXME: not implemented");
}

/*!
  FIXME: write doc
 */
void
SoSelection::removeDeselectionCallback(SoSelectionPathCB * /* f */, void * /* userData */)
{
  assert(0 && "FIXME: not implemented");
}

/*!
  FIXME: write doc
 */
void
SoSelection::addStartCallback(SoSelectionClassCB * /* f */, void * /* userData */)
{
  assert(0 && "FIXME: not implemented");
}

/*!
  FIXME: write doc
 */
void
SoSelection::removeStartCallback(SoSelectionClassCB * /* f */, void * /* userData */)
{
  assert(0 && "FIXME: not implemented");
}

/*!
  FIXME: write doc
 */
void
SoSelection::addFinishCallback(SoSelectionClassCB * /* f */, void * /* userData */)
{
  assert(0 && "FIXME: not implemented");
}

/*!
  FIXME: write doc
 */
void
SoSelection::removeFinishCallback(SoSelectionClassCB * /* f */, void * /* userData */)
{
  assert(0 && "FIXME: not implemented");
}

/*!
  FIXME: write doc
 */
void
SoSelection::setPickFilterCallback(SoSelectionPickCB * /* f */, void * /* userData */,
				   SbBool /* callOnlyIfSelectable */)
{
  assert(0 && "FIXME: not implemented");
}

/*!
  FIXME: write doc
 */
void
SoSelection::setPickMatching(SbBool /* pickTwice */)
{
  assert(0 && "FIXME: not implemented");
}

/*!
  FIXME: write doc
 */
SbBool
SoSelection::isPickMatching(void) const
{
  assert(0 && "FIXME: not implemented");
  return FALSE;
}

/*!
  FIXME: write doc
 */
SbBool
SoSelection::getPickMatching(void) const
{
  assert(0 && "FIXME: not implemented");
  return FALSE;
}

/*!
  FIXME: write doc
 */
void
SoSelection::addChangeCallback(SoSelectionClassCB * /* f */, void * /* userData */)
{
  assert(0 && "FIXME: not implemented");
}

/*!
  FIXME: write doc
 */
void
SoSelection::removeChangeCallback(SoSelectionClassCB * /* f */,
				  void * /* userData */)
{
  assert(0 && "FIXME: not implemented");
}

/*!
  FIXME: write doc
 */
void
SoSelection::invokeSelectionPolicy(SoPath * /* path */, SbBool /* shiftDown */)
{
  assert(0 && "FIXME: not implemented");
}

/*!
  FIXME: write doc
 */
void
SoSelection::performSingleSelection(SoPath * /* path */)
{
  assert(0 && "FIXME: not implemented");
}

/*!
  FIXME: write doc
 */
void
SoSelection::performToggleSelection(SoPath * /* path */)
{
  assert(0 && "FIXME: not implemented");
}

/*!
  FIXME: write doc
 */
SoPath *
SoSelection::copyFromThis(const SoPath * /* path */) const
{
  assert(0 && "FIXME: not implemented");
  return NULL;
}

/*!
  FIXME: write doc
 */
void
SoSelection::addPath(SoPath * /* path */)
{
  assert(0 && "FIXME: not implemented");
}

/*!
  FIXME: write doc
 */
void
SoSelection::removePath(int /* which */)
{
  assert(0 && "FIXME: not implemented");
}

/*!
  FIXME: write doc
 */
int
SoSelection::findPath(const SoPath * /* path */) const
{
  assert(0 && "FIXME: not implemented");
  return 0;
}

/*!
  FIXME: write doc
 */
void
SoSelection::handleEvent(SoHandleEventAction * /* action */)
{
  assert(0 && "FIXME: not implemented");
}

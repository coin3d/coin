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
  \class SoPath SoPath.h Inventor/SoPath.h
  \brief The SoPath class is a container class for traversal path descriptions.
  \ingroup general

  SoPath objects contain a list of SoNode pointers and a list of child
  indices. Indices are necessary to disambiguate situations where a
  node uses the same node as a child multiple times.
*/

// FIXME:
/*¡
  <ul>
  <li>implement change notification
  </ul>
*/

#include <Inventor/SoPath.h>

#include <Inventor/SbString.h>
#include <Inventor/SbName.h>
#include <coindefs.h> // COIN_STUB()
#include <Inventor/nodes/SoNode.h>
#include <Inventor/nodes/SoGroup.h>
#include <Inventor/misc/SoChildList.h>

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

SoType SoPath::classTypeId = SoType::badType();


/*!
  The default constructor.  \a approxlength is used to indicate in
  advance the length of the path to minimize the number of reallocations.
*/
SoPath::SoPath(const int approxlength)
  : nodes(approxlength), indices(approxlength),
    isauditing(TRUE), firsthidden(-1)
{
}

/*!
  A constructor. The argument is set as the \a head (and only) node of
  the path.
*/
SoPath::SoPath(SoNode * const head)
  : isauditing(TRUE), firsthidden(-1)
{
  this->setHead(head);
}

/*!
  Copy constructor. Not part of the original Open Inventor API.
*/
SoPath::SoPath(const SoPath & rhs)
  : inherited(), nodes(rhs.getFullLength()), indices(rhs.getFullLength())
{
  this->operator=(rhs);
}

/*!
  Assignment operator, copies the \a rhs path into this.  Not part of
  the original Open Inventor API.
*/
SoPath &
SoPath::operator=(const SoPath & rhs)
{
  this->firsthidden = rhs.firsthidden;
  this->isauditing = rhs.isauditing;
  this->nodes = rhs.nodes;
  this->indices = rhs.indices;
  return *this;
}


/*!
  The destructor is protected as destruction of SoPath instances
  happens through unref() (as for all objects derived from classes
  inheriting SoBase).
*/
SoPath::~SoPath(void)
{
  // The nodelist will automatically destruct, as it is part of SoPath
  // objects by value (not by pointer). As the nodelist destructs, the
  // nodes in the list will also be unref()'ed.
}

/*!
  This method sets the head of the path to \a node. If the object
  already has a path set, it will be truncated.
*/
void
SoPath::setHead(SoNode * const node)
{
  this->truncate(0);
  this->firsthidden = -1;
  this->append(node, 0);
}

/*!
  This method returns the head of the path, or \c NULL if the path is
  empty.
*/
SoNode *
SoPath::getHead(void) const
{
  if (this->nodes.getLength() == 0) return NULL;
  return (SoNode *) this->nodes[0];
}

/*!
  This function appends the \a childindex child of the path's tail to
  the end of the path.  It can only be used after the head is set.
*/
void
SoPath::append(const int childindex)
{
#if COIN_DEBUG
  if (this->nodes.getLength() == 0) {
    SoDebugError::post("SoPath::append(int child)",
                       "SoPath was empty.\n");
    return;
  }
#endif // COIN_DEBUG

  SoChildList * children =
    this->nodes[this->nodes.getLength()-1]->getChildren();
  assert(children);

#if COIN_DEBUG
  if (childindex >= children->getLength()) {
    SoDebugError::post("SoPath::append", "childindex (%d) out of bounds",
                       childindex);
    return;
  }
#endif // debug

  SoNode * node = (*children)[childindex];
  this->append(node, childindex);
}

/*!
  This function adds \a node to the end if it is a child of the
  current tail. If path is empty, this is equivalent to calling
  setHead().
*/
void
SoPath::append(SoNode * const node)
{
  if (this->nodes.getLength() == 0) {
    this->setHead(node);
    return;
  }

  SoChildList * children =
    this->nodes[this->nodes.getLength()-1]->getChildren();
  assert(children);

  const int idx = children->find((void*)node);
#if COIN_DEBUG && 1 // debug
  if (idx < 0) {
    SoDebugError::post("SoPath::append", "node not found as child of tail");
    return;
  }
#endif // debug

  this->append(node, idx);
}

/*!
  This method appends the path to the end of the path if the head of
  \a frompath is either the tail of the path or a child of the path's
  tail. If the head of \a frompath is used multiple times as a child
  of the path tail, the lowest child index is used.
*/
void
SoPath::append(const SoPath * const frompath)
{
  if (!this->nodes.getLength()) { // append to empty path
    this->operator=(*frompath);
    return;
  }

  if (!frompath->nodes.getLength()) // nothing to append
    return;

  SoNode * const head = frompath->getHead();
  SoNode * const tail = this->nodes[this->nodes.getLength() - 1];
  if (head == tail) { // easy
    const int length = frompath->nodes.getLength();
    for (int i = 1; i < length; i++) {
      this->append((SoNode *)frompath->nodes[i], frompath->indices[i]);
    }
    if (this->firsthidden == -1 && frompath->firsthidden >= 0) {
      this->setFirstHidden();
    }
    return;
  }
  // head of frompath must be child

  SoChildList * tailchildren = tail->getChildren();

#if COIN_DEBUG
  if (tailchildren == NULL) {
    SoDebugError::post("SoPath::append(SoPath)",
                       "The tail of this SoPath has no children node ('%s').\n",
                       tail->getTypeId().getName().getString());
    return;
  }
#endif // COIN_DEBUG

  const int kids = tailchildren->getLength();
  for (int kid = 0; kid < kids; kid++) {
    SoNode * child = (*tailchildren)[kid];
    if (child == head) {
      this->append(head, kid);
      const int length = frompath->nodes.getLength();
      for (int i = 1; i < length; i++) {
        this->append((SoNode*)frompath->nodes[i], frompath->indices[i]);
      }
      if (this->firsthidden == -1 && frompath->firsthidden >= 0) {
        this->setFirstHidden();
      }
      return;
    }
  }

#if COIN_DEBUG
  SoDebugError::post("SoPath::append(SoPath *)", "Could not join paths.\n");
#endif // COIN_DEBUG
  return;
}

// This method appends a node to the path, assuming the information is
// correct.
void
SoPath::append(SoNode * const node, const int index)
{
  if (this->firsthidden < 0) {
    if (this->hasHiddenChildren(node))
      this->firsthidden = this->nodes.getLength();
  }
  this->nodes.append(node);
  this->indices.append(index);
}

/*!
  This method pushes the child at \a childindex on the tail on the
  path.
*/
void
SoPath::push(const int childindex)
{
  this->append(childindex);
}

/*!
  This method pops the tail off the path.
*/
void
SoPath::pop(void)
{
  this->truncate(this->getFullLength() - 1);
}

/*!
  This method returns the tail node of the path.
*/
SoNode *
SoPath::getTail(void) const
{
  if (this->getLength() == 0) {
#if COIN_DEBUG
    SoDebugError::postWarning("SoPath::getTail()", "empty path!");
#endif // COIN_DEBUG
    return NULL;
  }
  return this->nodes[this->getLength() - 1];
}

/*!
  This method returns the node at \a index in the path, counted from 0
  at the head.
*/
SoNode *
SoPath::getNode(const int index) const
{
#if COIN_DEBUG
  if (index < 0 || index >= this->nodes.getLength()) {
    SoDebugError::post("SoPath::getNode", "index %d is out of bounds.",
                       index);
    return NULL;
  }
#endif // COIN_DEBUG
  return this->nodes[index];
}

/*!
  This method returns the node at \a index in the path, counting from
  the tail.
*/
SoNode *
SoPath::getNodeFromTail(const int index) const
{
#if COIN_DEBUG
  if (index < 0 || index >= this->getLength()) {
    SoDebugError::post("SoPath::getNodeFromTail()",
                       "index %d is out of bounds.", index);
    return NULL;
  }
#endif // COIN_DEBUG
  return this->nodes[this->getLength() - index - 1];
}

/*!
  This method returns the child index of the node at \a index in the
  path.
*/
int
SoPath::getIndex(const int index) const
{
#if COIN_DEBUG
  if (index < 0 || index >= this->indices.getLength()) {
    SoDebugError::post("SoPath::getIndex", "index %d is out of bounds.",
                       index);
    return -1;
  }
#endif // COIN_DEBUG
  return this->indices[index];
}

/*!
  This method returns the child index of the node at \a index in the
  path, counting from the tail.
*/
int
SoPath::getIndexFromTail(const int index) const
{
#if COIN_DEBUG
  if (index < 0 || index >= this->getLength()) {
    SoDebugError::post("SoPath::getIndexFromTail()",
                       "index %d is out of bounds.", index);
    return -1;
  }
#endif // COIN_DEBUG
  return this->indices[this->getLength() - index - 1];
}

/*!
  This method returns the number of nodes in the path. Only the
  "visible" nodes are counted, i.e. hidden nodes of e.g. nodekits are
  not included.
*/
int
SoPath::getLength(void) const
{
  if (this->firsthidden >= 0) {
    assert(this->firsthidden < this->nodes.getLength());
    return this->firsthidden + 1;
  }
  return this->nodes.getLength();
}

// This method returns the number of nodes in the path, including
// hidden nodes.
int
SoPath::getFullLength(void) const
{
  return this->nodes.getLength();
}

/*!
  This method truncates the path to the given \a length (i.e. all
  nodes in the path list from index \a length and onwards will be
  removed from the path list).
*/
void
SoPath::truncate(const int length)
{
  this->truncate(length, TRUE);
}

// This method truncates the path to the given length.  FIXME: the
// donotify flag is currently ignored.
void
SoPath::truncate(const int length, const SbBool /*donotify*/)
{
#if COIN_DEBUG
  if (length < 0 || length > this->nodes.getLength()) {
    SoDebugError::post("SoPath::truncate()", "invalid length %d", length);
    return;
  }
#endif // COIN_DEBUG
  this->nodes.truncate(length);
  this->indices.truncate(length);

  if (length <= this->firsthidden) this->setFirstHidden();
}

// *************************************************************************

/*!
  This method returns the index of the last node that is the same for
  both paths, or \c -1 if the paths don't even start on the same node.
*/
int
SoPath::findFork(const SoPath * const path) const
{
  const int len = SbMin(this->nodes.getLength(), path->nodes.getLength());
  if (len == 0 || this->nodes[0] != path->nodes[0]) return -1;
  int i;
  for (i = 1; i < len; i++) {
    if (this->indices[i] != path->indices[i]) break;
  }
  return i - 1;
}

/*!
  Returns the index of \a node if found in path, \c -1 otherwise.

  Note that this method is not part of the original Open Inventor API.
*/
int
SoPath::findNode(const SoNode * const node) const
{
  const int len = this->nodes.getLength();
  for (int i = 0; i < len; i++) {
    if (this->nodes[i] == node) return i;
  }
  return -1;
}

/*!
  This method returns \c TRUE is \a node is contained in path, and \c
  FALSE otherwise.
*/
SbBool
SoPath::containsNode(const SoNode * const node) const
{
  return (this->findNode(node) == -1) ? FALSE : TRUE;
}

/*!
  This method returns \c TRUE if path is contained in the path, and \c
  FALSE otherwise.
*/
SbBool
SoPath::containsPath(const SoPath * const path) const
{
  int thislen = this->nodes.getLength();
  int thatlen = path->nodes.getLength();
  if (thatlen == 0 || thatlen > thislen) return FALSE;

  int offset = this->findNode(path->nodes[0]); // find head in this path
  if ((offset < 0) || (offset + thatlen > thislen)) return FALSE;

  for (int i = 1; i < thatlen; i++) {
    if (this->indices[offset+i] != path->indices[i]) return FALSE;
  }
  return TRUE;
}

/*!
  Compares contents of path \a lhs and path \a rhs, and returns \c
  TRUE if they are equal.
*/
SbBool
operator==(const SoPath & lhs, const SoPath & rhs)
{
  if (&lhs == &rhs) return TRUE;
  const int len = lhs.nodes.getLength();
  if (len != rhs.nodes.getLength()) return FALSE;
  if (len == 0) return TRUE;
  if (lhs.nodes[0] != rhs.nodes[0]) return FALSE;

  for (int i = 1; i < len; i++) {
    if (lhs.indices[i] != rhs.indices[i]) return FALSE;
  }
  return TRUE;
}

/*!
  Returns \c TRUE if paths \a lhs and \a rhs does not contain the same
  nodes in the same order.
*/
SbBool
operator!=(const SoPath & lhs, const SoPath & rhs)
{
  return !this->operator==(lhs, rhs);
}

// *************************************************************************

/*!
  This method returns a copy of the path starting at \a
  startfromnodeindex with length \a numnodes.  If \a numnodes is 0
  (default) the path will be copied to the end.
*/
SoPath *
SoPath::copy(const int startfromnodeindex, int numnodes) const
{
#if COIN_DEBUG
  if (startfromnodeindex < 0 ||
      startfromnodeindex >= this->nodes.getLength()) {
    SoDebugError::post("SoPath::copy",
                       "startfromnodeindex was out of bounds with %d.",
                       startfromnodeindex);
    return NULL;
  }
#endif // COIN_DEBUG
  if (numnodes == 0) numnodes = this->nodes.getLength() - startfromnodeindex;

#if COIN_DEBUG
  if (numnodes <= 0 ||
      (startfromnodeindex + numnodes) > this->nodes.getLength()) {
    SoDebugError::post("SoPath::copy", "numnodes has invalid value %d",
                       numnodes);
    return NULL;
  }
#endif // COIN_DEBUG

  SoPath * newpath = new SoPath(numnodes);
  const int max = startfromnodeindex + numnodes;
  for (int i = startfromnodeindex; i < max; i++) {
    newpath->append(this->nodes[i], this->indices[i]);
  }
  newpath->setFirstHidden();
  return newpath;
}

// *************************************************************************

/*!
  This static method is for retrieving an SoPath by it's \a name.  The
  first SoPath found with the given \a name is returned, or \c NULL if
  no such SoPath exists.
*/
SoPath *
SoPath::getByName(const SbName name)
{
  COIN_STUB();
  return NULL;
}

/*!
  This static method is for finding all the paths with a given \a name
  and append them to the \a l list. The number of lists found is
  returned.
*/
int
SoPath::getByName(const SbName name, SoPathList & l)
{
  COIN_STUB();
  return 0;
}

// *************************************************************************

/*!
  This method is called when a node in the path chain has a child
  added, to update the index of it's child. \a newindex is the index
  of the new child.
*/
void
SoPath::insertIndex(SoNode * const parent, const int newindex)
{
  int pos = this->findNode(parent) + 1;
  assert(pos > 0); // shouldn't be notified if parent is not in path
  if (pos < this->nodes.getLength()) {
    if (newindex <= this->indices[pos]) {
      this->indices[pos] = this->indices[pos] + 1;
    }
  }
}

/*!
  This method is called when a node in the path chain has a child
  removed, to update the index of it's child. \a oldindex was the
  index of the removed child.
*/
void
SoPath::removeIndex(SoNode * const parent, const int oldindex)
{
  int pos = this->findNode(parent) + 1;
  assert(pos > 0); // shouldn't be notified if parent is not in path
  if (pos < this->nodes.getLength()) {
    if (oldindex < this->indices[pos]) {
      this->indices[pos] = this->indices[pos] - 1;
    }
    else if (oldindex == this->indices[pos]) {
      // if node in path is removed, we have to truncate path
      this->truncate(pos);
    }
  }
}

/*!
  This method is called when a node in the path chain replaces a
  child. \a index is the position of the replaced child.
*/
void
SoPath::replaceIndex(SoNode * const parent, const int index,
                     SoNode * const /*newchild*/)
{
  int pos = this->findNode(parent) + 1;
  assert(pos > 0); // shouldn't be notified if parent is not in path
  if (pos < this->nodes.getLength()) {
    if (index == this->indices[pos]) {
      // FIXME: not sure about this one. I don't think we should
      // use newchild in the path, since the path beyond newchild will
      // not be correct. I think it is best to truncate the path after
      // the parent node. pederb 2000-01-10
      this->truncate(pos);
    }
  }
}

// *************************************************************************

/*!
  This method is used to write an SoPath.
*/
void
SoPath::write(SoWriteAction * action) const
{
  COIN_STUB();
}

// *************************************************************************

/*!
  Not implemented yet.
*/
SbBool
SoPath::isRelevantNotification(SoNotList * const l) const
{
  COIN_STUB();
  return FALSE;
}

/*!
  Set whether or not to audit the nodes in the path to detect changes.
*/
void
SoPath::auditPath(const SbBool flag)
{
  this->isauditing = flag;
}

// Override from parent.
SoType
SoPath::getClassTypeId(void)
{
  return SoPath::classTypeId;
}

// Documented in parent class.
SoType
SoPath::getTypeId(void) const
{
  return SoPath::classTypeId;
}

// This static method creates a new SoPath object and returns a
// pointer to it. Used by the run-time type system.
void *
SoPath::createInstance(void)
{
  return (void *)new SoPath;
}

// Override from parent.
void
SoPath::initClass(void)
{
  assert((SoPath::classTypeId == SoType::badType()) &&
         "call SoPath::initClass only once!");

  SoPath::classTypeId = SoType::createType(inherited::getClassTypeId(),
                                           SbName("Path"),
                                           &SoPath::createInstance);
}

// *************************************************************************

// This virtual method is used to read a SoPath from the in stream.
SbBool
SoPath::readInstance(SoInput * in, unsigned short flags)
{
  COIN_STUB();
  return TRUE;
}

//
// Private convenience method that tests if a node has hidden
// children.  It would probably be a good idea to move this method to
// SoNode.
//
SbBool
SoPath::hasHiddenChildren(SoNode *node) const
{
  return (node->getChildren() != NULL) &&
    !node->isOfType(SoGroup::getClassTypeId());
}

//
// Private method that scans the nodes and finds the first (if any)
// node with hidden children. If a node with hidden children is found,
// getLength() will return this as the length of the path. This also
// affects all tail-operations.
//
void
SoPath::setFirstHidden(void)
{
  this->firsthidden = -1;
  int n = this->nodes.getLength();
  for (int i = 0; i < n; i++) {
    if (this->hasHiddenChildren(this->nodes[i])) {
      this->firsthidden = i;
      return;
    }
  }
}

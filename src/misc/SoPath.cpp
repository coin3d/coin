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
  \class SoPath Inventor/SoPath.h
  \brief The SoPath class is a container class for Open Inventor traversal
  path descriptions.

  SoPath objects contain a list of SoNode pointers and a list of child
  indices.  Indices are necessary to disambiguate situations where a
  node uses the same node as a child multiple times.
*/

/*¡
  <ul>
  <lh><b>TODO:</b>
  <li>implement change notification
  </ul>
*/

#include <Inventor/SoPath.h>

#include <Inventor/SbString.h>
#include <Inventor/SbName.h>
#include <Inventor/misc/SoBasic.h> // COIN_STUB()
#include <Inventor/nodes/SoNode.h>
#include <Inventor/nodes/SoGroup.h>
#include <Inventor/misc/SoChildList.h>

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

/*!
  \var SoPath::nodes

  FIXME: write doc.
*/

/*!
  \var SoPath::indices

  FIXME: write doc.
*/

/*!
  \var SoPath::doAuditors

  FIXME: write doc.
*/

/*!
  \var SoPath::classTypeId

  FIXME: write doc.
*/

SoType SoPath::classTypeId = SoType::badType();

/*!
  This static method returns the run-time type identifier for the SoPath
  class.
*/
SoType
SoPath::getClassTypeId(void)
{
  return SoPath::classTypeId;
}

/*!
  This virtual method returns the run-time type identifier for (in this case)
  this SoPath-derived object.
*/
SoType
SoPath::getTypeId(void) const
{
  return SoPath::classTypeId;
}

/*!
  This static method creates a new SoPath object and returns a pointer to it.
  Used by the run-time type system.  This is not standard Open Inventor.
*/
void *
SoPath::createInstance(void)
{
  return (void *) new SoPath;
}

/*!
  This static method initializes the run-time type system for the SoPath
  class.
*/
void
SoPath::initClass(void)
{
  assert((SoPath::classTypeId == SoType::badType()) &&
         "call SoPath::initClass only once!");

  SoPath::classTypeId = SoType::createType(SoBase::getClassTypeId(),
                                           SbName("Path"),
                                           &SoPath::createInstance);
}

/*!
  The default constructor.  \a approxLength is used to indicate in advance the
  length of the path so there won't be any reallocations.
*/
SoPath::SoPath(const int approxLength)
  : nodes(approxLength), indices(approxLength),
    doAuditors(FALSE), firstHidden(-1)
{
}

/*!
  A constructor.  The head argument is set as the head and only node of the
  path.
*/
SoPath::SoPath(SoNode * const head)
  : nodes(4), indices(4), doAuditors(FALSE)
{
  this->setHead(head);
}

/*!
  The copy constructor.

  This is not standard Open Inventor.
*/
SoPath::SoPath(const SoPath & rhs)
  : inherited(), nodes(rhs.getFullLength()), indices(rhs.getFullLength())
{
  operator = (rhs);
}

/*!
  Assignment operator.  Copies the rhs path into this.

  This is not standard Open Inventor.
*/
SoPath &
SoPath::operator = (const SoPath & rhs)
{
  if (this != &rhs) {
    this->truncate(0);
    const int length = rhs.getLength();
    for (int i = 0; i < length; i++) {
      this->append((SoNode *) rhs.nodes[i], rhs.indices[i]);
    }
  }
  return *this;
}

/*!
  The virtual destructor is protected.  Destruction happens through unref().
*/
SoPath::~SoPath(void)
{
  this->truncate(0); // to unref() the nodes.
}

/*!
  This method sets the head of the path.  If the object already has a path,
  it will be truncated.
*/
void
SoPath::setHead(SoNode * const node)
{
  this->truncate(0);
  this->firstHidden = -1;
  this->append(node, 0);
}

/*!
  This method returns the head of the path.
*/
SoNode *
SoPath::getHead(void) const
{
  if (this->nodes.getLength() == 0)
    return NULL;
  return (SoNode *) this->nodes[0];
}

/*!
  This function appends the childIndex'th child of the path's tail to the
  end of the path.  It can only be used after the head is set.
*/
void
SoPath::append(const int childIndex)
{
#if COIN_DEBUG
  if (this->nodes.getLength() == 0) {
    SoDebugError::postWarning("SoPath::append(int child)",
                              "SoPath was empty.\n");
    assert(0);
  }
#endif // COIN_DEBUG

  SoChildList *children =
    this->nodes[this->nodes.getLength()-1]->getChildren();
  assert(children);

#if COIN_DEBUG
  if (childIndex >= children->getLength()) {
    SoDebugError::postInfo("SoPath::append",
                           "childIndex (%d) out of bounds",
                           childIndex);
    assert(0 && "unexpected error");
  }
#endif // debug

  SoNode *node = (*children)[childIndex];
  this->append(node, childIndex);
}

/*!
  This function adds node to the end if it is a child of the current tail.
  If path is empty, this is equivalent to calling setHead().
*/
void
SoPath::append(SoNode * const node)
{
  if (this->nodes.getLength() == 0) {
    this->setHead(node);
    return;
  }
  SoChildList *children =
    this->nodes[this->nodes.getLength()-1]->getChildren();
  assert(children);

  const int idx = children->find((void*)node);
#if COIN_DEBUG && 1 // debug
  if (idx < 0) {
    SoDebugError::postInfo("SoPath::append",
                           "node not found as child of tail");
    assert(0 && "unexpected error");
  }
#endif // debug

  this->append(node, idx);
}

/*!
  This method appends the path to the end of the path if the head of
  fromPath is either the tail of the path or a child of the path's tail.
  If the head of fromPath is used multiple times as a child of the path
  tail, the lowest child index is used.
*/
void
SoPath::append(const SoPath * const fromPath)
{
  if (! this->nodes.getLength()) { // append to empty path
    operator = (*fromPath);
    return;
  }

  if (! fromPath->nodes.getLength()) // nothing to append
    return;

  SoNode * const head = fromPath->getHead();
  SoNode * const tail = this->nodes[ this->nodes.getLength() - 1 ];
  if (head == tail) { // easy
    const int length = fromPath->nodes.getLength();
    for (int i = 1; i < length; i++) {
      append((SoNode *) fromPath->nodes[i], fromPath->indices[i]);
    }
    if (this->firstHidden == -1 && fromPath->firstHidden >= 0) {
      this->findFirstHidden();
    }
    return;
  }
  // head of fromPath must be child

  SoChildList * tailchildren = tail->getChildren();

#if COIN_DEBUG
  if (tailchildren == NULL) {
    SoDebugError::postWarning("SoPath::append(SoPath)",
                              "The tail of this SoPath has no children node ('%s').\n",
                              tail->getTypeId().getName().getString());
    assert(0 && "programming error");
  }
#endif // COIN_DEBUG

  const int kids = tailchildren->getLength();
  for (int kid = 0; kid < kids; kid++) {
    SoNode * child = (*tailchildren)[kid];
    if (child == head) {
      this->append(head, kid);
      const int length = fromPath->nodes.getLength();
      for (int i = 1; i < length; i++) {
        append((SoNode*)fromPath->nodes[i], fromPath->indices[i]);
      }
      if (this->firstHidden == -1 && fromPath->firstHidden >= 0) {
        this->findFirstHidden();
      }
      return;
    }
  }

#if COIN_DEBUG
  SoDebugError::postWarning("SoPath::append(SoPath *)",
                            "Could not join paths.\n");
#endif // COIN_DEBUG
  assert(0);
}

/*!
  This method appends a node to the path, assuming the information is
  correct.
*/
void
SoPath::append(SoNode * const node, const int index)
{
  if (this->firstHidden < 0) {
    if (this->hasHiddenChildren(node))
      this->firstHidden = this->nodes.getLength();
  }
  this->nodes.append(node);
  this->indices.append(index);
}

/*!
  This method pushes the childIndex'th child of the tail on the path.
*/
void
SoPath::push(const int childIndex)
{
  this->append(childIndex);
}

/*!
  This method pops the tail of the path off the path.
*/
void
SoPath::pop(void)
{
  this->truncate(this->getFullLength() - 1);
}

/*!
  This method returns the tail of the path.
*/
SoNode *
SoPath::getTail(void) const
{
  if (this->getLength() == 0) {
#if COIN_DEBUG
    SoDebugError::postWarning("SoPath::getTail()", "Empty SoPath!\n");
#endif // COIN_DEBUG
    return NULL;
  }
  return (SoNode *) this->nodes[this->getLength() - 1];
}

/*!
  This method returns the index'th node in the path.
*/
SoNode *
SoPath::getNode(const int index) const
{
#if COIN_DEBUG
  if (index < 0 || index >= this->nodes.getLength()) {
    SoDebugError::postWarning("SoPath::getNode( int)",
                              "index is out of bounds.\n");
    assert(0);
  }
#endif // COIN_DEBUG
  return (SoNode *) this->nodes[index];
}

/*!
  This method returns the index'th node in the path, counting from the
  tail.
*/
SoNode *
SoPath::getNodeFromTail(const int index) const
{
#if COIN_DEBUG
  if (index < 0 || index >= this->getLength()) {
    SoDebugError::postWarning("SoPath::getNodeFromTail()",
                              "index is out of bounds.\n");
    assert(0);
  }
#endif // COIN_DEBUG
  return (SoNode *) this->nodes[this->getLength() - index - 1];
}

/*!
  This method returns the child index of the index'th node in the path.
*/
int
SoPath::getIndex(const int index) const
{
#if COIN_DEBUG
  if (index < 0 || index >= this->indices.getLength()) {
    SoDebugError::postWarning("SoPath::getIndex()",
                              "index is out of bounds.\n");
    assert(0);
  }
#endif // COIN_DEBUG
  return this->indices[index];
}

/*!
  This method returns the child index of the index'th node in the path,
  counting from the tail.
*/
int
SoPath::getIndexFromTail(const int index) const
{
#if COIN_DEBUG
  if (index < 0 || index >= this->getLength()) {
    SoDebugError::postWarning("SoPath::getIndexFromTail()",
                              "index is out of bounds.\n");
    assert(0);
  }
#endif // COIN_DEBUG
  return this->indices[this->getLength() - index - 1];
}

/*!
  This method returns the number of nodes in the path.
*/
int
SoPath::getLength(void) const
{
  if (this->firstHidden >= 0) {
    assert(this->firstHidden < this->nodes.getLength());
    return this->firstHidden + 1;
  }
  return this->nodes.getLength();
}

/*!
  This method returns the number of nodes in the path, including hidden nodes.
*/
int
SoPath::getFullLength(void) const
{
  return this->nodes.getLength();
}

/*!
  This method truncates the path to the given length.
*/
void
SoPath::truncate(const int length)
{
  this->truncate(length, TRUE);
}

/*!
  This method truncates the path to the given length.

  The doNotify flag is currently ignored.
*/
void
SoPath::truncate(const int length, const SbBool /*doNotify*/)
{
#if COIN_DEBUG
  if (length < 0 || length > this->nodes.getLength()) {
    SoDebugError::postWarning("SoPath::truncate()",
                              "\"truncate\" means shorten, not lengthen...\n");
    assert(0);
  }
#endif // COIN_DEBUG
  this->nodes.truncate(length);
  this->indices.truncate(length);

  if (length <= this->firstHidden) this->findFirstHidden();
}

// *************************************************************************

/*!
  This method returns the index of the last node that is the same for both
  paths, or -1 if the paths don't even start on the same node.
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
  Returns the node index if found in path, -1 otherwise
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

// *************************************************************************
// some tests

/*!
  This method returns TRUE is node is contained in path, and FALSE otherwise.
*/
SbBool
SoPath::containsNode(const SoNode * const node) const
{
  const int listlen = this->nodes.getLength();
  for (int i = 0; i < listlen; i++) {
    if (node == this->nodes[i]) return TRUE;
  }
  return FALSE;
}

/*!
  This method returns TRUE if path is contained in the path, and FALSE
  otherwise.
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
  This friend operator returns TRUE if path1 equals path2 and FALSE
  otherwise.
*/
SbBool
operator == (const SoPath & lhs, const SoPath & rhs)
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
  This friend operator returns FALSE if path1 equals path2 and TRUE otherwise.
*/
SbBool
operator != (const SoPath & lhs, const SoPath & rhs)
{
  return ! operator == (lhs, rhs);
}

// *************************************************************************

/*!
  This method returns a copy of the path starting at startFromNodeIndex with
  length numNodes.  If numNodes is 0 (default) the path will be copied to
  the end.
*/
SoPath *
SoPath::copy(const int startFromNodeIndex, int numNodes) const
{
#if COIN_DEBUG
  if (startFromNodeIndex < 0 ||
      startFromNodeIndex >= this->nodes.getLength()) {
    SoDebugError::postWarning("SoPath::copy()",
                              "startFromNodeIndex was out of bounds.");
    assert(0);
  }
#endif // COIN_DEBUG
  if (numNodes == 0)
    numNodes = this->nodes.getLength() - startFromNodeIndex;
  assert(numNodes > 0);

#if COIN_DEBUG
  if ((startFromNodeIndex + numNodes) > this->nodes.getLength()) {
    SoDebugError::postWarning("SoPath::copy()",
                              "Not numNodes nodes to copy.\n");
    assert(0);
  }
#endif // COIN_DEBUG

  SoPath * newPath = new SoPath(numNodes);
  const int max = startFromNodeIndex + numNodes;
  for (int i = startFromNodeIndex; i < max; i++) {
    newPath->append((SoNode *) this->nodes[ i ], this->indices[ i ]);
  }
  newPath->findFirstHidden();
  return newPath;
}

// *************************************************************************

/*!
  This static method is for retrieving a SoPath by it's name.
  The first SoPath found with the given name is returned, or NULL if no
  such SoPath exists.

  This method is not implemented.
*/
SoPath *
SoPath::getByName(const SbName /*name*/)
{
  COIN_STUB();
  return NULL;
}

/*!
  This static method is for finding all the paths with a given name and
  append them to list.  The number of lists found is returned.
*/
int
SoPath::getByName(const SbName /*name*/, SoPathList & /*list*/)
{
  COIN_STUB();
  return 0;
}

// *************************************************************************

/*!
  This method is called when a node in the path chain has a child added,
  to update the index of it's child.  newIndex is the index of the new
  child.
*/
void
SoPath::insertIndex(SoNode * const parent, const int newIndex)
{
  int pos = this->findNode(parent) + 1;
  assert(pos > 0); // shouldn't be notified if parent is not in path
  if (pos < this->nodes.getLength()) {
    if (newIndex <= this->indices[pos]) {
      this->indices[pos] = this->indices[pos] + 1;
    }
  }
}

/*!
  This method is called when a node in the path chain has a child removed,
  to update the index of it's child.  oldIndex was the index of the removed
  child.
*/
void
SoPath::removeIndex(SoNode * const parent, const int oldIndex)
{
  int pos = this->findNode(parent) + 1;
  assert(pos > 0); // shouldn't be notified if parent is not in path
  if (pos < this->nodes.getLength()) {
    if (oldIndex < this->indices[pos]) {
      this->indices[pos] = this->indices[pos] - 1;
    }
    else if (oldIndex == this->indices[pos]) {
      // if node in path is removed, we have to truncate path
      this->truncate(pos);
    }
  }
}

/*!
  This method is called when a node in the path chain replaces a child.
  Index is the index of the replaced child.
*/
void
SoPath::replaceIndex(SoNode * const parent, const int index,
                     SoNode * const /*newChild*/)
{
  int pos = this->findNode(parent) + 1;
  assert(pos > 0); // shouldn't be notified if parent is not in path
  if (pos < this->nodes.getLength()) {
    if (index == this->indices[pos]) {
      // FIXME: not sure about this one. I don't think we should
      // use newChild in the path, since the path beyond newChild will
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
SoPath::write(SoWriteAction * /*writeAction*/) const
{
  COIN_STUB();
}

// *************************************************************************

/*!
  Not implemented yet.
*/
SbBool
SoPath::isRelevantNotification(SoNotList * const /*list*/) const
{
  COIN_STUB();
  return FALSE;
}

/*!
  This method is not used.
*/
void
SoPath::auditPath(const SbBool flag)
{
  this->doAuditors = flag;
}

// *************************************************************************

/*!
  This virtual method is used to read a SoPath from the in stream.

  Not implemented.
*/
SbBool
SoPath::readInstance(SoInput * /*in*/, unsigned short /*flags*/)
{
  COIN_STUB();
  return TRUE;
}

//
// private convenience method that tests if a node has hidden children.
// It would probably be a good idea to move this method to SoNode.
//
SbBool
SoPath::hasHiddenChildren(SoNode *node) const
{
  return (node->getChildren() != NULL) &&
    !node->isOfType(SoGroup::getClassTypeId());
}

//
// private method that scans the nodes and finds the first (if any)
// node with hidden children. If a node with hidden children is found,
// getLength() will return this as the length of the path. This also
// affects all tail-operations.
//
void
SoPath::findFirstHidden()
{
  this->firstHidden = -1;
  int n = this->nodes.getLength();
  for (int i = 0; i < n; i++) {
    if (this->hasHiddenChildren(this->nodes[i])) {
      this->firstHidden = i;
      return;
    }
  }
}

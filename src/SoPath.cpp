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
#include <Inventor/nodes/SoNode.h>
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
  \var SoPath::numPublic

  FIXME: write doc.
*/

/*!
  \var SoPath::minNumPublic

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

SoType SoPath::classTypeId;

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
  SoPath::classTypeId = SoType::createType(SoBase::getClassTypeId(),
                            SbName("SoPath"), &SoPath::createInstance);
}

/*!
  This method should clean up after the SoPath class.  Does nothing at the
  moment.
*/

void
SoPath::cleanClass(void)
{
}

/*!
  The default constructor.  \a approxLength is used to indicate in advance the
  length of the path so there won't be any reallocations.
*/

SoPath::SoPath(const int approxLength)
  : nodes(approxLength), indices(approxLength)
{
  numPublic = minNumPublic = 0;
  doAuditors = FALSE;
}

/*!
  A constructor.  The head argument is set as the head and only node of the
  path.
*/

SoPath::SoPath(SoNode * const head)
  : nodes(1), indices(1)
{
  setHead(head);
}

/*!
  The copy constructor.

  This is not standard Open Inventor.
*/

SoPath::SoPath(const SoPath & rhs)
  : nodes(rhs.getFullLength()), indices(rhs.getFullLength())
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
  truncate(0);
  const int length = rhs.getLength();
  for (int i = 0; i < length; i++) {
    SoNode * const node = (SoNode *) rhs.nodes[i];
    append((SoNode *) rhs.nodes[i], rhs.indices[i]);
  }
  return *this;
}

/*!
  The virtual destructor is protected.  Destruction happens through unref().
*/

SoPath::~SoPath(void)
{
  truncate(0); // to unref() the nodes.
}

/*!
  This method sets the head of the path.  If the object already has a path,
  it will be truncated.
*/

void
SoPath::setHead(SoNode * const node)
{
  truncate(0);
  append(node, 0);
  this->numPublic = 1;
  this->minNumPublic = 1;
}

/*!
  This method returns the head of the path.
*/

SoNode *
SoPath::getHead(void) const
{
  if (! this->nodes.getLength())
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
  if (! this->nodes.getLength()) {
    SoDebugError::postWarning("SoPath::append(int child)",
      "SoPath was empty.\n");
    assert(0);
  }
#endif // COIN_DEBUG

  const int last = this->nodes.getLength() - 1;
  SoNode * parent = this->nodes[last];
  assert(parent);
  SoChildList * children = parent->getChildren();

#if COIN_DEBUG
  if (!children) {
    SoDebugError::postWarning("SoPath::append()",
			      "SoPath tail has no children ('%s').\n",
			      parent->getTypeId().getName().getString());
    assert(0);
  }
#endif // COIN_DEBUG
  
  SoNode * node = (*children)[childIndex];
  assert(node);

  append(node, childIndex);
}

/*!
  This function adds node to the end if it is a child of the current tail.
  If path is empty, this is equivalent to calling setHead().
*/

void
SoPath::append(SoNode * const node)
{
  if (! this->nodes.getLength()) {
    setHead(node);
    return;
  }

  const int last = this->nodes.getLength() - 1;
  SoNode * parent = this->nodes[last];
  SoChildList * children = parent->getChildren(); 
#if COIN_DEBUG
  if (! children) {
    SoDebugError::postWarning("SoPath::append(SoNode *)",
			       "SoPath tail has no children ('%s').",
			       parent->getTypeId().getName().getString());
    assert(0);
  }
#endif // COIN_DEBUG
  const int kids = children->getLength();
  int kid;
  for (kid = 0; kid < kids; kid++) {
    if ((*children)[kid] == node ) break;
  }
  assert(kid < kids);
  this->append(node, kid);
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

  if (! fromPath->getLength()) // nothing to append
    return;

  SoNode * const head = fromPath->getHead();
  SoNode * const tail = this->nodes[ this->nodes.getLength() - 1 ];
  if (head == tail) { // easy
    const int length = fromPath->getLength();
    for (int i = 1; i < length; i++)
      append((SoNode *) fromPath->nodes[i], fromPath->indices[i]);
    return;
  }
  // head of fromPath must be child

  SoChildList * tailchildren = tail->getChildren();

#if COIN_DEBUG
  if (! tailchildren) {
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
      append(head, kid);
      const int length = fromPath->getLength();
      for (int i = 1; i < length; i++) {
        append((SoNode *) fromPath->nodes[i], fromPath->indices[i]);
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
SoPath::append(
    SoNode * const node,
    const int index)
{
    this->nodes.append(node);
    this->indices.append(index);
    node->ref();
}

/*!
  This method pushes the childIndex'th child of the tail on the path.
*/

void
SoPath::push(const int childIndex)
{
  append(childIndex);
}

/*!
  This method pops the tail of the path off the path.
*/

void
SoPath::pop(void)
{
  this->truncate(getLength() - 1);
}

/*!
  This method returns the tail of the path.
*/

SoNode *
SoPath::getTail(void) const
{
  if (! this->nodes.getLength()) {
#if COIN_DEBUG
    SoDebugError::postWarning("SoPath::getTail()", "Empty SoPath!\n");
#endif // COIN_DEBUG
    return NULL;
  }
  return (SoNode *) this->nodes[ this->nodes.getLength() - 1 ];
}

/*!
  This method returns the index'th node in the path.
*/

SoNode *
SoPath::getNode(const int index) const
{
#if COIN_DEBUG
  if ( index < 0 || index >= this->nodes.getLength() ) {
    SoDebugError::postWarning( "SoPath::getNode( int )",
      "index is out of bounds.\n" );
    assert( 0 );
  }
#endif // COIN_DEBUG
  return (SoNode *) this->nodes[ index ];
}

/*!
  This method returns the index'th node in the path, counting from the
  tail.
*/

SoNode *
SoPath::getNodeFromTail(const int index) const
{
#if COIN_DEBUG
  if ( index < 0 || index >= this->nodes.getLength() ) {
    SoDebugError::postWarning( "SoPath::getNodeFromTail()",
      "index is out of bounds.\n" );
    assert( 0 );
  }
#endif // COIN_DEBUG
  return (SoNode *) this->nodes[ this->nodes.getLength() - index - 1 ];
}

/*!
  This method returns the child index of the index'th node in the path.
*/

int
SoPath::getIndex(const int index) const
{
#if COIN_DEBUG
  if ( index < 0 || index >= this->indices.getLength() ) {
    SoDebugError::postWarning( "SoPath::getIndex()",
      "index is out of bounds.\n" );
    assert( 0 );
  }
#endif // COIN_DEBUG
  return this->indices[ index ];
}

/*!
  This method returns the child index of the index'th node in the path,
  counting from the tail.
*/

int
SoPath::getIndexFromTail(const int index) const
{
#if COIN_DEBUG
  if ( index < 0 || index >= this->indices.getLength() ) {
    SoDebugError::postWarning( "SoPath::getIndexFromTail()",
      "index is out of bounds.\n" );
    assert( 0 );
  }
#endif // COIN_DEBUG
  return this->indices[ this->indices.getLength() - index - 1 ];
}

/*!
  This method returns the number of nodes in the path.
*/

int
SoPath::getLength(void) const
{
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
SoPath::truncate(const int length, const SbBool doNotify)
{
  if (doNotify);
#if COIN_DEBUG
  if (length < 0 || length > this->nodes.getLength()) {
    SoDebugError::postWarning("SoPath::truncate()",
      "\"truncate\" means shorten, not lengthen...\n");
    assert(0);
  }
#endif // COIN_DEBUG
  for (int i = this->nodes.getLength() - 1; i >= length; i--)
    this->nodes[i]->unref();
  this->nodes.truncate(length);
  this->indices.truncate(length);
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
  int i;
  for (i = 0;
        (i < len) &&
        (this->nodes[i] == path->nodes[i]) &&
        ((i == 0) || (this->indices[i] == path->indices[i]));
        i++);
  return i - 1;
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
    if (node == (SoNode *) this->nodes[i])
      return TRUE;
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
  const int thislen = this->nodes.getLength();
  const int pathlen = path->getFullLength();
  for (int i = 0; i < (thislen - pathlen); i++) {
    // this could be rewritten for readability...
    int j;
    for (j = 0;
          (j < pathlen) &&
          (path->nodes[j] == this->nodes[i+j]) &&
          ((j == 0) || (path->indices[j] == this->indices[i+j]));
          j++) { };
    if (j == pathlen)
      return TRUE;
  }
  return FALSE;
}

/*!
  This friend operator returns TRUE if path1 equals path2 and FALSE
  otherwise.
*/

SbBool
operator == (const SoPath & lhs, const SoPath & rhs)
{
  if (&lhs == &rhs)
    return TRUE;
  if (lhs.nodes.getLength() != rhs.nodes.getLength())
    return FALSE;

  const int len = lhs.nodes.getLength();
  for (int i = 0; i < len; i++)
    if ((lhs.nodes[i] != rhs.nodes[i]) ||
         ((i > 0) && (lhs.indices[i] != rhs.indices[i])))
      return FALSE;
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
  for (int i = startFromNodeIndex; i < max; i++)
    newPath->append((SoNode *) this->nodes[ i ], this->indices[ i ]);
  return newPath;
}

// *************************************************************************

/*!
  This static method is for retrieving a SoPath by it's name.
  The first SoPath found with the given name is returned, or NULL if no
  such SoPAth exists.

  This method is not implemented.
*/

SoPath *
SoPath::getByName(const SbName name)
{
  if (&name);
  assert(0);
  return NULL;
}

/*!
  This static method is for finding all the paths with a given name and
  append them to list.  The number of lists found is returned.
*/

int
SoPath::getByName(const SbName name, SoPathList & list)
{
  if (&name);
  if (&list);
  assert(0);
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
  const int len = this->nodes.getLength();
  for (int i = 0; i < (len - 1); i++) {
    if (this->nodes[i] == parent && newIndex <= this->indices[i+1]) {
      this->indices[i+1]++;
      return; // correct?
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
  const int len = this->nodes.getLength();
  for (int i = 0; i < (len - 1); i++) {
    if (this->nodes[i] == parent) {
      if (oldIndex < this->indices[i+1]) {
        this->indices[i+1]--;
        return; // ?
      } else if (oldIndex == this->indices[i+1]) {
        // child in chain is removed
        truncate(i, FALSE);
        return; // ?
      }
    }
  }
}

/*!
  This method is called when a node in the path chain replaces a child.
  Index is the index of the replaced child.
*/

void
SoPath::replaceIndex(
  SoNode * const parent, const int index, SoNode * const newChild)
{
  const int len = this->nodes.getLength();
  for (int i = 0; i < (len - 1); i++) {
    if (this->nodes[i] == parent && index == this->indices[i+1]) {
      // child in chain is replaced
      truncate(i, FALSE);
      append(newChild, index);
      return;
    }
  }
}

// *************************************************************************

/*!
  This method is used to write an SoPath.
*/

void
SoPath::write(SoWriteAction * writeAction) const
{
    assert(0); // FIXME: implement
    if (writeAction);
}

// *************************************************************************

// FIXME: rewrite doc.
/*!
  This function returns TRUE if the given notification list involves a
  change to a node that affects the path, and FALSE otherwise.  It is
  assumed that the last (most recent) node in the list is the head node of
  the path.

  Not implemented yet.
*/

SbBool
SoPath::isRelevantNotification(SoNotList * const list) const
{
    if (list);
    assert(0); // FIXME: imlement
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
SoPath::readInstance(SoInput * in, unsigned short flags)
{
  if (in);
  if (flags);
  assert(0); // FIXME: implement
  return TRUE;
}

/**************************************************************************\
 * Copyright (c) Kongsberg Oil & Gas Technologies AS
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 * 
 * Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 * 
 * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 * 
 * Neither the name of the copyright holder nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
\**************************************************************************/

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif // HAVE_CONFIG_H

#ifdef HAVE_NODEKITS

/*!
  \class SoNodeKitPath SoNodeKitPath.h Inventor/SoNodeKitPath.h
  \brief The SoNodeKitPath class presents the nodekit projection of a path.

  \ingroup coin_nodekits

  Only nodekits are visible through the nodekit-specific accessors, while
  the complete route is retained.
*/

// FIXME: SoNodeKitPath still needs access to SoPath's private route for
// materialization and mutation. 20020119 mortene, 20261006 Dikluwe.

#include <Inventor/SoNodeKitPath.h>

#include <cstdlib>
#include <vector>

#include <Inventor/nodekits/SoBaseKit.h>
#include <Inventor/misc/SoChildList.h>
#include <Inventor/actions/SoSearchAction.h>

#include "tidbitsp.h"

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

namespace {
class SearchChildrenGuard {
public:
  SearchChildrenGuard()
    : previous(SoBaseKit::isSearchingChildren())
  {
    SoBaseKit::setSearchingChildren(TRUE);
  }
  ~SearchChildrenGuard()
  {
    SoBaseKit::setSearchingChildren(this->previous);
  }
private:
  SbBool previous;
};
} // namespace

SoSearchAction * SoNodeKitPath::searchAction;

/*!
  \class SoNodeKitPathView SoPath.h Inventor/SoPath.h
  \brief A borrowed, allocation-free nodekit projection of a SoPath.

  Only nodekits in the complete route are visible, including the head only
  when it is a nodekit.
  The view reflects changes to its source path and must not outlive it.
  In builds without nodekit support, the projection is empty.
*/

/*!
  \fn SoNodeKitPathView SoPath::nodeKitPath(void) const
  Returns a borrowed nodekit projection without copying the path or changing
  reference counts. Use SoNodeKitPath::fromPath() when an independent,
  mutable nodekit path is required.
*/
SoNodeKitPathView
SoPath::nodeKitPath(void) const
{
  return SoNodeKitPathView(*this);
}

SoNodeKitPathView::SoNodeKitPathView(const SoPath & sourcepath)
  : path(&sourcepath)
{
}

/*!
  Returns the number of nodekits in the complete route.
*/
int
SoNodeKitPathView::getLength(void) const
{
  const int length = this->path->fullPath().getLength();
  int count = 0;
  for (int i = 0; i < length; ++i) {
    SoNode * node = this->path->getNode(i);
    if (node != NULL && node->isOfType(SoBaseKit::getClassTypeId())) ++count;
  }
  return count;
}

/*!
  Returns the last nodekit in the complete route, or \c NULL when there is
  no nodekit.
*/
SoNode *
SoNodeKitPathView::getTail(void) const
{
  const int length = this->path->fullPath().getLength();
  for (int i = length - 1; i >= 0; --i) {
    SoNode * node = this->path->getNode(i);
    if (node != NULL && node->isOfType(SoBaseKit::getClassTypeId())) return node;
  }
  return NULL;
}

/*!
  Returns nodekit number \a index, or \c NULL for an invalid index.
*/
SoNode *
SoNodeKitPathView::getNode(const int index) const
{
  const int length = this->path->fullPath().getLength();
  if (index < 0) return NULL;

  int count = 0;
  for (int i = 0; i < length; ++i) {
    SoNode * node = this->path->getNode(i);
    if (node != NULL && node->isOfType(SoBaseKit::getClassTypeId())) {
      if (count++ == index) return node;
    }
  }
  return NULL;
}

/*!
  Returns projected node \a index from the logical tail. Returns \c NULL
  for an invalid index.
*/
SoNode *
SoNodeKitPathView::getNodeFromTail(const int index) const
{
  const int length = this->getLength();
  if (index < 0 || index >= length) return NULL;
  return this->getNode(length - index - 1);
}

/*!
  A constructor.
*/
SoNodeKitPath::SoNodeKitPath(const int approxLength)
  : SoPath(approxLength)
{
}

/*!
  The destructor.
*/
SoNodeKitPath::~SoNodeKitPath()
{
}

/*!
  Materializes a genuine SoNodeKitPath containing an independent copy of the
  complete route in \a path. Returns \c NULL when \a path is \c NULL.
*/
SoNodeKitPath *
SoNodeKitPath::fromPath(const SoPath * path)
{
  if (path == NULL) return NULL;

  const int length = path->nodes.getLength();
  std::vector<SoChildList *> registered;
  registered.reserve(length);
  SoNodeKitPath * result = new SoNodeKitPath(length);
  try {
    // Register auditors before copying nodes. A child list can allocate or
    // throw; until registration is complete, the new path remains empty.
    for (int i = 0; i < length; i++) {
      SoNode * node = path->nodes[i];
      SoChildList * children = node ? node->getChildren() : NULL;
      if (children) {
        children->addPathAuditor(result);
        registered.push_back(children);
      }
    }

    // Both lists reserved length slots in the constructor.
    for (int i = 0; i < length; i++) {
      result->nodes.append(path->nodes[i]);
      result->indices.append(path->indices[i]);
    }
    result->firsthidden = path->firsthidden;
    result->firsthiddendirty = path->firsthiddendirty;
  }
  catch (...) {
    for (std::vector<SoChildList *>::reverse_iterator it = registered.rbegin();
         it != registered.rend(); ++it) {
      (*it)->removePathAuditor(result);
    }
    // Registrations have been removed, including if a list copy failed.
    // Avoid auditing a partially populated route during destruction.
    result->isauditing = FALSE;
    result->ref();
    result->unref();
    throw;
  }
  return result;
}

/*!
  Returns the number of nodekits in the complete route.
*/
int
SoNodeKitPath::getLength(void) const
{
  return this->nodeKitPath().getLength();
}

/*!
  Returns the tail of the path (the last nodekit in the path).
*/
SoNode *
SoNodeKitPath::getTail(void) const
{
  return this->nodeKitPath().getTail();
}

/*!
  Returns nodekit number \a idx in path.
*/
SoNode *
SoNodeKitPath::getNode(const int idx) const
{
  const SoNodeKitPathView view = this->nodeKitPath();
  if (idx >= 0 && idx < view.getLength()) return view.getNode(idx);
#if COIN_DEBUG
  SoDebugError::postInfo("SoNodeKitPath::getNode",
                         "index %d out of bounds", idx);
#endif // COIN_DEBUG
  return NULL;
}

/*!
  Returns nodekit number \a idx in the path, from the tail.
*/
SoNode *
SoNodeKitPath::getNodeFromTail(const int idx) const
{
  const SoNodeKitPathView view = this->nodeKitPath();
  if (idx >= 0 && idx < view.getLength()) return view.getNodeFromTail(idx);

#if COIN_DEBUG
  SoDebugError::postInfo("SoNodeKitPath::getNodeFromTail",
                         "index %d out of bounds", idx);
#endif // COIN_DEBUG
  return NULL;
}

/*!
  Truncates the path at nodekit number \a length.
*/
void
SoNodeKitPath::truncate(const int length)
{
  const int projectedlength = this->getLength();
  if (length == projectedlength) return;
  if (length >= 0 && length < projectedlength) {
    int cnt = 0;
    const int n = this->nodes.getLength();
    for (int i = 0; i < n; i++) {
      if (this->nodes[i] != NULL &&
        this->nodes[i]->isOfType(SoBaseKit::getClassTypeId()) &&
          cnt++ == length) {
        SoPath::truncate(i);
        return;
      }
    }
  }
#if COIN_DEBUG
  SoDebugError::postInfo("SoNodeKitPath::truncate",
                         "illegal length: %d", length);
#endif // COIN_DEBUG
}

/*!
  Pops off the last nodekit (truncates at last tail).
*/
void
SoNodeKitPath::pop(void)
{
  const int length = this->getLength();
  if (length > 0) this->truncate(length - 1);
}

/*!
  Appends \a childKit to the path. childKit should be a part in the
  tail nodekit of this path. In effect, the path from the tail to first
  occurrence of \a childKit will be appended to the path.
*/
void
SoNodeKitPath::append(SoBaseKit * childKit)
{
  if (this->getLength() == 0) {
    this->setHead(childKit);
    return;
  }

  SoNode * tailnode = this->getTail();
  if (tailnode == NULL ||
      !tailnode->isOfType(SoBaseKit::getClassTypeId())) {
#if COIN_DEBUG
    SoDebugError::postInfo("SoNodeKitPath::append",
                           "the logical tail is not a nodekit");
#endif // COIN_DEBUG
    return;
  }

  SoBaseKit * tail = static_cast<SoBaseKit *>(tailnode);
  SoSearchAction * sa = this->getSearchAction();
  sa->setNode(childKit);
  {
    SearchChildrenGuard searchchildren;
    sa->apply(tail);
  }

  SoPath * path = sa->getPath();
  if (path == NULL) {
#if COIN_DEBUG
    SoDebugError::postInfo("SoNodeKitPath::append",
                           "childKit not found as part of tail");
#endif // COIN_DEBUG
    return;
  }

  int tailindex = this->nodes.getLength() - 1;
  while (this->nodes[tailindex] != tail) --tailindex;
  SoPath::truncate(tailindex + 1);
  SoPath::append(path);
}

/*!
  Appends the nodekit path to this path. Head of \a fromPath must
  be a part in the current tail.
*/
void
SoNodeKitPath::append(const SoNodeKitPath * fromPath)
{
  if (fromPath->nodes.getLength() == 0) return;
  if (this->nodes.getLength() == 0) {
    this->SoPath::operator=(*fromPath);
    return;
  }

  SoNode * tailnode = this->getTail();
  if (tailnode == NULL ||
      !tailnode->isOfType(SoBaseKit::getClassTypeId())) {
#if COIN_DEBUG
    SoDebugError::postInfo("SoNodeKitPath::append",
                           "the logical tail is not a nodekit");
#endif // COIN_DEBUG
    return;
  }

  SoBaseKit * tail = static_cast<SoBaseKit *>(tailnode);
  SoNode * sourcehead = fromPath->nodes[0];
  SoPath * bridge = NULL;
  if (tail != sourcehead) {
    SoSearchAction * sa = this->getSearchAction();
    sa->setNode(sourcehead);
    {
      SearchChildrenGuard searchchildren;
      sa->apply(tail);
    }
    bridge = sa->getPath();
    if (bridge == NULL) {
#if COIN_DEBUG
      SoDebugError::postInfo("SoNodeKitPath::append",
                             "source head not found as part of tail");
#endif // COIN_DEBUG
      return;
    }
  }

  int tailindex = this->nodes.getLength() - 1;
  while (this->nodes[tailindex] != tail) --tailindex;
  SoPath::truncate(tailindex + 1);
  if (bridge != NULL) SoPath::append(bridge);
  SoPath::append(static_cast<const SoPath *>(fromPath));
}

/*!
  Returns \c TRUE if \a node is in this path.
*/
SbBool
SoNodeKitPath::containsNode(SoBaseKit * node) const
{
  const int length = this->getLength();
  for (int i = 0; i < length; i++) {
    if (this->getNode(i) == node) return TRUE;
  }
  return FALSE;
}

/*!
  Returns the index of last common nodekit, or -1 if head
  node differs.
*/
int
SoNodeKitPath::findFork(const SoNodeKitPath * path) const
{
  int i;
  const int n = SbMin(this->getLength(), path->getLength());
  for (i = 0; i < n; i++) {
    if (this->getNode(i) != path->getNode(i)) break;
  }
  return i-1;
}

/*!
  Returns \c TRUE if paths are equal, \c FALSE otherwise.
*/
int
operator==(const SoNodeKitPath & p1, const SoNodeKitPath & p2)
{
  if (&p1 == &p2) return TRUE;
  int n = p1.getLength();
  if (n != p2.getLength()) return FALSE;

  for (int i = 0; i < n; i++) {
    if (p1.getNode(i) != p2.getNode(i)) return FALSE;
  }
  return TRUE;
}

/*!
  Returns \c TRUE if paths are not equal, \c FALSE otherwise.
*/
int
operator!=(const SoNodeKitPath & p1, const SoNodeKitPath & p2)
{
  return !(p1 == p2);
}


//
// atexit() method
//
void
SoNodeKitPath::clean(void)
{
  delete SoNodeKitPath::searchAction;
  SoNodeKitPath::searchAction = NULL;
}

//
// returns a search action to be used while searching for nodes
//
SoSearchAction *
SoNodeKitPath::getSearchAction(void)
{
  if (SoNodeKitPath::searchAction == NULL) {
    SoNodeKitPath::searchAction = new SoSearchAction();
    searchAction->setInterest(SoSearchAction::FIRST);
    searchAction->setSearchingAll(FALSE);
    coin_atexit((coin_atexit_f *)SoNodeKitPath::clean, CC_ATEXIT_NORMAL);
  }
  return SoNodeKitPath::searchAction;
}

//
// private methods, just to keep the user from making mistakes
//

void
SoNodeKitPath::append(const int)
{

}

void
SoNodeKitPath::append(SoNode *)
{
}

void
SoNodeKitPath::append(const SoPath *)
{
}

void
SoNodeKitPath::push(const int)
{
}

int
SoNodeKitPath::getIndex(const int) const
{
  return 0;
}

int
SoNodeKitPath::getIndexFromTail(const int) const
{
  return 0;
}

void
SoNodeKitPath::insertIndex(SoNode *, const int)
{
}

void
SoNodeKitPath::removeIndex(SoNode *,const int)
{
}

void
SoNodeKitPath::replaceIndex(SoNode *, const int, SoNode *)
{
}

#endif // HAVE_NODEKITS

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

  The head is always visible. Later nodes that are not nodekits are hidden
  from the nodekit-specific accessors, while the complete route is retained.
*/

// FIXME: We now need a "friend class SoNodeKitPath;" in the SoPath
// definition -- could we do without it? That would clean up the
// implementation a bit. 20020119 mortene.

#include <Inventor/SoNodeKitPath.h>

#include <cstdlib>

#include <Inventor/nodekits/SoBaseKit.h>
#include <Inventor/actions/SoSearchAction.h>

#include "tidbitsp.h"

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

SoSearchAction * SoNodeKitPath::searchAction;

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
  SoNodeKitPath * result = new SoNodeKitPath(length);
  if (length > 0) {
    result->SoPath::setHead(path->nodes[0]);
    for (int i = 1; i < length; i++) {
      result->SoPath::append(path->nodes[i], path->indices[i]);
    }
  }
  return result;
}

/*!
  Returns the length of the nodekit projection. The head is always included;
  later non-nodekit nodes are hidden.
*/
int
SoNodeKitPath::getLength(void) const
{
  const int n = this->nodes.getLength();
  if (n == 0) return 0;

  int cnt = 1;
  for (int i = 1; i < n; i++) {
    if (this->nodes[i]->isOfType(SoBaseKit::getClassTypeId())) cnt++;
  }
  return cnt;
}

/*!
  Returns the tail of the path (the last nodekit in the path).
*/
SoNode *
SoNodeKitPath::getTail(void) const
{
  const int n = this->nodes.getLength();
  if (n == 0) return NULL;

  for (int i = n - 1; i > 0; i--) {
    if (this->nodes[i]->isOfType(SoBaseKit::getClassTypeId()))
      return this->nodes[i];
  }
  return this->nodes[0];
}

/*!
  Returns nodekit number \a idx in path.
*/
SoNode *
SoNodeKitPath::getNode(const int idx) const
{
  const int n = this->nodes.getLength();
  if (idx < 0 || n == 0) {
#if COIN_DEBUG
    SoDebugError::postInfo("SoNodeKitPath::getNode",
                           "index %d out of bounds", idx);
#endif // COIN_DEBUG
    return NULL;
  }
  if (idx == 0) return this->nodes[0];

  int cnt = 1;
  for (int i = 1; i < n; i++) {
    if (this->nodes[i]->isOfType(SoBaseKit::getClassTypeId())) {
      if (cnt++ == idx) return this->nodes[i];
    }
  }
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
  const int length = this->getLength();
  if (idx >= 0 && idx < length) return this->getNode(length - idx - 1);

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
  if (length == 0) {
    SoPath::truncate(0);
    return;
  }
  if (length > 0 && length < projectedlength) {
    int cnt = 1;
    const int n = this->nodes.getLength();
    for (int i = 1; i < n; i++) {
      if (this->nodes[i]->isOfType(SoBaseKit::getClassTypeId()) &&
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
  if (!tailnode->isOfType(SoBaseKit::getClassTypeId())) {
#if COIN_DEBUG
    SoDebugError::postInfo("SoNodeKitPath::append",
                           "the logical tail is not a nodekit");
#endif // COIN_DEBUG
    return;
  }

  SoBaseKit * tail = static_cast<SoBaseKit *>(tailnode);
  SoSearchAction * sa = this->getSearchAction();
  sa->setNode(childKit);
  const SbBool oldSearch = tail->isSearchingChildren();
  tail->setSearchingChildren(TRUE);
  sa->apply(tail);
  tail->setSearchingChildren(oldSearch);

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
  if (!tailnode->isOfType(SoBaseKit::getClassTypeId())) {
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
    const SbBool oldSearch = tail->isSearchingChildren();
    tail->setSearchingChildren(TRUE);
    sa->apply(tail);
    tail->setSearchingChildren(oldSearch);
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

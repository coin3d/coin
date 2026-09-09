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

/*!
  \class SoFullPath SoFullPath.h Inventor/SoFullPath.h
  \brief The SoFullPath class allows examination of hidden children in paths.

  \ingroup coin_general

  SoPath::getTail() and SoPath::getLength() stop at the first node with
  hidden children. To inspect the complete path, use
  SoPath::getFullTail(), SoPath::getFullLength(),
  SoPath::getFullNodeFromTail(), and SoPath::getFullIndexFromTail().

  Historically, callers cast SoPath instances to SoFullPath to access
  hidden children. Such a downcast has undefined behavior unless the
  object actually derives from SoFullPath. The full-path methods on
  SoPath provide this functionality without a cast.

  SoFullPath is retained for compatibility and as the base of
  SoTempPath. Its accessors delegate to the full-path methods on SoPath.
*/

/*!
  \fn void SoFullPath::pop(void)

  Removes the actual tail, including hidden children, just like
  SoPath::pop().
*/

#include <Inventor/SoFullPath.h>
#include <cassert>


/*!
  A constructor.
*/

SoFullPath::SoFullPath(const int approxLength)
  : SoPath(approxLength)
{
}

/*!
  The destructor.
*/

SoFullPath::~SoFullPath(void)
{
}

/*!
  This method overrides SoPath::getTail() to allow clients to get the
  tail node, counting internal path nodes.
*/
SoNode *
SoFullPath::getTail(void) const
{
  return this->getFullTail();
}

/*!
  This method overrides SoPath::getNodeFromTail() to allow clients to
  get the node positioned \a index nodes from the tail, counting
  internal path nodes.
*/
SoNode *
SoFullPath::getNodeFromTail(const int index) const
{
  return this->getFullNodeFromTail(index);
}

/*!
  This method overrides SoPath::getIndexFromTail() to allow clients to
  get the child index number for nodes based on their position from
  the tail, counting hidden nodes.
*/
int
SoFullPath::getIndexFromTail(const int index) const
{
  return this->getFullIndexFromTail(index);
}

/*!
  This method returns the length of the path, counting hidden nodes
  also.
*/
int
SoFullPath::getLength(void) const
{
  return this->getFullLength();
}

/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2002 by Systems in Motion.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Systems in Motion about acquiring
 *  a Coin Professional Edition License.
 *
 *  See <URL:http://www.coin3d.org> for  more information.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no>.
 *
\**************************************************************************/

/*!
  \class SoFullPath Inventor/SoFullPath.h
  \brief The SoFullPath class allows examination of hidden children in paths.
  \ingroup general

  SoPath allows only access from the head node to the first node with
  hidden children, but not any further.

  Since the SoFullPath is derived from SoPath and contains no private
  data, you can cast SoPath instances to the SoFullPath type.  This
  will allow you to examine hidden children.

  (Actually, you are not supposed to allocate instances of this class
  at all. It is only available as an "extended interface" into the
  superclass SoPath.)
*/

/*!
  \fn void SoFullPath::pop(void)

  This method overrides SoPath::pop() to allow clients to get at all
  the nodes in the path.
*/

#include <Inventor/SoFullPath.h>
#include <assert.h>


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
  return this->nodes[this->nodes.getLength() - 1];
}

/*!
  This method overrides SoPath::getNodeFromTail() to allow clients to
  get the node positioned \a index nodes from the tail, counting
  internal path nodes.
*/
SoNode *
SoFullPath::getNodeFromTail(const int index) const
{
  return this->nodes[this->nodes.getLength() - 1 - index];
}

/*!
  This method overrides SoPath::getIndexFromTail() to allow clients to
  get the child index number for nodes based on their position from
  the tail, counting hidden nodes.
*/
int
SoFullPath::getIndexFromTail(const int index) const
{
  return this->indices[this->nodes.getLength() - 1 - index];
}

/*!
  This method returns the length of the path, counting hidden nodes
  also.
*/
int
SoFullPath::getLength(void) const
{
  return this->nodes.getLength();
}

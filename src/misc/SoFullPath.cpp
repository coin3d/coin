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
  \class SoFullPath Inventor/SoFullPath.h
  \brief The SoFullPath class allows examination of hidden children in paths.

  SoPath allows only access from the head node to the first node with hidden
  children, but not any further.

  Since the SoFullPath is derived from SoPath and contains no private data,
  you can cast SoPath instances to the SoFullPath type.  This will allow you 
  to examine hidden children.
*/

#include <Inventor/SoFullPath.h>

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
  This method overrides SoPath::pop() to allow clients to get at all the nodes
  in the path.
*/

void
SoFullPath::pop(void)
{
  truncate(getFullLength() - 1);
}

/*!
  This method overrides SoPath::getTail() to allow clients to get the tail
  node, counting internal path nodes.
*/

SoNode *
SoFullPath::getTail(void) const
{
  return nodes[ getFullLength() - 1 ];
}

/*!
  This method overrides SoPath::getNodeFromTail() to allow clients to get the
  node positioned \a index nodes from the tail, counting internal path nodes.
*/

SoNode *
SoFullPath::getNodeFromTail(const int index) const
{
  return nodes[ getFullLength() - 1 - index ];
}

/*!
  This method overrides SoPath::getIndexFromTail() to allow clients to get
  the child index number for nodes based on their position from the tail,
  counting hidden nodes.
*/

int
SoFullPath::getIndexFromTail(const int index) const
{
  return indices[ getFullLength() - 1 - index ];
}

/*!
  This method returns the length of the path, counting hidden nodes also.
*/

int
SoFullPath::getLength(void) const
{
  return getFullLength();
}


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
  \class SoNodeKitPath SoNodeKitPath.h Inventor/SoNodeKitPath.h
  \brief The SoNodeKitPath class ...
  \ingroup nodekits

  FIXME: write class doc
*/

#include <Inventor/SoNodeKitPath.h>


/*!
  FIXME: write function documentation
*/
SoNodeKitPath::SoNodeKitPath(int /*approxLength*/)
{
  assert(0 && "FIXME: not implemented yet");
}

/*!
  FIXME: write function documentation
*/
SoNodeKitPath::~SoNodeKitPath()
{
  assert(0 && "FIXME: not implemented yet");
}

/*!
  FIXME: write function documentation
*/
int
SoNodeKitPath::getLength(void) const
{
  assert(0 && "FIXME: not implemented yet");
  return -1;
}

/*!
  FIXME: write function documentation
*/
SoNode *
SoNodeKitPath::getTail(void) const
{
  assert(0 && "FIXME: not implemented yet");
  return NULL;
}

/*!
  FIXME: write function documentation
*/
SoNode *
SoNodeKitPath::getNode(int /*i*/) const
{
  assert(0 && "FIXME: not implemented yet");
  return NULL;
}

/*!
  FIXME: write function documentation
*/
SoNode *
SoNodeKitPath::getNodeFromTail(int /*i*/) const
{
  assert(0 && "FIXME: not implemented yet");
  return NULL;
}

/*!
  FIXME: write function documentation
*/
void
SoNodeKitPath::truncate(int /*start*/)
{
  assert(0 && "FIXME: not implemented yet");
}

/*!
  FIXME: write function documentation
*/
void
SoNodeKitPath::pop(void)
{
  assert(0 && "FIXME: not implemented yet");
}

/*!
  FIXME: write function documentation
*/
void
SoNodeKitPath::append(SoBaseKit * /*childKit*/)
{
  assert(0 && "FIXME: not implemented yet");
}

/*!
  FIXME: write function documentation
*/
void
SoNodeKitPath::append(const SoNodeKitPath * /*fromPath*/)
{
  assert(0 && "FIXME: not implemented yet");
}

/*!
  FIXME: write function documentation
*/
SbBool
SoNodeKitPath::containsNode(SoBaseKit * /*node*/) const
{
  assert(0 && "FIXME: not implemented yet");
  return FALSE;
}

/*!
  FIXME: write function documentation
*/
int
SoNodeKitPath::findFork(const SoNodeKitPath * /*path*/) const
{
  assert(0 && "FIXME: not implemented yet");
  return 0;
}

/*!
  FIXME: write function documentation
*/
int
operator==(const SoNodeKitPath & /*p1*/, const SoNodeKitPath & /*p2*/)
{
  assert(0 && "FIXME: not implemented yet");
  return 0;
}

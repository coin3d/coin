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
  \class SoColorIndex SoColorIndex.h Inventor/nodes/SoColorIndex.h
  \brief The SoColorIndex class ...
  \ingroup nodes

  FIXME: write class doc
*/

#include <Inventor/nodes/SoColorIndex.h>



/*!
  \var SoMFInt32 SoColorIndex::index
  FIXME: write documentation for field
*/

// *************************************************************************

SO_NODE_SOURCE(SoColorIndex);

/*!
  Constructor.
*/
SoColorIndex::SoColorIndex()
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoColorIndex);

  SO_NODE_ADD_FIELD(index, (1));
}

/*!
  Destructor.
*/
SoColorIndex::~SoColorIndex()
{
}

/*!
  Does initialization common for all objects of the
  SoColorIndex class. This includes setting up the
  type system, among other things.
*/
void
SoColorIndex::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoColorIndex);
}

/*!
  FIXME: write doc
*/
void
SoColorIndex::GLRender(SoGLRenderAction * /* action */)
{
  COIN_STUB();
}

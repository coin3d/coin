/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2000 by Systems in Motion. All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public License
 *  version 2.1 as published by the Free Software Foundation. See the
 *  file LICENSE.LGPL at the root directory of the distribution for
 *  more details.
 *
 *  If you want to use Coin for applications not compatible with the
 *  LGPL, please contact SIM to aquire a Professional Edition license.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  http://www.sim.no support@sim.no Voice: +47 22114160 Fax: +47 22207097
 *
\**************************************************************************/

/*!
  \class SoColorIndex SoColorIndex.h Inventor/nodes/SoColorIndex.h
  \brief The SoColorIndex class ...
  \ingroup nodes

  FIXME: write class doc
*/

#include <Inventor/nodes/SoColorIndex.h>
#include <Inventor/nodes/SoSubNodeP.h>
#include <coindefs.h> // COIN_STUB()

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

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
#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/SbName.h>

/*!
  \var SoMFInt32 SoColorIndex::index
  FIXME: write documentation for field
*/

// *************************************************************************

//$ BEGIN TEMPLATE NodeSource( SoColorIndex )
SoType SoColorIndex::classTypeId = SoType::badType();

/*!
  Returns a new instance of the SoColorIndex node class.
*/
void *
SoColorIndex::createInstance(void)
{
  return new SoColorIndex;
}

/*!
  Returns the unique type identifier for the SoColorIndex class.
*/
SoType
SoColorIndex::getClassTypeId(void)
{
  return SoColorIndex::classTypeId;
}

/*!
  Returns type identifier for an object.
*/
SoType
SoColorIndex::getTypeId(void) const
{
  return SoColorIndex::classTypeId;
}
//$ END TEMPLATE NodeSource

/*!
  Constructor.
*/
SoColorIndex::SoColorIndex()
{
//$ BEGIN TEMPLATE NodeConstructor( SoColorIndex )
  // Make sure the class has been initialized.
  assert(SoColorIndex::classTypeId != SoType::badType());
//$ END TEMPLATE NodeConstructor

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
//$ BEGIN TEMPLATE InitNodeSource( ColorIndex )
  // Make sure we only initialize once.
  assert(SoColorIndex::classTypeId == SoType::badType());
  // Make sure superclass get initialized before subclass.
  assert(inherited::getClassTypeId() != SoType::badType());

  SoColorIndex::classTypeId =
    SoType::createType(inherited::getClassTypeId(), "ColorIndex",
                       &SoColorIndex::createInstance,
                       SoNode::nextActionMethodIndex++);
//$ END TEMPLATE InitNodeSource
}

/*!
  Clean out all statically allocated resources.
  This method is only useful for debugging purposes.
*/
void
SoColorIndex::cleanClass(void)
{
}

#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
/*!
  FIXME: write doc
*/
void
SoColorIndex::GLRender(SoGLRenderAction * action)
{
  assert(0 && "FIXME: not implemented");
}
#endif // !COIN_EXCLUDE_SOGLRENDERACTION

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
 *  LGPL, please contact SIM to acquire a Professional Edition license.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  http://www.sim.no support@sim.no Voice: +47 22114160 Fax: +47 22207097
 *
\**************************************************************************/

/*!
  \class SoPathSwitch SoPathSwitch.h Inventor/nodes/SoPathSwitch.h
  \brief The SoPathSwitch class ...
  \ingroup nodes

  FIXME: write class doc
*/


#include <Inventor/nodes/SoPathSwitch.h>
#include <Inventor/nodes/SoSubNodeP.h>
#include <coindefs.h> // COIN_STUB()


/*!
  \var SoSFPath SoPathSwitch::path
  FIXME: write documentation for field
*/

// *************************************************************************

SO_NODE_SOURCE(SoPathSwitch);

/*!
  Default constructor.
*/
SoPathSwitch::SoPathSwitch(void)
{
  this->commonConstructor();
}

/*!
  Specify the expected number of children this node will have, to make
  it possible to do more efficient resource allocation.
*/
SoPathSwitch::SoPathSwitch(int numchildren)
  : inherited(numchildren)
{
  this->commonConstructor();
}

// private
void
SoPathSwitch::commonConstructor(void)
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoPathSwitch);
  SO_NODE_ADD_FIELD(path, (NULL));
}

/*!
  Destructor.
*/
SoPathSwitch::~SoPathSwitch()
{
}

/*!
  Does initialization common for all objects of the
  SoPathSwitch class. This includes setting up the
  type system, among other things.
*/
void
SoPathSwitch::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoPathSwitch);
}

/*!
  FIXME: write function documentation
*/
void
SoPathSwitch::getBoundingBox(SoGetBoundingBoxAction * /* action */)
{
  COIN_STUB();
}

/*!
  FIXME: write doc
 */
void
SoPathSwitch::doAction(SoAction * /* action */)
{
  COIN_STUB();
}

/*!
  FIXME: write doc
 */
void
SoPathSwitch::GLRender(SoGLRenderAction * /* action */)
{
  COIN_STUB();
}

/*!
  FIXME: write doc
 */
void
SoPathSwitch::pick(SoPickAction * /* action */)
{
  COIN_STUB();
}

/*!
  FIXME: write doc
 */
void
SoPathSwitch::handleEvent(SoHandleEventAction * /* action */)
{
  COIN_STUB();
}

/*!
  FIXME: write doc
 */
void
SoPathSwitch::getMatrix(SoGetMatrixAction * /* action */)
{
  COIN_STUB();
}

/*!
  FIXME: write doc
 */
void
SoPathSwitch::search(SoSearchAction * /* action */)
{
  COIN_STUB();
}

/*!
  FIXME: write doc
 */
void
SoPathSwitch::getPrimitiveCount(SoGetPrimitiveCountAction * /* action */)
{
  COIN_STUB();
}

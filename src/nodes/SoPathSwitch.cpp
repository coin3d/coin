/**************************************************************************\
 *
 *  Copyright (C) 1998-2000 by Systems in Motion.  All rights reserved.
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
  \class SoPathSwitch SoPathSwitch.h Inventor/nodes/SoPathSwitch.h
  \brief The SoPathSwitch class ...
  \ingroup nodes

  FIXME: write class doc
*/


#include <Inventor/nodes/SoPathSwitch.h>


/*!
  \var SoSFPath SoPathSwitch::path
  FIXME: write documentation for field
*/

// *************************************************************************

SO_NODE_SOURCE(SoPathSwitch);

/*!
  Constructor.
*/
SoPathSwitch::SoPathSwitch()
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

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
  \class SoSurroundScale SoSurroundScale.h Inventor/nodes/SoSurroundScale.h
  \brief The SoSurroundScale class ...
  \ingroup nodes

  FIXME: write class doc
*/

#include <Inventor/nodes/SoSurroundScale.h>



/*!
  \var SoSFInt32 SoSurroundScale::numNodesUpToContainer
  FIXME: write documentation for field
*/
/*!
  \var SoSFInt32 SoSurroundScale::numNodesUpToReset
  FIXME: write documentation for field
*/


// *************************************************************************

SO_NODE_SOURCE(SoSurroundScale);

/*!
  Constructor.
*/
SoSurroundScale::SoSurroundScale()
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoSurroundScale);

  SO_NODE_ADD_FIELD(numNodesUpToContainer, (0));
  SO_NODE_ADD_FIELD(numNodesUpToReset, (0));
}

/*!
  Destructor.
*/
SoSurroundScale::~SoSurroundScale()
{
}

/*!
  Does initialization common for all objects of the
  SoSurroundScale class. This includes setting up the
  type system, among other things.
*/
void
SoSurroundScale::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoSurroundScale);
}


/*!
  FIXME: write doc
 */
void
SoSurroundScale::invalidate(void)
{
  COIN_STUB();
}

/*!
  FIXME: write doc
 */
void
SoSurroundScale::doAction(SoAction * /* action */)
{
  COIN_STUB();
}

/*!
  FIXME: write doc
 */
void
SoSurroundScale::setDoingTranslations(SbBool /* doEm */)
{
  COIN_STUB();
}

/*!
  FIXME: write doc
 */
SbBool
SoSurroundScale::isDoingTranslations(void)
{
  COIN_STUB();
  return FALSE;
}

/*!
  FIXME: write doc
 */
void
SoSurroundScale::callback(SoCallbackAction * /* action */)
{
  COIN_STUB();
}

/*!
  FIXME: write doc
 */
void
SoSurroundScale::GLRender(SoGLRenderAction * /* action */)
{
  COIN_STUB();
}

/*!
  FIXME: write doc
 */
void
SoSurroundScale::getBoundingBox(SoGetBoundingBoxAction * /* action */)
{
  COIN_STUB();
}

/*!
  FIXME: write doc
 */
void
SoSurroundScale::getMatrix(SoGetMatrixAction * /* action */)
{
  COIN_STUB();
}

/*!
  FIXME: write doc
 */
void
SoSurroundScale::pick(SoPickAction * /* action */)
{
  COIN_STUB();
}

/*!
  FIXME: write doc
 */
void
SoSurroundScale::updateMySurroundParams(SoAction * /* action */,
                                        const SbMatrix & /* inv */)
{
  COIN_STUB();
}

/*!
  FIXME: write doc
 */
void
SoSurroundScale::setIgnoreInBbox(SbBool /* newVal */)
{
  COIN_STUB();
}

/*!
  FIXME: write doc
 */
SbBool
SoSurroundScale::isIgnoreInBbox(void)
{
  COIN_STUB();
  return FALSE;
}

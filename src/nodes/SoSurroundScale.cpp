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
  \class SoSurroundScale SoSurroundScale.h Inventor/nodes/SoSurroundScale.h
  \brief The SoSurroundScale class ...
  \ingroup nodes

  FIXME: write class doc
*/

#include <Inventor/nodes/SoSurroundScale.h>
#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/SbName.h>

/*!
  \var SoSFInt32 SoSurroundScale::numNodesUpToContainer
  FIXME: write documentation for field
*/
/*!
  \var SoSFInt32 SoSurroundScale::numNodesUpToReset
  FIXME: write documentation for field
*/


// *************************************************************************

//$ BEGIN TEMPLATE NodeSource(SoSurroundScale)
SoType SoSurroundScale::classTypeId = SoType::badType();

/*!
  Returns a new instance of the SoSurroundScale node class.
*/
void *
SoSurroundScale::createInstance(void)
{
  return new SoSurroundScale;
}

/*!
  Returns the unique type identifier for the SoSurroundScale class.
*/
SoType
SoSurroundScale::getClassTypeId(void)
{
  return SoSurroundScale::classTypeId;
}

/*!
  Returns type identifier for an object.
*/
SoType
SoSurroundScale::getTypeId(void) const
{
  return SoSurroundScale::classTypeId;
}
//$ END TEMPLATE NodeSource

/*!
  Constructor.
*/
SoSurroundScale::SoSurroundScale()
{
//$ BEGIN TEMPLATE NodeConstructor(SoSurroundScale)
  // Make sure the class has been initialized.
  assert(SoSurroundScale::classTypeId != SoType::badType());
//$ END TEMPLATE NodeConstructor

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
//$ BEGIN TEMPLATE InitNodeSource(SurroundScale)
  // Make sure we only initialize once.
  assert(SoSurroundScale::classTypeId == SoType::badType());
  // Make sure superclass get initialized before subclass.
  assert(inherited::getClassTypeId() != SoType::badType());

  SoSurroundScale::classTypeId =
    SoType::createType(inherited::getClassTypeId(), "SurroundScale",
                       &SoSurroundScale::createInstance,
                       SoNode::nextActionMethodIndex++);
//$ END TEMPLATE InitNodeSource
}

/*!
  Clean out all statically allocated resources.
  This method is only useful for debugging purposes.
*/
void
SoSurroundScale::cleanClass(void)
{
}


/*!
  FIXME: write doc
 */
void
SoSurroundScale::invalidate(void)
{
  assert(0 && "FIXME: not implemented");
}

#if !defined(COIN_EXCLUDE_SOACTION)
/*!
  FIXME: write doc
 */
void
SoSurroundScale::doAction(SoAction * /* action */)
{
  assert(0 && "FIXME: not implemented");
}
#endif // !COIN_EXCLUDE_SOACTION

/*!
  FIXME: write doc
 */
void
SoSurroundScale::setDoingTranslations(SbBool /* doEm */)
{
  assert(0 && "FIXME: not implemented");
}

/*!
  FIXME: write doc
 */
SbBool
SoSurroundScale::isDoingTranslations(void)
{
  assert(0 && "FIXME: not implemented");
  return FALSE;
}

#if !defined(COIN_EXCLUDE_SOCALLBACKACTION)
/*!
  FIXME: write doc
 */
void
SoSurroundScale::callback(SoCallbackAction * /* action */)
{
  assert(0 && "FIXME: not implemented");
}
#endif // !COIN_EXCLUDE_SOCALLBACKACTION

#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
/*!
  FIXME: write doc
 */
void
SoSurroundScale::GLRender(SoGLRenderAction * /* action */)
{
  assert(0 && "FIXME: not implemented");
}
#endif // !COIN_EXCLUDE_SOGLRENDERACTION

#if !defined(COIN_EXCLUDE_SOGETBOUNDINGBOXACTION)
/*!
  FIXME: write doc
 */
void
SoSurroundScale::getBoundingBox(SoGetBoundingBoxAction * /* action */)
{
  assert(0 && "FIXME: not implemented");
}
#endif // !COIN_EXCLUDE_SOGETBOUNDINGBOXACTION

#if !defined(COIN_EXCLUDE_SOGETMATRIXACTION)
/*!
  FIXME: write doc
 */
void
SoSurroundScale::getMatrix(SoGetMatrixAction * /* action */)
{
  assert(0 && "FIXME: not implemented");
}
#endif // !COIN_EXCLUDE_SOGETMATRIXACTION

#if !defined(COIN_EXCLUDE_SOPICKACTION)
/*!
  FIXME: write doc
 */
void
SoSurroundScale::pick(SoPickAction * /* action */)
{
  assert(0 && "FIXME: not implemented");
}
#endif // !COIN_EXCLUDE_SOPICKACTION)

/*!
  FIXME: write doc
 */
void
SoSurroundScale::updateMySurroundParams(SoAction * /* action */,
					const SbMatrix & /* inv */)
{
  assert(0 && "FIXME: not implemented");
}

/*!
  FIXME: write doc
 */
void
SoSurroundScale::setIgnoreInBbox(SbBool /* newVal */)
{
  assert(0 && "FIXME: not implemented");
}

/*!
  FIXME: write doc
 */
SbBool
SoSurroundScale::isIgnoreInBbox(void)
{
  assert(0 && "FIXME: not implemented");
  return FALSE;
}

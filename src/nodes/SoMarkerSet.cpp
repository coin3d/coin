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
  \class SoMarkerSet SoMarkerSet.h Inventor/nodes/SoMarkerSet.h
  \brief The SoMarkerSet class ...
  \ingroup nodes

  FIXME: write class doc
*/

#include <Inventor/nodes/SoMarkerSet.h>
#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/SbName.h>

/*!
  \enum SoMarkerSet::MarkerType
  FIXME: write documentation for enum
*/
/*!
  \var SoMarkerSet::MarkerType SoMarkerSet::CROSS_5_5
  FIXME: write documentation for enum definition
*/
/*!
  \var SoMarkerSet::MarkerType SoMarkerSet::PLUS_5_5
  FIXME: write documentation for enum definition
*/
/*!
  \var SoMarkerSet::MarkerType SoMarkerSet::MINUS_5_5
  FIXME: write documentation for enum definition
*/
/*!
  \var SoMarkerSet::MarkerType SoMarkerSet::SLASH_5_5
  FIXME: write documentation for enum definition
*/
/*!
  \var SoMarkerSet::MarkerType SoMarkerSet::BACKSLASH_5_5
  FIXME: write documentation for enum definition
*/
/*!
  \var SoMarkerSet::MarkerType SoMarkerSet::BAR_5_5
  FIXME: write documentation for enum definition
*/
/*!
  \var SoMarkerSet::MarkerType SoMarkerSet::STAR_5_5
  FIXME: write documentation for enum definition
*/
/*!
  \var SoMarkerSet::MarkerType SoMarkerSet::Y_5_5
  FIXME: write documentation for enum definition
*/
/*!
  \var SoMarkerSet::MarkerType SoMarkerSet::LIGHTNING_5_5
  FIXME: write documentation for enum definition
*/
/*!
  \var SoMarkerSet::MarkerType SoMarkerSet::WELL_5_5
  FIXME: write documentation for enum definition
*/
/*!
  \var SoMarkerSet::MarkerType SoMarkerSet::CIRCLE_LINE_5_5
  FIXME: write documentation for enum definition
*/
/*!
  \var SoMarkerSet::MarkerType SoMarkerSet::SQUARE_LINE_5_5
  FIXME: write documentation for enum definition
*/
/*!
  \var SoMarkerSet::MarkerType SoMarkerSet::DIAMOND_LINE_5_5
  FIXME: write documentation for enum definition
*/
/*!
  \var SoMarkerSet::MarkerType SoMarkerSet::TRIANGLE_LINE_5_5
  FIXME: write documentation for enum definition
*/
/*!
  \var SoMarkerSet::MarkerType SoMarkerSet::RHOMBUS_LINE_5_5
  FIXME: write documentation for enum definition
*/
/*!
  \var SoMarkerSet::MarkerType SoMarkerSet::HOURGLASS_LINE_5_5
  FIXME: write documentation for enum definition
*/
/*!
  \var SoMarkerSet::MarkerType SoMarkerSet::SATELLITE_LINE_5_5
  FIXME: write documentation for enum definition
*/
/*!
  \var SoMarkerSet::MarkerType SoMarkerSet::PINE_TREE_LINE_5_5
  FIXME: write documentation for enum definition
*/
/*!
  \var SoMarkerSet::MarkerType SoMarkerSet::CAUTION_LINE_5_5
  FIXME: write documentation for enum definition
*/
/*!
  \var SoMarkerSet::MarkerType SoMarkerSet::SHIP_LINE_5_5
  FIXME: write documentation for enum definition
*/
/*!
  \var SoMarkerSet::MarkerType SoMarkerSet::CIRCLE_FILLED_5_5
  FIXME: write documentation for enum definition
*/
/*!
  \var SoMarkerSet::MarkerType SoMarkerSet::SQUARE_FILLED_5_5
  FIXME: write documentation for enum definition
*/
/*!
  \var SoMarkerSet::MarkerType SoMarkerSet::DIAMOND_FILLED_5_5
  FIXME: write documentation for enum definition
*/
/*!
  \var SoMarkerSet::MarkerType SoMarkerSet::TRIANGLE_FILLED_5_5
  FIXME: write documentation for enum definition
*/
/*!
  \var SoMarkerSet::MarkerType SoMarkerSet::RHOMBUS_FILLED_5_5
  FIXME: write documentation for enum definition
*/
/*!
  \var SoMarkerSet::MarkerType SoMarkerSet::HOURGLASS_FILLED_5_5
  FIXME: write documentation for enum definition
*/
/*!
  \var SoMarkerSet::MarkerType SoMarkerSet::SATELLITE_FILLED_5_5
  FIXME: write documentation for enum definition
*/
/*!
  \var SoMarkerSet::MarkerType SoMarkerSet::PINE_TREE_FILLED_5_5
  FIXME: write documentation for enum definition
*/
/*!
  \var SoMarkerSet::MarkerType SoMarkerSet::CAUTION_FILLED_5_5
  FIXME: write documentation for enum definition
*/
/*!
  \var SoMarkerSet::MarkerType SoMarkerSet::SHIP_FILLED_5_5
  FIXME: write documentation for enum definition
*/
/*!
  \var SoMarkerSet::MarkerType SoMarkerSet::CROSS_7_7
  FIXME: write documentation for enum definition
*/
/*!
  \var SoMarkerSet::MarkerType SoMarkerSet::PLUS_7_7
  FIXME: write documentation for enum definition
*/
/*!
  \var SoMarkerSet::MarkerType SoMarkerSet::MINUS_7_7
  FIXME: write documentation for enum definition
*/
/*!
  \var SoMarkerSet::MarkerType SoMarkerSet::SLASH_7_7
  FIXME: write documentation for enum definition
*/
/*!
  \var SoMarkerSet::MarkerType SoMarkerSet::BACKSLASH_7_7
  FIXME: write documentation for enum definition
*/
/*!
  \var SoMarkerSet::MarkerType SoMarkerSet::BAR_7_7
  FIXME: write documentation for enum definition
*/
/*!
  \var SoMarkerSet::MarkerType SoMarkerSet::STAR_7_7
  FIXME: write documentation for enum definition
*/
/*!
  \var SoMarkerSet::MarkerType SoMarkerSet::Y_7_7
  FIXME: write documentation for enum definition
*/
/*!
  \var SoMarkerSet::MarkerType SoMarkerSet::LIGHTNING_7_7
  FIXME: write documentation for enum definition
*/
/*!
  \var SoMarkerSet::MarkerType SoMarkerSet::WELL_7_7
  FIXME: write documentation for enum definition
*/
/*!
  \var SoMarkerSet::MarkerType SoMarkerSet::CIRCLE_LINE_7_7
  FIXME: write documentation for enum definition
*/
/*!
  \var SoMarkerSet::MarkerType SoMarkerSet::SQUARE_LINE_7_7
  FIXME: write documentation for enum definition
*/
/*!
  \var SoMarkerSet::MarkerType SoMarkerSet::DIAMOND_LINE_7_7
  FIXME: write documentation for enum definition
*/
/*!
  \var SoMarkerSet::MarkerType SoMarkerSet::TRIANGLE_LINE_7_7
  FIXME: write documentation for enum definition
*/
/*!
  \var SoMarkerSet::MarkerType SoMarkerSet::RHOMBUS_LINE_7_7
  FIXME: write documentation for enum definition
*/
/*!
  \var SoMarkerSet::MarkerType SoMarkerSet::HOURGLASS_LINE_7_7
  FIXME: write documentation for enum definition
*/
/*!
  \var SoMarkerSet::MarkerType SoMarkerSet::SATELLITE_LINE_7_7
  FIXME: write documentation for enum definition
*/
/*!
  \var SoMarkerSet::MarkerType SoMarkerSet::PINE_TREE_LINE_7_7
  FIXME: write documentation for enum definition
*/
/*!
  \var SoMarkerSet::MarkerType SoMarkerSet::CAUTION_LINE_7_7
  FIXME: write documentation for enum definition
*/
/*!
  \var SoMarkerSet::MarkerType SoMarkerSet::SHIP_LINE_7_7
  FIXME: write documentation for enum definition
*/
/*!
  \var SoMarkerSet::MarkerType SoMarkerSet::CIRCLE_FILLED_7_7
  FIXME: write documentation for enum definition
*/
/*!
  \var SoMarkerSet::MarkerType SoMarkerSet::SQUARE_FILLED_7_7
  FIXME: write documentation for enum definition
*/
/*!
  \var SoMarkerSet::MarkerType SoMarkerSet::DIAMOND_FILLED_7_7
  FIXME: write documentation for enum definition
*/
/*!
  \var SoMarkerSet::MarkerType SoMarkerSet::TRIANGLE_FILLED_7_7
  FIXME: write documentation for enum definition
*/
/*!
  \var SoMarkerSet::MarkerType SoMarkerSet::RHOMBUS_FILLED_7_7
  FIXME: write documentation for enum definition
*/
/*!
  \var SoMarkerSet::MarkerType SoMarkerSet::HOURGLASS_FILLED_7_7
  FIXME: write documentation for enum definition
*/
/*!
  \var SoMarkerSet::MarkerType SoMarkerSet::SATELLITE_FILLED_7_7
  FIXME: write documentation for enum definition
*/
/*!
  \var SoMarkerSet::MarkerType SoMarkerSet::PINE_TREE_FILLED_7_7
  FIXME: write documentation for enum definition
*/
/*!
  \var SoMarkerSet::MarkerType SoMarkerSet::CAUTION_FILLED_7_7
  FIXME: write documentation for enum definition
*/
/*!
  \var SoMarkerSet::MarkerType SoMarkerSet::SHIP_FILLED_7_7
  FIXME: write documentation for enum definition
*/
/*!
  \var SoMarkerSet::MarkerType SoMarkerSet::CROSS_9_9
  FIXME: write documentation for enum definition
*/
/*!
  \var SoMarkerSet::MarkerType SoMarkerSet::PLUS_9_9
  FIXME: write documentation for enum definition
*/
/*!
  \var SoMarkerSet::MarkerType SoMarkerSet::MINUS_9_9
  FIXME: write documentation for enum definition
*/
/*!
  \var SoMarkerSet::MarkerType SoMarkerSet::SLASH_9_9
  FIXME: write documentation for enum definition
*/
/*!
  \var SoMarkerSet::MarkerType SoMarkerSet::BACKSLASH_9_9
  FIXME: write documentation for enum definition
*/
/*!
  \var SoMarkerSet::MarkerType SoMarkerSet::BAR_9_9
  FIXME: write documentation for enum definition
*/
/*!
  \var SoMarkerSet::MarkerType SoMarkerSet::STAR_9_9
  FIXME: write documentation for enum definition
*/
/*!
  \var SoMarkerSet::MarkerType SoMarkerSet::Y_9_9
  FIXME: write documentation for enum definition
*/
/*!
  \var SoMarkerSet::MarkerType SoMarkerSet::LIGHTNING_9_9
  FIXME: write documentation for enum definition
*/
/*!
  \var SoMarkerSet::MarkerType SoMarkerSet::WELL_9_9
  FIXME: write documentation for enum definition
*/
/*!
  \var SoMarkerSet::MarkerType SoMarkerSet::CIRCLE_LINE_9_9
  FIXME: write documentation for enum definition
*/
/*!
  \var SoMarkerSet::MarkerType SoMarkerSet::SQUARE_LINE_9_9
  FIXME: write documentation for enum definition
*/
/*!
  \var SoMarkerSet::MarkerType SoMarkerSet::DIAMOND_LINE_9_9
  FIXME: write documentation for enum definition
*/
/*!
  \var SoMarkerSet::MarkerType SoMarkerSet::TRIANGLE_LINE_9_9
  FIXME: write documentation for enum definition
*/
/*!
  \var SoMarkerSet::MarkerType SoMarkerSet::RHOMBUS_LINE_9_9
  FIXME: write documentation for enum definition
*/
/*!
  \var SoMarkerSet::MarkerType SoMarkerSet::HOURGLASS_LINE_9_9
  FIXME: write documentation for enum definition
*/
/*!
  \var SoMarkerSet::MarkerType SoMarkerSet::SATELLITE_LINE_9_9
  FIXME: write documentation for enum definition
*/
/*!
  \var SoMarkerSet::MarkerType SoMarkerSet::PINE_TREE_LINE_9_9
  FIXME: write documentation for enum definition
*/
/*!
  \var SoMarkerSet::MarkerType SoMarkerSet::CAUTION_LINE_9_9
  FIXME: write documentation for enum definition
*/
/*!
  \var SoMarkerSet::MarkerType SoMarkerSet::SHIP_LINE_9_9
  FIXME: write documentation for enum definition
*/
/*!
  \var SoMarkerSet::MarkerType SoMarkerSet::CIRCLE_FILLED_9_9
  FIXME: write documentation for enum definition
*/
/*!
  \var SoMarkerSet::MarkerType SoMarkerSet::SQUARE_FILLED_9_9
  FIXME: write documentation for enum definition
*/
/*!
  \var SoMarkerSet::MarkerType SoMarkerSet::DIAMOND_FILLED_9_9
  FIXME: write documentation for enum definition
*/
/*!
  \var SoMarkerSet::MarkerType SoMarkerSet::TRIANGLE_FILLED_9_9
  FIXME: write documentation for enum definition
*/
/*!
  \var SoMarkerSet::MarkerType SoMarkerSet::RHOMBUS_FILLED_9_9
  FIXME: write documentation for enum definition
*/
/*!
  \var SoMarkerSet::MarkerType SoMarkerSet::HOURGLASS_FILLED_9_9
  FIXME: write documentation for enum definition
*/
/*!
  \var SoMarkerSet::MarkerType SoMarkerSet::SATELLITE_FILLED_9_9
  FIXME: write documentation for enum definition
*/
/*!
  \var SoMarkerSet::MarkerType SoMarkerSet::PINE_TREE_FILLED_9_9
  FIXME: write documentation for enum definition
*/
/*!
  \var SoMarkerSet::MarkerType SoMarkerSet::CAUTION_FILLED_9_9
  FIXME: write documentation for enum definition
*/
/*!
  \var SoMarkerSet::MarkerType SoMarkerSet::SHIP_FILLED_9_9
  FIXME: write documentation for enum definition
*/


/*!
  \var SoMFInt32 SoMarkerSet::markerIndex
  FIXME: write documentation for field
*/


// *************************************************************************

//$ BEGIN TEMPLATE NodeSource(SoMarkerSet)
SoType SoMarkerSet::classTypeId = SoType::badType();

/*!
  Returns a new instance of the SoMarkerSet node class.
*/
void *
SoMarkerSet::createInstance(void)
{
  return new SoMarkerSet;
}

/*!
  Returns the unique type identifier for the SoMarkerSet class.
*/
SoType
SoMarkerSet::getClassTypeId(void)
{
  return SoMarkerSet::classTypeId;
}

/*!
  Returns type identifier for an object.
*/
SoType
SoMarkerSet::getTypeId(void) const
{
  return SoMarkerSet::classTypeId;
}
//$ END TEMPLATE NodeSource

/*!
  Constructor.
*/
SoMarkerSet::SoMarkerSet()
{
//$ BEGIN TEMPLATE NodeConstructor(SoMarkerSet)
  // Make sure the class has been initialized.
  assert(SoMarkerSet::classTypeId != SoType::badType());
//$ END TEMPLATE NodeConstructor

  SO_NODE_ADD_FIELD(markerIndex, (0));
}

/*!
  Destructor.
*/
SoMarkerSet::~SoMarkerSet()
{
}

/*!
  Does initialization common for all objects of the
  SoMarkerSet class. This includes setting up the
  type system, among other things.
*/
void
SoMarkerSet::initClass(void)
{
//$ BEGIN TEMPLATE InitNodeSource(MarkerSet)
  // Make sure we only initialize once.
  assert(SoMarkerSet::classTypeId == SoType::badType());
  // Make sure superclass get initialized before subclass.
  assert(inherited::getClassTypeId() != SoType::badType());

  SoMarkerSet::classTypeId =
    SoType::createType(inherited::getClassTypeId(), "MarkerSet",
                       &SoMarkerSet::createInstance,
                       SoNode::nextActionMethodIndex++);
//$ END TEMPLATE InitNodeSource
}

/*!
  Clean out all statically allocated resources.
  This method is only useful for debugging purposes.
*/
void
SoMarkerSet::cleanClass(void)
{
}


#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
/*!
  FIXME: write doc
 */
void
SoMarkerSet::GLRender(SoGLRenderAction * /* action */)
{
  assert(0 && "FIXME: not implemented");
}
#endif // !COIN_EXCLUDE_SOGLRENDERACTION

#if !defined(COIN_EXCLUDE_SOGETPRIMITIVECOUNTACTION)
/*!
  FIXME: write doc
 */
void
SoMarkerSet::getPrimitiveCount(SoGetPrimitiveCountAction * /* action */)
{
  assert(0 && "FIXME: not implemented");
}
#endif // !COIN_EXCLUDE_SOGETPRIMITIVECOUNTACTION

/*!
  FIXME: write doc
 */
int
SoMarkerSet::getNumDefinedMarkers(void)
{
  assert(0 && "FIXME: not implemented");
  return 0;
}

/*!
  FIXME: write doc
 */
void
SoMarkerSet::addMarker(int /* markerIndex */, const SbVec2s & /* size */,
		       const unsigned char * /* bytes */, SbBool /* isLSBFirst */,
		       SbBool /* isUpToDown */)
{
  assert(0 && "FIXME: not implemented");
}

/*!
  FIXME: write doc
 */
SbBool
SoMarkerSet::getMarker(int /* markerIndex */, SbVec2s & /* size */,
		       const unsigned char *& /* bytes */, SbBool & /* isLSBFirst */)
{
  assert(0 && "FIXME: not implemented");
  return FALSE;
}

/*!
  FIXME: write doc
 */
SbBool
SoMarkerSet::removeMarker(int /* markerIndex */)
{
  assert(0 && "FIXME: not implemented");
  return FALSE;
}

/*!
  FIXME: write doc
 */
SbBool
SoMarkerSet::isMarkerBitSet(int /* markerIndex */, int /* bitNumber */)
{
  assert(0 && "FIXME: not implemented");
  return FALSE;
}

// Need some perl-magic from Larsa

//$ INSERT MARKER CROSS_5_5
// "x...x"
// ".x.x."
// "..x.."
// ".x.x."
// "x...x"

//$ INSERT MARKER PLUS_5_5
// "..x.."
// "..x.."
// "xxxxx"
// "..x.."
// "..x.."

//$ INSERT MARKER MINUS_5_5
// "..x.."
// "....."
// "xxxxx"
// "....."
// "..x.."




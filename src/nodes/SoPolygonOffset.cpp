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
  \class SoPolygonOffset SoPolygonOffset.h Inventor/nodes/SoPolygonOffset.h
  \brief The SoPolygonOffset class ...
  \ingroup nodes

  FIXME: write class doc
*/

#include <Inventor/nodes/SoPolygonOffset.h>
#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/SbName.h>

#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
#include <Inventor/actions/SoGLRenderAction.h>
#endif // !COIN_EXCLUDE_SOGLRENDERACTION

#if !defined(COIN_EXCLUDE_SOGLPOLYGONOFFSETELEMENT)
#include <Inventor/elements/SoGLPolygonOffsetElement.h>
#endif // !COIN_EXCLUDE_SOGLPOLYGONOFFSETELEMENT

/*!
  \enum SoPolygonOffset::Style
  FIXME: write documentation for enum
*/
/*!
  \var SoPolygonOffset::Style SoPolygonOffset::FILLED
  FIXME: write documentation for enum definition
*/
/*!
  \var SoPolygonOffset::Style SoPolygonOffset::LINES
  FIXME: write documentation for enum definition
*/
/*!
  \var SoPolygonOffset::Style SoPolygonOffset::POINTS
  FIXME: write documentation for enum definition
*/


/*!
  \var SoSFFloat SoPolygonOffset::factor
  FIXME: write documentation for field
*/
/*!
  \var SoSFFloat SoPolygonOffset::units
  FIXME: write documentation for field
*/
/*!
  \var SoSFBitMask SoPolygonOffset::styles
  FIXME: write documentation for field
*/
/*!
  \var SoSFBool SoPolygonOffset::on
  FIXME: write documentation for field
*/


// *************************************************************************

//$ BEGIN TEMPLATE NodeSource(SoPolygonOffset)
SoType SoPolygonOffset::classTypeId = SoType::badType();

/*!
  Returns a new instance of the SoPolygonOffset node class.
*/
void *
SoPolygonOffset::createInstance(void)
{
  return new SoPolygonOffset;
}

/*!
  Returns the unique type identifier for the SoPolygonOffset class.
*/
SoType
SoPolygonOffset::getClassTypeId(void)
{
  return SoPolygonOffset::classTypeId;
}

/*!
  Returns type identifier for an object.
*/
SoType
SoPolygonOffset::getTypeId(void) const
{
  return SoPolygonOffset::classTypeId;
}
//$ END TEMPLATE NodeSource

/*!
  Constructor.
*/
SoPolygonOffset::SoPolygonOffset()
{
//$ BEGIN TEMPLATE NodeConstructor(SoPolygonOffset)
  // Make sure the class has been initialized.
  assert(SoPolygonOffset::classTypeId != SoType::badType());
//$ END TEMPLATE NodeConstructor
  
  SO_NODE_ADD_FIELD(factor, (1.0f));
  SO_NODE_ADD_FIELD(units, (1.0f));
  SO_NODE_ADD_FIELD(styles, (SoPolygonOffset::FILLED));
  SO_NODE_ADD_FIELD(on, (TRUE));

  SO_NODE_DEFINE_ENUM_VALUE(Style, FILLED);
  SO_NODE_DEFINE_ENUM_VALUE(Style, LINES);
  SO_NODE_DEFINE_ENUM_VALUE(Style, POINTS);
  SO_NODE_SET_SF_ENUM_TYPE(styles, Style);
}

/*!
  Destructor.
*/
SoPolygonOffset::~SoPolygonOffset()
{
}

/*!
  Does initialization common for all objects of the
  SoPolygonOffset class. This includes setting up the
  type system, among other things.
*/
void
SoPolygonOffset::initClass(void)
{
//$ BEGIN TEMPLATE InitNodeSource(PolygonOffset)
  // Make sure we only initialize once.
  assert(SoPolygonOffset::classTypeId == SoType::badType());
  // Make sure superclass get initialized before subclass.
  assert(inherited::getClassTypeId() != SoType::badType());

  SoPolygonOffset::classTypeId =
    SoType::createType(inherited::getClassTypeId(), "PolygonOffset",
                       &SoPolygonOffset::createInstance,
                       SoNode::nextActionMethodIndex++);
//$ END TEMPLATE InitNodeSource

#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
  SO_ENABLE(SoGLRenderAction, SoGLPolygonOffsetElement);
#endif // !COIN_EXCLUDE_SOGLRENDERACTION
}

/*!
  Clean out all statically allocated resources.
  This method is only useful for debugging purposes.
*/
void
SoPolygonOffset::cleanClass(void)
{
}


#if !defined(COIN_EXCLUDE_SOACTION)
/*!
  FIXME: write doc
 */
void
SoPolygonOffset::doAction(SoAction * action)
{
  //
  // FIXME: check override
  //
  SoPolygonOffsetElement::set(action->getState(),
			      this,
			      factor.getValue(),
			      units.getValue(),
			      (SoPolygonOffsetElement::Style)
			      styles.getValue(),
			      on.getValue());
}
#endif // !COIN_EXCLUDE_SOACTION

#if !defined(COIN_EXCLUDE_SOCALLBACKACTION)
/*!
  FIXME: write doc
 */
void
SoPolygonOffset::callback(SoCallbackAction * action)
{
  SoPolygonOffset::doAction((SoAction*)action);
}
#endif // !COIN_EXCLUDE_SOCALLBACKACTION

#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
/*!
  FIXME: write doc
 */
void
SoPolygonOffset::GLRender(SoGLRenderAction * action)
{
  SoPolygonOffset::doAction((SoAction*)action);
}
#endif // !COIN_EXCLUDE_SOGLRENDERACTION

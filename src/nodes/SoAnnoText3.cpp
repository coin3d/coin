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
 *  http://www.sim.no/ sales@sim.no Voice: +47 73540378 Fax: +47 73943861
 *
\**************************************************************************/

/*!
  \class SoAnnoText3 SoAnnoText3.h Inventor/nodes/SoAnnoText3.h
  \brief The SoAnnoText3 class ...
  \ingroup nodes

  FIXME: write class doc
*/

#include <Inventor/nodes/SoAnnoText3.h>
#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/SbName.h>


/*!
  \enum SoAnnoText3::Justification
  FIXME: write documentation for enum
*/
/*!
  \var SoAnnoText3::Justification SoAnnoText3::LEFT
  FIXME: write documentation for enum definition
*/
/*!
  \var SoAnnoText3::Justification SoAnnoText3::RIGHT
  FIXME: write documentation for enum definition
*/
/*!
  \var SoAnnoText3::Justification SoAnnoText3::CENTER
  FIXME: write documentation for enum definition
*/


/*!
  \var SoMFString SoAnnoText3::string
  FIXME: write documentation for field
*/
/*!
  \var SoSFFloat SoAnnoText3::spacing
  FIXME: write documentation for field
*/
/*!
  \var SoSFEnum SoAnnoText3::justification
  FIXME: write documentation for field
*/
/*!
  \var SoSFNode SoAnnoText3::alternateRep
  FIXME: write documentation for field
*/

// *************************************************************************

//$ BEGIN TEMPLATE NodeSource( SoAnnoText3 )
SoType SoAnnoText3::classTypeId = SoType::badType();

/*!
  Returns a new instance of the SoAnnoText3 node class.
*/
void *
SoAnnoText3::createInstance(void)
{
  return new SoAnnoText3;
}

/*!
  Returns the unique type identifier for the SoAnnoText3 class.
*/
SoType
SoAnnoText3::getClassTypeId(void)
{
  return SoAnnoText3::classTypeId;
}

/*!
  Returns type identifier for an object.
*/
SoType
SoAnnoText3::getTypeId(void) const
{
  return SoAnnoText3::classTypeId;
}
//$ END TEMPLATE NodeSource

/*!
  Constructor.
*/
SoAnnoText3::SoAnnoText3()
{
//$ BEGIN TEMPLATE NodeConstructor( SoAnnoText3 )
  // Make sure the class has been initialized.
  assert(SoAnnoText3::classTypeId != SoType::badType());
//$ END TEMPLATE NodeConstructor

  SO_NODE_ADD_FIELD(string, (""));
  SO_NODE_ADD_FIELD(spacing, (1.0f));
  SO_NODE_ADD_FIELD(justification, (SoAnnoText3::LEFT));
  // FIXME: this is not correct -- fix.
  SO_NODE_ADD_FIELD(alternateRep, (NULL));

  SO_NODE_DEFINE_ENUM_VALUE(Justification, LEFT);
  SO_NODE_DEFINE_ENUM_VALUE(Justification, RIGHT);
  SO_NODE_DEFINE_ENUM_VALUE(Justification, CENTER);
  SO_NODE_SET_SF_ENUM_TYPE(justification, Justification);
}

/*!
  Destructor.
*/
SoAnnoText3::~SoAnnoText3()
{
}


/*!
  Does initialization common for all objects of the
  SoAnnoText3 class. This includes setting up the
  type system, among other things.
*/
void
SoAnnoText3::initClass(void)
{
//$ BEGIN TEMPLATE InitNodeSource( AnnoText3 )
  // Make sure we only initialize once.
  assert(SoAnnoText3::classTypeId == SoType::badType());
  // Make sure superclass get initialized before subclass.
  assert(inherited::getClassTypeId() != SoType::badType());

  SoAnnoText3::classTypeId =
    SoType::createType(inherited::getClassTypeId(), "AnnoText3",
                       &SoAnnoText3::createInstance,
                       SoNode::nextActionMethodIndex++);
//$ END TEMPLATE InitNodeSource
}


/*!
  Clean out all statically allocated resources.
  This method is only useful for debugging purposes.
*/
void
SoAnnoText3::cleanClass(void)
{
}

#if !defined(COIN_EXCLUDE_SOGETBOUNDINGBOXACTION)
/*!
  FIXME: write doc
*/
void
SoAnnoText3::computeBBox(SoAction * action, SbBox3f & box, SbVec3f & center)
{
  assert(0 && "FIXME: not implemented");
}
#endif // !COIN_EXCLUDE_SOGETBOUNDINGBOXACTION

/*!
  FIXME: write doc
*/
SbBox3f
SoAnnoText3::getCharacterBounds(SoState * state,
				int stringIndex, int charIndex)
{
  assert(0 && "FIXME: not implemented");
  static SbBox3f b;
  return b;
}

#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
/*!
  FIXME: write doc
*/
void
SoAnnoText3::GLRender(SoGLRenderAction * action)
{
  assert(0 && "FIXME: not implemented");
}
#endif // !COIN_EXCLUDE_SOGLRENDERACTION
#if !defined(COIN_EXCLUDE_SORAYPICKACTION)
/*!
  FIXME: write doc
*/
void
SoAnnoText3::rayPick(SoRayPickAction * action)
{
  assert(0 && "FIXME: not implemented");
}
#endif // !COIN_EXCLUDE_SORAYPICKACTION
#if !defined(COIN_EXCLUDE_SOWRITEACTION)
/*!
  FIXME: write doc
*/
void
SoAnnoText3::write(SoWriteAction * action)
{
  assert(0 && "FIXME: not implemented");
}
#endif // !COIN_EXCLUDE_SOWRITEACTION
#if !defined(COIN_EXCLUDE_SOGETPRIMITIVECOUNTACTION)
/*!
  FIXME: write doc
*/
void
SoAnnoText3::getPrimitiveCount(SoGetPrimitiveCountAction * action)
{
  assert(0 && "FIXME: not implemented");
}
#endif // !COIN_EXCLUDE_SOGETPRIMITIVECOUNTACTION
#if !defined(COIN_EXCLUDE_SOACTION)
/*!
  FIXME: write doc
*/
void
SoAnnoText3::generatePrimitives(SoAction *)
{
  assert(0 && "FIXME: not implemented");
}
#endif // !COIN_EXCLUDE_SOACTION

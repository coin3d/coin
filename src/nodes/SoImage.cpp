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
  \class SoImage SoImage.h Inventor/nodes/SoImage.h
  \brief The SoImage class ...
  \ingroup nodes

  FIXME: write class doc
*/

#include <Inventor/nodes/SoImage.h>



#include <Inventor/SoInput.h>

/*!
  \enum SoImage::VertAlignment
  FIXME: write documentation for enum
*/
/*!
  \var SoImage::VertAlignment SoImage::BOTTOM
  FIXME: write documentation for enum definition
*/
/*!
  \var SoImage::VertAlignment SoImage::HALF
  FIXME: write documentation for enum definition
*/
/*!
  \var SoImage::VertAlignment SoImage::TOP
  FIXME: write documentation for enum definition
*/
/*!
  \enum SoImage::HorAlignment
  FIXME: write documentation for enum
*/
/*!
  \var SoImage::HorAlignment SoImage::LEFT
  FIXME: write documentation for enum definition
*/
/*!
  \var SoImage::HorAlignment SoImage::CENTER
  FIXME: write documentation for enum definition
*/
/*!
  \var SoImage::HorAlignment SoImage::RIGHT
  FIXME: write documentation for enum definition
*/


/*!
  \var SoSFInt32 SoImage::width
  FIXME: write documentation for field
*/
/*!
  \var SoSFInt32 SoImage::height
  FIXME: write documentation for field
*/
/*!
  \var SoSFEnum SoImage::vertAlignment
  FIXME: write documentation for field
*/
/*!
  \var SoSFEnum SoImage::horAlignment
  FIXME: write documentation for field
*/
/*!
  \var SoSFImage SoImage::image
  FIXME: write documentation for field
*/
/*!
  \var SoSFString SoImage::filename
  FIXME: write documentation for field
*/


// *************************************************************************

SO_NODE_SOURCE(SoImage);

/*!
  Constructor.
*/
SoImage::SoImage()
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoImage);


  SO_NODE_ADD_FIELD(width, (-1));
  SO_NODE_ADD_FIELD(height, (-1));

  SO_NODE_ADD_FIELD(vertAlignment, (SoImage::BOTTOM));
  SO_NODE_ADD_FIELD(horAlignment, (SoImage::LEFT));
  SO_NODE_ADD_FIELD(image, (SbVec2s(0,0), 0, NULL));
  SO_NODE_ADD_FIELD(filename, (""));

  SO_NODE_DEFINE_ENUM_VALUE(VertAlignment, BOTTOM);
  SO_NODE_DEFINE_ENUM_VALUE(VertAlignment, HALF);
  SO_NODE_DEFINE_ENUM_VALUE(VertAlignment, TOP);
  SO_NODE_SET_SF_ENUM_TYPE(vertAlignment, VertAlignment);

  SO_NODE_DEFINE_ENUM_VALUE(HorAlignment, LEFT);
  SO_NODE_DEFINE_ENUM_VALUE(HorAlignment, CENTER);
  SO_NODE_DEFINE_ENUM_VALUE(HorAlignment, RIGHT);
  SO_NODE_SET_SF_ENUM_TYPE(horAlignment, HorAlignment);
}

/*!
  Destructor.
*/
SoImage::~SoImage()
{
}

/*!
  Does initialization common for all objects of the
  SoImage class. This includes setting up the
  type system, among other things.
*/
void
SoImage::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoImage);
}

#if !defined(COIN_EXCLUDE_SOGETBOUNDINGBOXACTION)
/*!
  FIXME: write function documentation
*/
void
SoImage::computeBBox(SoAction * /* action */,
		     SbBox3f & /* box */, SbVec3f & /* center */)
{
  assert(0 && "FIXME: not implemented yet");
}
#endif // !COIN_EXCLUDE_SOGETBOUNDINGBOXACTION


#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
/*!
  FIXME: write doc
*/
void
SoImage::GLRender(SoGLRenderAction * /* action */)
{
  assert(0 && "FIXME: not implemented");
}
#endif // !COIN_EXCLUDE_SOGLRENDERACTION

#if !defined(COIN_EXCLUDE_SORAYPICKACTION)
/*!
  FIXME: write doc
*/
void
SoImage::rayPick(SoRayPickAction * /* action */)
{
  assert(0 && "FIXME: not implemented");
}
#endif // !COIN_EXCLUDE_SORAYPICKACTION

#if !defined(COIN_EXCLUDE_SOGETPRIMITIVECOUNTACTION)
/*!
  FIXME: write doc
*/
void
SoImage::getPrimitiveCount(SoGetPrimitiveCountAction * /* action */)
{
  assert(0 && "FIXME: not implemented");
}
#endif // !COIN_EXCLUDE_SOGETPRIMITIVECOUNTACTION

#if !defined(COIN_EXCLUDE_SOACTION)
/*!
  FIXME: write doc
*/
void
SoImage::generatePrimitives(SoAction * /* action */)
{
  assert(0 && "FIXME: not implemented");
}
#endif // !COIN_EXCLUDE_SOACTION

/*!
  FIXME: write function documentation
*/
SbBool
SoImage::readInstance(SoInput * in, unsigned short /* flags */)
{
  assert(!in->isBinary() && "FIXME: not implemented yet");

  assert(0 && "FIXME: not implemented");
  return FALSE;
}

/*!
  FIXME: write function documentation
*/
int
SoImage::getReadStatus(void)
{
  assert(0 && "FIXME: not implemented");
  return 0;
}

/*!
  FIXME: write function documentation
*/
void
SoImage::setReadStatus(SbBool /* flag */)
{
  assert(0 && "FIXME: not implemented");
}

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
  \class SoAsciiText SoAsciiText.h Inventor/nodes/SoAsciiText.h
  \brief The SoAsciiText class renders flat 3D text.
  \ingroup nodes

  This class renders text using 3D geometry.

  It is different from the SoText2 node in that it rotates, scales,
  translates etc just like other geometry in the scene. It is
  different from the SoText3 node in that it renders the text "flat",
  i.e. does not extrude the fonts to have depth.
*/

// Metadon doc:
/*¡
  FIXME: not implemented yet. 20000312 mortene.
 */

#include <Inventor/nodes/SoAsciiText.h>
#include <coindefs.h> // COIN_STUB()

/*!
  \enum SoAsciiText::Justification
  Font justification values.
*/


/*!
  \var SoMFString SoAsciiText::string
  Lines of text to render. Default value is empty.
*/
/*!
  \var SoSFFloat SoAsciiText::spacing
  Spacing between each line. Defaults to 1.0.
*/
/*!
  \var SoSFEnum SoAsciiText::justification
  Justification value for layout of multiple lines. Default
  SoAsciiText::LEFT.
*/
/*!
  \var SoMFFloat SoAsciiText::width
  Defines the unit width of each line.
*/


// *************************************************************************

SO_NODE_SOURCE(SoAsciiText);

/*!
  Constructor.
*/
SoAsciiText::SoAsciiText(void)
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoAsciiText);

  SO_NODE_ADD_FIELD(string, (""));
  SO_NODE_ADD_FIELD(spacing, (1.0f));
  SO_NODE_ADD_FIELD(justification, (SoAsciiText::LEFT));
  SO_NODE_ADD_FIELD(width, (0.0f));

  SO_NODE_DEFINE_ENUM_VALUE(Justification, LEFT);
  SO_NODE_DEFINE_ENUM_VALUE(Justification, RIGHT);
  SO_NODE_DEFINE_ENUM_VALUE(Justification, CENTER);
  SO_NODE_SET_SF_ENUM_TYPE(justification, Justification);
}

/*!
  Destructor.
*/
SoAsciiText::~SoAsciiText()
{
}

// Doc in parent.
void
SoAsciiText::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoAsciiText);
}

// Doc in parent.
void
SoAsciiText::GLRender(SoGLRenderAction * action)
{
  COIN_STUB(); // FIXME
}

// Doc in parent.
void
SoAsciiText::rayPick(SoRayPickAction * action)
{
  COIN_STUB(); // FIXME
}

// Doc in parent.
void
SoAsciiText::getPrimitiveCount(SoGetPrimitiveCountAction * action)
{
  COIN_STUB(); // FIXME
}

// Doc in parent.
void
SoAsciiText::computeBBox(SoAction * action, SbBox3f & box, SbVec3f & center)
{
  COIN_STUB(); // FIXME
}

// Doc in parent.
void
SoAsciiText::generatePrimitives(SoAction * action)
{
  COIN_STUB(); // FIXME
}

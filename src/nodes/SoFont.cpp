/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2003 by Systems in Motion.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Systems in Motion about acquiring
 *  a Coin Professional Edition License.
 *
 *  See <URL:http://www.coin3d.org> for  more information.
 *
 *  Systems in Motion, Teknobyen, Abels Gate 5, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no>.
 *
\**************************************************************************/

/*!  
  \class SoFont SoFont.h Inventor/nodes/SoFont.h
  \brief The SoFont class is an appearance node for setting fonts.
  \ingroup nodes

  Successive text rendering nodes (like SoText2, SoText3, SoAsciiText,
  etc) will use the font specified from an SoFont node when
  visualizing text.

  The font name is recognized with the form "family:style". The range of
  supported font families depends on which fonts are installed on the
  system. A typical font family might be "Arial" or "Times New
  Roman". 

  Style is either "Bold", "Italic" or "Bold Italic".

  Spaces before or after the ":" will be ignored.

  If the chosen font cannot be found, the default font will be loaded
  instead. The default 2D font is a builtin 8x12 points font. The 3D
  font is a serif font ala Times New Roman. It is not possible to
  apply styles to any of the builtin fonts. It is not possible to
  specify a size for the default 2D font. 
  
  One can explicitly select the default font by setting "defaultFont"
  as the font name. The default fonts are always accessible by Coin
  as they are compiled into the library. If one needs to guarantee that
  the text will have the same appearance under all circumstances, the
  default font will be a safe choice. Another solution might be to use
  the \e FreeType font engine and explicitly name a font-file (This is
  closer described below).

  Here is a simple example on how to print a string using using a bold
  & Italic Arial font:

  \verbatim
  #Inventor V2.1 ascii
  
  Separator {
     Font {
       name "Arial:Bold Italic"
       size 14
     }
     Text2 {
       string ["This is a", "Coin font test"]
       justification CENTER
     }    
  }
  \endverbatim

  Coin has support for two different font APIs. On non-Windows
  platforms, the \e FreeType library is used. On Windows the Win32
  GDI library is used. \e FreeType is dynamically loaded by Coin at
  startup if accessible. It is possible to use the \e FreeType library
  on Windows also if one wishes.

  If Coin cannot load the \e FreeType library, only the default fonts
  will be accessible.

  It is possible to specify the TrueType font file directly if
  \e FreeType is used as the font engine. This is done by including the
  ".ttf" in the filename, i.e. "Comic_Sans_MS.ttf". Coin will then
  search the default font path and then the path specified by the
  "COIN_FONT_PATH" environment variable.

  It is not possible to directly specify a TrueType font file if
  Windows is handling the fonts. This is due to the way Windows is
  accessing the fonts through the system registry. All fonts must
  therefore be properly installed and given a system name. Open the
  "Control Panel" and double click on the "Fonts" icon for an overview
  of installed fonts and their names.

  Beware that some non-English versions of Windows are using different
  name for the styles (i.e. "Italique" instead of "Italic"). These
  names are supported in Coin, but it is recommended for portability
  purposes to only use the English terms.

  If the "COIN_DEBUG_FONTSUPPORT" environment variable is set to 1, an
  extensive amount of information about loading, initializing and
  using fonts will be output. Issues like missing fonts and other
  related problems will then be reported, so we advice you to first
  try to use that debugging option when something does not work quite
  as expected.

  \sa SoFontStyle, SoGlyph, SoText2, SoText3, SoAsciiText
*/



// FIXME: add correct and elaborate info on this to the class docs
// above;
//
//   Font files are searched for in COIN_FONT_PATH if that environment
//   variable is defined.
//
//   On Win32 systems the $WINDIR/Fonts directory will also be searched.
//
//   Currently, only TrueType fonts are supported.
//
// 20030612 mortene.  


#include <string.h>

#include <Inventor/nodes/SoFont.h>
#include <Inventor/nodes/SoSubNodeP.h>

#include <Inventor/actions/SoCallbackAction.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/actions/SoGetBoundingBoxAction.h>
#include <Inventor/actions/SoGetPrimitiveCountAction.h>
#include <Inventor/actions/SoPickAction.h>
#include <Inventor/elements/SoFontNameElement.h>
#include <Inventor/elements/SoFontSizeElement.h>
#include <Inventor/elements/SoOverrideElement.h>
#include <Inventor/errors/SoDebugError.h>

// *************************************************************************

/*!
  \var SoSFName SoFont::name

  Name of font.

  Which fontnames are available is rather system dependent, not only
  on whether or not you are running on a UNIX/Linux system, Microsoft
  Windows or whatever, but also on which fonts and font \e types (like
  TrueType) are installed on a particular user's system.

  All font rendering nodes have a built-in fallback font to use,
  though, so even though Coin can not find a font on the system of the
  specified type, the text should be rendered somehow.

  In summation, consider this node type and this particular field as a
  \e hint to the font rendering engines of Coin, and do \e not base
  your models on a particular font being available.
*/

/*!
  \var SoSFFloat SoFont::size

  Size of font. Defaults to 10.0.

  For 2D rendered bitmap fonts (like for SoText2), this value is the
  height of a character in screen pixels. For 3D text, this value is
  the world-space coordinates height of a character in the current
  units setting (see documentation for SoUnits node).
*/

// *************************************************************************

SO_NODE_SOURCE(SoFont);

/*!
  Constructor.
*/
SoFont::SoFont(void)
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoFont);

  SO_NODE_ADD_FIELD(name, ("defaultFont"));
  SO_NODE_ADD_FIELD(size, (10.0f));
}

/*!
  Destructor.
*/
SoFont::~SoFont()
{
}

// Doc from superclass.
void
SoFont::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoFont, SO_FROM_INVENTOR_1);

  SO_ENABLE(SoCallbackAction, SoFontNameElement);
  SO_ENABLE(SoCallbackAction, SoFontSizeElement);
  SO_ENABLE(SoGLRenderAction, SoFontNameElement);
  SO_ENABLE(SoGLRenderAction, SoFontSizeElement);
  SO_ENABLE(SoGetBoundingBoxAction, SoFontNameElement);
  SO_ENABLE(SoGetBoundingBoxAction, SoFontSizeElement);
  SO_ENABLE(SoGetPrimitiveCountAction, SoFontNameElement);
  SO_ENABLE(SoGetPrimitiveCountAction, SoFontSizeElement);
  SO_ENABLE(SoPickAction, SoFontNameElement);
  SO_ENABLE(SoPickAction, SoFontSizeElement);
}

// Doc from superclass.
void
SoFont::doAction(SoAction * action)
{
  SoState * state = action->getState();
  uint32_t flags = SoOverrideElement::getFlags(state);
    
#define TEST_OVERRIDE(bit) ((SoOverrideElement::bit & flags) != 0)
  
  if (!name.isIgnored() && !TEST_OVERRIDE(FONT_NAME)) {
    SoFontNameElement::set(state, this, this->name.getValue());
    if (this->isOverride()) {
      SoOverrideElement::setFontNameOverride(state, this, TRUE);
    }
  }
  if (!size.isIgnored() && !TEST_OVERRIDE(FONT_SIZE)) {
    SoFontSizeElement::set(state, this, this->size.getValue());
    if (this->isOverride()) {
      SoOverrideElement::setFontSizeOverride(state, this, TRUE);
    }
  }

#undef TEST_OVERRIDE
}

// Doc from superclass.
void
SoFont::getBoundingBox(SoGetBoundingBoxAction * action)
{
  SoFont::doAction(action);
}

// Doc from superclass.
void
SoFont::GLRender(SoGLRenderAction * action)
{
  SoFont::doAction(action);
}

// Doc from superclass.
void
SoFont::callback(SoCallbackAction * action)
{
  SoFont::doAction((SoAction *)action);
}

// Doc from superclass.
void
SoFont::pick(SoPickAction * action)
{
  SoFont::doAction(action);
}

// Doc from superclass.
void
SoFont::getPrimitiveCount(SoGetPrimitiveCountAction * action)
{
  SoFont::doAction(action);
}

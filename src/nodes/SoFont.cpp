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

  The mapping from SoFont::name to a font happens like this:
  
  <ol>

  <li> If the font name is on the built-in list of (font name, font
       file) pairs, use the font file from the list. FIXME: reproduce
       list here. 20030316 mortene. </li>

  <li> Try using the font name directly as a font file name. </li>

  <li> If all else fails, use the built-in default font. </li>

  </ol>

  Font files are searched for in SoInput::getDirectories(), and in
  COIN_FONT_PATH if that environment variable is
  defined. COIN_FONT_PATH can contain multiple directories, using the
  ':' character as separator.

  On Win32 systems the c:/WINDOWS/Fonts directory will also be
  searched. FIXME: update this doc when the Windows PATH has been
  corrected to use WINDIR or some such. 20030316 mortene.

  Currently, font files in the TrueType format (.ttf) are supported.
  
  \sa SoFontStyle, SoGlyph, SoText2, SoText3, SoAsciiText
*/

#include <Inventor/nodes/SoFont.h>
#include <Inventor/nodes/SoSubNodeP.h>

#include <Inventor/C/tidbits.h>
#include <Inventor/C/tidbitsp.h>
#include <Inventor/actions/SoCallbackAction.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/actions/SoGetBoundingBoxAction.h>
#include <Inventor/actions/SoGetPrimitiveCountAction.h>
#include <Inventor/actions/SoPickAction.h>
#include <Inventor/elements/SoFontNameElement.h>
#include <Inventor/elements/SoFontSizeElement.h>
#include <Inventor/elements/SoOverrideElement.h>
#include <Inventor/misc/SoFontLib.h>

#include <string.h>

/*!
  \var SoSFName SoFont::name

  Name of font.

  Which fontnames are available is rather systemdependent, not only on
  whether or not you are running on a UNIX/Linux system, Microsoft
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

  Size of font. Defaults to 12.0.

  For 2D rendered bitmap fonts (like for SoText2), this value is the
  height of a character in screen pixels. For 3D text, this value is
  the world-space coordinates height of a character in the current
  units setting (see documentation for SoUnits node).
*/

// *************************************************************************


#ifndef DOXYGEN_SKIP_THIS

class SoFontP {
 public:
  SoFont * owner;
  SbString lastfontname;
  float lastsize;
  SbBool firsttime;
  
  SoFontP(SoFont * owner) {
    this->owner = owner;
    this->lastsize = 0.0;
    this->firsttime = TRUE;
  }
};

// The following missing_pimpl_workaround taken from Coin-1 SoWWWInline.cpp

// We forgot about this class before locking the Coin 2.0 ABI, so we
// have to use an SbDict to store per-instance data members for this
// class. In Coin 3.0 it's ok to break ABI-compatibility again, so we
// don't use this little hack there, but rather the usual Cheshire Cat
// pattern.

static SbDict * sofont_private_data_dict = NULL;

static void
sofont_private_data_cleanup(void)
{
  delete sofont_private_data_dict;
  sofont_private_data_dict = NULL;
}

static SoFontP *
sofont_get_private_data(const SoFont * thisp)
{
  if (sofont_private_data_dict == NULL) {
    sofont_private_data_dict = new SbDict;
    coin_atexit((coin_atexit_f *)sofont_private_data_cleanup, 0);
  }
  void * pimpl;
  if (!sofont_private_data_dict->find((unsigned long) thisp, pimpl)) {
    pimpl = (void*) new SoFontP((SoFont*) thisp);
    (void) sofont_private_data_dict->enter((unsigned long) thisp, pimpl);
  }
  return (SoFontP*) pimpl;
}

#endif // DOXYGEN_SKIP_THIS

SO_NODE_SOURCE(SoFont);

#undef THIS
// WARNING: Slow! Don't use this macro if you need speed
#define THIS (sofont_get_private_data(this))

/*!
  Constructor.
*/
SoFont::SoFont(void)
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoFont);

  SO_NODE_ADD_FIELD(name, ("defaultFont"));
  SO_NODE_ADD_FIELD(size, (12.0f));
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
  
  SoFontP * pimpl = THIS;
  const char * this_name = this->name.getValue().getString();
  const float this_size = this->size.getValue();
  if (pimpl->firsttime || 
      pimpl->lastsize != this_size || 
      strcmp(pimpl->lastfontname.getString(), this_name)) {
    pimpl->lastfontname = this->name.getValue();
    SoFontLib::createFont(this->name.getValue(), SbString(""), SbVec2s((short)this_size, (short)this_size));
    pimpl->lastsize = this_size;
    pimpl->firsttime = FALSE;
  }
  
#define TEST_OVERRIDE(bit) ((SoOverrideElement::bit & flags) != 0)
  
  if (!name.isIgnored() && !TEST_OVERRIDE(FONT_NAME)) {
    SoFontNameElement::set(state, this, this->name.getValue());
    if (this->isOverride()) {
      SoOverrideElement::setFontNameOverride(state, this, TRUE);
    }
  }
  if (!size.isIgnored() && !TEST_OVERRIDE(FONT_SIZE)) {
    SoFontSizeElement::set(state, this, this_size);
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

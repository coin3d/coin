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
  \class SoFontLib include/Inventor/misc/SoFontLib.h
  \brief The SoFontLib class is an interface between Coin and the font library wrapper.

  This is an internal class.
*/

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <Inventor/misc/SoFontLib.h>

#include <Inventor/C/threads/threadsutilp.h>
#include <Inventor/C/tidbits.h>
#include <Inventor/SbName.h>
#include <Inventor/lists/SbList.h>
#include <Inventor/SbVec2s.h>
#include <Inventor/SbString.h>
#include <Inventor/lists/SbStringList.h>
#include <Inventor/SoInput.h>

/*************************************************************************/

class SoFontLibP {
public:
  static void * apimutex;
  static SbStringList * fontfiles;
};

void * SoFontLibP::apimutex = NULL;
SbStringList * SoFontLibP::fontfiles = NULL;

/*************************************************************************/

/*!
  Constructor.
*/
SoFontLib::SoFontLib(void)
{
}

/*!
  Destructor.
*/
SoFontLib::~SoFontLib()
{
}

void
SoFontLib::initialize(void)
{
  if (SoFontLibP::apimutex == NULL)
    CC_MUTEX_CONSTRUCT(SoFontLibP::apimutex);
  CC_MUTEX_LOCK(SoFontLibP::apimutex);

  // Where to look for font files
  if (coin_getenv("COIN_FONT_PATH") != NULL) {
    SoInput::addEnvDirectoriesLast("COIN_FONT_PATH", NULL);
  }

#ifdef HAVE_WINDOWS_H
  // FIXME: shouldn't this rather be set from a $WINDIR type env var?
  // AFAIK, the Windows system directory doesn't have to be under
  // C:/WINDOWS. 20030316 mortene.
  SoInput::addDirectoryLast("c:/WINDOWS/Fonts");
#endif

  // Built-in mappings from font name to font file name
  const char * fontmappings[] = {
    "Arial", "arial.ttf", "Arial Bold", "arialbd.ttf",
    "Arial Bold Italic", "arialbi.ttf", "Arial Italic", "ariali.ttf",
    "Century Gothic", "gothic.ttf", "Century Gothic Bold", "gothicb.ttf",
    "Century Gothic Bold Italic", "gothicbi.ttf",
    "Century Gothic Italic", "gothici.ttf",
    "Courier", "cour.ttf", "Courier Bold", "courbd.ttf",
    "Courier Bold Italic", "courbi.ttf", "Courier Italic", "couri.ttf",
    "Simian", "simtoran.ttf", "Times New Roman", "times.ttf",
    "Times New Roman Bold", "timesbd.ttf",
    "Times New Roman Bold Italic", "timesbi.ttf",
    "Times New Roman Italic", "timesi.ttf",
    "Verdana", "verdana.ttf", "Verdana Bold", "verdanab.ttf",
    "Verdana Bold Italic", "verdanaz.ttf", "Verdana Italic", "verdanai.ttf"
  };

  SoFontLibP::fontfiles = new SbStringList;
  for (unsigned int i=0; i < (sizeof(fontmappings) / sizeof(fontmappings[0])); i++) {
    SoFontLibP::fontfiles->append(new SbString(fontmappings[i]));
  }

  cc_flwInitialize();
  CC_MUTEX_UNLOCK(SoFontLibP::apimutex);
}

void
SoFontLib::exit()
{
  int i;
  CC_MUTEX_LOCK(SoFontLibP::apimutex);
  cc_flwExit();
  for (i = 0; i < SoFontLibP::fontfiles->getLength(); i++) {
    delete (*SoFontLibP::fontfiles)[i];
  }
  delete SoFontLibP::fontfiles;
  CC_MUTEX_UNLOCK(SoFontLibP::apimutex);
  if (SoFontLibP::apimutex != NULL)
    CC_MUTEX_DESTRUCT(SoFontLibP::apimutex);
}

SbString
SoFontLib::createFont(const SbName &fontname, const SbName &stylename, const SbVec2s &size)
{
  CC_MUTEX_LOCK(SoFontLibP::apimutex);
  SbString path;
  char name[300];
  int fileidx, i, retval;
  SbStringList emptylist;
  fileidx = -1;
  for (i=0; i<SoFontLibP::fontfiles->getLength(); i+=2)
    if (!strcmp((*SoFontLibP::fontfiles)[i]->getString(), fontname.getString())) {
      fileidx = i;
      i = SoFontLibP::fontfiles->getLength();
    }
  // fprintf(stderr,"createFont: fileidx %d\n", fileidx);  // DEBUG
  if (fileidx >= 0) {  // Valid font name
    path = SoInput::searchForFile(*(*SoFontLibP::fontfiles)[fileidx+1], SoInput::getDirectories(), emptylist);
  } else {  // Treat as font file name
    path = SoInput::searchForFile(fontname.getString(), SoInput::getDirectories(), emptylist);
  }
  // fprintf(stderr,"createFont: path %s\n", path.getString());  // DEBUG
  int font = cc_flwCreateFont( path.getString(), name, 300, 12, 12 );
  if (font >= 0)
    cc_flwSetCharSize(font, (int)size[0], (int)size[1]);
  CC_MUTEX_UNLOCK(SoFontLibP::apimutex);
  return SbString(name);
}

int
SoFontLib::getFont(const SbName &fontname, const SbVec2s &size)
{
  CC_MUTEX_LOCK(SoFontLibP::apimutex);
  int font = cc_flwGetFont( fontname.getString(), (int)size[0], (int)size[1]);
  // cc_flwSetFontRotation(font, 45.0);  // DEBUG
  CC_MUTEX_UNLOCK(SoFontLibP::apimutex);
  return font;
}

void
SoFontLib::doneFont(const int font)
{
  CC_MUTEX_LOCK(SoFontLibP::apimutex);
  if (font >= 0)
    cc_flwDoneFont(font);
  CC_MUTEX_UNLOCK(SoFontLibP::apimutex);
}

int
SoFontLib::getNumCharmaps(const int font)
{
  CC_MUTEX_LOCK(SoFontLibP::apimutex);
  int retval = 0;
  if (font >= 0)
    retval = cc_flwGetNumCharmaps(font);
  CC_MUTEX_UNLOCK(SoFontLibP::apimutex);
  return retval;
}

SbName
SoFontLib::getCharmapName(const int font, const int charmap)
{
  CC_MUTEX_LOCK(SoFontLibP::apimutex);
  char namebuf[80];
  sprintf(namebuf,"unknown");
  if (font >= 0)
    cc_flwGetCharmapName(font, charmap, namebuf, 80);
  CC_MUTEX_UNLOCK(SoFontLibP::apimutex);
  return SbName(namebuf);
}

void
SoFontLib::setCharmap(const int font, const int charmap)
{
  CC_MUTEX_LOCK(SoFontLibP::apimutex);
  if (font >= 0)
    cc_flwSetCharmap(font, charmap);
  CC_MUTEX_UNLOCK(SoFontLibP::apimutex);
}

void
SoFontLib::setCharSize(const int font, const SbVec2s &size)
{
  CC_MUTEX_LOCK(SoFontLibP::apimutex);
  if (font >= 0)
    cc_flwSetCharSize(font, (int)size[0], (int)size[1]);
  CC_MUTEX_UNLOCK(SoFontLibP::apimutex);
}

void
SoFontLib::setFontRotation(const int font, const float angle)
{
  CC_MUTEX_LOCK(SoFontLibP::apimutex);
  if (font >= 0)
    cc_flwSetFontRotation(font, angle);
  CC_MUTEX_UNLOCK(SoFontLibP::apimutex);
}

int
SoFontLib::getGlyph(const int font, const int charidx)
{
  CC_MUTEX_LOCK(SoFontLibP::apimutex);
  int retval = -1;
  if (font >= 0)
    retval = cc_flwGetGlyph(font, charidx);
  CC_MUTEX_UNLOCK(SoFontLibP::apimutex);
  return retval;
}

SbVec2s
SoFontLib::getAdvance(const int font, const int glyph)
{
  CC_MUTEX_LOCK(SoFontLibP::apimutex);
  SbVec2s retval(0, 0);
  if (font >= 0) {
    float x, y;
    int result = cc_flwGetAdvance(font, glyph, &x, &y);
    if (result==0)
      retval = SbVec2s((short)x, (short)y);
  }
  CC_MUTEX_UNLOCK(SoFontLibP::apimutex);
  return retval;
}

SbVec2s
SoFontLib::getKerning(const int font, const int leftglyph, const int rightglyph)
{
  CC_MUTEX_LOCK(SoFontLibP::apimutex);
  SbVec2s retval(0, 0);
  if (font >= 0) {
    float x, y;
    int result = cc_flwGetKerning(font, leftglyph, rightglyph, &x, &y);
    if (result==0)
      retval = SbVec2s((short)x, (short)y);
  }
  CC_MUTEX_UNLOCK(SoFontLibP::apimutex);
  return retval;
}

void
SoFontLib::doneGlyph(const int font, const int glyph)
{
  CC_MUTEX_LOCK(SoFontLibP::apimutex);
  if (font >= 0)
    cc_flwDoneGlyph(font, glyph);
  CC_MUTEX_UNLOCK(SoFontLibP::apimutex);
}


// antialiased -> 8 pixels per byte (grayscale)
// !antialiased  -> 1 pixel per byte
// Bitmaps are cached in fontlib_wrapper, no need to free any data.
// FIXME: Support antialiased fonts. preng 20030224.
unsigned char *
SoFontLib::getBitmap(const int font, const int glyph, SbVec2s &size, SbVec2s &pos, const SbBool antialiased)
{
  CC_MUTEX_LOCK(SoFontLibP::apimutex);
  unsigned char * retval = NULL;
  if (font >= 0 && glyph >= 0) {
    FLWbitmap * bm = cc_flwGetBitmap(font, glyph);
    if (bm) {
      size[0] = bm->pitch * 8;
      size[1] = bm->rows;
      pos[0] = bm->bearingX;
      pos[1] = bm->bearingY;
      retval = bm->buffer;
    }
  }
  CC_MUTEX_UNLOCK(SoFontLibP::apimutex);
  return retval;
}

int
SoFontLib::getOutline(const int font, const int glyph)
{
  CC_MUTEX_LOCK(SoFontLibP::apimutex);
  int retval = -1;
  if (font >= 0)
    retval = cc_flwGetOutline(font, glyph);
  CC_MUTEX_UNLOCK(SoFontLibP::apimutex);
  return retval;
}

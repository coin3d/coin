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

#include <Inventor/misc/SoFontLib.h>

#include <Inventor/C/threads/threadsutilp.h>
#include <Inventor/SbName.h>
#include <Inventor/lists/SbList.h>
#include <Inventor/SbVec2s.h>
#include <Inventor/SbString.h>
#include <Inventor/lists/SbStringList.h>
#include <Inventor/SoInput.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


static void * SoFontLib_mutex = NULL;
static SbStringList fontfiles;

/*!
  Constructor.
*/
SoFontLib::SoFontLib() 
{
}

/*!
  Destructor.
*/
SoFontLib::~SoFontLib() 
{
}

void 
SoFontLib::initialize() 
{
  if (SoFontLib_mutex == NULL)
    CC_MUTEX_CONSTRUCT(SoFontLib_mutex);
  CC_MUTEX_LOCK(SoFontLib_mutex);
  
  // Where to look for font files
  SoInput::addEnvDirectoriesLast("COIN_FONT_PATH", NULL);
#ifdef HAVE_WINDOWS_H
  SoInput::addDirectoryLast("c:/WINDOWS/Fonts");
#endif
  
  // Built-in mappings from font name to font file name
  fontfiles.truncate(0);
  fontfiles.append(new SbString("Arial"));
  fontfiles.append(new SbString("arial.ttf"));
  fontfiles.append(new SbString("Arial Bold"));
  fontfiles.append(new SbString("arialbd.ttf"));
  fontfiles.append(new SbString("Arial Bold Italic"));
  fontfiles.append(new SbString("arialbi.ttf"));
  fontfiles.append(new SbString("Arial Italic"));
  fontfiles.append(new SbString("ariali.ttf"));
  fontfiles.append(new SbString("Century Gothic"));
  fontfiles.append(new SbString("gothic.ttf"));
  fontfiles.append(new SbString("Century Gothic Bold"));
  fontfiles.append(new SbString("gothicb.ttf"));
  fontfiles.append(new SbString("Century Gothic Bold Italic"));
  fontfiles.append(new SbString("gothicbi.ttf"));
  fontfiles.append(new SbString("Century Gothic Italic"));
  fontfiles.append(new SbString("gothici.ttf"));
  fontfiles.append(new SbString("Courier"));
  fontfiles.append(new SbString("cour.ttf"));
  fontfiles.append(new SbString("Courier Bold"));
  fontfiles.append(new SbString("courbd.ttf"));
  fontfiles.append(new SbString("Courier Bold Italic"));
  fontfiles.append(new SbString("courbi.ttf"));
  fontfiles.append(new SbString("Courier Italic"));
  fontfiles.append(new SbString("couri.ttf"));
  fontfiles.append(new SbString("Simian"));
  fontfiles.append(new SbString("simtoran.ttf"));
  fontfiles.append(new SbString("Times New Roman"));
  fontfiles.append(new SbString("times.ttf"));
  fontfiles.append(new SbString("Times New Roman Bold"));
  fontfiles.append(new SbString("timesbd.ttf"));
  fontfiles.append(new SbString("Times New Roman Bold Italic"));
  fontfiles.append(new SbString("timesbi.ttf"));
  fontfiles.append(new SbString("Times New Roman Italic"));
  fontfiles.append(new SbString("timesi.ttf"));
  fontfiles.append(new SbString("Verdana"));
  fontfiles.append(new SbString("verdana.ttf"));
  fontfiles.append(new SbString("Verdana Bold"));
  fontfiles.append(new SbString("verdanab.ttf"));
  fontfiles.append(new SbString("Verdana Bold Italic"));
  fontfiles.append(new SbString("verdanaz.ttf"));
  fontfiles.append(new SbString("Verdana Italic"));
  fontfiles.append(new SbString("verdanai.ttf"));

  flwInitialize();
  CC_MUTEX_UNLOCK(SoFontLib_mutex);
}

void 
SoFontLib::exit() 
{
  CC_MUTEX_LOCK(SoFontLib_mutex);
  flwExit();
  fontfiles.truncate(0);
  CC_MUTEX_UNLOCK(SoFontLib_mutex);
  if (SoFontLib_mutex != NULL)
    CC_MUTEX_DESTRUCT(SoFontLib_mutex);
}

const SbString
SoFontLib::createFont(const SbName &fontname, const SbName &stylename, const SbVec2s &size) 
{
  CC_MUTEX_LOCK(SoFontLib_mutex);
  SbString path;
  char name[300];
  int fileidx, i, retval;
  SbStringList emptylist;
  fileidx = -1;
  for (i=0; i<fontfiles.getLength(); i+=2)
    if (!strcmp(fontfiles[i]->getString(), fontname.getString())) {
      fileidx = i;
      i = fontfiles.getLength();
    }
  // fprintf(stderr,"createFont: fileidx %d\n", fileidx);  // DEBUG
  if (fileidx >= 0) {  // Valid font name
    path = SoInput::searchForFile(*fontfiles[fileidx+1], SoInput::getDirectories(), emptylist);
  } else {  // Treat as font file name
    path = SoInput::searchForFile(fontname.getString(), SoInput::getDirectories(), emptylist);
  }
  // fprintf(stderr,"createFont: path %s\n", path.getString());  // DEBUG
  int font = flwCreateFont( path.getString(), name, 300, 12, 12 );
  if (font >= 0) 
    flwSetCharSize(font, (int)size[0], (int)size[1]);
  CC_MUTEX_UNLOCK(SoFontLib_mutex);
  return SbString(name);
}

int
SoFontLib::getFont(const SbName &fontname, const SbVec2s &size) 
{
  CC_MUTEX_LOCK(SoFontLib_mutex);
  int font = flwGetFont( fontname.getString(), (int)size[0], (int)size[1]);
  // flwSetFontRotation(font, 45.0);  // DEBUG
  CC_MUTEX_UNLOCK(SoFontLib_mutex);
  return font;
}

void 
SoFontLib::doneFont(const int font)
{
  CC_MUTEX_LOCK(SoFontLib_mutex);
  if (font >= 0)
    flwDoneFont(font);
  CC_MUTEX_UNLOCK(SoFontLib_mutex);
}

int
SoFontLib::getNumCharmaps(const int font)
{
  CC_MUTEX_LOCK(SoFontLib_mutex);
  int retval = 0;
  if (font >= 0)
    retval = flwGetNumCharmaps(font);
  CC_MUTEX_UNLOCK(SoFontLib_mutex);
  return retval;
}

SbName 
SoFontLib::getCharmapName(const int font, const int charmap) 
{
  CC_MUTEX_LOCK(SoFontLib_mutex);
  char namebuf[80];
  sprintf(namebuf,"unknown");
  if (font >= 0)
    flwGetCharmapName(font, charmap, namebuf, 80);
  CC_MUTEX_UNLOCK(SoFontLib_mutex);
  return SbName(namebuf);
}

void 
SoFontLib::setCharmap(const int font, const int charmap) 
{
  CC_MUTEX_LOCK(SoFontLib_mutex);
  if (font >= 0)
    flwSetCharmap(font, charmap);
  CC_MUTEX_UNLOCK(SoFontLib_mutex);
}

void 
SoFontLib::setCharSize(const int font, const SbVec2s &size) 
{
  CC_MUTEX_LOCK(SoFontLib_mutex);
  if (font >= 0)
    flwSetCharSize(font, (int)size[0], (int)size[1]);
  CC_MUTEX_UNLOCK(SoFontLib_mutex);
}

void 
SoFontLib::setFontRotation(const int font, const float angle) 
{
  CC_MUTEX_LOCK(SoFontLib_mutex);
  if (font >= 0)
    flwSetFontRotation(font, angle);
  CC_MUTEX_UNLOCK(SoFontLib_mutex);
}

int
SoFontLib::getGlyph(const int font, const int charidx) 
{
  CC_MUTEX_LOCK(SoFontLib_mutex);
  int retval = -1;
  if (font >= 0)
    retval = flwGetGlyph(font, charidx);
  CC_MUTEX_UNLOCK(SoFontLib_mutex);
  return retval;
}

SbVec2s 
SoFontLib::getAdvance(const int font, const int glyph) 
{
  CC_MUTEX_LOCK(SoFontLib_mutex);
  SbVec2s retval(0, 0);
  if (font >= 0) {
    float x, y;
    int result = flwGetAdvance(font, glyph, &x, &y);
    if (result==0)
      retval = SbVec2s(x, y);
  }
  CC_MUTEX_UNLOCK(SoFontLib_mutex);
  return retval;
}

SbVec2s 
SoFontLib::getKerning(const int font, const int leftglyph, const int rightglyph) 
{
  CC_MUTEX_LOCK(SoFontLib_mutex);
  SbVec2s retval(0, 0);
  if (font >= 0) {
    float x, y;
    int result = flwGetKerning(font, leftglyph, rightglyph, &x, &y);
    if (result==0)
      retval = SbVec2s(x, y);
  }
  CC_MUTEX_UNLOCK(SoFontLib_mutex);
  return retval;
}

void 
SoFontLib::doneGlyph(const int font, const int glyph) 
{
  CC_MUTEX_LOCK(SoFontLib_mutex);
  if (font >= 0)
    flwDoneGlyph(font, glyph);
  CC_MUTEX_UNLOCK(SoFontLib_mutex);
}


// antialiased -> 8 pixels per byte (grayscale)
// !antialiased  -> 1 pixel per byte
// Bitmaps are cached in fontlib_wrapper, no need to free any data.
// FIXME: Support antialiased fonts. preng 20030224.
unsigned char * 
SoFontLib::getBitmap(const int font, const int glyph, SbVec2s &size, SbVec2s &pos, const SbBool antialiased) 
{
  CC_MUTEX_LOCK(SoFontLib_mutex);
  unsigned char * retval = NULL;
  if (font >= 0 && glyph >= 0) {
    FLWbitmap * bm = flwGetBitmap(font, glyph);
    if (bm) {
      size[0] = bm->pitch * 8;
      size[1] = bm->rows;
      pos[0] = bm->bearingX;
      pos[1] = bm->bearingY;
      retval = bm->buffer;
    }
  }
  CC_MUTEX_UNLOCK(SoFontLib_mutex);
  return retval;
}

int 
SoFontLib::getOutline(const int font, const int glyph) 
{
  CC_MUTEX_LOCK(SoFontLib_mutex);
  int retval = -1;
  if (font >= 0)
    retval = flwGetOutline(font, glyph);
  CC_MUTEX_UNLOCK(SoFontLib_mutex);
  return retval;
}



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
#ifndef DOXYGEN_SKIP_THIS

class SoFontLibP {
public:
  static void * apimutex;
  static SbStringList * fontfiles;
  static SbDict openfonts;
};

void * SoFontLibP::apimutex = NULL;
SbStringList * SoFontLibP::fontfiles = NULL;
SbDict SoFontLibP::openfonts;

/*************************************************************************/

#endif DOXYGEN_SKIP_THIS

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
  if (SoFontLibP::apimutex == NULL) {
    // Construct & initialize static vars
    CC_MUTEX_CONSTRUCT(SoFontLibP::apimutex);
    SoFontLibP::openfonts.clear();
  }
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

  cc_flw_initialize();
  
  // Create default font
  SoFontLib::createFont(SbName("defaultFont"), SbName(""), SbVec2s(12, 12));
  
  CC_MUTEX_UNLOCK(SoFontLibP::apimutex);
}

void
SoFontLib::exit()
{
  int i;
  CC_MUTEX_LOCK(SoFontLibP::apimutex);
  cc_flw_exit();
  // Clean up fontfiles
  for (i = 0; i < SoFontLibP::fontfiles->getLength(); i++) {
    delete (*SoFontLibP::fontfiles)[i];
  }
  delete SoFontLibP::fontfiles;
  // Clean up openfonts dict
  SbPList keys, values;
  SoFontLibP::openfonts.makePList(keys, values);
  for (i = 0; i < values.getLength(); i++) {
    delete (SbString*) values[i];
  }
  CC_MUTEX_UNLOCK(SoFontLibP::apimutex);
  if (SoFontLibP::apimutex != NULL)
    CC_MUTEX_DESTRUCT(SoFontLibP::apimutex);
}

int
SoFontLib::createFont(const SbName &fontname, const SbName &stylename, const SbVec2s &size)
{
  CC_MUTEX_LOCK(SoFontLibP::apimutex);
  SbString path, *strptr;
  int fileidx, i;
  SbStringList emptylist;
  fileidx = -1;
  int font = -1;
  // Check if we already know the requestname for this fontname
  if (SoFontLibP::openfonts.find((unsigned long)fontname.getString(), (void *&)strptr)) {
    path = *strptr;
    font = cc_flw_create_font( path.getString(), size[0], size[1] );
  } 
  else {
    // Check if we know the font file for this font name
    for (i = 0; i < SoFontLibP::fontfiles->getLength(); i+=2) {
      if (!strcmp((*SoFontLibP::fontfiles)[i]->getString(), fontname.getString())) {
        fileidx = i;
        i = SoFontLibP::fontfiles->getLength();
      }
    }
    if (fileidx >= 0) {  // Known font name, look for the font file
      path = SoInput::searchForFile(*(*SoFontLibP::fontfiles)[fileidx+1], SoInput::getDirectories(), emptylist);
    } 
    else {  // Unknown font name, treat it as a font file name
      path = SoInput::searchForFile(fontname.getString(), SoInput::getDirectories(), emptylist);
    }
    font = cc_flw_create_font( path.getString(), size[0], size[1] );
    // Add font to openfonts dict
    if (font >= 0) {
      SbString * newfont = new SbString(path);
      SoFontLibP::openfonts.enter((unsigned long)fontname.getString(), (void *)newfont);
    }
  }
  CC_MUTEX_UNLOCK(SoFontLibP::apimutex);
  return font;
}

int
SoFontLib::getFont(const SbName &fontname, const SbVec2s &size)
{
  CC_MUTEX_LOCK(SoFontLibP::apimutex);
  SbString * requestname;
  int font = -1;
  if ( SoFontLibP::openfonts.find((unsigned long)fontname.getString(), (void *&)requestname) ) {
    font = cc_flw_get_font( requestname->getString(), (int)size[0], (int)size[1]);
  }
  CC_MUTEX_UNLOCK(SoFontLibP::apimutex);
  return font;
}

void
SoFontLib::doneFont(const int font)
{
  CC_MUTEX_LOCK(SoFontLibP::apimutex);
  if (font >= 0)
    cc_flw_done_font(font);
  CC_MUTEX_UNLOCK(SoFontLibP::apimutex);
}

int
SoFontLib::getNumCharmaps(const int font)
{
  CC_MUTEX_LOCK(SoFontLibP::apimutex);
  int retval = 0;
  if (font >= 0)
    retval = cc_flw_get_num_charmaps(font);
  CC_MUTEX_UNLOCK(SoFontLibP::apimutex);
  return retval;
}

SbString
SoFontLib::getCharmapName(const int font, const int charmap)
{
  CC_MUTEX_LOCK(SoFontLibP::apimutex);
  cc_string * name;
  if (font >= 0) {
    name = cc_flw_get_charmap_name(font, charmap);
  } else {
    name = cc_string_construct_new();
    cc_string_set_text(name, "unknown");
  }
  SbString retval = cc_string_get_text(name);
  cc_string_destruct(name);
  CC_MUTEX_UNLOCK(SoFontLibP::apimutex);
  return retval;
}

void
SoFontLib::setCharmap(const int font, const int charmap)
{
  CC_MUTEX_LOCK(SoFontLibP::apimutex);
  if (font >= 0)
    cc_flw_set_charmap(font, charmap);
  CC_MUTEX_UNLOCK(SoFontLibP::apimutex);
}

void
SoFontLib::setCharSize(const int font, const SbVec2s &size)
{
  CC_MUTEX_LOCK(SoFontLibP::apimutex);
  if (font >= 0)
    cc_flw_set_char_size(font, (int)size[0], (int)size[1]);
  CC_MUTEX_UNLOCK(SoFontLibP::apimutex);
}

void
SoFontLib::setFontRotation(const int font, const float angle)
{
  CC_MUTEX_LOCK(SoFontLibP::apimutex);
  if (font >= 0)
    cc_flw_set_font_rotation(font, angle);
  CC_MUTEX_UNLOCK(SoFontLibP::apimutex);
}

int
SoFontLib::getGlyph(const int font, const int charidx)
{
  CC_MUTEX_LOCK(SoFontLibP::apimutex);
  int retval = -1;
  if (font >= 0)
    retval = cc_flw_get_glyph(font, charidx);
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
    int result = cc_flw_get_advance(font, glyph, &x, &y);
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
    int result = cc_flw_get_kerning(font, leftglyph, rightglyph, &x, &y);
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
    cc_flw_done_glyph(font, glyph);
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
    cc_FLWbitmap * bm = cc_flw_get_bitmap(font, glyph);
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
    retval = cc_flw_get_outline(font, glyph);
  CC_MUTEX_UNLOCK(SoFontLibP::apimutex);
  return retval;
}

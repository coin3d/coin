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

// The SoFontLib class is an interface between Coin and the font
// library wrapper.
//
// It's main purpose is to map between Inventor's SoFont-type font
// names and the actual system font (or font-file) names.
//
// This is an internal class.
//
// FIXME: it seems a bit unnecessary to add this extra layer on top of
// src/glue/fontlib_wrapper.c -- would moving the functionality of
// this class down into fontlib_wrapper (or up into the next layer
// above) clean up the code design a bit? Investigate. 20030526 mortene.
//
// FIXME: the mutex locking looks excessive. Audit & fix. 20030526 mortene.
//
// FIXME: most of the functions looks overly robust -- should probably
// replace all "if(font>=0)" with asserts. 20030526 mortene.

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <Inventor/misc/SoFontLib.h>

#include <Inventor/C/threads/threadsutilp.h>
#include <Inventor/C/tidbits.h>
#include <Inventor/SbName.h>
#include <Inventor/SbString.h>
#include <Inventor/SbVec2s.h>
#include <Inventor/SoInput.h>
#include <Inventor/errors/SoDebugError.h>
#include <Inventor/lists/SbList.h>
#include <Inventor/lists/SbStringList.h>

/*************************************************************************/

class SoFontLibP {
public:
  static void * apimutex;
  static SbStringList * fontfiles;
  static SbDict * openfonts;
};

void * SoFontLibP::apimutex = NULL;
SbStringList * SoFontLibP::fontfiles = NULL;
SbDict * SoFontLibP::openfonts = NULL;


/*************************************************************************/

void
SoFontLib::initialize(void)
{
  if (SoFontLibP::apimutex == NULL) {
    // Construct & initialize static vars
    CC_MUTEX_CONSTRUCT(SoFontLibP::apimutex);
    SoFontLibP::openfonts = new SbDict;
  }
  CC_MUTEX_LOCK(SoFontLibP::apimutex);

  // Where to look for font files
  if (coin_getenv("COIN_FONT_PATH") != NULL) {
    SoInput::addEnvDirectoriesLast("COIN_FONT_PATH", NULL);
  }

  // FIXME: bad #ifdef, should ideally be a run-time check for
  // MSWindows. 20030526 mortene.
#ifdef _WIN32
  const char * windir = coin_getenv("WINDIR");
  if (windir) {
    SbString winfontpath = windir;
    winfontpath += "/Fonts";
    // FIXME: $WINDIR/Fonts might not exist, so either check that
    // SoInput is robust when adding non-existing directory, or add a
    // stat() call to see if $WINDIR/Fonts exists. 20030526 mortene.
    SoInput::addDirectoryLast(winfontpath.getString());
  }
#endif // _WIN32

  // Built-in mappings from font name to font file name.
  // NB! Prepend font names with ' ' to separate them from file names.
  const char * fontmappings[] = {
    " Arial", "arial.ttf", 
    " Arial Bold", "arialbd.ttf",
    " Arial Bold Italic", "arialbi.ttf", 
    " Arial Italic", "ariali.ttf",
    " Century Gothic", "gothic.ttf", 
    " Century Gothic Bold", "gothicb.ttf",
    " Century Gothic Bold Italic", "gothicbi.ttf",
    " Century Gothic Italic", "gothici.ttf",
    " Courier", "cour.ttf", 
    " Courier Bold", "courbd.ttf",
    " Courier Bold Italic", "courbi.ttf", 
    " Courier Italic", "couri.ttf",
    " Simian", "simtoran.ttf", "simtgori.ttf", "simtchimp.ttf",
    " Times New Roman", "times.ttf",
    " Times New Roman Bold", "timesbd.ttf",
    " Times New Roman Bold Italic", "timesbi.ttf",
    " Times New Roman Italic", "timesi.ttf",
    " Verdana", "verdana.ttf", 
    " Verdana Bold", "verdanab.ttf",
    " Verdana Bold Italic", "verdanaz.ttf", 
    " Verdana Italic", "verdanai.ttf",
    " OpenSymbol", "opens___.ttf",
    " Small", "smalle.fon"
  };
  
  SoFontLibP::fontfiles = new SbStringList;
  for (unsigned int i=0; i < (sizeof(fontmappings) / sizeof(fontmappings[0])); i++) {
    SoFontLibP::fontfiles->append(new SbString(fontmappings[i]));
  }

  cc_flw_initialize();
    
  CC_MUTEX_UNLOCK(SoFontLibP::apimutex);

  // Create default font.
  //
  // FIXME: this is a hack, I don't like it. The default font should
  // be set up in cc_flw_initialize(), methinks. 20030527 mortene.
  SoFontLib::createFont(SbName("defaultFont"), SbName(""), SbVec2s(12, 12));
}

void
SoFontLib::exit(void)
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
  SoFontLibP::openfonts->makePList(keys, values);
  for (i = 0; i < values.getLength(); i++) {
    delete (SbString*) values[i];
  }
  CC_MUTEX_UNLOCK(SoFontLibP::apimutex);
  if (SoFontLibP::apimutex != NULL)
    CC_MUTEX_DESTRUCT(SoFontLibP::apimutex);

  delete SoFontLibP::openfonts;
  SoFontLibP::openfonts = NULL;
}

int
SoFontLib::createFont(const SbName & fontname, const SbName & stylename,
                      const SbVec2s & size)
{
  CC_MUTEX_LOCK(SoFontLibP::apimutex);
  SbString path, *strptr;
  int fileidx, i, listlen;
  const char * tmp;
  SbBool done;
  SbStringList emptylist;
  fileidx = -1;
  int font = -1;

  // Check if we already know the requestname for this fontname
  //
  // FIXME: this seems like an unnecessary optimization -- remove it
  // to simplify code? 20030527 mortene.
  if (SoFontLibP::openfonts->find((unsigned long)fontname.getString(), (void *&)strptr)) {
    path = *strptr;
    font = cc_flw_create_font(path.getString(), size[0], size[1]);
  } 
  else {
    // Check if we know the font file for this font name

    // FIXME: use a dict for this to clean up the code. 20030527 mortene.

    i = 0;
    listlen = SoFontLibP::fontfiles->getLength();
    done = FALSE;
    while (i < listlen) {
      tmp = (*SoFontLibP::fontfiles)[i]->getString();
      if (tmp[0] == ' ') {  // Font name, check if it matches fontname parameter
        if (!strcmp(tmp+1, fontname.getString())) {
          while (!done) {
            i++;
            tmp = (*SoFontLibP::fontfiles)[i]->getString();
            if (tmp[0] != ' ') {   // file name, check if file can be found
              path = SoInput::searchForFile(*(*SoFontLibP::fontfiles)[i], SoInput::getDirectories(), emptylist);
              if (path.getLength() > 0) {
                fileidx = i;
                done = TRUE;
              }
            } else {  // Next font name, means font file was not found
              done = TRUE;
            }
          }
          i = listlen;
        }
      }
      i++;
    }
    
    if (fileidx < 0) {  // Unknown font name, treat it as a font file name
      path = SoInput::searchForFile(fontname.getString(), SoInput::getDirectories(), emptylist);
    }
    
#if COIN_DEBUG
    if (cc_flw_debug()) {
      SoDebugError::postInfo("SoFontLib::createFont",
                             "font '%s' maps to font filename '%s'", 
                             fontname.getString(), 
                             path.getLength() > 0 ?
                             path.getString() :
                             "<defaultFont> (will use fontname as-is)");
    }
#endif // COIN_DEBUG

    // File not to be found anywhere, use fontname as is (and get
    // default font).
    if (path.getLength() == 0) { path = fontname; }

    font = cc_flw_create_font(path.getString(), size[0], size[1]);

    // 'font' should _always_ be >= 0, due to our fall-back on the
    // default font.
    assert((font >= 0) && "could not create font");

#if COIN_DEBUG
    if (cc_flw_debug()) {
      const char * createdfontname = cc_flw_get_font_name(font);
      SoDebugError::postInfo("SoFontLib::createFont",
                             "attempt at creating font from '%s', "
                             "actual font name is '%s'",
                             path.getString(), createdfontname);
    }
#endif // COIN_DEBUG

    // Add font to openfonts cache dict.
    SbString * newfont = new SbString(path);
    SoFontLibP::openfonts->enter((unsigned long)fontname.getString(), newfont);
  }
  CC_MUTEX_UNLOCK(SoFontLibP::apimutex);
  SoFontLib::setDefaultCharmap(font);
  return font;
}

int
SoFontLib::getFont(const SbName &fontname, const SbVec2s &size)
{
  CC_MUTEX_LOCK(SoFontLibP::apimutex);
  void * val;
  int font = -1;
  if (SoFontLibP::openfonts->find((unsigned long)fontname.getString(), val)) {
    SbString * reqname = (SbString *)val;
    font = cc_flw_get_font(reqname->getString(), (int)size[0], (int)size[1]);
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
  const char * name = "unknown";
  if (font >= 0) {
    name = cc_flw_get_charmap_name(font, charmap);
  }

  SbString retval(name);
  CC_MUTEX_UNLOCK(SoFontLibP::apimutex);
  return retval;
}

void
SoFontLib::setCharmap(const int font, const int charmap)
{
  CC_MUTEX_LOCK(SoFontLibP::apimutex);
  assert(font >= 0);
  cc_flw_set_charmap(font, charmap);
  CC_MUTEX_UNLOCK(SoFontLibP::apimutex);
}

void
SoFontLib::setDefaultCharmap(const int font)
{
  assert(font >= 0);
  CC_MUTEX_LOCK(SoFontLibP::apimutex);
  const char * str;
  const char * name;
  int defidx = -1;
  int preference = 0;
  char * charmap = "unknown";
  name = cc_flw_get_font_name(font);
  int cnt = cc_flw_get_num_charmaps(font);
#if COIN_DEBUG
  if (cc_flw_debug()) {
    SoDebugError::postInfo("SoFontLib::setDefaultCharmap", "font '%s' has %d charmaps", 
                           name, cnt);
  }
#endif
  for (int i=0; i<cnt; i++) {
    str = cc_flw_get_charmap_name(font, i);
#if COIN_DEBUG
    if (cc_flw_debug()) {
      SoDebugError::postInfo("SoFontLib::setDefaultCharmap", "charmap %d is '%s'", 
                             i, str ? str : "<undefined>");
    }
#endif
    if (str) {
      // We prefer latin_1, but as a fallback unicode will get most characters right.
      if (preference < 2 && !strcmp(str, "latin_1")) {
        defidx = i;
        charmap = "latin_1";
        preference = 2;
      } else if (preference < 1 && !strcmp(str, "unicode")) {
        defidx = i;
        charmap = "unicode";
        preference = 1;
      }
    }
  }
  if (defidx >= 0) {
    cc_flw_set_charmap(font, defidx);
#if COIN_DEBUG
    if (cc_flw_debug()) {
      SoDebugError::postInfo("SoFontLib::setDefaultCharmap", "setting charmap '%s' for font '%s'", 
                             charmap, name);
    }
#endif
  }
  else {
#if COIN_DEBUG
    if (cc_flw_debug()) {
      SoDebugError::postWarning("SoFontLib::setDefaultCharmap", "no default charmap for font '%s'", 
                                name);
    }
#endif
  }
  CC_MUTEX_UNLOCK(SoFontLibP::apimutex);
}

void
SoFontLib::setCharSize(const int font, const SbVec2s &size)
{
  CC_MUTEX_LOCK(SoFontLibP::apimutex);
  assert(font >= 0);
  cc_flw_set_char_size(font, (int)size[0], (int)size[1]);
  CC_MUTEX_UNLOCK(SoFontLibP::apimutex);
}

void
SoFontLib::setFontRotation(const int font, const float angle)
{
  CC_MUTEX_LOCK(SoFontLibP::apimutex);
  assert(font >= 0);
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
    struct cc_flw_bitmap * bm = cc_flw_get_bitmap(font, glyph);
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

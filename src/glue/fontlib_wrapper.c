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

/* FIXME: Get rid of (typecast)0 hack. preng 20030225 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <stddef.h>

#include <Inventor/C/glue/fontlib_wrapper.h>
#include <Inventor/C/tidbits.h>
#include <Inventor/C/errors/debugerror.h>
#include <Inventor/C/base/string.h>
#include "../misc/defaultfonts.h"

#ifdef HAVE_FREETYPE
#include <Inventor/C/glue/flwfreetype.h>
#endif

struct cc_glyphstruct {
  cc_FLWglyph glyph;
  cc_FLWbitmap * bitmap;
  int defaultglyph;
};

struct cc_fontstruct {
  void * font;
  cc_string * fontname;
  cc_string * requestname;
  struct cc_glyphstruct * glyphs;
  int glyphcnt;
  int glyphmax;
  int sizex, sizey;
  int defaultfont;
};

static struct cc_fontstruct **fonts;
static int fontcnt;
static int fontmax = 0;
static SbBool wrapper_initialized = FALSE;
  
#define NOGLYPH -1

/*
  BEGIN Internal functions
*/

static cc_FLWbitmap *
get_default_bitmap(unsigned int character)
{
  cc_FLWbitmap * bm;
  if (character < 256) {
    const int fontsize = coin_default2dfont_get_size();
    const int * isomapping = coin_default2dfont_get_isolatin1_mapping();
    const unsigned char * fontdata = coin_default2dfont_get_data();

    bm = (cc_FLWbitmap *)malloc(sizeof(cc_FLWbitmap));
    bm->buffer = (unsigned char *) fontdata + 
      fontsize * isomapping[character];
    bm->bearingX = 0;
    bm->bearingY = fontsize;
    bm->rows = fontsize;
    bm->width = 8;
    bm->pitch = 1;
    return bm;
  }
  return NULL;
}

static void
glyphstruct_init(struct cc_glyphstruct * gs)
{
  gs->glyph = (cc_FLWglyph)NOGLYPH;
  gs->bitmap = (cc_FLWbitmap *)0;
  gs->defaultglyph = 0;
}

static struct cc_fontstruct *
fontstruct_new(void * font)
{
  int i;
  struct cc_fontstruct * fs;
  fs = (struct cc_fontstruct *)malloc(sizeof(struct cc_fontstruct));
  fs->font = font;
  fs->fontname = NULL;
  fs->requestname = NULL;
  fs->glyphcnt = 0;
  fs->glyphmax = 0;
  fs->sizex = 0;
  fs->sizey = 0;
  fs->defaultfont = 0;
  fs->glyphs = (struct cc_glyphstruct *)malloc(10*sizeof(struct cc_glyphstruct));
  fs->glyphmax = 10;
  for (i=0; i<fs->glyphmax; i++)
    glyphstruct_init(&fs->glyphs[i]);
  return fs;
}

static void
fontstruct_set_fontname(struct cc_fontstruct * fs, const char * newname)
{
  assert (fs && newname);
  if (!fs->fontname)
    fs->fontname = cc_string_construct_new();
  cc_string_set_text(fs->fontname, newname);
}

static void
fontstruct_set_requestname(struct cc_fontstruct * fs, const char * newname)
{
  assert (fs && newname);
  if (!fs->requestname)
    fs->requestname = cc_string_construct_new();
  cc_string_set_text(fs->requestname, newname);
}

static void
fontstruct_set_size(struct cc_fontstruct * fs, const int x, const int y)
{
  assert (fs);
  fs->sizex = x;
  fs->sizey = y;
}

static void
fontstruct_expand()
{
  int i;
  struct cc_fontstruct **newfonts;
  assert (fonts);
  newfonts = (struct cc_fontstruct **)malloc(sizeof(struct cc_fontstruct*) * 2 * fontmax);
  for (i=0; i<fontmax; i++)
    newfonts[i] = fonts[i];
  for (i=fontmax; i<fontmax*2; i++)
    newfonts[i] = (struct cc_fontstruct *)0;
  free(fonts);
  fonts = newfonts;
  fontmax *= 2;
}

static int
fontstruct_insert(struct cc_fontstruct * fs)
{
  int i;
  assert (fs && fonts);
  /* Check for (and reuse) empty slots */
  for (i=0; i<fontmax; i++) {
    if (!fonts[i]) {
      fonts[i] = fs;
      if (i>=fontcnt)
        fontcnt = i+1;
      return i;
    }
  }
  /* Expand fonts array and use next slot */
  fontstruct_expand();
  fonts[fontcnt++] = fs;
  return fontcnt-1;
}

static void
fontstruct_rmglyph(struct cc_fontstruct * fs, int glyph)
{
  assert (fs && glyph<fs->glyphcnt && fs->glyphs[glyph].glyph != NOGLYPH);
  fs->glyphs[glyph].glyph = NOGLYPH;
  cc_flw_done_bitmap(fs->glyphs[glyph].bitmap);
  fs->glyphs[glyph].bitmap = NULL;
}

static void
fontstruct_rmfont(int font)
{
  int i;
  assert (font<fontcnt && fonts[font]);
  if (fonts[font]->fontname)
    cc_string_destruct(fonts[font]->fontname);
  if (fonts[font]->requestname)
    cc_string_destruct(fonts[font]->requestname);
  for (i=0; i<fonts[font]->glyphcnt; i++) {
    fontstruct_rmglyph(fonts[font], i);
  }
  free( fonts[font]->glyphs );
  free( fonts[font] );
  fonts[font] = NULL;
}

static void
fontstruct_cleanup()
{
  int i;
  assert (fonts);
  for (i=0; i<fontcnt; i++)
    fontstruct_rmfont(i);
  free(fonts);
}

static void
fontstruct_dumpfont(struct cc_fontstruct * fs)
{
  assert (fs);
  if (cc_fontlib_debug()) {
    cc_debugerror_postinfo("fontstruct_dumpfont", "  Font %s / %s :\n", cc_string_get_text(fs->fontname), cc_string_get_text(fs->requestname));
    cc_debugerror_postinfo("fontstruct_dumpfont", "    glyphcnt %d\n", fs->glyphcnt);
    cc_debugerror_postinfo("fontstruct_dumpfont", "    %s\n", fs->defaultfont ? "default font" : "regular font");
    cc_debugerror_postinfo("fontstruct_dumpfont", "    size (%d, %d)\n", fs->sizex, fs->sizey);
  }
}

static void
fontstruct_dump()
{
  int i;
  assert (fonts); 
  if (cc_fontlib_debug()) {
    cc_debugerror_postinfo("fontstruct_dump", "Fontlib cache: %d fonts.\n", fontcnt);
    for (i=0; i<fontcnt; i++)
      fontstruct_dumpfont(fonts[i]);
  }
}

static void
fontstruct_expand_glyphs(struct cc_fontstruct * fs)
{
  int i;
  struct cc_glyphstruct * newglyphs;
  assert (fs && fs->glyphs);
  newglyphs = (struct cc_glyphstruct *)malloc(sizeof(struct cc_glyphstruct) * 2 * fs->glyphmax);
  for (i=0; i<fs->glyphmax; i++)
    newglyphs[i] = fs->glyphs[i];
  for (i=fs->glyphmax; i<fs->glyphmax*2; i++)
    glyphstruct_init(&newglyphs[i]);
  free(fs->glyphs);
  fs->glyphs = newglyphs;
  fs->glyphmax *= 2;
}

static int
fontstruct_insert_glyph(int font, cc_FLWglyph glyph, int defaultglyph)
{
  int i;
  struct cc_fontstruct * fs;
  assert (font<fontcnt && fonts && fonts[font]);
  fs = fonts[font];
  /* Check for (and reuse) empty slots */
  for (i=0; i<fs->glyphmax; i++) {
    if (fs->glyphs[i].glyph == NOGLYPH) {
      fs->glyphs[i].glyph = glyph;
      if (i>=fs->glyphcnt)
        fs->glyphcnt = i+1;
      fs->glyphs[i].defaultglyph = defaultglyph;
      return i;
    }
  }
  /* Expand glyphs array and use next slot */
  fontstruct_expand_glyphs(fs);
  fs->glyphs[fs->glyphcnt].glyph = glyph;
  fs->glyphs[fs->glyphcnt].defaultglyph = defaultglyph;
  return fs->glyphcnt++;
}


/*
  END Internal functions
*/


SbBool
cc_fontlib_debug(void)
{
  const char * env = coin_getenv("COIN_DEBUG_FONTSUPPORT");
  return env && (atoi(env) > 0);
}

void
cc_flw_initialize(void)
{
  int i;

  assert(wrapper_initialized == FALSE && "init only once");
  wrapper_initialized = TRUE;

  fontcnt = 0;
  fontmax = 0;
  fonts = (struct cc_fontstruct**)malloc(10*sizeof(struct cc_fontstruct *));
  fontmax = 10;
  for (i=0; i<fontmax; i++)
    fonts[i] = NULL;

#ifdef HAVE_FREETYPE
  if (cc_fontlib_debug()) { cc_debugerror_postinfo("cc_flw_initialize", "using FreeType library"); }

  /* FIXME: return value just ignored. Should be handled (by disabling
     all use of FreeType library if init fails). 20030316 mortene. */
  (void)cc_flwft_initialize();
#endif
}

void
cc_flw_exit()
{
#ifdef HAVE_FREETYPE
  cc_flwft_exit();
#endif
  fontstruct_cleanup();
}

/*!
  Create one font. \a fontname is the name & style \e requested, \a
  outname will hold the name of the font actually created - these will
  typically be different.  If a font has already been created for this
  \a fontname, the function will not create a duplicate, but simply
  return that font.
*/
int
cc_flw_create_font(const char * fontname, const int sizex, const int sizey)
{
  struct cc_fontstruct * fs;
  void * font;
  int i, idx;
  cc_string * createdfontname;
  
  /* Don't create font if one has already been created for this
     requestname and size. */
  i = cc_flw_get_font(fontname, sizex, sizey);
  if (i != -1) { return i; }

  font = NULL;
  
#ifdef HAVE_FREETYPE
  font = cc_flwft_get_font(fontname);
#endif

  if (font) {
    fs = fontstruct_new(font);
    fontstruct_set_requestname(fs, fontname);
    fontstruct_set_size(fs, sizex, sizey);
#ifdef HAVE_FREETYPE
    cc_flwft_set_char_size(font, sizex, sizey);
#endif
#ifdef HAVE_FREETYPE
    createdfontname = cc_flwft_get_font_name(font);
    fontstruct_set_fontname(fs, cc_string_get_text(createdfontname));
    cc_string_destruct(createdfontname);
    cc_flwft_set_char_size(font, sizex, sizey);
#else
    fontstruct_set_fontname(fs, "unknown");
#endif
    idx = fontstruct_insert(fs);

    if (cc_fontlib_debug()) {
      cc_debugerror_postinfo("cc_flw_create_font",
                             "'%s', size==<%d, %d> => idx==%d %s",
                             fontname, sizex, sizey, idx,
                             (idx == -1) ? "" : (fonts[idx]->defaultfont ? "(defaultfont)" : "(not defaultfont)"));
    }
    
    return idx;
  }
  else {
    /* Use the default font for the given fontname and size. */
    /* FIXME: the defaultfont size is now fixed at 12-pt. Should scale
       it to match sizey. 20030317 mortene. */
    fs = fontstruct_new(NULL);
    fontstruct_set_requestname(fs, fontname);
    fontstruct_set_size(fs, 0, 0);
    fontstruct_set_fontname(fs, "defaultFont");
    fs->defaultfont = 1;
    return fontstruct_insert(fs);
  }
}

/*!
  Returns internal index of given font specification, for subsequent
  use as input argument to other functions as identification.

  Returns -1 if no font with the given name and size has been made
  yet.
*/
int
cc_flw_get_font(const char * fontname, const int sizex, const int sizey)
{
  int i;
  for (i=0; i< fontcnt; i++) {
    if ((fonts[i]->defaultfont ||
         (fonts[i]->sizex == sizex && fonts[i]->sizey == sizey)) &&
        !strcmp(fontname, cc_string_get_text(fonts[i]->requestname)))
      break;
  }

  if (i == fontcnt) { i = -1; }

  if (cc_fontlib_debug()) {
    cc_debugerror_postinfo("cc_flw_get_font",
                           "'%s', size==<%d, %d> => idx==%d %s",
                           fontname, sizex, sizey, i,
                           (i == -1) ? "" : (fonts[i]->defaultfont ? "(defaultfont)" : "(not defaultfont)"));
  }

  return i;
}

void
cc_flw_done_font(int font)
{
  assert (font >= 0 && font < fontcnt && fonts[font]);
#ifdef HAVE_FREETYPE
  cc_flwft_done_font(fonts[font]->font);
#endif
  fontstruct_rmfont(font);
}

int
cc_flw_get_num_charmaps(int font)
{
  assert (font >= 0 && font < fontcnt && fonts[font]);
#ifdef HAVE_FREETYPE
  return cc_flwft_get_num_charmaps(fonts[font]->font);
#else
  return 0;
#endif
}

cc_string *
cc_flw_get_charmap_name(int font, int charmap)
{
  assert (font >= 0 && font < fontcnt && fonts[font]);
#ifdef HAVE_FREETYPE
  return cc_flwft_get_charmap_name(fonts[font]->font, charmap);
#else
  return NULL;
#endif
}


cc_string *
cc_flw_get_font_name(int font)
{
  assert (font >= 0 && fonts[font]);
  return fonts[font]->fontname;
}

int
cc_flw_set_charmap(int font, int charmap)
{
  assert (font >= 0 && font < fontcnt && fonts[font]);
#ifdef HAVE_FREETYPE
  return cc_flwft_set_charmap(fonts[font]->font, charmap);
#else
  return -1;
#endif
}

int
cc_flw_set_char_size(int font, int width, int height)
{
  assert (font >= 0 && font < fontcnt && fonts[font]);
  fonts[font]->sizex = width;
  fonts[font]->sizey = height;
#ifdef HAVE_FREETYPE
  return cc_flwft_set_char_size(fonts[font]->font, width, height);
#else
  return -1;
#endif
}

int
cc_flw_set_font_rotation(int font, float angle)
{
  assert (font >= 0 && font < fontcnt && fonts[font]);
#ifdef HAVE_FREETYPE
  return cc_flwft_set_font_rotation(fonts[font]->font, angle);
#else
  return -1;
#endif
}

int
cc_flw_get_glyph(int font, int charidx)
{
  cc_FLWglyph glyph;

  assert(font >= 0 && font < fontcnt && fonts[font]);
  
  glyph = 0;
  
  if ( !fonts[font]->defaultfont ) {
#ifdef HAVE_FREETYPE
    glyph = cc_flwft_get_glyph(fonts[font]->font, charidx);
#endif
    
    if (glyph > 0) {
      return fontstruct_insert_glyph(font, glyph, 0);
    }
    else {
      /* Create glyph from default font, mark as default. */
      
      /* FIXME: shouldn't it rather be handled by making an empty
         rectangle glyph of the correct size, like it's at least done
         for X11 (and probably other systems aswell)?
         
         Or perhaps this strategy _is_ better, but then we should at
         least scale the defaultfont glyph to the correct size.
         20030317 mortene. */
      
      if (cc_fontlib_debug()) {
        cc_debugerror_postwarning("cc_flw_get_glyph",
                                  "no character 0x%x was found in font '%s'",
                                  charidx, fonts[font]->fontname);
      }
    }
  }
  return fontstruct_insert_glyph(font, charidx, 1);
}

int
cc_flw_get_advance(int font, int glyph, float *x, float *y)
{
  struct cc_fontstruct * fs;
  assert (font >= 0 && font < fontcnt && fonts[font]);
  fs = fonts[font];
  if (fs->defaultfont || fs->glyphs[glyph].defaultglyph) {
    *x = 8;
    *y = 0;
    return 0;
  } else if (glyph<fs->glyphcnt && fs->glyphs[glyph].glyph != NOGLYPH) {
#ifdef HAVE_FREETYPE
    return cc_flwft_get_advance(fs->font, fs->glyphs[glyph].glyph, x, y);
#endif
  }
  return -1;
}

int
cc_flw_get_kerning(int font, int glyph1, int glyph2, float *x, float *y)
{
  struct cc_fontstruct * fs;
  assert (font >= 0 && font < fontcnt && fonts[font]);
  fs = fonts[font];
  if (fs->defaultfont || fs->glyphs[glyph1].defaultglyph || fs->glyphs[glyph2].defaultglyph) {
    *x = 0;
    *y = 0;
    return 0;
  } else if (glyph1<fs->glyphcnt && fs->glyphs[glyph1].glyph != NOGLYPH
             && glyph2<fs->glyphcnt && fs->glyphs[glyph2].glyph != NOGLYPH) {
#ifdef HAVE_FREETYPE
    return cc_flwft_get_kerning(fs->font, fs->glyphs[glyph1].glyph, fs->glyphs[glyph2].glyph, x, y);
#endif
  }
  return -1;
}

void
cc_flw_done_glyph(int font, int glyph)
{
  struct cc_fontstruct * fs;
  assert (font >= 0 && font < fontcnt && fonts[font]);
  fs = fonts[font];
  if (glyph<fs->glyphcnt && fs->glyphs[glyph].glyph != NOGLYPH) {
#ifdef HAVE_FREETYPE
    cc_flwft_done_glyph(fs->font, fs->glyphs[glyph].glyph);
#endif
    fontstruct_rmglyph(fs, glyph);
  }
}

cc_FLWbitmap *
cc_flw_get_bitmap(int font, int glyph)
{
  unsigned char * buf;
  struct cc_fontstruct * fs;
  cc_FLWbitmap * bm;
  int i, defaultglyph;
  defaultglyph = 0;
  bm = NULL;
  assert (font >= 0 && font < fontcnt && fonts[font]);
  fs = fonts[font];
  if (glyph<fs->glyphcnt && fs->glyphs[glyph].glyph != NOGLYPH) {
    if (fs->glyphs[glyph].bitmap)
      return fs->glyphs[glyph].bitmap;
#ifdef HAVE_FREETYPE
    if (!fs->defaultfont && !fs->glyphs[glyph].defaultglyph) {
      bm = cc_flwft_get_bitmap(fs->font, fs->glyphs[glyph].glyph);
    };
#endif
    if (!bm) {
      /* glyph handle == char value in default font. &255 to avoid index out of range. */
      bm = get_default_bitmap(fs->glyphs[glyph].glyph & 0xff);
      defaultglyph = 1;
    }
    if (bm) {
      if (!defaultglyph) {
        buf = (unsigned char *)malloc(bm->pitch * bm->rows);
        /* Copy & reverse buffer to OpenGL "up" direction. */
        for (i=0; i<(int)bm->rows; i++)
          memcpy(buf + i*bm->pitch, bm->buffer + (bm->rows-i-1)*bm->pitch, bm->pitch);
        free(bm->buffer);
        bm->buffer = buf;
      }
    }
    fonts[font]->glyphs[glyph].bitmap = bm;
    return bm;
  }
  return NULL;
}

int
cc_flw_get_outline(int font, int glyph)
{
  cc_debugerror_postinfo("cc_flw_get_outline","Function has not been implemented yet.\n");
  return -1;
}

void
cc_flw_done_bitmap(cc_FLWbitmap * bitmap)
{
  assert( bitmap);
  if (bitmap->buffer)
    free(bitmap->buffer);
  free(bitmap);
}

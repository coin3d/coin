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

// FIXME: Get rid of (typecast)0 hack. preng 20030225

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <Inventor/C/glue/fontlib_wrapper.h>
#include <Inventor/C/tidbits.h>
#include <Inventor/C/errors/debugerror.h>

#ifdef HAVE_FREETYPE
#include <Inventor/C/glue/flwfreetype.h>
#endif

struct glyphstruct {
  FLWglyph glyph;
  FLWbitmap * bitmap;
  int defaultglyph;
};

struct fontstruct {
  FLWfont font;
  char * fontname;
  char * requestname;
  struct glyphstruct * glyphs;
  int glyphcnt;
  int glyphmax;
  int sizex, sizey;
  int defaultfont;
};

static struct fontstruct **fonts;
static int fontcnt;
static int fontmax = 0;
  
  /* The default bitmap font, as a fallback. */
#include "../misc/default2dfont.ic"

#define NOGLYPH -1

/*
  BEGIN Internal functions
*/

static SbBool
cc_fontlib_debug(void)
{
  const char * env = coin_getenv("COIN_DEBUG_FONTSUPPORT");
  return env && (atoi(env) > 0);
}

FLWbitmap *
get_default_bitmap(unsigned int character)
{
  FLWbitmap * bm;
  if (character < 256) {
    bm = (FLWbitmap *)malloc(sizeof(FLWbitmap));
    if (bm) {
      bm->buffer = (unsigned char *)coin_default2dfont + 12 * coin_default2dfont_isolatin1_mapping[character];
      bm->bearingX = 0;
      bm->bearingY = 12;
      bm->rows = 12;
      bm->width = 8;
      bm->pitch = 1;
      return bm;
    }
  }
  return (FLWbitmap *)0;
}

void
glyphstruct_init(struct glyphstruct * gs)
{
  gs->glyph = (FLWglyph)NOGLYPH;
  gs->bitmap = (FLWbitmap *)0;
  gs->defaultglyph = 0;
}

struct fontstruct *
fontstruct_new(FLWfont font)
{
  int i;
  struct fontstruct * fs;
  fs = (struct fontstruct *)malloc(sizeof(struct fontstruct));
  if (!fs)
    return (struct fontstruct *)0;
  fs->font = font;
  fs->fontname = (char *)0;
  fs->requestname = (char *)0;
  fs->glyphcnt = 0;
  fs->glyphmax = 0;
  fs->sizex = 0;
  fs->sizey = 0;
  fs->defaultfont = 0;
  fs->glyphs = (struct glyphstruct *)malloc(10*sizeof(struct glyphstruct));
  if (fs->glyphs)
    fs->glyphmax = 10;
  for (i=0; i<fs->glyphmax; i++)
    glyphstruct_init(&fs->glyphs[i]);
  return fs;
}

int
fontstruct_set_fontname(struct fontstruct * fs, const char * newname)
{
  if (fs) {
    if (fs->fontname)
      free(fs->fontname);
    fs->fontname = (char *)malloc(strlen(newname) + 1);
    if (fs->fontname) {
      strcpy(fs->fontname, newname);
      return 0;
    }
  }
  return -1;
}

int
fontstruct_set_requestname(struct fontstruct * fs, const char * newname)
{
  if (fs) {
    if (fs->requestname)
      free(fs->requestname);
    fs->requestname = (char *)malloc(strlen(newname) + 1);
    if (fs->requestname) {
      strcpy(fs->requestname, newname);
      return 0;
    }
  }
  return -1;
}

int
fontstruct_set_size(struct fontstruct * fs, const int x, const int y)
{
  if (fs) {
    fs->sizex = x;
    fs->sizey = y;
    return 0;
  }
  return -1;
}

int
fontstruct_expand()
{
  int i;
  struct fontstruct **newfonts;
  if (fonts) {
    newfonts = (struct fontstruct **)malloc(sizeof(struct fontstruct*) * 2 * fontmax);
    if (newfonts) {
      for (i=0; i<fontmax; i++)
        newfonts[i] = fonts[i];
      for (i=fontmax; i<fontmax*2; i++)
        newfonts[i] = (struct fontstruct *)0;
      free(fonts);
      fonts = newfonts;
      fontmax *= 2;
      return 0;
    }
  }
  return -1;
}

int
fontstruct_insert(struct fontstruct * fs)
{
  int i;
  if (fs && fonts) {
    // Check for (and reuse) empty slots
    for (i=0; i<fontmax; i++)
      if (!fonts[i]) {
        fonts[i] = fs;
        if (i>=fontcnt)
          fontcnt = i+1;
        return i;
      }
    // Expand fonts array and use next slot
    if (fontstruct_expand() == 0) {
      fonts[fontcnt++] = fs;
      return fontcnt-1;
    }
  }
  return -1;
}

void
fontstruct_rmglyph(struct fontstruct * fs, int glyph)
{
  if (fs && glyph<fs->glyphcnt && fs->glyphs[glyph].glyph != NOGLYPH) {
    fs->glyphs[glyph].glyph = NOGLYPH;
    flwDoneBitmap(fs->glyphs[glyph].bitmap);
    fs->glyphs[glyph].bitmap = (FLWbitmap *)0;
  }
}

void
fontstruct_rmfont(int font)
{
  int i;
  if (font<fontcnt && fonts[font]) {
    if (fonts[font]->fontname)
      free(fonts[font]->fontname);
    if (fonts[font]->requestname)
      free(fonts[font]->requestname);
    for (i=0; i<fonts[font]->glyphcnt; i++) {
      fontstruct_rmglyph(fonts[font], i);
    }
    free( fonts[font]->glyphs );
    free( fonts[font] );
    fonts[font] = (struct fontstruct *)0;
  }
}

void
fontstruct_cleanup()
{
  int i;
  if (fonts) {
    for (i=0; i<fontcnt; i++)
      fontstruct_rmfont(i);
    free(fonts);
  }
}

void
fontstruct_dumpfont(struct fontstruct * fs)
{
  if (fs) {
    printf("  Font %s / %s :\n", fs->fontname, fs->requestname);
    printf("    glyphcnt %d\n", fs->glyphcnt);
    printf("    %s\n", fs->defaultfont ? "default font" : "regular font");
    printf("    size (%d, %d)\n", fs->sizex, fs->sizey);
  }
}

void
fontstruct_dump()
{
  int i;
  if (fonts) {
    printf("Fontlib cache: %d fonts.\n", fontcnt);
    for (i=0; i<fontcnt; i++)
      fontstruct_dumpfont(fonts[i]);
  }
}

int
fontstruct_expand_glyphs(struct fontstruct * fs)
{
  int i;
  struct glyphstruct * newglyphs;
  if (fs && fs->glyphs) {
    newglyphs = (struct glyphstruct *)malloc(sizeof(struct glyphstruct) * 2 * fs->glyphmax);
    if (newglyphs) {
      for (i=0; i<fs->glyphmax; i++)
        newglyphs[i] = fs->glyphs[i];
      for (i=fs->glyphmax; i<fs->glyphmax*2; i++)
        glyphstruct_init(&newglyphs[i]);
      free(fs->glyphs);
      fs->glyphs = newglyphs;
      fs->glyphmax *= 2;
      return 0;
    }
  }
  return -1;
}

int
fontstruct_insert_glyph(int font, FLWglyph glyph, int defaultglyph)
{
  int i;
  struct fontstruct * fs;
  if (font<fontcnt && fonts && fonts[font]) {
    fs = fonts[font];
    // Check for (and reuse) empty slots
    for (i=0; i<fs->glyphmax; i++)
      if (fs->glyphs[i].glyph == NOGLYPH) {
        fs->glyphs[i].glyph = glyph;
        if (i>=fs->glyphcnt)
          fs->glyphcnt = i+1;
        fs->glyphs[i].defaultglyph = defaultglyph;
        return i;
      }
    // Expand glyphs array and use next slot
    if (fontstruct_expand_glyphs(fs) == 0) {
      fs->glyphs[fs->glyphcnt].glyph = glyph;
      fs->glyphs[fs->glyphcnt].defaultglyph = defaultglyph;
      return fs->glyphcnt++;
    }
  }
  return -1;
}


/*
  END Internal functions
*/


int
flwInitialize()
{
  int i, result;
  char dummy[80];
  if (fontmax > 0)
    return -1;
  fontcnt = 0;
  fontmax = 0;
  fonts = (struct fontstruct**)malloc(10*sizeof(struct fontstruct *));
  if (!fonts)
    return -1;
  fontmax = 10;
  for (i=0; i<fontmax; i++)
    fonts[i] = (struct fontstruct *)0;
#ifdef HAVE_FREETYPE
  result = flwftInitialize();
#else
  result = 0;
#endif
  flwCreateFont("defaultFont", dummy, 80, 12, 12);
  return result;
}

void
flwExit()
{
#ifdef HAVE_FREETYPE
  flwftExit();
#endif
  fontstruct_cleanup();
}

/*
  Create one font. fontname is the name & style _requested_, outname will hold
  the name of the font actually created - these will typically be different.
  If a font has already been created for this fontname, the function will not
  create a duplicate, but simply return that font. If lookup, the method will
  look up whether the font already exists, and fail (return -1) if it doesn't.
*/
int
flwCreateFont(const char * fontname, char * outname, const int outnamelen, const int sizex, const int sizey)
{
  struct fontstruct * fs;
  char newname[300];
  FLWfont font;
  int i;

  // Don't create font if one has already been created for this requestname and size
  for (i=0; i<fontcnt; i++)
    if (fonts[i]->sizex == sizex && fonts[i]->sizey == sizey && !strcmp(fontname, fonts[i]->requestname)) {
      if ((int) strlen(fonts[i]->fontname) < outnamelen)
        sprintf(outname, fonts[i]->fontname);
      return i;
    }
#ifdef HAVE_FREETYPE
  font = flwftGetFont(fontname);
#else
  font = (FLWfont)0;
#endif
  if (font) {
    fs = fontstruct_new(font);
    if (!fs)
      return -1;
    fontstruct_set_requestname(fs, fontname);
    fontstruct_set_size(fs, sizex, sizey);
#ifdef HAVE_FREETYPE
    flwftGetFontName(font, newname, 300);
    flwftSetCharSize(font, sizex, sizey);
#else
    sprintf(newname, "unknown");
#endif
    fontstruct_set_fontname(fs, newname);
    if ((int) strlen(newname) < outnamelen)
      sprintf(outname, newname);
    return fontstruct_insert(fs);
  } else {
    // Return default font.
    fs = fontstruct_new((FLWfont)0);
    if (!fs)
      return -1;
    fontstruct_set_requestname(fs, fontname);
    fontstruct_set_size(fs, 0, 0);
    fontstruct_set_fontname(fs, "defaultFont");
    if ((int) strlen(fs->fontname) < outnamelen)
      sprintf(outname, fs->fontname);
    fs->defaultfont = 1;
    return fontstruct_insert(fs);
  }
}

int
flwGetFont(const char * fontname, const int sizex, const int sizey)
{
  int i;
  for (i=0; i< fontcnt; i++) {
    if ((fonts[i]->defaultfont ||
         (fonts[i]->sizex == sizex && fonts[i]->sizey == sizey)) &&
        !strcmp(fontname, fonts[i]->fontname))
      break;
  }

  if (i == fontcnt) { i = -1; }

  if (cc_fontlib_debug()) {
    cc_debugerror_postinfo("flwGetFont", "'%s', size==<%d, %d> => idx %d",
                           fontname, sizex, sizey, i);
  }

  return i;
}

void
flwDoneFont(int font)
{
  if (font >= 0 && font < fontcnt && fonts[font]) {
#ifdef HAVE_FREETYPE
    flwftDoneFont(fonts[font]->font);
#endif
  }
  fontstruct_rmfont(font);
}

int
flwGetNumCharmaps(int font)
{
  if (font >= 0 && font < fontcnt && fonts[font]) {
#ifdef HAVE_FREETYPE
    return flwftGetNumCharmaps(fonts[font]->font);
#endif
  }
  return 0;
}

int
flwGetCharmapName(int font, int charmap, char * buffer, int bufsize)
{
  if (font >= 0 && font < fontcnt && fonts[font]) {
#ifdef HAVE_FREETYPE
    return flwftGetCharmapName(fonts[font]->font, charmap, buffer, bufsize);
#endif
  }
  return -1;
}


int
flwGetFontName(int font, char * buffer, int bufsize)
{
  if (font >= 0 && fonts[font]) {
    if ((int)strlen(fonts[font]->fontname) < bufsize) {
      memcpy(buffer, fonts[font]->fontname, strlen(fonts[font]->fontname)+1);
      return 0;
    }
  }
  return -1;
}

int
flwSetCharmap(int font, int charmap)
{
  if (font >= 0 && font < fontcnt && fonts[font]) {
#ifdef HAVE_FREETYPE
    return flwftSetCharmap(fonts[font]->font, charmap);
#endif
  }
  return -1;
}

int
flwSetCharSize(int font, int width, int height)
{
  if (font >= 0 && font < fontcnt && fonts[font]) {
    fonts[font]->sizex = width;
    fonts[font]->sizey = height;
#ifdef HAVE_FREETYPE
    return flwftSetCharSize(fonts[font]->font, width, height);
#endif
  }
  return -1;
}

int
flwSetFontRotation(int font, float angle)
{
  if (font >= 0 && font < fontcnt && fonts[font]) {
#ifdef HAVE_FREETYPE
    return flwftSetFontRotation(fonts[font]->font, angle);
#endif
  }
  return -1;
}

int
flwGetGlyph(int font, int charidx)
{
  FLWglyph glyph;
  int idx;
  if (font >= 0 && font < fontcnt && fonts[font]) {
#ifdef HAVE_FREETYPE
    glyph = flwftGetGlyph(fonts[font]->font, charidx);
#else
    glyph = (FLWglyph)0;
#endif
    if (glyph)
      return fontstruct_insert_glyph(font, glyph, 0);
    else {
      // Create glyph from default font, mark as default.
      return fontstruct_insert_glyph(font, charidx, 1);
    }
  }
  return -1;
}

int
flwGetAdvance(int font, int glyph, float *x, float *y)
{
  struct fontstruct * fs;
  if (font >= 0 && font < fontcnt && fonts[font]) {
    fs = fonts[font];
    if (fs->defaultfont || fs->glyphs[glyph].defaultglyph) {
      *x = 8;
      *y = 0;
      return 0;
    } else if (glyph<fs->glyphcnt && fs->glyphs[glyph].glyph != NOGLYPH) {
#ifdef HAVE_FREETYPE
      return flwftGetAdvance(fs->font, fs->glyphs[glyph].glyph, x, y);
#endif
    }
  }
  return -1;
}

int
flwGetKerning(int font, int glyph1, int glyph2, float *x, float *y)
{
  struct fontstruct * fs;
  if (font >= 0 && font < fontcnt && fonts[font]) {
    fs = fonts[font];
    if (fs->defaultfont || fs->glyphs[glyph1].defaultglyph || fs->glyphs[glyph2].defaultglyph) {
      *x = 0;
      *y = 0;
      return 0;
    } else if (glyph1<fs->glyphcnt && fs->glyphs[glyph1].glyph != NOGLYPH)
      if (glyph2<fs->glyphcnt && fs->glyphs[glyph2].glyph != NOGLYPH) {
#ifdef HAVE_FREETYPE
        return flwftGetKerning(fs->font, fs->glyphs[glyph1].glyph, fs->glyphs[glyph2].glyph, x, y);
#endif
      }
  }
  return -1;
}

void
flwDoneGlyph(int font, int glyph)
{
  struct fontstruct * fs;
  if (font >= 0 && font < fontcnt && fonts[font]) {
    fs = fonts[font];
    if (glyph<fs->glyphcnt && fs->glyphs[glyph].glyph != NOGLYPH) {
#ifdef HAVE_FREETYPE
      flwftDoneGlyph(fs->font, fs->glyphs[glyph].glyph);
#endif
      fontstruct_rmglyph(fs, glyph);
    }
  }
}

FLWbitmap *
flwGetBitmap(int font, int glyph)
{
  unsigned char * buf;
  struct fontstruct * fs;
  FLWbitmap * bm;
  int i, defaultglyph;
  defaultglyph = 0;
  bm = (FLWbitmap *)0;
  if (font >= 0 && font < fontcnt && fonts[font]) {
    fs = fonts[font];
    if (glyph<fs->glyphcnt && fs->glyphs[glyph].glyph != NOGLYPH) {
      if (fs->glyphs[glyph].bitmap)
        return fs->glyphs[glyph].bitmap;
#ifdef HAVE_FREETYPE
      if (!fs->defaultfont && !fs->glyphs[glyph].defaultglyph) {
        bm = flwftGetBitmap(fs->font, fs->glyphs[glyph].glyph);
      };
#endif
      if (!bm) {
        // glyph handle == char value in default font. &255 to avoid index out of range.
        bm = get_default_bitmap(fs->glyphs[glyph].glyph & 0xff);
        defaultglyph = 1;
      }
      if (bm) {
        if (!defaultglyph) {
          buf = (unsigned char *)malloc(bm->pitch * bm->rows);
          if (!buf) {
            flwDoneBitmap(bm);
            return (FLWbitmap *)0;
          }
          // Copy & reverse buffer to OpenGL "up" direction.
          for (i=0; i<(int)bm->rows; i++)
            memcpy(buf + i*bm->pitch, bm->buffer + (bm->rows-i-1)*bm->pitch, bm->pitch);
          free(bm->buffer);
          bm->buffer = buf;
        }
      }
      fonts[font]->glyphs[glyph].bitmap = bm;
      return bm;
    }
  }
  return (FLWbitmap *)0;
}

int
flwGetOutline(int font, int glyph)
{
  fprintf(stderr,"flwGetOutline has not been implemented yet.\n");
  return -1;
}

void
flwDoneBitmap(FLWbitmap * bitmap)
{
  if( bitmap) {
    if (bitmap->buffer)
      free(bitmap->buffer);
    free(bitmap);
  }
}

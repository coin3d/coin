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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */

#include <stdlib.h>
#include <assert.h>
#include <Inventor/C/glue/flwfreetype.h>
#include <Inventor/C/glue/freetype.h>

/* ************************************************************************* */

/*
  Implementation note: no part of the code has to be reentrant, as the
  complete interface is protected from multiple threads accessing it
  at the same time by locking in the cc_flw_* functions (which should
  be the only callers).
*/

/* ************************************************************************* */

#if !defined(HAVE_FREETYPE) && !defined(FREETYPE_RUNTIME_LINKING)

/* Dummy versions of all functions. Only cc_flwft_initialize() will be
   used from generic wrapper. */

SbBool cc_flwft_initialize(void) { return FALSE; }
void cc_flwft_exit(void) { }

void * cc_flwft_get_font(const char * fontname) { assert(FALSE); return NULL; }
void cc_flwft_get_font_name(void * font, cc_string * str) { assert(FALSE); }
void cc_flwft_done_font(void * font) { assert(FALSE); }

int cc_flwft_get_num_charmaps(void * font) { assert(FALSE); return 0; }
const char * cc_flwft_get_charmap_name(void * font, int charmap) { assert(FALSE); return NULL; }
void cc_flwft_set_charmap(void * font, int charmap) { assert(FALSE); }

void cc_flwft_set_char_size(void * font, int width, int height) { assert(FALSE); }
void cc_flwft_set_font_rotation(void * font, float angle) { assert(FALSE); }
  
int cc_flwft_get_glyph(void * font, unsigned int charidx) { assert(FALSE); return 0; }
void cc_flwft_get_advance(void * font, int glyph, float *x, float *y) { assert(FALSE); }
void cc_flwft_get_kerning(void * font, int glyph1, int glyph2, float *x, float *y) { assert(FALSE); }
void cc_flwft_done_glyph(void * font, int glyph) { assert(FALSE); }
  
struct cc_flw_bitmap * cc_flwft_get_bitmap(void * font, int glyph) { assert(FALSE); return NULL; }

#else /* HAVE_FREETYPE || FREETYPE_RUNTIME_LINKING */

#include <string.h>
#include <math.h>
#include <stdio.h>
#include <stddef.h>

#include <sys/stat.h>
#if HAVE_UNISTD_H
#include <unistd.h>
#endif /* HAVE_UNISTD_H */
#if HAVE_SYS_TYPES_H
/* According to Coin user Ralf Corsepius, at least SunOS4 needs to
   include sys/types.h before netinet/in.h. There have also been a
   problem report for FreeBSD which seems to indicate that the same
   dependency exists on that platform aswell. */
#include <sys/types.h>
#endif /* HAVE_SYS_TYPES_H */

#include <Inventor/C/tidbits.h>
#include <Inventor/C/errors/debugerror.h>
#include <Inventor/C/base/string.h>
#include <Inventor/C/base/hash.h>
#include <Inventor/C/base/dynarray.h>
#include <Inventor/C/base/namemap.h>
#include <Inventor/C/glue/fontlib_wrapper.h>

/* ************************************************************************* */

/* FIXME: the following snippet duplicated from SoInput.cpp -- should
   collect common code. 20030604 mortene. */

/* This (POSIX-compliant) macro is missing from the Win32 API header
   files for MSVC++ 6.0. */
#ifndef S_ISDIR
/* The _S_IFDIR bitpattern is not in the POSIX standard, but MSVC++
   header files has it. */
 #ifdef _S_IFDIR
 #define S_ISDIR(s) (s & _S_IFDIR)
#else /* Ai. */
 #error Can neither find nor make an S_ISDIR macro to test stat structures.
#endif /* !_S_IFDIR */
#endif /* !S_ISDIR */

/* ************************************************************************* */

#ifndef FT_ENC_TAG
#define FT_ENC_TAG(value, a, b, c, d)         \
          value = (((FT_UInt32)(a) << 24) |  \
                    ((FT_UInt32)(b) << 16) |  \
                    ((FT_UInt32)(c) <<  8) |  \
                      (FT_UInt32)(d)        )

#endif /* FT_ENC_TAG */

enum Coin_FT_Encoding {
  FT_ENC_TAG(FT_ENCODING_NONE, 0, 0, 0, 0),
  
  FT_ENC_TAG(FT_ENCODING_MS_SYMBOL,  's', 'y', 'm', 'b'),
  FT_ENC_TAG(FT_ENCODING_UNICODE,    'u', 'n', 'i', 'c'),
  
  FT_ENC_TAG(FT_ENCODING_MS_SJIS,    's', 'j', 'i', 's'),
  FT_ENC_TAG(FT_ENCODING_MS_GB2312,  'g', 'b', ' ', ' '),
  FT_ENC_TAG(FT_ENCODING_MS_BIG5,    'b', 'i', 'g', '5'),
  FT_ENC_TAG(FT_ENCODING_MS_WANSUNG, 'w', 'a', 'n', 's'),
  FT_ENC_TAG(FT_ENCODING_MS_JOHAB,   'j', 'o', 'h', 'a'),

  FT_ENC_TAG(FT_ENCODING_ADOBE_STANDARD, 'A', 'D', 'O', 'B'),
  FT_ENC_TAG(FT_ENCODING_ADOBE_EXPERT,   'A', 'D', 'B', 'E'),
  FT_ENC_TAG(FT_ENCODING_ADOBE_CUSTOM,   'A', 'D', 'B', 'C'),
  FT_ENC_TAG(FT_ENCODING_ADOBE_LATIN_1,  'l', 'a', 't', '1'),
  
  FT_ENC_TAG(FT_ENCODING_LATIN_2, 'l', 'a', 't', '2'),
  
  FT_ENC_TAG(FT_ENCODING_APPLE_ROMAN, 'a', 'r', 'm', 'n')
  
};

/* ************************************************************************* */

static FT_Library library;

/* Built-in mappings from font names to font file names.

   First item is a generic font name, then comes a list of possible
   file names (in sorted order of priority), then a NULL pointer, then
   a new generic font name, etc.
*/
static const char * fontfilenames[] = {
  /* FIXME: different font _styles_ are just jumbled together below,
     and are not really supported. It will take some work to sort out
     that mess. 20030606 mortene. */

  /*
    Names of some TrueType font files on MS Windows installations.

    FIXME: should provide more information about this -- e.g. is this
    a complete set? Ask Preng why he wrote up exactly these
    files. Where do these file names come from anyway? Just from
    peeking in $WINDIR/Fonts/? 20030606 mortene.
  */
  "Arial", "arial.ttf", NULL,
  "Arial Bold", "arialbd.ttf", NULL,
  "Arial Bold Italic", "arialbi.ttf", NULL,
  "Arial Italic", "ariali.ttf", NULL,
  "Century Gothic", "gothic.ttf", NULL,
  "Century Gothic Bold", "gothicb.ttf", NULL,
  "Century Gothic Bold Italic", "gothicbi.ttf", NULL,
  "Century Gothic Italic", "gothici.ttf", NULL,
  "Courier", "cour.ttf", NULL,
  "Courier Bold", "courbd.ttf", NULL,
  "Courier Bold Italic", "courbi.ttf", NULL,
  "Courier Italic", "couri.ttf", NULL,
  "Simian", "simtoran.ttf", "simtgori.ttf", "simtchimp.ttf", NULL,
  "Times New Roman", "times.ttf", NULL,
  "Times New Roman Bold", "timesbd.ttf", NULL,
  "Times New Roman Bold Italic", "timesbi.ttf", NULL,
  "Times New Roman Italic", "timesi.ttf", NULL,
  "Verdana", "verdana.ttf", NULL,
  "Verdana Bold", "verdanab.ttf", NULL,
  "Verdana Bold Italic", "verdanaz.ttf", NULL,
  "Verdana Italic", "verdanai.ttf", NULL,
  "OpenSymbol", "opens___.ttf", NULL,
  "Small", "smalle.fon", NULL,

  /* These are the TrueType fonts installed from the Debian Linux
     "msttcorefonts" package, as of version 1.1.2. */
  "Andale Mono", "Andale_Mono.ttf", NULL,
  "Arial", "Arial.ttf", NULL,
  "Arial Black", "Arial_Black.ttf", NULL,
  "Arial Bold", "Arial_Bold.ttf", NULL,
  "Arial Bold Italic", "Arial_Bold_Italic.ttf", NULL,
  "Arial Italic", "Arial_Italic.ttf", NULL,
  "Comic Sans MS", "Comic_Sans_MS.ttf", NULL,
  "Comic Sans MS Bold", "Comic_Sans_MS_Bold.ttf", NULL,
  "Courier", "Courier_New.ttf", NULL,
  "Courier New", "Courier_New.ttf", NULL,
  "Courier New Bold", "Courier_New_Bold.ttf", NULL,
  "Courier New Bold Italic", "Courier_New_Bold_Italic.ttf", NULL,
  "Courier New Italic", "Courier_New_Italic.ttf", NULL,
  "Georgia", "Georgia.ttf", NULL,
  "Georgia Bold", "Georgia_Bold.ttf", NULL,
  "Georgia Bold Italic", "Georgia_Bold_Italic.ttf", NULL,
  "Georgia Italic", "Georgia_Italic.ttf", NULL,
  "Impact", "Impact.ttf", NULL,
  "Times", "Times_New_Roman.ttf", NULL,
  "Times New Roman", "Times_New_Roman.ttf", NULL,
  "Times New Roman Bold", "Times_New_Roman_Bold.ttf", NULL,
  "Times New Roman Bold Italic", "Times_New_Roman_Bold_Italic.ttf", NULL,
  "Times New Roman Italic", "Times_New_Roman_Italic.ttf", NULL,
  "Trebuchet MS", "Trebuchet_MS.ttf", NULL,
  "Trebuchet MS Bold", "Trebuchet_MS_Bold.ttf", NULL,
  "Trebuchet MS Bold Italic", "Trebuchet_MS_Bold_Italic.ttf", NULL,
  "Trebuchet MS Italic", "Trebuchet_MS_Italic.ttf", NULL,
  "Verdana", "Verdana.ttf", NULL,
  "Verdana Bold", "Verdana_Bold.ttf", NULL,
  "Verdana Bold Italic", "Verdana_Bold_Italic.ttf", NULL,
  "Verdana Italic", "Verdana_Italic.ttf", NULL,
  "Webdings", "Webdings.ttf", NULL
};

static cc_hash * fontname2filename = NULL;
static cc_dynarray * fontfiledirs = NULL;

/* ************************************************************************* */

SbBool
cc_flwft_initialize(void)
{
  FT_Error error;
  if (!cc_ftglue_available()) {
    return FALSE;
  }
  error = cc_ftglue_FT_Init_FreeType(&library);
  if (error) {
    if (cc_flw_debug()) cc_debugerror_post("cc_flwft_initialize", "error %d", error);
    library = NULL;
    return FALSE;
  }

  if (cc_flw_debug()) {
    FT_Int major, minor, patch;
    cc_ftglue_FT_Library_Version(library, &major, &minor, &patch);
    cc_debugerror_postinfo("cc_flwft_initialize",
                           "FreeType library version is %d.%d.%d",
                           major, minor, patch);
  }
  
  /* FIXME: test major and minor version to decide whether library is
     ok. peder, 2003-07-09
  */
  assert((fontname2filename == NULL) && "call cc_flwft_initialize only once!");

  /* Set up hash of font name to array of file name mappings. */
  fontname2filename = cc_hash_construct(50, 0.75);
  {
    unsigned int i = 0;
    while (i < sizeof(fontfilenames) / sizeof(fontfilenames[0])) {
      void * val;
      SbBool found, unused;
      cc_dynarray * array;
      unsigned long key = (unsigned long)cc_namemap_get_address(fontfilenames[i]);

      found = cc_hash_get(fontname2filename, key, &val);
      if (found) {
        array = (cc_dynarray *)val;
      }
      else {
        array = cc_dynarray_new();
        unused = cc_hash_put(fontname2filename, key, array);
        assert(unused);
      }

      while (fontfilenames[++i] != NULL) {
        cc_dynarray_append(array, (void *)fontfilenames[i]);
      }
      
      i++;
    }
  }

  /* Set up where to look for font files. */
  {
    const char * env;
    char * str;
    fontfiledirs = cc_dynarray_new();

    if ((env = coin_getenv("COIN_FONT_PATH")) != NULL) {
      str = strdup(env);
      assert(str);
      cc_dynarray_append(fontfiledirs, str);
    }

    /* FIXME: bad #ifdef, should ideally be a *run-time* check for
       MSWindows. 20030526 mortene. */
#ifdef _WIN32
    if ((env = coin_getenv("WINDIR")) != NULL) {
      cc_string fullpath;
      cc_string_construct(&fullpath);

      cc_string_set_text(&fullpath, env);
      cc_string_append_text(&fullpath, "/Fonts");

      str = strdup(cc_string_get_text(&fullpath));
      assert(str);
      cc_dynarray_append(fontfiledirs, str);

      cc_string_clean(&fullpath);
    }
#endif /* _WIN32 */

    /* Try current working directory aswell. */
    str = strdup("./");
    assert(str);
    cc_dynarray_append(fontfiledirs, str);
  }

  return TRUE;
}

static void
clean_fontmap_hash(unsigned long key, void * val, void * closure)
{
  cc_dynarray * array = (cc_dynarray *)val;
  cc_dynarray_destruct(array);
}

void
cc_flwft_exit(void)
{
  unsigned int i, n = cc_dynarray_length(fontfiledirs);
  for (i = 0; i < n; i++) {
    /* All string pointers should have been allocated with
       strdup(). */
    free(cc_dynarray_get(fontfiledirs, i));
  }
  cc_dynarray_destruct(fontfiledirs);

  cc_hash_apply(fontname2filename, clean_fontmap_hash, NULL);
  cc_hash_destruct(fontname2filename);

  cc_ftglue_FT_Done_FreeType(library);
}

static const char *
find_font_file(const char * fontname)
{
  struct stat buf;
  cc_string str;
  unsigned int i, j, n;
  cc_dynarray * possiblefilenames;
  void * val;
  const char * foundfile = NULL;
  SbBool found;
  unsigned long key;

  key = (unsigned long)cc_namemap_get_address(fontname);
  found = cc_hash_get(fontname2filename, key, &val);
  if (!found) {
    if (cc_flw_debug()) {
      cc_debugerror_postinfo("find_font_file",
                             "fontname '%s' not found in name hash",
                             fontname);
    }
    return NULL;
  }

  possiblefilenames = (cc_dynarray *)val;
  n = cc_dynarray_length(possiblefilenames);

  cc_string_construct(&str);
  for (i = 0; i < n; i++) {
    /* FIXME: the following code is generic code for finding a file in
       a list of directories. Should move this to a new C ADT
       "cc_file" (or "cc_dir"?). If done, should also wrap the SoInput
       functions which does the same around it. 20030604 mortene. */
    const unsigned int dirs = cc_dynarray_length(fontfiledirs);
    for (j = 0; j < dirs; j++) {
      SbBool found = FALSE;

      cc_string_set_text(&str, (const char *)cc_dynarray_get(fontfiledirs, j));
      cc_string_append_char(&str, '/');
      cc_string_append_text(&str, (const char *)cc_dynarray_get(possiblefilenames, i));

      found = (stat(cc_string_get_text(&str), &buf) == 0) && !S_ISDIR(buf.st_mode);
      if (cc_flw_debug()) {
        cc_debugerror_postinfo("find_font_file", "'%s' %s",
                               cc_string_get_text(&str),
                               found ? "found!" : "NOT found");
      }

      if (found) {
        /* Stor permanent in global name hash. */
        foundfile = cc_namemap_get_address(cc_string_get_text(&str));
        goto done;
      }
    }
  }

 done:
  cc_string_clean(&str);
  return foundfile;
}

void *
cc_flwft_get_font(const char * fontname)
{
  FT_Face face;
  const char * fontfilename = find_font_file(fontname);
  FT_Error error =
    cc_ftglue_FT_New_Face(library, fontfilename ? fontfilename : fontname, 0, &face);

  if (error) {
    if (cc_flw_debug()) {
      cc_debugerror_postwarning("cc_flwft_get_font",
                                "error %d for fontname '%s' (filename '%s')",
                                error, fontname,
                                fontfilename ? fontfilename : "(null)");
    }
    return NULL;
  }

  if (cc_flw_debug()) {
    cc_debugerror_postinfo("cc_flwft_get_font",
                           "FT_New_Face(..., \"%s\" / \"%s\", ...) => "
                           "family \"%s\" and style \"%s\"",
                           fontname, fontfilename ? fontfilename : "(null)",
                           face->family_name, face->style_name);
  }
  return face;
}

void
cc_flwft_get_font_name(void * font, cc_string * str)
{
  FT_Face face;
  assert(font);
  face = (FT_Face)font;
  cc_string_sprintf(str, "%s %s", face->family_name, face->style_name);
}

void
cc_flwft_done_font(void * font)
{
  FT_Error error;
  FT_Face face;
  assert(font);
  face = (FT_Face)font;
  error = cc_ftglue_FT_Done_Face(face);
  if (error) {
    if (cc_flw_debug()) cc_debugerror_postinfo("cc_flwft_done_font", "Error %d\n", error);
  }
}

int
cc_flwft_get_num_charmaps(void * font)
{
  assert(font);
  return ((FT_Face)font)->num_charmaps;
}

const char *
cc_flwft_get_charmap_name(void * font, int charmap)
{
  FT_Face face;
  const char * name = "unknown";
  assert(font);
  face = (FT_Face)font;

  if (charmap < face->num_charmaps) {
    switch (face->charmaps[charmap]->encoding) {
    case FT_ENCODING_UNICODE:	 
      name = "unicode"; break; 
    case FT_ENCODING_MS_SYMBOL: 
      name = "symbol"; break; 
    case FT_ENCODING_MS_SJIS:	 
      name = "sjis"; break; 
    case FT_ENCODING_MS_GB2312: 
      name = "gb2312"; break; 
    case FT_ENCODING_MS_BIG5:	 
      name = "big5"; break; 
    case FT_ENCODING_MS_WANSUNG:	 
      name = "wansung"; break; 
    case FT_ENCODING_MS_JOHAB:	 
      name = "johab"; break; 
    case FT_ENCODING_ADOBE_STANDARD: 
      name = "adobe_standard"; break; 
    case FT_ENCODING_ADOBE_EXPERT: 
      name = "adobe_expert"; break; 
    case FT_ENCODING_ADOBE_CUSTOM: 
      name = "adobe_custom"; break; 
    case FT_ENCODING_ADOBE_LATIN_1:	  
      name = "latin_1"; break;  
    case FT_ENCODING_APPLE_ROMAN: 
      name = "apple_roman"; break; 
    default:
      if (cc_flw_debug()) {
        cc_debugerror_postwarning("cc_flwft_get_charmap_name",
                                  "unknown encoding: 0x%x",
                                  face->charmaps[charmap]->encoding);
      }
      /* name will be set to unknown */
      break;
    }
  }

  return name;
}

void
cc_flwft_set_charmap(void * font, int charmap)
{
  FT_Error error;
  FT_Face face;
  assert(font);
  face = (FT_Face)font;
  if (charmap < face->num_charmaps) {
    error = cc_ftglue_FT_Select_Charmap(face, face->charmaps[charmap]->encoding);
    if (error) {
      cc_debugerror_post("cc_flwft_set_charmap",
                         "FT_Select_Charmap(.., %d) returned error %d",
                         charmap, error);
    }
  }
}

void
cc_flwft_set_char_size(void * font, int width, int height)
{
  FT_Error error;
  FT_Face face;
  assert(font);
  face = (FT_Face)font;
  /* FIXME: the input arguments for width and height looks
     bogus. Check against the FreeType API doc. 20030515 mortene. */
  width <<= 6;
  height <<= 6;
  error = cc_ftglue_FT_Set_Char_Size(face, width, height, 72, 72);
  if (error) {
    cc_debugerror_post("cc_flwft_set_char_size",
                       "FT_Set_Char_Size(.., %d, %d, ..) returned error code %d",
                       width, height, error);
  }
}

void
cc_flwft_set_font_rotation(void * font, float angle)
{
  FT_Matrix matrix;
  FT_Face face;
  assert(font);
  face = (FT_Face)font;
  matrix.xx = (FT_Fixed)(cos(angle)*0x10000);
  matrix.xy = (FT_Fixed)(-sin(angle)*0x10000);
  matrix.yx = (FT_Fixed)(sin(angle)*0x10000);
  matrix.yy = (FT_Fixed)(cos(angle)*0x10000);
  cc_ftglue_FT_Set_Transform(face, &matrix, 0);
}

/*
  Returns the glyph index. If the character code is undefined, returns
  0.
*/
int
cc_flwft_get_glyph(void * font, unsigned int charidx)
{
  FT_Face face;
  /* assert(font);  */
  face = (FT_Face)font;
  
  /* FIXME: check code paths & reenable assert. Comments follow */
  
  /* disabled 2003-03-17, pederb. Triggers too often for my taste. The
     documentation states that NULL should be returned if glyph is not
     found */
  
  /* update 20030317 mortene: the assert() isn't really too strict,
     it's the caller(s) that should avoid trying to get a glyph out of
     a non-existent font. Reenable assert when code paths are fixed. */
  
  if (face == NULL) { return 0; }

  return cc_ftglue_FT_Get_Char_Index(face, charidx);
}

void
cc_flwft_get_advance(void * font, int glyph, float *x, float *y)
{
  FT_Error error;
  FT_Face face;
  int tmp;
  assert(font);
  face = (FT_Face)font;
  error = cc_ftglue_FT_Load_Glyph(face, glyph, FT_LOAD_DEFAULT);
  assert(error == 0 && "FT_Load_Glyph() unexpected failure, investigate");

  tmp = face->glyph->advance.x;
  x[0] = tmp / (float)64.0;
  tmp = face->glyph->advance.y;
  y[0] = tmp / (float)64.0;
}

void
cc_flwft_get_kerning(void * font, int glyph1, int glyph2, float *x, float *y)
{
  FT_Error error;
  FT_Vector kerning;
  FT_Face face;
  assert(font);
  face = (FT_Face)font;
  if (FT_HAS_KERNING(face)) {
    error = cc_ftglue_FT_Get_Kerning(face, glyph1, glyph2, ft_kerning_default, &kerning);
    if (error) {
      cc_debugerror_post("cc_flwft_get_kerning", "FT_Get_Kerning() => %d", error);
    }
    *x = kerning.x / (float)64.0;
    *y = kerning.y / (float)64.0;
  }
  else {
    *x = 0.0;
    *y = 0.0;
  }
}

void
cc_flwft_done_glyph(void * font, int glyph)
{
  /* No action, since an int is just an index. */
}

struct cc_flw_bitmap *
cc_flwft_get_bitmap(void * font, int glyph)
{
  FT_Error error;
  struct cc_flw_bitmap * bm;
  FT_Face face;
  FT_Glyph g;
  FT_BitmapGlyph tfbmg;
  FT_Bitmap * tfbm;
  assert(font);
  
  face = (FT_Face)font;
  error = cc_ftglue_FT_Load_Glyph(face, glyph, FT_LOAD_DEFAULT);
  if (error) {
    if (cc_flw_debug()) cc_debugerror_post("cc_flwft_get_bitmap",
                                                "FT_Load_Glyph() => error %d",
                                                error);
    return NULL;
  }
  error = cc_ftglue_FT_Get_Glyph(face->glyph, &g);
  if (error) {
    if (cc_flw_debug()) cc_debugerror_post("cc_flwft_get_bitmap",
                                                "FT_Get_Glyph() => error %d",
                                                error);
    return NULL;
  }
  error = cc_ftglue_FT_Glyph_To_Bitmap(&g, ft_render_mode_mono, 0, 1);
  if (error) {
    if (cc_flw_debug()) cc_debugerror_post("cc_flwft_get_bitmap",
                                                "FT_Glyph_To_Bitmap() => error %d",
                                                error);
    return NULL;
  }
  tfbmg = (FT_BitmapGlyph)g;
  tfbm = &tfbmg->bitmap;
  /* FIXME: allocating the structure here is bad design. 20030528 mortene. */
  bm = (struct cc_flw_bitmap *)malloc(sizeof(struct cc_flw_bitmap));
  bm->buffer = (unsigned char *)malloc(tfbm->rows * tfbm->pitch);
  bm->bearingX = tfbmg->left;
  bm->bearingY = tfbmg->top;
  bm->rows = tfbm->rows;
  bm->width = tfbm->width;
  bm->pitch = tfbm->pitch;
  memcpy(bm->buffer, tfbm->buffer, tfbm->rows * tfbm->pitch);
  cc_ftglue_FT_Done_Glyph(g);
  return bm;
}

#endif /* HAVE_FREETYPE */

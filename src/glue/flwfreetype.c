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

#include <Inventor/C/glue/flwfreetype.h>
#include <stdlib.h>
#include <assert.h>

#ifndef HAVE_FREETYPE

// Dummy versions of all functions. Only cc_flwft_initialize() and
// cc_flwft_exit() will be used from generic wrapper.

SbBool cc_flwft_initialize(void) { return FALSE; }
void cc_flwft_exit(void) { }

void * cc_flwft_get_font(const char * fontname) { assert(FALSE); return NULL; }
cc_string * cc_flwft_get_font_name(void * font) { assert(FALSE); return NULL; }
cc_string * cc_flwft_get_font_style(void * font) { assert(FALSE); return NULL; }
void cc_flwft_done_font(void * font) { assert(FALSE); }

int cc_flwft_get_num_charmaps(void * font) { assert(FALSE); return 0; }
cc_string * cc_flwft_get_charmap_name(void * font, int charmap) { assert(FALSE); return NULL; }
int cc_flwft_set_charmap(void * font, int charmap) { assert(FALSE); return 0; }

int cc_flwft_set_char_size(void * font, int width, int height) { assert(FALSE); return 0; }
int cc_flwft_set_font_rotation(void * font, float angle) { assert(FALSE); return 0; }
  
cc_FLWglyph cc_flwft_get_glyph(void * font, unsigned int charidx) { assert(FALSE); return 0; }
int cc_flwft_get_advance(void * font, cc_FLWglyph glyph, float *x, float *y) { assert(FALSE); return 0; }
int cc_flwft_get_kerning(void * font, cc_FLWglyph glyph1, cc_FLWglyph glyph2, float *x, float *y) { assert(FALSE); return 0; }
void cc_flwft_done_glyph(void * font, cc_FLWglyph glyph) { assert(FALSE); }
  
cc_FLWbitmap * cc_flwft_get_bitmap(void * font, cc_FLWglyph glyph) { assert(FALSE); return NULL; }
int cc_flwft_get_outline(void * font, cc_FLWglyph glyph) { assert(FALSE); return 0; }

#else // HAVE_FREETYPE

#include <string.h>
#include <math.h>
#include <stdio.h>
#include <stddef.h>

#include <ft2build.h>
/* FIXME: FT build macros don't work for MSVC dsp builds. preng 2003-03-11
 * #include FT_FREETYPE_H
 * #include FT_GLYPH_H */
#include <freetype/freetype.h>
#include <freetype/ftglyph.h>

#include <Inventor/C/tidbits.h>
#include <Inventor/C/errors/debugerror.h>
#include <Inventor/C/base/string.h>

/* workaround for freetype < 2.1 */
#if FREETYPE_MAJOR == 2 && FREETYPE_MINOR < 1

#ifndef FT_ENC_TAG
#define FT_ENC_TAG( value, a, b, c, d )         \
          value = ( ( (FT_UInt32)(a) << 24 ) |  \
                    ( (FT_UInt32)(b) << 16 ) |  \
                    ( (FT_UInt32)(c) <<  8 ) |  \
                      (FT_UInt32)(d)         )

#endif /* FT_ENC_TAG */

enum Coin_FT_Encoding {
  FT_ENC_TAG( FT_ENCODING_NONE, 0, 0, 0, 0 ),
  
  FT_ENC_TAG( FT_ENCODING_MS_SYMBOL,  's', 'y', 'm', 'b' ),
  FT_ENC_TAG( FT_ENCODING_UNICODE,    'u', 'n', 'i', 'c' ),
  
  FT_ENC_TAG( FT_ENCODING_MS_SJIS,    's', 'j', 'i', 's' ),
  FT_ENC_TAG( FT_ENCODING_MS_GB2312,  'g', 'b', ' ', ' ' ),
  FT_ENC_TAG( FT_ENCODING_MS_BIG5,    'b', 'i', 'g', '5' ),
  FT_ENC_TAG( FT_ENCODING_MS_WANSUNG, 'w', 'a', 'n', 's' ),
  FT_ENC_TAG( FT_ENCODING_MS_JOHAB,   'j', 'o', 'h', 'a' ),

  FT_ENC_TAG( FT_ENCODING_ADOBE_STANDARD, 'A', 'D', 'O', 'B' ),
  FT_ENC_TAG( FT_ENCODING_ADOBE_EXPERT,   'A', 'D', 'B', 'E' ),
  FT_ENC_TAG( FT_ENCODING_ADOBE_CUSTOM,   'A', 'D', 'B', 'C' ),
  FT_ENC_TAG( FT_ENCODING_ADOBE_LATIN_1,  'l', 'a', 't', '1' ),
  
  FT_ENC_TAG( FT_ENCODING_LATIN_2, 'l', 'a', 't', '2' ),
  
  FT_ENC_TAG( FT_ENCODING_APPLE_ROMAN, 'a', 'r', 'm', 'n' )
  
};

#endif /* FREETYPE_VERSION < 2.1 */

static FT_Library library;

static SbBool
cc_freetype_debug(void)
{
  const char * env = coin_getenv("COIN_DEBUG_FREETYPE");
  return env && (atoi(env) > 0);
}

SbBool
cc_flwft_initialize(void)
{
  FT_Error error = FT_Init_FreeType(&library);
  if (error) {
    if (cc_freetype_debug()) cc_debugerror_post("cc_flwft_initialize", "error %d", error);
    library = NULL;
  }
  else if (cc_freetype_debug()) {
    FT_Int major, minor, patch;
    FT_Library_Version(library, &major, &minor, &patch);
    cc_debugerror_postinfo("cc_flwft_initialize",
                           "FreeType library version is %d.%d.%d",
                           major, minor, patch);
  }

  return error == 0 ? TRUE : FALSE;
}

void
cc_flwft_exit(void)
{
  FT_Done_FreeType(library);
}

void *
cc_flwft_get_font(const char * fontname)
{
  FT_Error error;
  FT_Face face;
  error = FT_New_Face(library, fontname, 0, &face);
  if (error) {
    if (cc_freetype_debug()) {
      cc_debugerror_postwarning("cc_flwft_get_font", "error %d for fontname '%s'",
                                error, fontname);
    }
    return NULL;
  }

  if (cc_freetype_debug()) {
    cc_debugerror_postinfo("cc_flwft_get_font",
                           "FT_New_Face(..., \"%s\", ...) => family \"%s\" and style \"%s\"",
                           fontname, face->family_name, face->style_name);
  }
  return face;
}

cc_string *
cc_flwft_get_font_name(void * font)
{
  FT_Face face;
  cc_string * name;
  assert (font);
  face = (FT_Face)font;
  name = cc_string_construct_new();
  cc_string_sprintf(name, "%s %s", face->family_name, face->style_name);
  return name;
}

cc_string *
cc_flwft_get_font_style(void * font)
{
  FT_Face face;
  cc_string * name;
  assert (font);
  face = (FT_Face)font;
  name = cc_string_construct_new();
  cc_string_set_text(name, face->style_name);
  return name;
}

void
cc_flwft_done_font(void * font)
{
  FT_Error error;
  FT_Face face;
  assert (font);
  face = (FT_Face)font;
  error = FT_Done_Face(face);
  if ( error ) {
    if (cc_freetype_debug()) cc_debugerror_postinfo("cc_flwft_done_font", "Error %d\n", error);
  }
}

int
cc_flwft_get_num_charmaps(void * font)
{
  assert (font);
  return ((FT_Face)font)->num_charmaps;
}

cc_string *
cc_flwft_get_charmap_name(void * font, int charmap)
{
  FT_Face face;
  char * name = "unknown";
  cc_string * charmapname;
  assert (font);
  face = (FT_Face)font;
  charmapname = cc_string_construct_new();
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
      if (cc_freetype_debug()) {
        cc_debugerror_postwarning("cc_flwft_get_charmap_name",
                                  "unknown encoding: 0x%x",
                                  face->charmaps[charmap]->encoding);
      }
      /* name will be set to unknown */
      break;
    }
  }
  cc_string_set_text(charmapname, name);
  return charmapname;
}

int
cc_flwft_set_charmap(void * font, int charmap)
{
  FT_Error error;
  FT_Face face;
  assert (font);
  face = (FT_Face)font;
  if ( charmap < face->num_charmaps) {
    error = FT_Select_Charmap(face, face->charmaps[charmap]->encoding);
    if (error)
      if (cc_freetype_debug()) cc_debugerror_postwarning("cc_flwft_set_charmap", "ERROR: set charmap %d returned error %d\n", charmap, error);
    return error;
  }
  return -1;
}

int
cc_flwft_set_char_size(void * font, int width, int height)
{
  FT_Error error;
  FT_Face face;
  assert (font);
  face = (FT_Face)font;
  error = FT_Set_Char_Size(face, width << 6, height << 6, 72, 72);
  if (error) {
    if (cc_freetype_debug()) cc_debugerror_postwarning("cc_flwft_set_char_size" ,"ERROR %d\n", error);
    return error;
  }
  return 0;
}

int
cc_flwft_set_font_rotation(void * font, float angle)
{
  FT_Matrix matrix;
  FT_Face face;
  assert (font);
  face = (FT_Face)font;
  matrix.xx = (FT_Fixed)( cos(angle)*0x10000);
  matrix.xy = (FT_Fixed)(-sin(angle)*0x10000);
  matrix.yx = (FT_Fixed)( sin(angle)*0x10000);
  matrix.yy = (FT_Fixed)( cos(angle)*0x10000);
  FT_Set_Transform(face, &matrix, 0);
  return 0;
}

/*
  Returns the glyph index. If the character code is undefined, returns
  0.
*/
cc_FLWglyph
cc_flwft_get_glyph(void * font, unsigned int charidx)
{
  FT_Face face;
  /* assert (font);  */
  face = (FT_Face)font;
  
  /* FIXME: check code paths & reenable assert. Comments follow */
  
  /* disabled 2003-03-17, pederb. Triggers too often for my taste. The
     documentation states that NULL should be returned if glyph is not
     found */
  
  /* update 20030317 mortene: the assert() isn't really too strict,
     it's the caller(s) that should avoid trying to get a glyph out of
     a non-existent font. Reenable assert when code paths are fixed. */
  
  if (face == NULL) { return 0; }

  return FT_Get_Char_Index(face, charidx);
}

int
cc_flwft_get_advance(void * font, cc_FLWglyph glyph, float *x, float *y)
{
  FT_Error error;
  FT_Face face;
  int tmp;
  assert (font);
  face = (FT_Face)font;
  error = FT_Load_Glyph(face, glyph, FT_LOAD_DEFAULT);
  if (error) {
    if (cc_freetype_debug()) cc_debugerror_postwarning("cc_flwft_get_advance", "ERROR %d\n", error);
    return error;
  }
  tmp = face->glyph->advance.x;
  x[0] = tmp / (float)64.0;
  tmp = face->glyph->advance.y;
  y[0] = tmp / (float)64.0;
  return 0;
}

int
cc_flwft_get_kerning(void * font, cc_FLWglyph glyph1, cc_FLWglyph glyph2, float *x, float *y)
{
  FT_Error error;
  FT_Vector kerning;
  FT_Face face;
  assert (font);
  face = (FT_Face)font;
  if ( FT_HAS_KERNING(face) ) {
    error = FT_Get_Kerning(face, glyph1, glyph2, ft_kerning_default, &kerning);
    if (error) {
      if (cc_freetype_debug()) cc_debugerror_postwarning("cc_flwft_get_kerning", "ERROR %d\n", error);
      return error;
    }
    *x = kerning.x / (float)64.0;
    *y = kerning.y / (float)64.0;
    return 0;
  } else {
    *x = 0.0;
    *y = 0.0;
    return 0;
  }
}

void
cc_flwft_done_glyph(void * font, cc_FLWglyph glyph)
{
  /* No action, since an cc_FLWglyph is just an index. */
}

cc_FLWbitmap *
cc_flwft_get_bitmap(void * font, cc_FLWglyph glyph)
{
  FT_Error error;
  cc_FLWbitmap * bm;
  FT_Face face;
  FT_Glyph g;
  FT_BitmapGlyph tfbmg;
  FT_Bitmap * tfbm;
  assert (font);
  
  face = (FT_Face)font;
  error = FT_Load_Glyph(face, glyph, FT_LOAD_DEFAULT);
  if (error) {
    if (cc_freetype_debug()) cc_debugerror_postwarning("cc_flwft_get_bitmap", "ERROR in FT_Load_Glyph %d\n", error);
    return NULL;
  }
  error = FT_Get_Glyph(face->glyph, &g);
  if (error) {
    if (cc_freetype_debug()) cc_debugerror_postwarning("cc_flwft_get_bitmap", "ERROR in FT_Get_Glyph %d\n", error);
    return NULL;
  }
  error = FT_Glyph_To_Bitmap(&g, ft_render_mode_mono, 0, 1);
  if (error) {
    if (cc_freetype_debug()) cc_debugerror_postwarning("cc_flwft_get_bitmap", "ERROR in FT_Glyph_To_Bitmap %d\n", error);
    return NULL;
  }
  tfbmg = (FT_BitmapGlyph)g;
  tfbm = &tfbmg->bitmap;
  bm = (cc_FLWbitmap *)malloc(sizeof(cc_FLWbitmap));
  bm->buffer = (unsigned char *)malloc(tfbm->rows * tfbm->pitch);
  bm->bearingX = tfbmg->left;
  bm->bearingY = tfbmg->top;
  bm->rows = tfbm->rows;
  bm->width = tfbm->width;
  bm->pitch = tfbm->pitch;
  memcpy(bm->buffer, tfbm->buffer, tfbm->rows * tfbm->pitch);
  FT_Done_Glyph(g);
  return bm;
}

int
cc_flwft_get_outline(void * font, cc_FLWglyph glyph)
{
  /* FIXME: implement. */
  if (cc_freetype_debug()) cc_debugerror_postinfo("cc_flwft_get_outline", "Function has not been implemented yet.\n");
  return 0;
}

#endif // HAVE_FREETYPE

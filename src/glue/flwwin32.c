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

#include <Inventor/C/glue/flwwin32.h>
#include <stdlib.h>
#include <assert.h>

/* FIXME: should be based on a configure check. 20030515 mortene. */
#ifdef _WIN32
#define HAVE_WIN32_API
#endif /* _WIN32 */

#ifndef HAVE_WIN32_API

/* Dummy versions of all functions. Only cc_flww32_initialize() will be
   used from generic wrapper. */

SbBool cc_flww32_initialize(void) { return FALSE; }
void cc_flww32_exit(void) { }

void * cc_flww32_get_font(const char * fontname, int sizex, int sizey) { assert(FALSE); return NULL; }
cc_string * cc_flww32_get_font_name(void * font) { assert(FALSE); return NULL; }
cc_string * cc_flww32_get_font_style(void * font) { assert(FALSE); return NULL; }
void cc_flww32_done_font(void * font) { assert(FALSE); }

int cc_flww32_get_num_charmaps(void * font) { assert(FALSE); return 0; }
const char * cc_flww32_get_charmap_name(void * font, int charmap) { assert(FALSE); return NULL; }
void cc_flww32_set_charmap(void * font, int charmap) { assert(FALSE); }

void cc_flww32_set_char_size(void * font, int width, int height) { assert(FALSE); }
void cc_flww32_set_font_rotation(void * font, float angle) { assert(FALSE); }
  
int cc_flww32_get_glyph(void * font, unsigned int charidx) { assert(FALSE); return 0; }
int cc_flww32_get_advance(void * font, int glyph, float *x, float *y) { assert(FALSE); return 0; }
int cc_flww32_get_kerning(void * font, int glyph1, int glyph2, float *x, float *y) { assert(FALSE); return 0; }
void cc_flww32_done_glyph(void * font, int glyph) { assert(FALSE); }
  
struct cc_flw_bitmap * cc_flww32_get_bitmap(void * font, int glyph) { assert(FALSE); return NULL; }
int cc_flww32_get_outline(void * font, int glyph) { assert(FALSE); return 0; }

#else /* HAVE_WIN32_API */

#include <string.h>
#include <math.h>
#include <stdio.h>
#include <stddef.h>

/*  #define WIN32_LEAN_AND_MEAN */
#include <windows.h>
#include <wingdi.h>

#include <Inventor/C/tidbits.h>
#include <Inventor/C/errors/debugerror.h>
#include <Inventor/C/base/string.h>
#include <Inventor/C/glue/win32api.h>


struct cc_flww32_globals_s {
  HDC devctx;
};

static struct cc_flww32_globals_s cc_flww32_globals = {
  NULL /* devctx */
};

/* dumps debug information about one of the system fonts */
static int CALLBACK
font_enum_proc(ENUMLOGFONTEX * logicalfont, NEWTEXTMETRICEX * physicalfont,
               int fonttype, LPARAM userdata)
{
  cc_string str;
  cc_string_construct(&str);

  cc_string_sprintf(&str,
                    "fontenum: fullname=='%s', style=='%s', script=='%s' "
                    "<h, w>==<%d, %d>",
                    logicalfont->elfFullName, logicalfont->elfStyle,
                    logicalfont->elfScript,
                    logicalfont->elfLogFont.lfHeight,
                    logicalfont->elfLogFont.lfWidth);

  cc_string_append_text(&str, " -- fonttype: ");
  switch (fonttype) {
  case DEVICE_FONTTYPE: cc_string_append_text(&str, "DEVICE"); break;
  case RASTER_FONTTYPE: cc_string_append_text(&str, "RASTER"); break;
  case TRUETYPE_FONTTYPE: cc_string_append_text(&str, "TRUETYPE"); break;
  default: cc_string_append_text(&str, "<unknown>"); break;
  }

  cc_debugerror_postinfo("font_enum_proc", "%s", cc_string_get_text(&str));

  cc_string_clean(&str);
  return 1; /* non-0 to continue enumeration */
}

SbBool
cc_flww32_initialize(void)
{
  HFONT wfont;

  if (cc_flw_debug()) { /* list all fonts on system */
    LOGFONT logfont; /* logical font information */

    /* Only these are inspected by the EnumFontFamiliesEx()
       function. Set up to list as many fonts as possible. */
    logfont.lfCharSet = DEFAULT_CHARSET;
    logfont.lfFaceName[0] = '\0';
    logfont.lfPitchAndFamily = 0;

    (void)EnumFontFamiliesEx(cc_flww32_globals.devctx,
                             (LPLOGFONT)&logfont,
                             (FONTENUMPROC)font_enum_proc,
                             0, /* user data for callback */
                             0); /* reserved, must be zero */
  }

  cc_flww32_globals.devctx = CreateDC("DISPLAY", NULL, NULL, NULL);
  if (cc_flww32_globals.devctx == NULL) {
    cc_win32_print_error("cc_flww32_initialize", "CreateDC()", GetLastError());
    return FALSE;
  }

  return TRUE;
}

void
cc_flww32_exit(void)
{
  BOOL ok = DeleteDC(cc_flww32_globals.devctx);
  if (!ok) {
    cc_win32_print_error("cc_flww32_exit", "DeleteDC()", GetLastError());
  }
}

/* Allocates and returns a new font id matching the exact fontname.
   Returns NULL on error. */
void *
cc_flww32_get_font(const char * fontname, int sizex, int sizey)
{
  /* FIXME: unimplemented. 20030515 mortene. */

  HGDIOBJ previousfont;

  HFONT wfont = CreateFont(sizey,
                           /* FIXME: should we let width==0 for better
                              chance of getting a match? 20030530 mortene. */
                           sizex,
                           0, 0, /* escapement, orientation */
                           FW_DONTCARE, /* weight */
                           FALSE, FALSE, FALSE, /* italic, underline, strikeout */
                           /* FIXME: using DEFAULT_CHARSET is probably
                              not what we should do, AFAICT from a
                              quick read-over of the CreateFont() API
                              doc. 20030530 mortene. */
                           DEFAULT_CHARSET,
                           /* FIXME: should be OUT_TT_PRECIS or
                              OUT_OUTLINE_PRECIS? 20030530 mortene. */
                           OUT_DEFAULT_PRECIS, /* output precision */
                           CLIP_DEFAULT_PRECIS, /* clipping precision */
                           PROOF_QUALITY, /* output quality */
                           DEFAULT_PITCH, /* pitch and family */
                           fontname /* XXX "Verdana" XXX */); /* typeface name */

  if (!wfont) {
    DWORD lasterr = GetLastError();
    cc_string * str = cc_string_construct_new();
    cc_string_sprintf(str, "CreateFont(%d, %d, ..., '%s')",
                      sizey, sizex, fontname);
    cc_win32_print_error("cc_flww32_get_font", cc_string_get_text(str), lasterr);
    cc_string_destruct(str);
    return NULL;
  }

  previousfont = SelectObject(cc_flww32_globals.devctx, wfont);
  if (previousfont == NULL) {
    cc_win32_print_error("cc_flww32_get_font", "SelectObject()", GetLastError());
    /* FIXME: handle bad return value. 20030530 mortene. */
    (void)DeleteObject(wfont);
    return NULL;
  }

  /* FIXME: need to call DeleteObject() on the font to clean up when
     we're done. 20030530 mortene. */

  return (void *)wfont;
}

/* Allocates a cc_string and returns with name of given font id. */
cc_string *
cc_flww32_get_font_name(void * font)
{
  /* FIXME: fix interface silliness of allocating a cc_string -- that
     should be done on the client side. 20030515 mortene. */
  cc_string * str = cc_string_construct_new();

  int size = cc_win32()->GetTextFace(cc_flww32_globals.devctx, 0, NULL);
  char * s = (char *)malloc(size);
  assert(s); /* FIXME: handle alloc problem better. 20030530 mortene. */
  (void)cc_win32()->GetTextFace(cc_flww32_globals.devctx, size, s);
  
  cc_string_set_text(str, s);
  free(s);

  return str;
}

/* Allocates a cc_string and returns with style of given font id. */
cc_string *
cc_flww32_get_font_style(void * font)
{
  /* FIXME: unimplemented. 20030515 mortene. */

  /* FIXME: fix interface silliness of allocating a string -- that
     should be done on the client side. 20030515 mortene. */
  cc_string * str = cc_string_construct_new();
  cc_string_set_text(str, "0xdeadbeef-fontstyle");
  return str;
}

/* Deallocates the resources connected with the given font id. Assumes
   that the font will no longer be used. */
void
cc_flww32_done_font(void * font)
{
  /* FIXME: unimplemented. 20030515 mortene. */
}

/* Returns the number of character mappings available for the given
   font. A character mapping can e.g. be "unicode" or "latin_1". */
int
cc_flww32_get_num_charmaps(void * font)
{
  /* FIXME: unimplemented. 20030515 mortene. */
  return 0;
}

/* Returns the name of the character mapping given by the index
   number. */
const char *
cc_flww32_get_charmap_name(void * font, int charmapidx)
{
  /* FIXME: unimplemented. 20030515 mortene. */
  return "unknown";
}

/* Set the current character translation map. */
void
cc_flww32_set_charmap(void * font, int charmap)
{
  /* FIXME: unimplemented. 20030515 mortene. */
}

/* Set the character dimensions of the given font. */
void
cc_flww32_set_char_size(void * font, int width, int height)
{
  /* FIXME: unimplemented. 20030515 mortene. */
}

/* Set a transformation on the font characters that rotates them the
   given angle. Angle specified in radians. */
void
cc_flww32_set_font_rotation(void * font, float angle)
{
  /* FIXME: unimplemented. 20030515 mortene. */
}

/* Returns the glyph index for the given character code. If the
   character code is undefined for this font, returns 0. */
int
cc_flww32_get_glyph(void * font, unsigned int charidx)
{
  /* FIXME: unimplemented. 20030515 mortene. */
  return charidx;
}

/* Returns, in x and y input arguments, how much to advance cursor
   after rendering glyph. */
int
cc_flww32_get_advance(void * font, int glyph, float * x, float * y)
{
  /* FIXME: unimplemented. 20030515 mortene. */
  *x = 10.0f;
  *y = 0.0f;
  return 0;
}

/* Returns kerning, in x and y input arguments, for a pair of
   glyphs. */
int
cc_flww32_get_kerning(void * font, int glyph1, int glyph2, float * x, float * y)
{
  /* FIXME: unimplemented. 20030515 mortene. */
  *x = 0.0f;
  *y = 0.0f;
  return 0;
}

/* Client should use this to indicate it's done with a glyph, so the
   resources associated with it can be deallocated. */
void
cc_flww32_done_glyph(void * font, int glyph)
{
  /* FIXME: unimplemented. 20030515 mortene. */
}

/* Draws a bitmap for the given glyph. */
struct cc_flw_bitmap *
cc_flww32_get_bitmap(void * font, int glyph)
{
  /* FIXME: so far, this is just a mock-up implementation. 20030515 mortene. */
  
  struct cc_flw_bitmap * bm = NULL;

  /* The GetGlyphOutline function retrieves the outline or bitmap for
     a character in the TrueType font that is selected into the
     specified device context. */
  GLYPHMETRICS gm;
  static const MAT2 identitymatrix = { { 0, 1 }, { 0, 0 },
                                       { 0, 0 }, { 0, 1 } };
  DWORD ret;
  DWORD size = 0;
  uint8_t * w32bitmap = NULL;

  ret = GetGlyphOutline(cc_flww32_globals.devctx,
                        glyph, /* character to query */
                        GGO_BITMAP, /* format of data to return */
                        &gm, /* metrics */
                        0, /* size of buffer for data */
                        NULL, /* buffer for data */
                        &identitymatrix /* transformation matrix */
                        );

  if (ret == GDI_ERROR) {
    cc_string str;
    cc_string_construct(&str);
    cc_string_sprintf(&str,
                      "GetGlyphOutline(%p, 0x%x '%c', GGO_BITMAP, "
                      "<metricsstruct>, 0, NULL, <idmatrix>)",
                      cc_flww32_globals.devctx, glyph, (unsigned char)glyph);
    cc_win32_print_error("cc_flww32_get_bitmap", cc_string_get_text(&str), GetLastError());
    cc_string_clean(&str);
    return NULL;
  }

  assert((ret >= 0) && (ret < 1024*1024) && "bogus buffer size");
  size = ret;

  if (size == 0) {
    cc_debugerror_post("cc_flww32_get_bitmap",
                       "something funny is going on, got zero-size buffer for "
                       "glyph 0x%x ('%c')", glyph, (unsigned char)glyph);
    return NULL;
  }

  w32bitmap = (uint8_t *)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, ret);
  assert(w32bitmap != NULL); /* FIXME: be robust. 20030530 mortene. */

  ret = GetGlyphOutline(cc_flww32_globals.devctx,
                        glyph, /* character to query */
                        GGO_BITMAP, /* format of data to return */
                        &gm, /* metrics */
                        size, /* size of buffer for data */
                        w32bitmap, /* buffer for data */
                        &identitymatrix /* transformation matrix */
                        );

  if (ret == GDI_ERROR) {
    cc_string str;
    cc_string_construct(&str);
    cc_string_sprintf(&str,
                      "GetGlyphOutline(%p, 0x%x '%c', GGO_BITMAP, "
                      "<metricsstruct>, %d, <buffer>, <idmatrix>)",
                      cc_flww32_globals.devctx, glyph, (unsigned char)glyph, size);
    cc_win32_print_error("cc_flww32_get_bitmap", cc_string_get_text(&str), GetLastError());
    cc_string_clean(&str);

    /* FIXME: catch error return. 20030530 mortene. */
    (void)HeapFree(GetProcessHeap(), 0, w32bitmap);
    return NULL;
  }

  bm = (struct cc_flw_bitmap *)malloc(sizeof(struct cc_flw_bitmap));
  assert(bm);
  bm->bearingX = gm.gmptGlyphOrigin.x;
  bm->bearingY = gm.gmptGlyphOrigin.y;
  bm->advanceX = gm.gmCellIncX;
  bm->advanceY = gm.gmCellIncY;
  bm->rows = gm.gmBlackBoxY;
  bm->width = gm.gmBlackBoxX;
  bm->pitch = (bm->width + 7) / 8;
  bm->buffer = (unsigned char *)malloc(bm->rows * bm->pitch);
  assert(bm->buffer);
  {
    int i;
    for (i = 0; i < bm->rows; i++) {
      (void)memcpy(&(bm->buffer[i * bm->pitch]),
                   /* the win32 bitmap is doubleword aligned pr row */
                   w32bitmap + i * (((bm->pitch + 3) / 4) * 4),
                   bm->pitch);
    }
  }
  
  /* FIXME: catch error return. 20030530 mortene. */
  (void)HeapFree(GetProcessHeap(), 0, w32bitmap);
  
  return bm;
}

/* Extract a vector outline of the glyph. */
int
cc_flww32_get_outline(void * font, int glyph)
{
  /* FIXME: implement. */
  return 0;
}

#endif /* HAVE_WIN32_API */

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

void * cc_flww32_get_font(const char * fontname) { assert(FALSE); return NULL; }
cc_string * cc_flww32_get_font_name(void * font) { assert(FALSE); return NULL; }
cc_string * cc_flww32_get_font_style(void * font) { assert(FALSE); return NULL; }
void cc_flww32_done_font(void * font) { assert(FALSE); }

int cc_flww32_get_num_charmaps(void * font) { assert(FALSE); return 0; }
const char * cc_flww32_get_charmap_name(void * font, int charmap) { assert(FALSE); return NULL; }
int cc_flww32_set_charmap(void * font, int charmap) { assert(FALSE); return 0; }

int cc_flww32_set_char_size(void * font, int width, int height) { assert(FALSE); return 0; }
int cc_flww32_set_font_rotation(void * font, float angle) { assert(FALSE); return 0; }
  
int cc_flww32_get_glyph(void * font, unsigned int charidx) { assert(FALSE); return 0; }
int cc_flww32_get_advance(void * font, int glyph, float *x, float *y) { assert(FALSE); return 0; }
int cc_flww32_get_kerning(void * font, int glyph1, int glyph2, float *x, float *y) { assert(FALSE); return 0; }
void cc_flww32_done_glyph(void * font, int glyph) { assert(FALSE); }
  
struct cc_FLWbitmap * cc_flww32_get_bitmap(void * font, int glyph) { assert(FALSE); return NULL; }
int cc_flww32_get_outline(void * font, int glyph) { assert(FALSE); return 0; }

#else /* HAVE_WIN32_API */

#include <string.h>
#include <math.h>
#include <stdio.h>
#include <stddef.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <wingdi.h>

#include <Inventor/C/tidbits.h>
#include <Inventor/C/errors/debugerror.h>
#include <Inventor/C/base/string.h>

struct cc_flww32_globals_s {
  HDC devctx;
};

static struct cc_flww32_globals_s cc_flww32_globals = {
  NULL /* devctx */
};

static SbBool
cc_debug(void)
{
  const char * env = coin_getenv("COIN_DEBUG_WIN32_FONTS");
  return env && (atoi(env) > 0);
}

SbBool
cc_flww32_initialize(void)
{
  cc_flww32_globals.devctx = CreateDC("DISPLAY", NULL, NULL, NULL);
  if (cc_flww32_globals.devctx == NULL) {
    /* FIXME: use GetLastError() to report problem, or wrap
       CreateDC() like in SoWin's Win32API.cpp. 20030526 mortene. */
    assert(FALSE && "CreateDC() failed");
    return FALSE;
  }
  return TRUE;
}

void
cc_flww32_exit(void)
{
  BOOL ok = DeleteDC(cc_flww32_globals.devctx);
  if (!ok) {
    /* FIXME: use GetLastError() to report problem, or wrap like in
       SoWin's Win32API.cpp. 20030526 mortene. */
    assert(FALSE && "DeleteDC() failed");
  }
}

/* Allocates and returns a new font id matching the exact fontname.
   Returns NULL on error. */
void *
cc_flww32_get_font(const char * fontname)
{
  /* FIXME: unimplemented. 20030515 mortene. */
  return NULL;
}

/* Allocates a cc_string and returns with name of given font id. */
cc_string *
cc_flww32_get_font_name(void * font)
{
  /* FIXME: unimplemented. 20030515 mortene. */

  /* FIXME: fix interface silliness of allocating a string -- that
     should be done on the client side. 20030515 mortene. */
  cc_string * str = cc_string_construct_new();
  cc_string_sprintf(str, "hupp");
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
  cc_string_set_text(str, "hopp");
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
int
cc_flww32_set_charmap(void * font, int charmap)
{
  /* FIXME: unimplemented. 20030515 mortene. */
  return -1;
}

/* Set the character dimensions of the given font. */
int
cc_flww32_set_char_size(void * font, int width, int height)
{
  /* FIXME: unimplemented. 20030515 mortene. */
  return 0;
}

/* Set a transformation on the font characters that rotates them the
   given angle. Angle specified in radians. */
int
cc_flww32_set_font_rotation(void * font, float angle)
{
  /* FIXME: unimplemented. 20030515 mortene. */
  return 0;
}

/* Returns the glyph index for the given character code. If the
   character code is undefined for this font, returns 0. */
int
cc_flww32_get_glyph(void * font, unsigned int charidx)
{
  /* FIXME: unimplemented. 20030515 mortene. */
  return 0;
}

/* Returns, in x and y input arguments, how much to advance cursor
   after rendering glyph. */
int
cc_flww32_get_advance(void * font, int glyph, float * x, float * y)
{
  /* FIXME: unimplemented. 20030515 mortene. */
  *x = 0.0f;
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
struct cc_FLWbitmap *
cc_flww32_get_bitmap(void * font, int glyph)
{
  /* FIXME: unimplemented. 20030515 mortene. */
  return NULL;
}

/* Extract a vector outline of the glyph. */
int
cc_flww32_get_outline(void * font, int glyph)
{
  /* FIXME: implement. */
  return 0;
}

#endif /* HAVE_WIN32_API */

/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2009 by Kongsberg SIM.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Kongsberg SIM about acquiring
 *  a Coin Professional Edition License.
 *
 *  See http://www.coin3d.org/ for more information.
 *
 *  Kongsberg SIM, Postboks 1283, Pirsenteret, 7462 Trondheim, NORWAY.
 *  http://www.sim.no/  sales@sim.no  coin-support@coin3d.org
 *
\**************************************************************************/

#include "fonts/coretext.h"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif /* HAVE_CONFIG_H */

#include "glue/GLUWrapper.h"

/* ************************************************************************* */

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* ************************************************************************* */

#ifdef HAVE_CORETEXT

/* ************************************************************************* */

#include <ApplicationServices/ApplicationServices.h>

/* ************************************************************************* */

SbBool
cc_flwct_initialize(void)
{
  return TRUE;
}

void
cc_flwct_exit(void)
{}

void *
cc_flwct_get_font(const char * fontname, unsigned int pixelsize)
{
  return NULL;
}

void
cc_flwct_get_font_name(void * font, cc_string * str)
{}

void
cc_flwct_done_font(void * font)
{
  CFRelease((CTFontRef)font);
}

int
cc_flwct_get_num_charmaps(void * font)
{
  return 0;
}

const char *
cc_flwct_get_charmap_name(void * font, int charmap)
{
  return "UNICODE";
}

void
cc_flwct_set_charmap(void * font, int charmap)
{}

void
cc_flwct_set_char_size(void * font, int height)
{}

void
cc_flwct_set_font_rotation(void * font, float angle)
{}

int
cc_flwct_get_glyph(void * font, unsigned int charidx)
{
  return 0;
}

void
cc_flwct_get_vector_advance(void * font, int glyph, float * x, float * y)
{}

void
cc_flwct_get_bitmap_kerning(void * font, int glyph1, int glyph2, int *x, int *y)
{}


void
cc_flwct_get_vector_kerning(void * font, int glyph1, int glyph2, float *x, float *y)
{}

void
cc_flwct_done_glyph(void * font, int glyph)
{}

struct cc_font_bitmap *
cc_flwct_get_bitmap(void * font, unsigned int glyph)
{
  return NULL;
}

struct cc_font_vector_glyph *
cc_flwct_get_vector_glyph(void * font, unsigned int glyphindex, float complexity)
{
  return NULL;
}

#endif /* HAVE_CORETEXT */

#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */

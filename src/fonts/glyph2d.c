/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2004 by Systems in Motion.  All rights reserved.
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
 *  See <URL:http://www.coin3d.org/> for more information.
 *
 *  Systems in Motion, Teknobyen, Abels Gate 5, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no/>.
 *
\**************************************************************************/

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <string.h>
#include <assert.h>
#include <stdio.h>

#include <Inventor/C/base/hash.h>
#include <Inventor/C/base/hashp.h>
#include <Inventor/C/base/string.h>
#include <Inventor/C/threads/threadsutilp.h>

#include "fontlib_wrapper.h"
#include "freetype.h"
#include "glyph2d.h"

static SbBool glyph2d_specmatch(const cc_font_specification * spec1, const cc_font_specification * spec2);

struct cc_glyph2d {
  int fontidx;    
  int glyphidx;
  float angle;
  unsigned short width;
  unsigned short height;
  unsigned short bitmapwidth;
  unsigned short bitmapheight;
  short bitmapoffsetx;
  short bitmapoffsety;
  cc_font_specification * fontspec;
  unsigned char * bitmap;
};

static cc_hash * glyph2d_fonthash = NULL;

/*
  Mutex lock for the static ang global font hash
*/
static void * glyph2d_fonthash_lock = NULL;

/* Set '#if 1' to enable debug output to stderr for tracking mutex locking. */
#if 0
#define GLYPH2D_MUTEX_LOCK(m) \
  do { \
    (void)fprintf(stderr, "glyph2d mutex lock in %s\n", __func__); \
    CC_MUTEX_LOCK(m); \
  } while (0)
#define GLYPH2D_MUTEX_UNLOCK(m) \
  do { \
    (void)fprintf(stderr, "glyph2d mutex unlock in %s\n", __func__); \
    CC_MUTEX_UNLOCK(m); \
  } while (0)
#else
#define GLYPH2D_MUTEX_LOCK(m) CC_MUTEX_LOCK(m)
#define GLYPH2D_MUTEX_UNLOCK(m) CC_MUTEX_UNLOCK(m)
#endif

static void
cc_glyph2d_initialize()
{

  static SbBool initialized = FALSE;
  CC_MUTEX_CONSTRUCT(glyph2d_fonthash_lock);
  GLYPH2D_MUTEX_LOCK(glyph2d_fonthash_lock);
  
  if (initialized) {
    GLYPH2D_MUTEX_UNLOCK(glyph2d_fonthash_lock);
    return;
  }
  initialized = TRUE;
  
  glyph2d_fonthash = cc_hash_construct(15, 0.75);

  GLYPH2D_MUTEX_UNLOCK(glyph2d_fonthash_lock);
}


cc_glyph2d * 
cc_glyph2d_getglyph(uint32_t character, const cc_font_specification * spec, float angle)
{

  void * val;
  cc_glyph2d * glyph;
  int fontidx;
  int glyphidx;
  int i;
  cc_flw_bitmap * bm;
  cc_font_specification * newspec;
  int namelen = 0;
  cc_string * fonttoload;
  cc_list * glyphlist;


  /* Beacuse this function is the entry point for glyph2d, the mutex
     is initialized here. */
  if (glyph2d_fonthash_lock == NULL) 
    cc_glyph2d_initialize();
  
  assert(spec);

  GLYPH2D_MUTEX_LOCK(glyph2d_fonthash_lock);

  /* Has the glyph been created before? */
  if (cc_hash_get(glyph2d_fonthash, (unsigned long) character, &val)) {
    glyphlist = (cc_list *) val;
    for (i=0;i<cc_list_get_length(glyphlist);++i) {
      glyph = (cc_glyph2d *) cc_list_get(glyphlist, i);
      if (glyph2d_specmatch(spec, glyph->fontspec)) {
        GLYPH2D_MUTEX_UNLOCK(glyph2d_fonthash_lock);
        return glyph;
      }
    }    
  } else {
    /* No glyphlist for this character is found. Create one and
       add it to the hashtable. */
    glyphlist = cc_list_construct();
    cc_hash_put(glyph2d_fonthash, (unsigned long) character, glyphlist);
  }

  assert(glyphlist);

  /* build a new glyph struct with bitmap */    
  glyph = (cc_glyph2d *) malloc(sizeof(cc_glyph2d));
  
  newspec = (cc_font_specification *) malloc(sizeof(cc_font_specification)); 
  assert(newspec);
  cc_fontspec_copy(spec, newspec);

  glyph->fontspec = newspec;

  /* FIXME: fonttoload variable should be allocated on the
     stack. 20030921 mortene. */
  fonttoload = cc_string_construct_new();
  cc_string_set_text(fonttoload, cc_string_get_text(&spec->name));
  if (cc_string_length(&spec->style) > 0) {
    cc_string_append_text(fonttoload, " ");
    cc_string_append_string(fonttoload, &spec->style);
  }

  fontidx = cc_flw_get_font(cc_string_get_text(fonttoload), (int)(newspec->size), (int)(newspec->size), angle);
  cc_string_destruct(fonttoload);
  assert(fontidx >= 0);


  /* Should _always_ be able to get hold of a glyph -- if no glyph is
     available for a specific character, a default empty rectangle
     should be used.  -mortene. */
  glyphidx = cc_flw_get_glyph(fontidx, character);
  assert(glyphidx >= 0);

  glyph->glyphidx = glyphidx;
  glyph->fontidx = fontidx;
  glyph->angle = angle;

  bm = cc_flw_get_bitmap(fontidx, glyphidx);
  assert(bm);
  glyph->width = bm->width;
  glyph->height = bm->rows;
  glyph->bitmapwidth = bm->pitch * 8;
  glyph->bitmapheight = bm->rows;
  glyph->bitmapoffsetx = bm->bearingX;
  glyph->bitmapoffsety = bm->bearingY;
  glyph->bitmap = bm->buffer;
  
  /* Store newly created glyph in the list for this character */
  cc_list_append(glyphlist, glyph);
  
  GLYPH2D_MUTEX_UNLOCK(glyph2d_fonthash_lock);
  return glyph;
}

static SbBool 
glyph2d_specmatch(const cc_font_specification * spec1, 
                  const cc_font_specification * spec2)
{

  assert(spec1);
  assert(spec2);

  if ((!cc_string_compare(&spec1->name, &spec2->name)) &&
      (!cc_string_compare(&spec1->style, &spec2->style)) &&
      (spec1->size == spec2->size)) {
    /* No need to compare complexity for 2D fonts */
    return TRUE;
  }
  else return FALSE;
  
}

void 
cc_glyph2d_getadvance(const cc_glyph2d * g, int * x, int * y)
{
  cc_flw_get_bitmap_advance(g->fontidx, g->glyphidx, x, y);
}

void 
cc_glyph2d_getkerning(const cc_glyph2d * left, const cc_glyph2d * right, int * x, int * y)
{
  cc_flw_get_bitmap_kerning(right->fontidx, left->glyphidx, right->glyphidx, x, y);
}

unsigned int 
cc_glyph2d_getwidth(const cc_glyph2d * g)
{
  return (int) g->width;
}

const unsigned char * 
cc_glyph2d_getbitmap(const cc_glyph2d * g, int * size, int * offset)
{
  size[0] = g->bitmapwidth;
  size[1] = g->bitmapheight;
  offset[0] = g->bitmapoffsetx;
  offset[1] = g->bitmapoffsety;

  return g->bitmap;
}

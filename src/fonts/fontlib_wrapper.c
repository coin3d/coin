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

/* ********************************************************************** */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <stddef.h>

#include <Inventor/C/base/dynarray.h>
#include <Inventor/C/base/string.h>
#include <Inventor/C/base/hash.h>
#include <Inventor/C/errors/debugerror.h>

#include <Inventor/C/threads/threadsutilp.h>
#include <Inventor/C/tidbits.h>
#include <Inventor/C/tidbitsp.h>

#include "fontlib_wrapper.h"
#include "freetype.h"
#include "win32.h"
#include "defaultfonts.h"

/* ********************************************************************** */

/*
  All public interface functions are protected with this "file-global"
  lock, to simplify the implementation of the underlying font-import
  modules (as they won't have to be made reentrant in any way).
*/

static void * flw_global_lock = NULL;
static int flw_global_font_index = 0;
static int flw_global_glyph_index = 0;

/* Debug: enable this in case code hangs waiting for a lock.  A hang
   will typically happen for one out of two reasons:

   1) The mutex is locked but not released. To check whether or not
   this is the cause, look for multiple exit points (return
   statements) in a function.

   2) A cc_flw_*() function locks the global mutex, then calls another
   cc_flw_*() function, which when attempting to lock the same
   mutex will simply hang.

   -- mortene.
*/
#if 0

#define FLW_MUTEX_LOCK(m) \
  do { \
    (void)fprintf(stderr, "mutex lock in %s\n", __func__); \
    CC_MUTEX_LOCK(m); \
  } while (0)

#define FLW_MUTEX_UNLOCK(m) \
  do { \
    (void)fprintf(stderr, "mutex unlock in %s\n", __func__); \
    CC_MUTEX_UNLOCK(m); \
  } while (0)

#else

#define FLW_MUTEX_LOCK(m) CC_MUTEX_LOCK(m)
#define FLW_MUTEX_UNLOCK(m) CC_MUTEX_UNLOCK(m)

#endif

/* ********************************************************************** */

struct cc_flw_glyph {
  unsigned int glyph;
  struct cc_flw_bitmap * bitmap;
  SbBool fromdefaultfont;
  uint32_t charidx;
  int glyphindex;
};

struct cc_flw_font {
  void * font;
  cc_string * fontname;
  cc_string * requestname;
  cc_hash * glyphdict;
  unsigned int sizex, sizey;
  float angle;
  SbBool defaultfont;
  int fontindex;
  int refcount;
};

static cc_dynarray * fontarray = NULL;

/* This is the file-global flag that indicates whether or not we can
   and will use the FreeType library. */
static SbBool freetypelib = FALSE;

/* This is the file-global flag that indicates whether or not we can
   and will use the Win32 API. */
static SbBool win32api = FALSE;

/* ********************************************************************** */
/* BEGIN Internal functions */

static struct cc_flw_bitmap *
get_default_bitmap(unsigned int character)
{
  struct cc_flw_bitmap * bm;

  if (character < 256) {
    const int fontsize = coin_default2dfont_get_size();
    const int * isomapping = coin_default2dfont_get_isolatin1_mapping();
    const unsigned char * fontdata = coin_default2dfont_get_data();

    bm = (struct cc_flw_bitmap *)malloc(sizeof(struct cc_flw_bitmap));
    bm->buffer = (unsigned char *) fontdata +
      fontsize * isomapping[character];
    bm->bearingX = 0;
    bm->bearingY = fontsize;
    bm->rows = fontsize;
    bm->width = 7;
    bm->pitch = 1;
    return bm;
  }
  return NULL;
}

static struct cc_flw_glyph *
glyphstruct_new(void)
{
  struct cc_flw_glyph * gs = (struct cc_flw_glyph *)malloc(sizeof(struct cc_flw_glyph));
  gs->glyph = 0;
  gs->bitmap = NULL;
  gs->fromdefaultfont = FALSE;
  gs->charidx = 0;
  gs->glyphindex = flw_global_glyph_index++;
  return gs;
}

static struct cc_flw_font *
fontstruct_new(void * font)
{
  struct cc_flw_font * fs;
  fs = (struct cc_flw_font *)malloc(sizeof(struct cc_flw_font));
  fs->font = font;
  fs->fontname = NULL;
  fs->requestname = NULL;
  fs->sizex = 0;
  fs->sizey = 0;
  fs->angle = 0.0f;
  fs->defaultfont = FALSE;
  fs->glyphdict = cc_hash_construct(256, 0.7f);
  fs->fontindex = flw_global_font_index++;
  fs->refcount = 0;
  return fs;
}

static void
fontstruct_set_fontname(struct cc_flw_font * fs, const char * newname)
{
  assert(fs && newname);
  if (!fs->fontname)
    fs->fontname = cc_string_construct_new();
  cc_string_set_text(fs->fontname, newname);
}

static void
fontstruct_set_requestname(struct cc_flw_font * fs, const char * newname)
{
  assert(fs && newname);
  if (!fs->requestname)
    fs->requestname = cc_string_construct_new();
  cc_string_set_text(fs->requestname, newname);
}

static void
fontstruct_set_size(struct cc_flw_font * fs, const int x, const int y)
{
  assert(fs);
  fs->sizex = x;
  fs->sizey = y;
}

static void
fontstruct_set_angle(struct cc_flw_font * fs, const float angle)
{
  assert(fs);
  fs->angle = angle;
}


static void
flw_done_bitmap(struct cc_flw_bitmap * bitmap)
{
  assert(bitmap);
  if (bitmap->buffer) { free(bitmap->buffer); }
  free(bitmap);
}

static struct cc_flw_glyph *
flw_glyphidx2glyphptr(struct cc_flw_font * fs, unsigned int glyphidx)
{
  void * tmp;
  struct cc_flw_glyph * gs = NULL;
  
  if (cc_hash_get(fs->glyphdict, glyphidx, &tmp)) {
    gs = (struct cc_flw_glyph *) tmp;
  }
  else {
    assert(0 && "glyph not found");
  }  
  return gs;
}

static void
fontstruct_rmglyph(struct cc_flw_font * fs, unsigned int glyph, int removefromdict)
{
  struct cc_flw_glyph * gs = flw_glyphidx2glyphptr(fs, glyph);
  if (gs->bitmap) { /* might be NULL for 3D fonts */
    /* workaround so that flw_done_bitmap() doesn't free our static data */
    if (gs->fromdefaultfont) gs->bitmap->buffer = NULL;
    flw_done_bitmap(gs->bitmap);
  }
  free(gs);

  if (removefromdict) {
    if (!cc_hash_remove(fs->glyphdict, glyph)) {
      assert(0 && "glyph to remove not found");
    }
  }
}

static void 
fontstruct_rmglyph_apply(unsigned long key, void * val, void * closure)
{
  fontstruct_rmglyph((struct cc_flw_font*) closure,
                     key, 0);
}

static void
fontstruct_rmfont(int font)
{
  int i, n;
  int arrayindex;
  struct cc_flw_font * fs = NULL;
  
  n = cc_dynarray_length(fontarray);
  for (i = 0; i < n; i++) {
    fs = (struct cc_flw_font *)cc_dynarray_get(fontarray, i);
    if (fs->fontindex == font) break;
  }
  assert(i < n);
  arrayindex = i;

  if (fs->fontname) cc_string_destruct(fs->fontname);
  if (fs->requestname) cc_string_destruct(fs->requestname);
    
  cc_hash_apply(fs->glyphdict, fontstruct_rmglyph_apply, fs);  
  cc_hash_destruct(fs->glyphdict);
  free(fs);
  cc_dynarray_remove_idx(fontarray, arrayindex);
}

static struct cc_flw_font *
flw_fontidx2fontptr(int fontidx)
{
  struct cc_flw_font * fs;
  int i, n;

  n = (int) cc_dynarray_length(fontarray);
  for (i = 0; i < n; i++) {
    fs = (struct cc_flw_font *) cc_dynarray_get(fontarray, i);
    if (fs->fontindex == fontidx) break;
  }
  assert(i < n);

  return fs;
}

/* Map the given font name to the built-in default font. Returns index
   in array of the font struct that was made. */
static unsigned int
flw_map_fontname_to_defaultfont(const char * reqname)
{
  struct cc_flw_font * fs = fontstruct_new(NULL);
  fontstruct_set_requestname(fs, reqname);
  fontstruct_set_fontname(fs, "defaultFont");
  fontstruct_set_size(fs, 0, 0);
  fontstruct_set_angle(fs, 0.0f);
  fs->defaultfont = TRUE;  
  cc_dynarray_append(fontarray, fs);
  return fs->fontindex;
}

static void
flw_exit(void)
{
  unsigned int n;

  n = cc_dynarray_length(fontarray);
  
  while (n--) {
    fontstruct_rmfont(n);
  }
  cc_dynarray_destruct(fontarray);

  if (freetypelib) { cc_flwft_exit(); }
  if (win32api) { cc_flww32_exit(); }
  
  CC_MUTEX_DESTRUCT(flw_global_lock);
}

static void
flw_initialize(void)
{
  int idx;
  const char * env;
  static SbBool initialized = FALSE;
  /* CC_MUTEX_CONSTRUCT uses a global mutex to be thread safe */
  CC_MUTEX_CONSTRUCT(flw_global_lock);
  FLW_MUTEX_LOCK(flw_global_lock);

  if (initialized) {
    FLW_MUTEX_UNLOCK(flw_global_lock);
    return;
  }
  initialized = TRUE;

  fontarray = cc_dynarray_new();

  freetypelib = !((env = coin_getenv("COIN_FORCE_FREETYPE_OFF")) && (atoi(env) > 0));
  freetypelib = freetypelib && cc_flwft_initialize();
  if (cc_flw_debug()) {
    cc_debugerror_postinfo("cc_flw_initialize",
                           "FreeType library %s",
                           freetypelib ? "can be used" : "can not be used");
  }

  win32api = !((env = coin_getenv("COIN_FORCE_WIN32FONTS_OFF")) && (atoi(env) > 0));
  win32api = win32api && cc_flww32_initialize();
  if (cc_flw_debug()) {
    cc_debugerror_postinfo("cc_flw_initialize",
                           "Win32 API %s for font support",
                           win32api ? "can be used" : "can not be used");
  }

  /* Allow only one of the availability flags to be set, as it's too
     easy to get bugs in our code in this file if we depend on always
     checking one particular flag before the other.

     We prefer to consistently use the FreeType library over the Win32
     API if available -- assuming that FreeType has been installed for
     a good reason on the Windows machine in question.
  */
  if (win32api && freetypelib) {
    if (cc_flw_debug()) {
      cc_debugerror_postinfo("cc_flw_initialize",
                             "FreeType library will take precedence "
                             "over Win32 API");
    }
    cc_flww32_exit();
    win32api = FALSE;
  }

  /* Set up an item at the front of the list for the default font, so
     that the first request for a font with the name "defaultFont"
     will not make the underlying font import libraries try to find a
     best match (and get a positive result for some random font). */
  idx = flw_map_fontname_to_defaultfont("defaultFont");

  coin_atexit((coin_atexit_f *)flw_exit, 0);

  FLW_MUTEX_UNLOCK(flw_global_lock);
  cc_flw_ref_font(idx); /* increase refcount for default font */
}

/* END Internal functions */
/* ********************************************************************** */

SbBool
cc_flw_debug(void)
{
  static int dbg = -1;
  if (dbg == -1) {
    const char * env = coin_getenv("COIN_DEBUG_FONTSUPPORT");
    dbg = env && (atoi(env) > 0);
  }
  return dbg;
}


/*
  Returns internal index of given font specification, for subsequent
  use as input argument to other functions as identification.

  Returns -1 if no font with the given name and size has been made
  yet.
*/
static int
flw_find_font(const char * fontname, const unsigned int sizex, const unsigned int sizey, const float angle)
{
  unsigned int i, n;
  
  /* This function is a common entry spot for first access into the
     wrapper (unless client code is doing something wrong, but then
     we'll likely catch that with an assert somewhere), so we call the
     initialization code from here.

     flw_initialize() is re-entrant, so this should be safe in a
     multithreaded environment.
  */
  if (flw_global_lock == NULL) { flw_initialize(); }

  FLW_MUTEX_LOCK(flw_global_lock);

  n = cc_dynarray_length(fontarray);
  for (i = 0; i < n; i++) {
    struct cc_flw_font * fs = (struct cc_flw_font *)cc_dynarray_get(fontarray, i);
    if ((fs->defaultfont || (fs->sizex == sizex && fs->sizey == sizey)) &&
        (strcmp(fontname, cc_string_get_text(fs->requestname))==0) &&
        (fs->angle == angle)) {
      FLW_MUTEX_UNLOCK(flw_global_lock);
      return fs->fontindex;
    }
  }
  FLW_MUTEX_UNLOCK(flw_global_lock);
  return -1;
}


void 
cc_flw_ref_font(int fontid)
{
  int i, n;
  struct cc_flw_font * fs;
  FLW_MUTEX_LOCK(flw_global_lock);
  n = (int) cc_dynarray_length(fontarray);
  for (i = 0; i < n; i++) {
    fs = (struct cc_flw_font *)cc_dynarray_get(fontarray, i);
    if (fs->fontindex == fontid) break;
  }
  assert(i < n);
  fs->refcount++;
  FLW_MUTEX_UNLOCK(flw_global_lock);
}

void 
cc_flw_unref_font(int fontid)
{
  int i, n;
  struct cc_flw_font * fs;

  FLW_MUTEX_LOCK(flw_global_lock);

  n = (int) cc_dynarray_length(fontarray);
  for (i = 0; i < n; i++) {
    fs = (struct cc_flw_font *)cc_dynarray_get(fontarray, i);
    if (fs->fontindex == fontid) break;
  }
  assert(i < n);
  fs->refcount--;
  if (fs->refcount == 0) {
    /* fprintf(stderr,"unref font: %s\n", cc_string_get_text(fs->requestname)); */
    
    if (win32api) {
      if (!fs->defaultfont) { cc_flww32_done_font(fs->font); }
    }
    else if (freetypelib) {
      if (!fs->defaultfont) { cc_flwft_done_font(fs->font); }
    }
    fontstruct_rmfont(fontid);
  }

  FLW_MUTEX_UNLOCK(flw_global_lock);
}

/*!
  Set up an internal representation of a font from the \a fontname,
  and returns a font id >= 0 to be used in successive calls.

  \a fontname is the name and style \e requested. The actual font
  created will typically differ slightly, depending on what is
  available on the run-time system.  If a font has already been
  created for this \a fontname and size, the function will not create
  a duplicate, but simply return that font.

  If no font could sensibly be made from the given \a fontname, the
  internal, hard-coded default font will be used. One is therefore
  guaranteed that this function will execute and return without
  needing any error checking on behalf of the client code.
*/
int
cc_flw_get_font_id(const char * fontname, const unsigned int sizex, const unsigned int sizey, 
                   const float angle, const float complexity)
{
  void * font;
  int idx;

  /* Don't create font if one has already been created for this name
     and size. */
  idx = flw_find_font(fontname, sizex, sizey, angle);

  if (idx != -1) { return idx; }

  font = NULL;

  /* fprintf(stderr,"new font: %s\n", fontname); */

  FLW_MUTEX_LOCK(flw_global_lock);

  if (win32api) { 
    font = cc_flww32_get_font(fontname, sizex, sizey, angle);
  } else if (freetypelib) { 
    font = cc_flwft_get_font(fontname);    
  }


  if (font) {
    struct cc_flw_font * fs;
    cc_string realname;
    cc_string_construct(&realname);

    fs = fontstruct_new(font);
    fontstruct_set_requestname(fs, fontname);
    fontstruct_set_size(fs, sizex, sizey);
    fontstruct_set_angle(fs, angle);

    if (win32api) {
      cc_flww32_get_font_name(font, &realname);
      fontstruct_set_fontname(fs, cc_string_get_text(&realname));
    }
    else if (freetypelib) {
      cc_flwft_set_char_size(font, sizex, sizey);
      cc_flwft_set_font_rotation(font, angle);
      cc_flwft_get_font_name(font, &realname);
      fontstruct_set_fontname(fs, cc_string_get_text(&realname));
    }
    else {
      assert(FALSE && "incomplete code path");
    }

    if (cc_flw_debug()) {
      cc_debugerror_postinfo("cc_flw_get_font",
                             "'%s', size==<%d, %d> => realname='%s', %s",
                             fontname, sizex, sizey,
                             cc_string_get_text(&realname),
                             fs->defaultfont ?
                             "(defaultfont)" : "(not defaultfont)");
    }

    cc_string_clean(&realname);

    cc_dynarray_append(fontarray, fs);
    idx = fs->fontindex;
  }
  else {
    /* Use the default font for the given fontname and size. */
    /* FIXME: the defaultfont size is now fixed at 12-pt. Should scale
       it to match sizey. 20030317 mortene. */
    idx = flw_map_fontname_to_defaultfont(fontname);
  }

  FLW_MUTEX_UNLOCK(flw_global_lock);
  return idx;
}

/*
  Returns the native (win32 or freetype) font handle.
*/
void * 
cc_flw_get_font_handle(int fontid)
{
  static struct cc_flw_font * font;
  font = flw_fontidx2fontptr(fontid);
  assert(font);
  
  return font->font;
}

const char *
cc_flw_get_font_name(int font)
{
  const char * name;
  struct cc_flw_font * fs;

  FLW_MUTEX_LOCK(flw_global_lock);

  fs = flw_fontidx2fontptr(font);
  name = cc_string_get_text(fs->fontname);

  FLW_MUTEX_UNLOCK(flw_global_lock);
  return name;
}


unsigned int
cc_flw_get_glyph(int font, unsigned int charidx)
{
  unsigned int glyph = 0;
  int fsid = -1;
  struct cc_flw_font * fs;

  FLW_MUTEX_LOCK(flw_global_lock);

  fs = flw_fontidx2fontptr(font);

  if (!fs->defaultfont) {
    if (win32api) { glyph = cc_flww32_get_glyph(fs->font, charidx); }
    else if (freetypelib) { glyph = cc_flwft_get_glyph(fs->font, charidx); }

    if (glyph > 0) {
      struct cc_flw_glyph * gs = glyphstruct_new();
      gs->glyph = glyph;
      gs->charidx = charidx;
      gs->fromdefaultfont = FALSE;
      fsid = gs->glyphindex;
      if (!cc_hash_put(fs->glyphdict, fsid, gs)) {
        assert(0 && "glyph already exists");
      }
    }
    else {
      /* Create glyph from default font, mark as default. */

      /* FIXME: shouldn't it rather be handled by making an empty
         rectangle glyph of the correct size, like it's at least done
         for X11 (and probably other systems aswell)?

         Or perhaps this strategy _is_ better, but then we should at
         least scale the defaultfont glyph to the correct size.
         20030317 mortene. */

      /* Correct fix is to make an empty rectangle. Using a char from
         the default font only gives a blank since most fonts contain
         a superset of the chars in the default font. (The default
         font characters being rendered was due to a bug in the
         character mapping, causing failure to find special chars such
         as 'ØÆÅ'. The bug has since been fixed).  20030327 preng */

      if (cc_flw_debug()) {
        cc_debugerror_postwarning("cc_flw_get_glyph",
                                  "no character 0x%x was found in font '%s'",
                                  charidx, cc_string_get_text(fs->fontname));
      }
    }
  }

  if (fsid == -1) {
    struct cc_flw_glyph * gs = glyphstruct_new();
    gs->glyph = charidx;
    gs->fromdefaultfont = TRUE;
    gs->charidx = charidx;
    fsid = gs->glyphindex;
    if (!cc_hash_put(fs->glyphdict, fsid, gs)) {
      assert(0 && "glyph already exists");
    }
  }

  FLW_MUTEX_UNLOCK(flw_global_lock);
  return fsid;
}

void 
cc_flw_get_bitmap_advance(int font, unsigned int glyph, int * x, int * y)
{
  struct cc_flw_font * fs;
  struct cc_flw_glyph * gs;

  FLW_MUTEX_LOCK(flw_global_lock);

  fs = flw_fontidx2fontptr(font);
  gs = flw_glyphidx2glyphptr(fs, glyph);

  *x = *y = 0;

  if (fs->defaultfont || gs->fromdefaultfont) {
    *x = 7; /* All chars are equal in width. No need for a lookup table */
  }
  else {
    if (win32api) { cc_flww32_get_bitmap_advance(fs->font, gs->glyph, x, y); }
    else if (freetypelib) { cc_flwft_get_bitmap_advance(fs->font, gs->glyph, x, y); }
  }

  FLW_MUTEX_UNLOCK(flw_global_lock);
}

void
cc_flw_get_vector_advance(int font, unsigned int glyph, float * x, float * y)
{
  struct cc_flw_font * fs;
  struct cc_flw_glyph * gs;

  FLW_MUTEX_LOCK(flw_global_lock);

  fs = flw_fontidx2fontptr(font);
  gs = flw_glyphidx2glyphptr(fs, glyph);

  *x = *y = 0.0f;

  if (fs->defaultfont || gs->fromdefaultfont) {
    *x = coin_default3dfont_get_advance(gs->charidx);
  }
  else {
    if (win32api) { cc_flww32_get_vector_advance(fs->font, gs->glyph, x, y); }
    else if (freetypelib) { cc_flwft_get_vector_advance(fs->font, gs->glyph, x, y); }
  }

  FLW_MUTEX_UNLOCK(flw_global_lock);
}

void
cc_flw_get_bitmap_kerning(int font, unsigned int glyph1, unsigned int glyph2,
                          int * x, int * y)
{
  struct cc_flw_font * fs;
  struct cc_flw_glyph * gs1, * gs2;

  FLW_MUTEX_LOCK(flw_global_lock);

  fs = flw_fontidx2fontptr(font);
  gs1 = flw_glyphidx2glyphptr(fs, glyph1);
  gs2 = flw_glyphidx2glyphptr(fs, glyph2);

  *x = *y = 0;

  if (fs->defaultfont == FALSE) {
    if (win32api) {
      cc_flww32_get_bitmap_kerning(fs->font, gs1->glyph, gs2->glyph, x, y);
    }
    else if (freetypelib) {
      cc_flwft_get_bitmap_kerning(fs->font, gs1->glyph, gs2->glyph, x, y);
    }
  }

  FLW_MUTEX_UNLOCK(flw_global_lock);
}


void
cc_flw_get_vector_kerning(int font, unsigned int glyph1, unsigned int glyph2,
                          float * x, float * y)
{
  struct cc_flw_font * fs;
  struct cc_flw_glyph * gs1, * gs2;

  FLW_MUTEX_LOCK(flw_global_lock);

  fs = flw_fontidx2fontptr(font);
  gs1 = flw_glyphidx2glyphptr(fs, glyph1);
  gs2 = flw_glyphidx2glyphptr(fs, glyph2);

  *x = *y = 0.0f;

  if (fs->defaultfont == FALSE) {
    if (win32api) {
      cc_flww32_get_vector_kerning(fs->font, gs1->glyph, gs2->glyph, x, y);
    }
    else if (freetypelib) {
      cc_flwft_get_vector_kerning(fs->font, gs1->glyph, gs2->glyph, x, y);
    }
  }

  FLW_MUTEX_UNLOCK(flw_global_lock);
}

void
cc_flw_done_glyph(int font, unsigned int glyph)
{
  struct cc_flw_font * fs;
  struct cc_flw_glyph * gs;

  FLW_MUTEX_LOCK(flw_global_lock);

  fs = flw_fontidx2fontptr(font);
  gs = flw_glyphidx2glyphptr(fs, glyph);

  if (win32api && !fs->defaultfont) {
    cc_flww32_done_glyph(fs->font, gs->charidx);
  }
  else if (freetypelib && !fs->defaultfont) {
    cc_flwft_done_glyph(fs->font, gs->charidx);
  }

  fontstruct_rmglyph(fs, glyph, 1);

  FLW_MUTEX_UNLOCK(flw_global_lock);
}

struct cc_flw_bitmap *
cc_flw_get_bitmap(int font, unsigned int glyph)
{
  unsigned char * buf;
  struct cc_flw_font * fs;
  struct cc_flw_glyph * gs;
  struct cc_flw_bitmap * bm = NULL;
  unsigned int i;

  FLW_MUTEX_LOCK(flw_global_lock);

  fs = flw_fontidx2fontptr(font);
  gs = flw_glyphidx2glyphptr(fs, glyph);

  if (gs->bitmap) {
    bm = gs->bitmap;
    goto done;
  }

  if (win32api && !fs->defaultfont && !gs->fromdefaultfont) {
    bm = cc_flww32_get_bitmap(fs->font, gs->glyph);
  }
  else if (freetypelib && !fs->defaultfont && !gs->fromdefaultfont) {
    bm = cc_flwft_get_bitmap(fs->font, gs->glyph);
  }

  if (!bm) {
    /* glyph handle == char value in default font. &255 to avoid
       index out of range. */
    bm = get_default_bitmap(gs->glyph & 0xff);
  }
  else if (bm && bm->buffer) {
    buf = (unsigned char *)malloc(bm->pitch * bm->rows);
    /* Copy & reverse buffer to OpenGL "up" direction. */
    for (i = 0; i < bm->rows; i++) {
      (void)memcpy(buf + i*bm->pitch,
                   bm->buffer + (bm->rows-i-1) * bm->pitch,
                   bm->pitch);
    }
    free(bm->buffer);
    bm->buffer = buf;
  }

  gs->bitmap = bm;
 done:
  FLW_MUTEX_UNLOCK(flw_global_lock);
  return bm;
}

struct cc_flw_vector_glyph *
cc_flw_get_vector_glyph(int font, unsigned int glyph, float complexity)
{

  struct cc_flw_vector_glyph * vector_glyph = NULL;
  struct cc_flw_font * fs;

  FLW_MUTEX_LOCK(flw_global_lock);

  fs = flw_fontidx2fontptr(font);

  if (freetypelib) {
    vector_glyph = cc_flwft_get_vector_glyph(fs->font, glyph, complexity);
  }
  else if (win32api) {
    vector_glyph = cc_flww32_get_vector_glyph(fs->font, glyph, complexity);		
  }
  else {
    vector_glyph = NULL;
  }

  FLW_MUTEX_UNLOCK(flw_global_lock);
  return vector_glyph;

}

const float *
cc_flw_get_vector_glyph_coords(struct cc_flw_vector_glyph * vecglyph)
{
  if (freetypelib)
    return cc_flwft_get_vector_glyph_coords(vecglyph);
  else if (win32api) {
    return cc_flww32_get_vector_glyph_coords(vecglyph);
  } else {
    /* FIXME: Should one assert here instead? It should be impossible
       to call this method with an valid glyph and get NULL
       returned... Goes for the others below aswell (20030912
       handegar)
    */
    return NULL;
  }
}

const int *
cc_flw_get_vector_glyph_faceidx(struct cc_flw_vector_glyph * vecglyph)
{
  if (freetypelib)
    return cc_flwft_get_vector_glyph_faceidx(vecglyph);
  else if (win32api) {
    return cc_flww32_get_vector_glyph_faceidx(vecglyph);
  } else {
    return NULL;
  }
}

const int *
cc_flw_get_vector_glyph_edgeidx(struct cc_flw_vector_glyph * vecglyph)
{
  if (freetypelib)
    return cc_flwft_get_vector_glyph_edgeidx(vecglyph);
  else if (win32api) {
    return cc_flww32_get_vector_glyph_edgeidx(vecglyph);
  } else {
    return NULL;
  }
}


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
static SbBool glyph2d_initialized = FALSE;

/*
  Mutex lock for the static ang global font hash
*/
static void * glyph2d_fonthash_lock = NULL;

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

void
cc_glyph2d_initialize()
{
  if (!glyph2d_initialized) {
    CC_MUTEX_CONSTRUCT(glyph2d_fonthash_lock);
    GLYPH2D_MUTEX_LOCK(glyph2d_fonthash_lock);
    glyph2d_fonthash = cc_hash_construct(15, 0.75);
    GLYPH2D_MUTEX_UNLOCK(glyph2d_fonthash_lock);
    glyph2d_initialized = TRUE;
  }
}




cc_glyph2d * 
cc_glyph2d_getglyph(uint32_t character, const cc_font_specification * spec, float angle)
{

  void * val;
  cc_glyph2d * glyph;
  int fontidx;
  int glyphidx;
  struct cc_flw_bitmap * bm;
  struct cc_font_specification * newspec;
  int namelen = 0;

  assert(spec);
 

  GLYPH2D_MUTEX_LOCK(glyph2d_fonthash_lock);

  /* Has the glyph been created before? */
  if (cc_hash_get(glyph2d_fonthash, (unsigned long) character, &val)) {    
    glyph = (cc_glyph2d *) val;
    if (angle == glyph->angle) {
      if (glyph2d_specmatch(spec, glyph->fontspec)) {
        GLYPH2D_MUTEX_UNLOCK(glyph2d_fonthash_lock);         
        return glyph;
      }
    }
  } 
  
  /* build a new glyph struct with bitmap */    
  glyph = (cc_glyph2d *) malloc(sizeof(cc_glyph2d));
  
  /* FIXME: Must add family and style support (2Sep2003 handegar) */
  newspec = (cc_font_specification *) malloc(sizeof(cc_font_specification)); 
  assert(newspec);
  newspec->size = spec->size;
  newspec->name = cc_string_construct_new();
  cc_string_set_text(newspec->name, cc_string_get_text(spec->name));
  

  glyph->fontspec = newspec; 
  fontidx = cc_flw_get_font(cc_string_get_text(newspec->name), (int)(newspec->size), (int)(newspec->size));
  assert(fontidx >= 0);
  

  /* Should _always_ be able to get hold of a glyph -- if no glyph is
     available for a specific character, a default empty rectangle
     should be used.  -mortene. */
  glyphidx = cc_flw_get_glyph(fontidx, character);
  assert(glyphidx >= 0);

  if(angle != 0)
    cc_flw_set_font_rotation(fontidx, angle);
  
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
  
  /* Insert the new glyph into the hash-table using 'character' as key */
  cc_hash_put(glyph2d_fonthash, (unsigned long) character, glyph);
  
  GLYPH2D_MUTEX_UNLOCK(glyph2d_fonthash_lock);
  return glyph;
}

static SbBool 
glyph2d_specmatch(const cc_font_specification * spec1, 
                  const cc_font_specification * spec2)
{

  assert(spec1);
  assert(spec2);

  /*
  // FIXME: Add compare for family and style (20030902 handegar)
  */
  if ((!cc_string_compare(spec1->name, spec2->name)) &&
      (spec1->size == spec2->size)) {
    return TRUE;
  }
  else return FALSE;
  
}

void 
cc_glyph2d_getadvance(const cc_glyph2d * g, int * x, int * y)
{
  float advancex, advancey;
  cc_flw_get_advance(g->fontidx, g->glyphidx, &advancex, &advancey);

  *x = (int) advancex;
  *y = (int) advancey;
}

void 
cc_glyph2d_getkerning(const cc_glyph2d * left, const cc_glyph2d * right, int * x, int * y)
{
  float kx, ky;
  cc_flw_get_kerning(right->fontidx, left->glyphidx, right->glyphidx, &kx, &ky);

  *x = (int) kx;
  *y = (int) ky;
}

unsigned int 
cc_glyph2d_getwidth(const cc_glyph2d * g)
{
  return (int) g->width;
}

unsigned char * 
cc_glyph2d_getbitmap(const cc_glyph2d * g, int * size, int * offset)
{
  size[0] = g->bitmapwidth;
  size[1] = g->bitmapheight;
  offset[0] = g->bitmapoffsetx;
  offset[1] = g->bitmapoffsety;

  return g->bitmap;
}



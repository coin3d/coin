
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <float.h> /* FLT_MIN */

#include <string.h>
#include <assert.h>
#include <stdio.h>


#include <Inventor/C/base/hash.h>
#include <Inventor/C/base/hashp.h>
#include <Inventor/C/base/string.h>
#include <Inventor/C/threads/threadsutilp.h>

#include <Inventor/C/glue/freetype.h>

#include "fontlib_wrapper.h"
#include "freetype.h"
#include "glyph3d.h"
#include "../misc/defaultfonts.h"

static SbBool glyph3d_specmatch(const cc_font_specification * spec1, const cc_font_specification * spec2);
static void glyph3d_calcboundingbox(cc_glyph3d * g);

struct cc_glyph3d {
  int fontidx;
  int glyphidx;
  float width;
  float * bbox;
  cc_flw_vector_glyph * vectorglyph;
  cc_font_specification * fontspec;
};

static cc_hash * glyph3d_fonthash = NULL;
static int glyph3d_spaceglyphindices[] = { -1, -1 };
static float glyph3d_spaceglyphvertices[] = { 0, 0 };
static SbBool glyph3d_initialized = FALSE;

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

/* Mutex lock for the static ang global font hash */
static void * glyph3d_fonthash_lock = NULL;

/* Set '#if 1' to enable debug info to stderr when tracking mutex locking. */
#if 0
#define GLYPH3D_MUTEX_LOCK(m) \
  do { \
    (void)fprintf(stderr, "glyph3d mutex lock in %s\n", __func__); \
    CC_MUTEX_LOCK(m); \
  } while (0)
#define GLYPH3D_MUTEX_UNLOCK(m) \
  do { \
    (void)fprintf(stderr, "glyph3d mutex unlock in %s\n", __func__); \
    CC_MUTEX_UNLOCK(m); \
  } while (0)
#else
#define GLYPH3D_MUTEX_LOCK(m) CC_MUTEX_LOCK(m)
#define GLYPH3D_MUTEX_UNLOCK(m) CC_MUTEX_UNLOCK(m)
#endif

static void
cc_glyph3d_initialize()
{

  static SbBool initialized = FALSE;
  CC_MUTEX_CONSTRUCT(glyph3d_fonthash_lock);
  GLYPH3D_MUTEX_LOCK(glyph3d_fonthash_lock);
  
  if (initialized) {
    GLYPH3D_MUTEX_UNLOCK(glyph3d_fonthash_lock);
    return;
  }
  initialized = TRUE;
  
  glyph3d_fonthash = cc_hash_construct(15, 0.75);

  GLYPH3D_MUTEX_UNLOCK(glyph3d_fonthash_lock);  
}


cc_glyph3d *
cc_glyph3d_getglyph(uint32_t character, const cc_font_specification * spec)
{

  cc_glyph3d * glyph;
  int glyphidx;
  int fontidx;
  void * val;
  cc_font_specification * newspec;
  int namelen = 0;


  /* Beacuse this function is the entry point for glyph3d, the mutex
     is initialized here. */
  if (glyph3d_fonthash_lock == NULL) 
    cc_glyph3d_initialize();
  
  assert(spec);

  /* Has the glyph been created before? */
  GLYPH3D_MUTEX_LOCK(glyph3d_fonthash_lock);
  if (cc_hash_get(glyph3d_fonthash, (unsigned long) character, &val)) {
    glyph = (cc_glyph3d *) val;
    if (glyph3d_specmatch(spec, glyph->fontspec)) {
      GLYPH3D_MUTEX_UNLOCK(glyph3d_fonthash_lock);
      return glyph;
    }
  }

  /* build a new glyph struct */
  glyph = (cc_glyph3d *) malloc(sizeof(cc_glyph3d));

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

  glyph->glyphidx = glyphidx;
  glyph->fontidx = fontidx;
  glyph->bbox = (float *) malloc(sizeof(float) * 4);

  if (fontidx != 0)
    glyph->vectorglyph = cc_flw_get_vector_glyph(fontidx, character);
  else
    glyph->vectorglyph = NULL;


  /* Setup buildin default font if no character was found */
  if (glyph->vectorglyph == NULL) {
    glyph->vectorglyph = (struct cc_flw_vector_glyph *) malloc(sizeof(struct cc_flw_vector_glyph));

    if (character <= 32 || character >= 127) {

      /* FIXME: Charachers other than space, should be replaced with
         squares. (20030910 handegar) */

      /* treat all these characters as spaces*/
      glyph->vectorglyph->vertices = (float *) glyph3d_spaceglyphvertices;
      glyph->vectorglyph->faceindices = (int *) glyph3d_spaceglyphindices;
      glyph->vectorglyph->edgeindices = (int *) glyph3d_spaceglyphindices;
    }
    else {
      glyph->vectorglyph->vertices = (float *) coin_default3dfont_get_coords()[character-33];
      glyph->vectorglyph->faceindices = (int *) coin_default3dfont_get_faceidx()[character-33];
      glyph->vectorglyph->edgeindices = (int *) coin_default3dfont_get_edgeidx()[character-33];
    }

  }

  glyph3d_calcboundingbox(glyph);
  glyph->width = glyph->bbox[2] - glyph->bbox[0];

  cc_hash_put(glyph3d_fonthash, (unsigned long) character, glyph);

  GLYPH3D_MUTEX_UNLOCK(glyph3d_fonthash_lock);
  return glyph;
}

float *
cc_glyph3d_getcoords(const cc_glyph3d * g)
{
  return cc_flw_get_vector_glyph_coords(g->vectorglyph);
}

int *
cc_glyph3d_getfaceindices(const cc_glyph3d * g)
{
  return cc_flw_get_vector_glyph_faceidx(g->vectorglyph);
}

int *
cc_glyph3d_getedgeindices(const cc_glyph3d * g)
{
  return cc_flw_get_vector_glyph_edgeidx(g->vectorglyph);
}

int *
cc_glyph3d_getnextcwedge(const cc_glyph3d * g, int edgeidx)
{

  int findidx;
  int * ptr;
  int * edgeptr;
  int idx = edgeidx * 2;

  edgeptr = cc_glyph3d_getedgeindices(g);

  /* test for common case */
  if (edgeidx > 0) {
    if (edgeptr[idx] == edgeptr[idx-1])
      return &edgeptr[idx-2];
  }
  /* do a linear search */
  findidx = edgeptr[idx];
  ptr = edgeptr;
  while (*ptr >= 0) {
    if (ptr[1] == findidx) return ptr;
    ptr += 2;
  }

  return NULL;
}

int *
cc_glyph3d_getnextccwedge(const cc_glyph3d * g, int edgeidx)
{
  int findidx;
  int * ptr;
  int * edgeptr;  
  int idx = edgeidx * 2;
  
  edgeptr = cc_glyph3d_getedgeindices(g);

  /* test for common case */
  if (edgeptr[idx+1] == edgeptr[idx+2])
    return &edgeptr[idx+2];

  /* do a linear search */
  findidx = edgeptr[idx+1];
  ptr = edgeptr;
  while (*ptr >= 0) {
    if (*ptr == findidx) 
      return ptr;
    ptr += 2;
  }

  return NULL;

}


float
cc_glyph3d_getwidth(const cc_glyph3d * g)
{
  return g->width;
}

static void
glyph3d_calcboundingbox(cc_glyph3d * g)
{
  float * coordptr;
  int * edgeptr;

  g->bbox[0] = 0;
  g->bbox[1] = 0;
  g->bbox[2] = 0;
  g->bbox[3] = 0;

  coordptr = cc_glyph3d_getcoords(g);
  edgeptr = cc_glyph3d_getedgeindices(g);

  while ((*edgeptr >= 0) && (*edgeptr != -1)) {

    g->bbox[0] = MIN(coordptr[(*edgeptr)*2], g->bbox[0]);
    g->bbox[1] = MIN(coordptr[(*edgeptr)*2 + 1], g->bbox[1]);
    g->bbox[2] = MAX(coordptr[(*edgeptr)*2], g->bbox[2]);
    g->bbox[3] = MAX(coordptr[(*edgeptr)*2 + 1], g->bbox[3]);

    *edgeptr++;
  }

}

float *
cc_glyph3d_getboundingbox(const cc_glyph3d * g)
{
  return g->bbox;
}

void
cc_glyph3d_getadvance(const cc_glyph3d * g, float * x, float * y)
{
  float advancex, advancey;
  cc_flw_get_advance(g->fontidx, g->glyphidx, &advancex, &advancey);

  *x = advancex;
  *y = advancey;
}

void
cc_glyph3d_getkerning(const cc_glyph3d * left, const cc_glyph3d * right,
                      float * x, float * y)
{
  float kx, ky;
  cc_flw_get_kerning(right->fontidx, left->glyphidx, right->glyphidx, &kx, &ky);

  *x = kx;
  *y = ky;
}

static SbBool
glyph3d_specmatch(const cc_font_specification * spec1, 
                  const cc_font_specification * spec2)
{

  assert(spec1);
  assert(spec2);

  /* FIXME: Add compare for family and style (20030902 handegar) */
  if ((!cc_string_compare(spec1->name, spec2->name)) &&
      (spec1->size == spec2->size)) {
    return TRUE;
  }
  else return FALSE;

}

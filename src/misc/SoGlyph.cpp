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

/*!
  \class SoGlyph SoGlyph.h Inventor/misc/SoGlyph.h
  \brief The SoGlyph class is used to generate and reuse font glyph bitmaps and outlines.

  <b>This class is now obsolete, and will be removed from a later
  version of Coin.</b>
  
  SoGlyph is the public interface all text nodes (both built-in and
  extensions) should use to generate bitmaps and outlines for font
  glyphs. It maintains an internal cache of previously requested
  glyphs to avoid needless calls into the font library.
  
  Primer: a \e glyph is the graphical representation of a given
  character of a given font at a given size and orientation. It can be
  either a \e bitmap (pixel aligned with the viewport) or an \e
  outline (polygonal representation) that can be transformed or
  extruded like any other 3D geometry. Bitmaps are used by SoText2,
  while the other text nodes uses outlines.
  
  \COIN_CLASS_EXTENSION

  \since Coin 2.0

  \sa SoText2, SoText3, SoAsciiText
*/

// SoGlyph uses the Coin-internal font lib wrapper functions
// (cc_flw_*()) to provide bitmaps and outlines.
//
// FIXME: font support for outline glyphs. 200303?? preng.
  
#include <Inventor/misc/SoGlyph.h>

#include <Inventor/C/threads/threadsutilp.h>
#include <Inventor/C/tidbits.h>
#include <Inventor/C/tidbitsp.h>
#include <Inventor/SbName.h>
#include <Inventor/SbString.h>
#include <Inventor/SbVec2f.h>
#include <Inventor/SbVec2s.h>
#include <Inventor/lists/SbList.h>
#include <Inventor/elements/SoFontNameElement.h>
#include <Inventor/elements/SoFontSizeElement.h>
#include <Inventor/C/glue/fontlib_wrapper.h>
#include <stdlib.h>
#include <string.h>

// FIXME: when is this defined? Looks bogus -- default 2D and 3D fonts
// should always be present. 20030526 mortene.
#if !defined(COIN_NO_DEFAULT_3DFONT)
#include "../misc/defaultfonts.h"
#endif // COIN_NO_DEFAULT_3DFONT

class SoGlyphP {
public:
  SbVec2f * coords;
  SbBox2f bbox;
  int * faceidx;
  int * edgeidx;
  int refcount;
  float ymin, ymax;
  
  int fontidx;
  int glyphidx;
  float angle;
  SbVec2s size;
  unsigned int character;
  
  struct {
    unsigned int didalloccoords : 1;
    unsigned int didallocfaceidx : 1;
    unsigned int didallocedgeidx : 1;
    unsigned int didcalcbbox : 1;
  } flags;

  static SoGlyph * createSystemGlyph(const char character, const SbName & font);
  static SoGlyph * createSystemGlyph(const unsigned int character, SoState * state) {return NULL;};
};

#define PRIVATE(p) ((p)->pimpl)

/*!
  Constructor.
*/
SoGlyph::SoGlyph(void)
{
  PRIVATE(this) = new SoGlyphP();
  PRIVATE(this)->refcount = 0;
  PRIVATE(this)->flags.didalloccoords = 0;
  PRIVATE(this)->flags.didallocfaceidx  = 0;
  PRIVATE(this)->flags.didallocedgeidx = 0;
  PRIVATE(this)->flags.didcalcbbox = 0;
  PRIVATE(this)->coords = NULL;
  PRIVATE(this)->faceidx = NULL;
  PRIVATE(this)->edgeidx = NULL;
  PRIVATE(this)->ymin = 0.0f;
  PRIVATE(this)->ymax = 0.0f;
}

/*!
  Destructor.
*/
SoGlyph::~SoGlyph()
{
  if (PRIVATE(this)->flags.didalloccoords) delete [] PRIVATE(this)->coords;
  if (PRIVATE(this)->flags.didallocfaceidx) delete [] PRIVATE(this)->faceidx;
  if (PRIVATE(this)->flags.didallocedgeidx) delete [] PRIVATE(this)->edgeidx;
  delete PRIVATE(this);
}

/*!
  Should be called when a node no longer will use a glyph. Will
  free memory used by this glyph when it is no longer used by any node.
*/
void
SoGlyph::unref(void) const
{
  SoGlyph::unrefGlyph((SoGlyph*)this);
}

/*!
  Returns coordinates for this glyph.
*/
const SbVec2f *
SoGlyph::getCoords(void) const
{
  return PRIVATE(this)->coords;
}

/*!
  Returns face indices for this glyph.
*/
const int *
SoGlyph::getFaceIndices(void) const
{
  return PRIVATE(this)->faceidx;
}

/*!
  Returns edge indices for this glyph.
*/
const int *
SoGlyph::getEdgeIndices(void) const
{
  return PRIVATE(this)->edgeidx;
}

/*!
  Returns a pointer to the next clockwise edge. Returns NULL if
  none could be found.
*/
const int *
SoGlyph::getNextCWEdge(const int edgeidx) const
{
  int idx = edgeidx * 2;
  // test for common case
  if (edgeidx > 0) {
    if (PRIVATE(this)->edgeidx[idx] == PRIVATE(this)->edgeidx[idx-1])
      return &PRIVATE(this)->edgeidx[idx-2];
  }
  // do a linear search
  int findidx = PRIVATE(this)->edgeidx[idx];
  int * ptr = PRIVATE(this)->edgeidx;
  while (*ptr >= 0) {
    if (ptr[1] == findidx) return ptr;
    ptr += 2;
  }
  return NULL;
}

/*!
  Returns a pointer to the next counter clockwise edge.
  NULL if none could be found.
*/
const int *
SoGlyph::getNextCCWEdge(const int edgeidx) const
{
  int idx = edgeidx * 2;
  // test for common case
  if (PRIVATE(this)->edgeidx[idx+1] == PRIVATE(this)->edgeidx[idx+2])
    return &PRIVATE(this)->edgeidx[idx+2];
  // do a linear search
  int findidx = PRIVATE(this)->edgeidx[idx+1];
  int * ptr = PRIVATE(this)->edgeidx;
  while (*ptr >= 0) {
    if (*ptr == findidx) return ptr;
    ptr += 2;
  }
  return NULL;
}

/*!
  Convenience method which returns the exact width of the glyph.
*/
float
SoGlyph::getWidth(void) const
{
  const SbBox2f & box = this->getBoundingBox();
  return box.getMax()[0] - box.getMin()[0];
}

/*!
  Returns the bounding box of this glyph. This value is cached for performance.
*/
const SbBox2f &
SoGlyph::getBoundingBox(void) const
{
  // this method needs to be const, so cast away constness
  SoGlyph * thisp = (SoGlyph*) this;
  if (!PRIVATE(this)->flags.didcalcbbox) {
    PRIVATE(thisp)->bbox.makeEmpty();
    int *ptr = PRIVATE(this)->edgeidx;
    int idx = *ptr++;
    while (idx >= 0) {
      PRIVATE(thisp)->bbox.extendBy(PRIVATE(this)->coords[idx]);
      idx = *ptr++;
    }
    PRIVATE(thisp)->flags.didcalcbbox = 1;
  }
  return PRIVATE(this)->bbox;
}

/*!
  Sets the coordinates for this glyph. If \a numcoords > 0, the data
  will be copied before returing. If \a numcoords <= 0, \a coords will
  be used directly.
*/
void
SoGlyph::setCoords(SbVec2f *coords, int numcoords)
{
  if (PRIVATE(this)->flags.didalloccoords) delete [] PRIVATE(this)->coords;
  if (numcoords > 0) {
    PRIVATE(this)->coords = new SbVec2f[numcoords];
    memcpy(PRIVATE(this)->coords, coords, numcoords*sizeof(SbVec2f));
    PRIVATE(this)->flags.didalloccoords = 1;
  }
  else {
    PRIVATE(this)->coords = coords;
    PRIVATE(this)->flags.didalloccoords = 0;
  }
}

/*!
  Sets the face indices for this glyph. If \a numindices > 0, the
  data will be copied before returning. If \a numcoords <= 0,
  \a indices will be used directly.
*/
void
SoGlyph::setFaceIndices(int *indices, int numindices)
{
  if (PRIVATE(this)->flags.didallocfaceidx) delete [] PRIVATE(this)->faceidx;
  if (numindices > 0) {
    PRIVATE(this)->faceidx = new int[numindices];
    memcpy(PRIVATE(this)->faceidx, indices, numindices*sizeof(int));
    PRIVATE(this)->flags.didallocfaceidx = 1;
  }
  else {
    PRIVATE(this)->faceidx = indices;
    PRIVATE(this)->flags.didallocfaceidx = 0;
  }
}

/*!
  Sets the edge indices for this glyph. If \a numindices > 0, the
  data will be copied before returning. If \a numcoords <= 0,
  \a indices will be used directly.
*/
void
SoGlyph::setEdgeIndices(int *indices, int numindices)
{
  if (PRIVATE(this)->flags.didallocedgeidx) delete [] PRIVATE(this)->edgeidx;
  if (numindices > 0) {
    PRIVATE(this)->edgeidx = new int[numindices];
    memcpy(PRIVATE(this)->edgeidx, indices, numindices*sizeof(int));
    PRIVATE(this)->flags.didallocedgeidx = 1;
  }
  else {
    PRIVATE(this)->edgeidx = indices;
    PRIVATE(this)->flags.didallocedgeidx = 0;
  }
}



//
// static methods to handle glyph reusage.
//
// FIXME: use SbDict to look up glyphs a bit faster. pederb, 20000323
//

class coin_glyph_info {
public:
  coin_glyph_info() {
    this->character = 0;
    this->size = 0.0;
    this->glyph = NULL;
    this->angle = 0.0;
  }
  coin_glyph_info(const unsigned int character, const float size, const SbName &font, SoGlyph *glyph, const float angle)
    : character(character), size(size), font(font), glyph(glyph), angle(angle) {}
  
  // Note: bitmap glyphs have valid size, polygonal glyphs have size=-1.0
  SbBool matches(const unsigned int character, const float size, const SbName font, const float angle) {
    return (this->character == character) && (this->size == size) && (this->font == font) && (this->angle == angle);
  }
  
  // AIX native compiler xlC needs equality and inequality operators
  // to compile templates where these operators are referenced (even
  // if they are actually never used).
  
  SbBool operator==(const coin_glyph_info & gi) {
    return this->matches(gi.character, gi.size, gi.font, gi.angle) && this->glyph == gi.glyph;
  }
  SbBool operator!=(const coin_glyph_info & gi) {
    return !(*this == gi);
  }

  unsigned int character;
  float size;
  SbName font;
  SoGlyph * glyph;
  float angle;
};

static SbList <coin_glyph_info> * activeGlyphs = NULL;
static void * SoGlyph_mutex = NULL;

static void
SoGlyph_cleanup(void)
{
  delete activeGlyphs;
  CC_MUTEX_DESTRUCT(SoGlyph_mutex);
}

/*!
  Returns a character of the specified font, suitable for polygonal
  rendering.
*/
const SoGlyph *
SoGlyph::getGlyph(const char character, const SbName & font)
{
  // FIXME: the API and implementation of this class isn't consistent
  // with regard to the paraneters and variabls that are glyph codes
  // -- some places they are "char", other places "int", some places
  // signed, other places unsigned. Should audit and fix as much as
  // possible without breaking API and ABI compatibility. *sigh*
  // 20030611 mortene.

  // Similar code in start of getGlyph(..., state) - keep in sync.
  if (SoGlyph_mutex == NULL) {
    CC_MUTEX_CONSTRUCT(SoGlyph_mutex);
  }

  // FIXME: it would probably be a good idea to have a small LRU-type
  // glyph cache to avoid freeing glyphs too early. If for instance
  // the user creates a single SoText3 node which is used several
  // times in a graph with different fonts, glyphs will be freed and
  // recreated all the time. pederb, 20000324

  CC_MUTEX_LOCK(SoGlyph_mutex);

  if (activeGlyphs == NULL) {
    activeGlyphs = new SbList <coin_glyph_info>;
    coin_atexit((coin_atexit_f *)SoGlyph_cleanup, 0);
  }

  int i, n = activeGlyphs->getLength();
  for (i = 0; i < n; i++) {
    // Search for fontsize -1 to avoid getting a bitmap glyph.
    if ((*activeGlyphs)[i].matches(character, -1.0, font, 0.0)) break;
  }
  if (i < n) {
    SoGlyph *glyph = (*activeGlyphs)[i].glyph;
    PRIVATE(glyph)->refcount++;
    CC_MUTEX_UNLOCK(SoGlyph_mutex);
    return glyph;
  }

  SoGlyph * glyph = SoGlyphP::createSystemGlyph(character, font);

  // FIXME: don't think this is necessary, we should _always_ get a
  // glyph. If none exist in the font, we should eventually fall back
  // on making a square in the code we're calling into. Move the code
  // below to handle this deeper down into the call-stack. 20030527 mortene.
  if (glyph == NULL) { // no system font could be loaded
#if defined(COIN_NO_DEFAULT_3DFONT)
    // just create a square to render something
    glyph = new SoGlyph;
    static float dummycoords[] = {0.0f, 0.0f, 0.7f, 0.0f, 0.7f, 0.7f, 0.0f, 0.7f};
    static int dummyfaceidx[] = {0,1,2,0,2,3,-1};
    static int dummyedgeidx[] = {0,1,1,2,2,3,3,0,-1};
    glyph->setCoords((SbVec2f*)dummycoords);
    glyph->setFaceIndices(dummyfaceidx);
    glyph->setEdgeIndices(dummyedgeidx);
#else // ! COIN_NO_DEFAULT_3DFONT
    glyph = new SoGlyph;
    if (character <= 32 || character >= 127) {
      // treat all these characters as spaces
      static int spaceidx[] = { -1 };
      glyph->setCoords(NULL);
      glyph->setFaceIndices(spaceidx);
      glyph->setEdgeIndices(spaceidx);
      PRIVATE(glyph)->bbox.setBounds(SbVec2f(0.0f, 0.0f), SbVec2f(0.2f, 0.0f));
      PRIVATE(glyph)->flags.didcalcbbox = 1;
    }
    else {
      const int idx = character-33;
      glyph->setCoords((SbVec2f*)coin_default3dfont_get_coords()[idx]);
      glyph->setFaceIndices((int*)coin_default3dfont_get_faceidx()[idx]);
      glyph->setEdgeIndices((int*)coin_default3dfont_get_edgeidx()[idx]);
    }
#endif // COIN_NO_DEFAULT_3DFONT
  }
  // Use impossible font size to avoid mixing polygonal & bitmap glyphs.
  coin_glyph_info info(character, -1.0, font, glyph, 0.0);
  PRIVATE(glyph)->refcount++;
  activeGlyphs->append(info);
  CC_MUTEX_UNLOCK(SoGlyph_mutex);
  return glyph;
}

// private method that removed glyph from active list when deleted
void
SoGlyph::unrefGlyph(SoGlyph *glyph)
{
  CC_MUTEX_LOCK(SoGlyph_mutex);
  assert(activeGlyphs);
  assert(PRIVATE(glyph)->refcount > 0);
  PRIVATE(glyph)->refcount--;
  if (PRIVATE(glyph)->refcount == 0) {
    int i, n = activeGlyphs->getLength();
    for (i = 0; i < n; i++) {
      if ((*activeGlyphs)[i].glyph == glyph) break;
    }
    assert(i < n);
    activeGlyphs->removeFast(i);
    delete glyph;
  }
  CC_MUTEX_UNLOCK(SoGlyph_mutex);
}

/*!
  Returns a character of the specified font, suitable for bitmap
  rendering.  The size parameter overrides state's SoFontSizeElement
  (if != SbVec2s(0,0))
*/
const SoGlyph *
SoGlyph::getGlyph(SoState * state,
                  const unsigned int character,
                  const SbVec2s & size,
                  const float angle)
{
  assert(state);

  SbName state_name = SoFontNameElement::get(state);
  float state_size = SoFontSizeElement::get(state);
  if (state_name == SbName("")) {
    state_name = SbName("defaultFont");
    state_size = 12.0;
  }
  SbVec2s fontsize((short)state_size, (short)state_size);
  if (size != SbVec2s(0,0)) { fontsize = size; }
  
  // Similar code in start of getGlyph(..., fontname) - keep in sync.
  if (SoGlyph_mutex == NULL) {
    CC_MUTEX_CONSTRUCT(SoGlyph_mutex);
  }

  CC_MUTEX_LOCK(SoGlyph_mutex);
  
  if (activeGlyphs == NULL) {
    activeGlyphs = new SbList <coin_glyph_info>;
    coin_atexit((coin_atexit_f *)SoGlyph_cleanup, 0);
  }

  int i, n = activeGlyphs->getLength();
  for (i = 0; i < n; i++) {
    if ((*activeGlyphs)[i].matches(character, fontsize[0], state_name, angle)) break;
  }
  if (i < n) {
    SoGlyph *glyph = (*activeGlyphs)[i].glyph;
    PRIVATE(glyph)->refcount++;
    CC_MUTEX_UNLOCK(SoGlyph_mutex);
    return glyph;
  }

  // FIXME: use font style in addition to font name. preng 2003-03-03
  SbString fontname = state_name.getString();
  const int font =
    cc_flw_get_font(fontname.getString(), fontsize[0], fontsize[1]);
  // Should _always_ be able to get hold of a font.
  assert(font >= 0);

  cc_flw_set_font_rotation(font, angle);
  const int glyphidx = cc_flw_get_glyph(font, character);
  // Should _always_ be able to get hold of a glyph -- if no glyph is
  // available for a specific character, a default empty rectangle
  // should be used.  -mortene.
  assert(glyphidx >= 0);

  SoGlyph * g = new SoGlyph();
  PRIVATE(g)->fontidx = font;
  PRIVATE(g)->glyphidx = glyphidx;
  PRIVATE(g)->size = fontsize;
  PRIVATE(g)->angle = angle;
  PRIVATE(g)->character = character;
  coin_glyph_info info(character, fontsize[0], state_name, g, angle);
  PRIVATE(g)->refcount++;
  activeGlyphs->append(info);

  CC_MUTEX_UNLOCK(SoGlyph_mutex);
  return g;
}

// Pixel advance for this glyph.
SbVec2s
SoGlyph::getAdvance(void) const
{
  assert(PRIVATE(this)->fontidx >= 0 && PRIVATE(this)->glyphidx >= 0);

  float x, y;
  cc_flw_get_advance(PRIVATE(this)->fontidx, PRIVATE(this)->glyphidx, &x, &y);
  return SbVec2s((short)x, (short)y);
}

// Pixel kerning when rightglyph is placed to the right of this.
SbVec2s
SoGlyph::getKerning(const SoGlyph & rightglyph) const
{
  assert(PRIVATE(this)->fontidx >= 0 && PRIVATE(this)->glyphidx >= 0);
  assert(PRIVATE(&rightglyph)->fontidx >= 0 && PRIVATE(&rightglyph)->glyphidx >= 0);

  float x, y;
  cc_flw_get_kerning(PRIVATE(this)->fontidx,
                     PRIVATE(this)->glyphidx, PRIVATE(&rightglyph)->glyphidx,
                     &x, &y);
  return SbVec2s((short)x, (short)y);
}

/*!
  Bitmap for glyph. \a size and \a pos are return parameters.
  Antialiased bitmap graphics not yet supported.

  Note that this function may return \c NULL if the glyph has no
  visible pixels (like the space character).
*/
unsigned char *
SoGlyph::getBitmap(SbVec2s & size, SbVec2s & pos, const SbBool antialiased) const
{
  struct cc_flw_bitmap * bm =
    cc_flw_get_bitmap(PRIVATE(this)->fontidx, PRIVATE(this)->glyphidx);
  assert(bm);

  size[0] = bm->pitch * 8;
  size[1] = bm->rows;
  pos[0] = bm->bearingX;
  pos[1] = bm->bearingY;
  return bm->buffer;
}

// should handle platform-specific font loading
SoGlyph *
SoGlyphP::createSystemGlyph(const char character, const SbName & font)
{
  // FIXME: implement me somebody, please
  return NULL;
}

#undef PRIVATE

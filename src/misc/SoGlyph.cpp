/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2001 by Systems in Motion.  All rights reserved.
 *  
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  version 2 as published by the Free Software Foundation.  See the
 *  file LICENSE.GPL at the root directory of this source distribution
 *  for more details.
 *
 *  If you desire to use Coin with software that is incompatible
 *  licensewise with the GPL, and / or you would like to take
 *  advantage of the additional benefits with regard to our support
 *  services, please contact Systems in Motion about acquiring a Coin
 *  Professional Edition License.  See <URL:http://www.coin3d.org> for
 *  more information.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  <URL:http://www.sim.no>, <mailto:support@sim.no>
 *
\**************************************************************************/

/*!
  \class SoGlyph include/Inventor/misc/SoGlyph.h
  \brief The SoGlyph class is used to control and reuse font glyphs.

  This is an internal class.
*/

#include <Inventor/misc/SoGlyph.h>
#include <Inventor/lists/SbList.h>
#include <Inventor/SbVec2f.h>
#include <Inventor/SbName.h>

#include <string.h>
#include <stdlib.h>

#if !defined(COIN_NO_DEFAULT_3DFONT)
// our default font (misc/default3dfont.cpp)
extern float * coin_defaultfont_coords[];
extern int * coin_defaultfont_faceidx[];
extern int * coin_defaultfont_edgeidx[];
#endif // COIN_NO_DEFAULT_3DFONT

/*!
  Constructor.
*/
SoGlyph::SoGlyph()
{
  this->refcount = 0;
  this->flags.didalloccoords = 0;
  this->flags.didallocfaceidx  = 0;
  this->flags.didallocedgeidx = 0;
  this->flags.didcalcbbox = 0;
  this->coords = NULL;
  this->faceidx = NULL;
  this->edgeidx = NULL;
  this->ymin = 0.0f;
  this->ymax = 0.0f;
}

/*!
  Destructor.
*/
SoGlyph::~SoGlyph()
{
  if (this->flags.didalloccoords) delete [] this->coords;
  if (this->flags.didallocfaceidx) delete [] this->faceidx;
  if (this->flags.didallocedgeidx) delete [] this->edgeidx;
}

/*!
  Should be called when a node no longer will use a glyph. Will
  free memory used by this glyph when it is no longer used by any node.
*/
void
SoGlyph::unref() const
{
  SoGlyph::unrefGlyph((SoGlyph*)this);
}

/*!
  Returns coordinates for this glyph.
*/
const SbVec2f *
SoGlyph::getCoords(void) const
{
  return this->coords;
}

/*!
  Returns face indices for this glyph.
*/
const int *
SoGlyph::getFaceIndices(void) const
{
  return this->faceidx;
}

/*!
  Returns edge indices for this glyph.
*/
const int *
SoGlyph::getEdgeIndices(void) const
{
  return this->edgeidx;
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
    if (this->edgeidx[idx] == this->edgeidx[idx-1])
      return &this->edgeidx[idx-2];
  }
  // do a linear search
  int findidx = this->edgeidx[idx];
  int * ptr = this->edgeidx;
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
  if (this->edgeidx[idx+1] == this->edgeidx[idx+2])
    return &this->edgeidx[idx+2];
  // do a linear search
  int findidx = this->edgeidx[idx+1];
  int * ptr = this->edgeidx;
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
  if (!this->flags.didcalcbbox) {
    thisp->flags.didcalcbbox = 1;
    thisp->bbox.makeEmpty();
    int *ptr = this->edgeidx;
    int idx = *ptr++;
    while (idx >= 0) {
      thisp->bbox.extendBy(this->coords[idx]);
      idx = *ptr++;
    }
  }
  return this->bbox;
}

/*!
  Sets the coordinates for this glyph. If \a numcoords > 0, the data
  will be copied before returing. If \a numcoords <= 0, \a coords will
  be used directly.
*/
void
SoGlyph::setCoords(SbVec2f *coords, int numcoords)
{
  if (this->flags.didalloccoords) delete [] this->coords;
  if (numcoords > 0) {
    this->coords = new SbVec2f[numcoords];
    memcpy(this->coords, coords, numcoords*sizeof(SbVec2f));
    this->flags.didalloccoords = 1;
  }
  else {
    this->coords = coords;
    this->flags.didalloccoords = 0;
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
  if (this->flags.didallocfaceidx) delete [] this->faceidx;
  if (numindices > 0) {
    this->faceidx = new int[numindices];
    memcpy(this->faceidx, indices, numindices*sizeof(int));
    this->flags.didallocfaceidx = 1;
  }
  else {
    this->faceidx = indices;
    this->flags.didallocfaceidx = 0;
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
  if (this->flags.didallocedgeidx) delete [] this->edgeidx;
  if (numindices > 0) {
    this->edgeidx = new int[numindices];
    memcpy(this->edgeidx, indices, numindices*sizeof(int));
    this->flags.didallocedgeidx = 1;
  }
  else {
    this->edgeidx = indices;
    this->flags.didallocedgeidx = 0;
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
    this->glyph = NULL;
  }
  coin_glyph_info(const char character, const SbName &font, SoGlyph *glyph)
    : character(character), font(font), glyph(glyph) {}

  SbBool matches(const char character, const SbName font) {
    return (this->character == character) && (this->font == font);
  }

  // AIX native compiler xlC needs equality and inequality operators
  // to compile templates where these operators are referenced (even
  // if they are actually never used).

  SbBool operator==(const coin_glyph_info & gi) {
    return this->matches(gi.character, gi.font) && this->glyph == gi.glyph;
  }
  SbBool operator!=(const coin_glyph_info & gi) {
    return !(*this == gi);
  }

  char character;
  SbName font;
  SoGlyph *glyph;
};

static SbList <coin_glyph_info> *activeGlyphs = NULL;

void SoGlyph_cleanup(void)
{
  delete activeGlyphs;
}

/*!
  Returns a character of the specified font.
*/
const SoGlyph *
SoGlyph::getGlyph(const char character, const SbName &font)
{
  // FIXME: it would probably be a good idea to have a small LRU-type
  // glyph cache to avoid freeing glyphs too early. If for instance the user
  // creates a single SoText3 node which is used several times in a
  // graph with differnet fonts, glyphs will be freed and recreated
  // all the time. pederb, 20000324

  if (activeGlyphs == NULL) {
    activeGlyphs = new SbList <coin_glyph_info>;
    atexit(SoGlyph_cleanup);
  }
  int i, n = activeGlyphs->getLength();
  for (i = 0; i < n; i++) {
    if ((*activeGlyphs)[i].matches(character, font)) break;
  }
  if (i < n) {
    SoGlyph *glyph = (*activeGlyphs)[i].glyph;
    glyph->refcount++;
    return glyph;
  }

  SoGlyph *glyph = SoGlyph::createSystemGlyph(character, font);
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
      glyph->bbox.setBounds(SbVec2f(0.0f, 0.0f), SbVec2f(0.2f, 0.0f));
      glyph->flags.didcalcbbox = 1;
    }
    else {
      glyph->setCoords((SbVec2f*)coin_defaultfont_coords[character-33]);
      glyph->setFaceIndices(coin_defaultfont_faceidx[character-33]);
      glyph->setEdgeIndices(coin_defaultfont_edgeidx[character-33]);
    }
#endif // COIN_NO_DEFAULT_3DFONT
  }
  coin_glyph_info info(character, font, glyph);
  glyph->refcount++;
  activeGlyphs->append(info);
  return glyph;
}

// should handle platform-specific font loading
SoGlyph *
SoGlyph::createSystemGlyph(const char character, const SbName &font)
{
  // FIXME: implement me somebody, please
  return NULL;
}

// private method that removed glyph from active list when deleted
void
SoGlyph::unrefGlyph(SoGlyph *glyph)
{
  assert(activeGlyphs);
  assert(glyph->refcount > 0);
  glyph->refcount--;
  if (glyph->refcount == 0) {
    int i, n = activeGlyphs->getLength();
    for (i = 0; i < n; i++) {
      if ((*activeGlyphs)[i].glyph == glyph) break;
    }
    assert(i < n);
    activeGlyphs->removeFast(i);
    delete glyph;
  }
}

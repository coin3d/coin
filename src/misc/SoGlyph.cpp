/**************************************************************************\
 *
 *  Copyright (C) 1998-2000 by Systems in Motion.  All rights reserved.
 *
 *  This file is part of the Coin library.
 *
 *  This file may be distributed under the terms of the Q Public License
 *  as defined by Troll Tech AS of Norway and appearing in the file
 *  LICENSE.QPL included in the packaging of this file.
 *
 *  If you want to use Coin in applications not covered by licenses
 *  compatible with the QPL, you can contact SIM to aquire a
 *  Professional Edition license for Coin.
 *
 *  Systems in Motion AS, Prof. Brochs gate 6, N-7030 Trondheim, NORWAY
 *  http://www.sim.no/ sales@sim.no Voice: +47 22114160 Fax: +47 67172912
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
  this->coords = NULL;
  this->faceidx = NULL;
  this->edgeidx = NULL;
  this->width = 0.0f;
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
  Returns the width of the glyph.
*/
float
SoGlyph::getWidth(void) const
{
  if (this->width > 0.0f) return this->width;  
  int *ptr = this->edgeidx;
  int idx = *ptr++;
  float maxval = 0.0f;
  while (idx >= 0) {
    SbVec2f v = this->coords[idx];
    if (v[0] > maxval) maxval = v[0];
    idx = *ptr++;
  }
  maxval += 0.05f; // need some space between letters
  ((SoGlyph*)this)->width = maxval;
  return maxval;
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
  char character;
  SbName font;
  SoGlyph *glyph;
};

static SbList <coin_glyph_info> *activeGlyphs = NULL;

void cleanup(void)
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
    atexit(cleanup);
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
      glyph->width = 0.2f;
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


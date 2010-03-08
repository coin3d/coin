/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) by Kongsberg Oil & Gas Technologies.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Kongsberg Oil & Gas Technologies
 *  about acquiring a Coin Professional Edition License.
 *
 *  See http://www.coin3d.org/ for more information.
 *
 *  Kongsberg Oil & Gas Technologies, Bygdoy Alle 5, 0257 Oslo, NORWAY.
 *  http://www.sim.no/  sales@sim.no  coin-support@coin3d.org
 *
\**************************************************************************/

/*!
  \class SoGlyphCache include/Inventor/SoGlyphCache.h
  The SoGlyphClass is used to cache glyphs.

  \internal
*/

#include "caches/SoGlyphCache.h"

#include <cassert>

#include <Inventor/lists/SbList.h>
#include <Inventor/elements/SoFontNameElement.h>
#include <Inventor/elements/SoFontSizeElement.h>
#include <Inventor/elements/SoComplexityElement.h>
#include <Inventor/errors/SoDebugError.h>

#include "tidbitsp.h"

class SoGlyphCacheP {
public:
  SbList <cc_glyph2d*> glyphlist2d;
  SbList <cc_glyph3d*> glyphlist3d;
  cc_font_specification * fontspec;
};

#define PRIVATE(obj) ((obj)->pimpl)

SoGlyphCache::SoGlyphCache(SoState * state)
  : SoCache(state)
{
  PRIVATE(this) = new SoGlyphCacheP;
  PRIVATE(this)->fontspec = NULL;

#if COIN_DEBUG
  if (coin_debug_caching_level() > 0) {
    SoDebugError::postInfo("SoGlyphCache::SoGlyphCache",
                           "Cache constructed: %p", this);

  }
#endif // debug
}

SoGlyphCache::~SoGlyphCache()
{
#if COIN_DEBUG
  if (coin_debug_caching_level() > 0) {
    SoDebugError::postInfo("SoGlyphCache::~SoGlyphCache",
                           "Cache destructed: %p", this);

  }
#endif // debug

  int i;
  this->readFontspec(NULL);
  for (i = 0; i < PRIVATE(this)->glyphlist2d.getLength(); i++) {
    cc_glyph2d_unref(PRIVATE(this)->glyphlist2d[i]);
  }
  for (i = 0; i < PRIVATE(this)->glyphlist3d.getLength(); i++) {
    cc_glyph3d_unref(PRIVATE(this)->glyphlist3d[i]);
  }
  delete PRIVATE(this);
}

/*
  Add a glyph that is created using cc_glyph2d_ref(). The cache will
  call cc_glyph2d_unref() when destructed.
*/
void
SoGlyphCache::addGlyph(cc_glyph2d * glyph)
{
  PRIVATE(this)->glyphlist2d.append(glyph);
}

/*
  Add a glyph that is created using cc_glyph2d_ref(). The cache will
  call cc_glyph2d_unref() when destructed.
*/

void
SoGlyphCache::addGlyph(cc_glyph3d * glyph)
{
  PRIVATE(this)->glyphlist3d.append(glyph);
}

/*!
  Read and store current fontspec. Will create cache dependencies
  since some elements are read. We can't read the fontspec in the
  constructor since we need to update SoCacheElement before reading
  the elements.
*/
void
SoGlyphCache::readFontspec(SoState * state)
{
  if (PRIVATE(this)->fontspec) {
    cc_fontspec_clean(PRIVATE(this)->fontspec);
    delete PRIVATE(this)->fontspec;
    PRIVATE(this)->fontspec = NULL;
  }
  if (state) {
    PRIVATE(this)->fontspec = new cc_font_specification;
    cc_fontspec_construct(PRIVATE(this)->fontspec,
                          SoFontNameElement::get(state).getString(),
                          SoFontSizeElement::get(state),
                          SoComplexityElement::get(state));
  }
}

/*!
  Returns the cached fontspec.
*/
const cc_font_specification *
SoGlyphCache::getCachedFontspec(void) const
{
  assert(PRIVATE(this)->fontspec);
  return PRIVATE(this)->fontspec;
}


#undef PRIVATE

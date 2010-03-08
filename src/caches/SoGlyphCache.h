#ifndef COIN_SOGLYPHCACHE_H
#define COIN_SOGLYPHCACHE_H

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

#ifndef COIN_INTERNAL
#error this is a private header file
#endif /* !COIN_INTERNAL */

// *************************************************************************

#include <Inventor/caches/SoCache.h>
#include <Inventor/SbVec3f.h>
#include <Inventor/SbVec4f.h>
#include <Inventor/SbVec2f.h>
#include "../src/fonts/glyph2d.h"
#include "../src/fonts/glyph3d.h"
#include "../src/fonts/fontspec.h"

class SoGlyphCacheP;
class SoState;

// *************************************************************************

class SoGlyphCache : public SoCache {
  typedef SoCache inherited;

public:
  SoGlyphCache(SoState * state);
  virtual ~SoGlyphCache();

  void readFontspec(SoState * state);
  const cc_font_specification * getCachedFontspec(void) const;

  void addGlyph(cc_glyph2d * glyph);
  void addGlyph(cc_glyph3d * glyph);

private:
  friend class SoGlyphCacheP;
  SoGlyphCacheP * pimpl;
};

// *************************************************************************

#endif // !COIN_SOGLYPHCACHE_H

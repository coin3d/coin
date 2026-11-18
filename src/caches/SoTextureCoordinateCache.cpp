/**************************************************************************\
 * Copyright (c) Kongsberg Oil & Gas Technologies AS
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 * 
 * Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 * 
 * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 * 
 * Neither the name of the copyright holder nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
\**************************************************************************/

/*!
  \class SoTextureCoordinateCache SoTextureCoordinateCache.h Inventor/caches/SoTextureCoordinateCache.h
  \brief The SoTextureCoordinateCache class is used to cache generated texture coordinates.

  \ingroup coin_caches
*/

#include <Inventor/caches/SoTextureCoordinateCache.h>
#include <Inventor/lists/SbList.h>
#include <Inventor/SbBox3f.h>
#include <Inventor/SbVec2f.h>
#include <cmath>
#include <cfloat>

namespace {
float coin_cache_ratio(float point, float minimum, float size,
                       double extent, bool finitebox)
{
  const float numerator = point-minimum;
  const float legacy = numerator/size;
  if (finitebox && std::isfinite(point) &&
      (!std::isfinite(size) || !std::isfinite(numerator))) {
    // A finite Float size is part of the historical mapping: do not
    // recompute it more accurately just because the numerator overflowed.
    const double denominator = std::isfinite(size) ? double(size) : extent;
    const double result = (double(point)-minimum)/denominator;
    if (std::isfinite(result) && std::fabs(result) <= FLT_MAX) return float(result);
  }
  return legacy;
}
}


#ifndef DOXYGEN_SKIP_THIS

class SoTextureCoordinateCacheP {
public:
  SbList <SbVec2f> texCoords;
};

#endif // DOXYGEN_SKIP_THIS

#define PRIVATE(obj) ((obj)->pimpl)

/*!
  Constructor.
*/
SoTextureCoordinateCache::SoTextureCoordinateCache(SoState * const state)
  : SoCache(state)
{
  PRIVATE(this) = new SoTextureCoordinateCacheP;
}

/*!
  Destructor.
*/
SoTextureCoordinateCache::~SoTextureCoordinateCache()
{
  delete PRIVATE(this);
}

/*!
  Generates texture coordinates based on the bounding box of the
  geometry. This is usually called default texture coordinates
  in OIV.
*/
void
SoTextureCoordinateCache::generate(const SbBox3f & bbox,
                                   const SbVec3f * vertices,
                                   const int numvertices)
{
  // FIXME: Support 3D texture coordinates. This functionality
  // is more or less a duplicate of that in SoTextureCoordinateBundle
  // (kintel 20020203)
  float sizes[3];
  float minvalues[3];
  double ordering[3];
  int   offsets[3];

  bbox.getSize(sizes[0], sizes[1], sizes[2]);
  minvalues[0] = bbox.getMin()[0];
  minvalues[1] = bbox.getMin()[1];
  minvalues[2] = bbox.getMin()[2];
  offsets[0] = 0;
  offsets[1] = 1;
  offsets[2] = 2;

  bool finitebox = !bbox.isEmpty();
  for (int i = 0; i < 3; ++i) {
    if (!std::isfinite(bbox.getMin()[i]) || !std::isfinite(bbox.getMax()[i]) ||
        bbox.getMax()[i] < bbox.getMin()[i]) finitebox = false;
  }
  for (int i = 0; i < 3; ++i) {
    ordering[i] = sizes[i];
    if (finitebox && !std::isfinite(sizes[i]))
      ordering[i] = double(bbox.getMax()[i])-bbox.getMin()[i];
  }

  // bubblesort :-)
  if (ordering[0] < ordering[1]) {
    SbSwap(ordering[0], ordering[1]);
    SbSwap(sizes[0], sizes[1]);
    SbSwap(minvalues[0], minvalues[1]);
    SbSwap(offsets[0], offsets[1]);
  }
  if (ordering[1] < ordering[2]) {
    SbSwap(ordering[1], ordering[2]);
    SbSwap(sizes[1], sizes[2]);
    SbSwap(minvalues[1], minvalues[2]);
    SbSwap(offsets[1], offsets[2]);
  }
  if (ordering[0] < ordering[1]) {
    SbSwap(ordering[0], ordering[1]);
    SbSwap(sizes[0], sizes[1]);
    SbSwap(minvalues[0], minvalues[1]);
    SbSwap(offsets[0], offsets[1]);
  }

  float s, t;
  for (int i = 0; i < numvertices; i++) {
    s = coin_cache_ratio(vertices[i][offsets[0]], minvalues[0], sizes[0],
                         ordering[0], finitebox);
    t = coin_cache_ratio(vertices[i][offsets[1]], minvalues[1], sizes[1],
                         ordering[1], finitebox);
    // expand list array as needed
    if (i >= PRIVATE(this)->texCoords.getLength()) PRIVATE(this)->texCoords.append(SbVec2f());
    PRIVATE(this)->texCoords[i].setValue(s, t);
  }

  // fit list array in case we used to have more items than now
  PRIVATE(this)->texCoords.truncate(numvertices);
}

/*!
  Returns the generated texture coordinates.
*/
const SbVec2f *
SoTextureCoordinateCache::get(void) const
{
  return PRIVATE(this)->texCoords.getArrayPtr();
}

/*!
  Returns the number of generated texture coordinates.
*/
int
SoTextureCoordinateCache::getNum(void) const
{
  return PRIVATE(this)->texCoords.getLength();
}

#undef PRIVATE

/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2000 by Systems in Motion. All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public License
 *  version 2.1 as published by the Free Software Foundation. See the
 *  file LICENSE.LGPL at the root directory of the distribution for
 *  more details.
 *
 *  If you want to use Coin for applications not compatible with the
 *  LGPL, please contact SIM to acquire a Professional Edition license.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  http://www.sim.no support@sim.no Voice: +47 22114160 Fax: +47 22207097
 *
\**************************************************************************/

/*!
  \class SoTextureCoordinateCache include/Inventor/SoTextureCoordinateCache.h
  The SoTextureCoordinateClass is used to cache generated texture coordinates.
*/

#include <Inventor/caches/SoTextureCoordinateCache.h>

/*!
  Constructor.
*/
SoTextureCoordinateCache::SoTextureCoordinateCache(SoState * const state)
  : SoCache(state)
{
}

/*!
  Destructor.
*/
SoTextureCoordinateCache::~SoTextureCoordinateCache()
{

}

/*!
  Generates texture coordinates based on the bounding box of the
  geometry. This is usually called default texture coordinates
  in OIV.
*/
void
SoTextureCoordinateCache::generate(const SbBox3f &bbox,
                                   const SbVec3f *vertices,
                                   const int numvertices)
{
  float sizes[3];
  float minvalues[3];
  int   offsets[3];

  bbox.getSize(sizes[0], sizes[1], sizes[2]);
  minvalues[0] = bbox.getMin()[0];
  minvalues[1] = bbox.getMin()[1];
  minvalues[2] = bbox.getMin()[2];
  offsets[0] = 0;
  offsets[1] = 1;
  offsets[2] = 2;

  // bubblesort :-)
  if (sizes[0] < sizes[1]) {
    SbSwap(sizes[0], sizes[1]);
    SbSwap(minvalues[0], minvalues[1]);
    SbSwap(offsets[0], offsets[1]);
  }
  if (sizes[1] < sizes[2]) {
    SbSwap(sizes[1], sizes[2]);
    SbSwap(minvalues[1], minvalues[2]);
    SbSwap(offsets[1], offsets[2]);
  }
  if (sizes[0] < sizes[1]) {
    SbSwap(sizes[0], sizes[1]);
    SbSwap(minvalues[0], minvalues[1]);
    SbSwap(offsets[0], offsets[1]);
  }

  float s, t;
  for (int i = 0; i < numvertices; i++) {
    s = vertices[i][offsets[0]];
    t = vertices[i][offsets[1]];
    s -= minvalues[0];
    t -= minvalues[1];
    s /= sizes[0];
    t /= sizes[1];
    // expand list array as needed
    if (i >= this->texCoords.getLength()) this->texCoords.append(SbVec2f());
    this->texCoords[i].setValue(s, t);
  }

  // fit list array in case we used to have more items than now
  this->texCoords.truncate(numvertices);
}

/*!
  Returns the generated texture coordinates.
*/
const SbVec2f *
SoTextureCoordinateCache::get() const
{
  return this->texCoords.getArrayPtr();
}

/*!
  Returns the number of generated texture coordinates.
*/
int
SoTextureCoordinateCache::getNum() const
{
  return this->texCoords.getLength();
}

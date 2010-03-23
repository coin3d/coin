#ifndef COIN_VBO_CACHE_H
#define COIN_VBO_CACHE_H

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

#include <Inventor/caches/SoCache.h>

class SoVBOCacheP;
class SoVBO;
class SoVertexArrayIndexer;

class SoVBOCache : public SoCache {
  typedef SoCache inherited;
public:
  SoVBOCache(SoState * state);
  virtual ~SoVBOCache();

  SoVertexArrayIndexer * getVertexArrayIndexer(const SbBool createifnull = TRUE);
  SoVBO * getCoordVBO(const SbBool createifnull = TRUE);
  SoVBO * getNormalVBO(const SbBool createifnull = TRUE);
  SoVBO * getColorVBO(const SbBool createifnull = TRUE);
  SoVBO * getTexCoordVBO(const int unit, const SbBool createifnull = TRUE);
  
private:
  SoVBOCacheP * pimpl;
};



#endif // COIN_VBO_CACHE_H

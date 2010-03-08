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
  \class SoVBOCache SoVBOCache.h
  
  \brief The SoVBOCache is used to organize pointers to SoVBO and SoVertexArrayIndexer instances.
  
*/

#include "caches/SoVBOCache.h"
#include "rendering/SoVBO.h"
#include "rendering/SoVertexArrayIndexer.h"
#include <Inventor/lists/SbList.h>

class SoVBOCacheP {
public:
  SoVBOCacheP(void) {
    this->vaindexer = NULL;
    this->coordvbo = NULL;
    this->colorvbo = NULL;
    this->normalvbo = NULL;
  }
  ~SoVBOCacheP() {
    delete this->vaindexer;
    delete this->coordvbo;
    delete this->colorvbo;
    delete this->normalvbo;
    
    for (int i = 0; i < this->texcoordvbo.getLength(); i++) {
      delete this->texcoordvbo[i];
    }
  }
  
  SoVertexArrayIndexer * vaindexer;
  SoVBO * coordvbo;
  SoVBO * colorvbo;
  SoVBO * normalvbo;
  SbList <SoVBO*> texcoordvbo;
};

/*!
  Constructor.
*/
SoVBOCache::SoVBOCache(SoState * state)
  : SoCache(state)
{
  this->pimpl = new SoVBOCacheP();
}

/*!
  Destructor.
*/
SoVBOCache::~SoVBOCache()
{
  delete this->pimpl;
}

/*!
  Returns the vertex array indexer. If \a createifnull is TRUE, a
  indexer will be created if it doesn't exist.
*/
SoVertexArrayIndexer * 
SoVBOCache::getVertexArrayIndexer(const SbBool createifnull)
{
  if (createifnull && (this->pimpl->vaindexer == NULL)) {
    this->pimpl->vaindexer = new SoVertexArrayIndexer;
  }
  return this->pimpl->vaindexer;
}

/*!
  Returns the coordinate VBO instance. If \a createifnull is TRUE, a
  VBO instance will be created if it doesn't exist.
*/
SoVBO * 
SoVBOCache::getCoordVBO(const SbBool createifnull)
{
  if (createifnull && (this->pimpl->coordvbo == NULL)) {
    this->pimpl->coordvbo = new SoVBO;
  }
  return this->pimpl->coordvbo;
}

/*!
  Returns the normal vertex VBO instance. If \a createifnull is TRUE, a
  VBO instance will be created if it doesn't exist.
*/

SoVBO * 
SoVBOCache::getNormalVBO(const SbBool createifnull)
{
  if (createifnull && (this->pimpl->normalvbo == NULL)) {
    this->pimpl->normalvbo = new SoVBO;
  }
  return this->pimpl->normalvbo;
}

/*!
  Returns the color VBO instance. If \a createifnull is TRUE, a
  VBO instance will be created if it doesn't exist.
*/
SoVBO * 
SoVBOCache::getColorVBO(const SbBool createifnull)
{
  if (createifnull && (this->pimpl->colorvbo == NULL)) {
    this->pimpl->colorvbo = new SoVBO;
  }
  return this->pimpl->colorvbo;
}

/*!
  Returns the texture coordinate VBO instance. If \a createifnull is TRUE, a
  VBO instance will be created if it doesn't exist.
*/
SoVBO * 
SoVBOCache::getTexCoordVBO(const int unit, const SbBool createifnull)
{
  if (createifnull) {
    while (this->pimpl->texcoordvbo.getLength() <= unit) {
      this->pimpl->texcoordvbo.append(NULL);
    }
    if (this->pimpl->texcoordvbo[unit] == NULL) {
      this->pimpl->texcoordvbo[unit] = new SoVBO;
    }
  }
  if (this->pimpl->texcoordvbo.getLength() > unit) {
    return this->pimpl->texcoordvbo[unit];
  }
  return NULL;
}

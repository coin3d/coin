#ifndef COIN_SOSHAPE_BUMPRENDER_H
#define COIN_SOSHAPE_BUMPRENDER_H

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

#include <Inventor/SbVec3f.h>
#include <Inventor/SbVec2f.h>
#include <Inventor/SbVec2s.h>
#include <Inventor/lists/SbList.h>
#include <Inventor/SoPrimitiveVertex.h>
#include <Inventor/C/glue/gl.h>

#include "misc/SbHash.h"

// *************************************************************************

class SoState;
class SoLight;
class SoGLImage;
class SbMatrix;
class SoPrimitiveVertexCache;

// *************************************************************************

// FIXME: inherit from SoCache to avoid regenerating everything every frame

class soshape_bumprender {
public:
  soshape_bumprender(void);
  ~soshape_bumprender();

  void calcTangentSpace(const SoPrimitiveVertexCache * cache);
  void renderBump(SoState * state,
                  const SoPrimitiveVertexCache * cache,
                  SoLight * light, const SbMatrix & toobjectspace);
  void renderBumpSpecular(SoState * state,
                          const SoPrimitiveVertexCache * cache,
                          SoLight * light, const SbMatrix & toobjectspace);
  void renderNormal(SoState * state, const SoPrimitiveVertexCache * cache);

private:

  void initLight(SoLight * light, const SbMatrix & m);
  void calcTSBCoords(const SoPrimitiveVertexCache * cache, SoLight * light);
  SbVec3f getLightVec(const SbVec3f & v) const;
  void initPrograms(const cc_glglue * glue, SoState * state);
  void initDiffusePrograms(const cc_glglue * glue, SoState * state);

  void soshape_diffuseprogramdeletion(unsigned long key, void * value);
  void soshape_specularprogramdeletion(unsigned long key, void * value);

  struct spec_programidx {
    const cc_glglue * glue;
    GLuint dirlight;
    GLuint pointlight;
    GLuint fragment;
  };

  struct diffuse_programidx {
    const cc_glglue * glue;
    GLuint pointlight; // Pointlight diffuse rendering not implemented as a program yet.
    GLuint dirlight;
    GLuint normalrendering;
  };

  SbList <SbVec3f> cubemaplist;
  SbList <SbVec3f> tangentlist;

  SbVec3f lightvec;
  SbBool ispointlight;

  typedef SbHash<int, struct diffuse_programidx *> ContextId2DiffuseStruct;
  ContextId2DiffuseStruct diffuseprogramdict;

  typedef SbHash<int, struct spec_programidx *> ContextId2SpecStruct;
  ContextId2SpecStruct specularprogramdict;

  GLuint fragmentprogramid;
  GLuint dirlightvertexprogramid;
  GLuint pointlightvertexprogramid;
  SbBool programsinitialized;

  GLuint normalrenderingvertexprogramid;
  GLuint diffusebumpdirlightvertexprogramid;
  SbBool diffuseprogramsinitialized;
};

#endif // COIN_SOSHAPE_BUMPRENDER

#ifndef COIN_SONORMALBUNDLE_H
#define COIN_SONORMALBUNDLE_H

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

#include <Inventor/bundles/SoBundle.h>
#include <Inventor/misc/SoNormalGenerator.h>
#include <Inventor/elements/SoGLNormalElement.h>

class SoNormalBundleP;

class COIN_DLL_API SoNormalBundle : public SoBundle {
public:
  SoNormalBundle(SoAction * action, SbBool forrendering);
  ~SoNormalBundle();

  SbBool shouldGenerate(int numneeded);
  void initGenerator(int initnum = 100);
  void beginPolygon(void);
  void polygonVertex(const SbVec3f & v);
  void endPolygon(void);
  
  void triangle(const SbVec3f & p1,
                const SbVec3f & p2,
                const SbVec3f & p3);
  void generate(int startindex = 0,
                SbBool addtostate = TRUE);
  const SbVec3f * getGeneratedNormals(void) const;
  int getNumGeneratedNormals(void) const;
  void set(int32_t num, const SbVec3f * normals);
  const SbVec3f & get(int index) const;
  void send(int index) const;
  
  SoNormalGenerator * generator; // SoINTERNAL public
  
private:
  const SoNormalElement * elem;
  const SoGLNormalElement * glelem;
  SoNode * node;
  SoNormalBundleP * pimpl; // for future use
};


#endif // !COIN_SONORMALBUNDLE_H

/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2002 by Systems in Motion.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Systems in Motion about acquiring
 *  a Coin Professional Edition License.
 *
 *  See <URL:http://www.coin3d.org> for  more information.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no>.
 *
\**************************************************************************/

#include "soshape_bigtexture.h"
#include <Inventor/SbClip.h>
#include <Inventor/SbPlane.h>
#include <Inventor/SoPrimitiveVertex.h>
#include <Inventor/SbVec2f.h>
#include <Inventor/misc/SoGLBigImage.h>
#include <Inventor/SbBox3f.h>
#include <Inventor/SbBox2f.h>
#include <Inventor/elements/SoCullElement.h>
#include <Inventor/bundles/SoMaterialBundle.h>
#include <Inventor/nodes/SoShape.h>
#include <Inventor/C/tidbits.h> // coin_atexit()
#include <stdlib.h>

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <Inventor/system/gl.h>

soshape_bigtexture::soshape_bigtexture(void)
{
  this->clipper = NULL;
  this->pvlist = NULL;
  this->regions = NULL;
  this->numallocregions = 0;
}

soshape_bigtexture::~soshape_bigtexture()
{
  delete[] this->regions;
  delete this->clipper;
  
  if (this->pvlist) {
    int n = this->pvlist->getLength();
    for (int i = 0; i < n; i++) {
      delete (*this->pvlist)[i];
    }
    delete this->pvlist;
  }
}

void
soshape_bigtexture::beginShape(SoGLBigImage * image,
                               const float quality)
{
  this->image = image;
  this->quality = quality;
  this->pvlistcnt = 0;

  int num = image->initSubImages(SbVec2s(256, 256));
  this->numregions = num;

  if (this->clipper == NULL) {
    this->clipper = new SbClip(clipcb, this);
    this->pvlist = new SbList <SoPrimitiveVertex*>;
    this->regions = new bt_region[num];
    this->numallocregions = num;
  }
  if (num > this->numallocregions) {
    delete[] this->regions;
    this->regions = new bt_region[num];
    this->numallocregions = num;
  }
  for (int i = 0; i < num; i++) {
    bt_region & reg = this->regions[i];
    reg.facelist.truncate(0);
    reg.pvlist.truncate(0);
    this->image->handleSubImage(i,
                                reg.start,
                                reg.end,
                                reg.tcmul);
    reg.planes[0] =
      SbPlane(SbVec3f(1.0f, 0.0f, 0.0f), reg.start[0]);
    reg.planes[1] =
      SbPlane(SbVec3f(0.0f, 1.0f, 0.0f), reg.start[1]);
    reg.planes[2] =
      SbPlane(SbVec3f(-1.0f, 0.0f, 0.0f), -reg.end[0]);
    reg.planes[3] =
      SbPlane(SbVec3f(0.0f, -1.0f, 0.0f), -reg.end[1]);
  }
}

SbBool
soshape_bigtexture::endShape(SoState * state,
                             SoShape * shape,
                             SoMaterialBundle & mb)
{
  const int numreg = this->numregions;
  for (int i = 0; i < numreg; i++) {
    int numv, j;

    const bt_region & reg = this->regions[i];
    int numface = reg.facelist.getLength();
    if (numface == 0) continue;

    numv = reg.pvlist.getLength();
    SbBox3f bbox;
    for (j = 0; j < numv; j++) {
      bbox.extendBy(reg.pvlist[j]->getPoint());
    }
    SbVec2s rectsize;
    shape->getScreenSize(state, bbox, rectsize);
    this->image->applySubImage(state, i, this->quality, rectsize);
    int vcnt = 0;
    for (j = 0; j < numface; j++) {
      glBegin(GL_TRIANGLE_FAN);
      numv = reg.facelist[j];
      for (int k = 0; k < numv; k++) {
        SoPrimitiveVertex * v = reg.pvlist[vcnt++];
        SbVec4f tc = v->getTextureCoords();
        tc[0] -= reg.start[0];
        tc[1] -= reg.start[1];
        tc[0] /= (reg.end[0]-reg.start[0]);
        tc[1] /= (reg.end[1]-reg.start[1]);
        glTexCoord4fv(tc.getValue());
        glNormal3fv(v->getNormal().getValue());
        mb.send(v->getMaterialIndex(), TRUE);
        glVertex3fv(v->getPoint().getValue());
      }
      glEnd();
    }
  }  
  // return TRUE if all textures were created in the correct resolution
  return ! this->image->exceededChangeLimit();
}

void
soshape_bigtexture::triangle(SoState * state,
                             const SoPrimitiveVertex * v1,
                             const SoPrimitiveVertex * v2,
                             const SoPrimitiveVertex * v3)
{
  const SbVec4f & tc1 = v1->getTextureCoords();
  const SbVec4f & tc2 = v2->getTextureCoords();
  const SbVec4f & tc3 = v3->getTextureCoords();

  SbBox2f bbox;
  bbox.extendBy(SbVec2f(tc1[0], tc1[1]));
  bbox.extendBy(SbVec2f(tc2[0], tc2[1]));
  bbox.extendBy(SbVec2f(tc3[0], tc3[1]));
  SbBox2f regbbox;

  for (int i = 0; i < this->numregions; i++) {
    bt_region & reg = this->regions[i];
    regbbox.makeEmpty();
    regbbox.extendBy(reg.start);
    regbbox.extendBy(reg.end);

    // check if there is a chance for an intersection
    if (regbbox.intersect(bbox)) {
      this->clipper->reset();

      // need copies
      SoPrimitiveVertex * pv1 = this->get_new_pv();
      *pv1 = *v1;
      SoPrimitiveVertex * pv2 = this->get_new_pv();
      *pv2 = *v2;
      SoPrimitiveVertex * pv3 = this->get_new_pv();
      *pv3 = *v3;

      this->clipper->addVertex(SbVec3f(tc1[0], tc1[1],0.0f), pv1);
      this->clipper->addVertex(SbVec3f(tc2[0], tc2[1],0.0f), pv2);
      this->clipper->addVertex(SbVec3f(tc3[0], tc3[1],0.0f), pv3);

      this->clipper->clip(reg.planes[0]);
      this->clipper->clip(reg.planes[1]);
      this->clipper->clip(reg.planes[2]);
      this->clipper->clip(reg.planes[3]);

      const int numv = this->clipper->getNumVertices();
      if (numv >= 3) {
        int j;
        SbBox3f obox;
        for (j = 0; j < numv; j++) {
          SoPrimitiveVertex * v = (SoPrimitiveVertex*) this->clipper->getVertexData(j);
          obox.extendBy(v->getPoint());
        }
        if (!SoCullElement::cullTest(state, obox)) {
          reg.facelist.append(numv);
          for (j = 0; j < numv; j++) {
            reg.pvlist.append((SoPrimitiveVertex*) this->clipper->getVertexData(j));
          }
        }
      }
    }
  }
}

SoPrimitiveVertex *
soshape_bigtexture::get_new_pv(void)
{
  if (this->pvlistcnt < this->pvlist->getLength())
    return (*this->pvlist)[this->pvlistcnt++];
  else {
    SoPrimitiveVertex * pv = new SoPrimitiveVertex;
    this->pvlistcnt++;
    this->pvlist->append(pv);
    return pv;
  }
}

void *
soshape_bigtexture::clipcb(const SbVec3f & v0, void * vdata0,
                           const SbVec3f & v1, void * vdata1,
                           const SbVec3f & newvertex,
                           void * userdata)
{
  soshape_bigtexture * thisp = (soshape_bigtexture*) userdata;

  SoPrimitiveVertex * pv0 = (SoPrimitiveVertex*) vdata0;
  SoPrimitiveVertex * pv1 = (SoPrimitiveVertex*) vdata1;

  float dist = (v1-v0).length();
  float newdist = (newvertex-v0).length();
  if (dist == 0.0f) newdist = 0.0f;
  else newdist /= dist;

  SoPrimitiveVertex * pv = thisp->get_new_pv();
  pv->setPoint(pv0->getPoint() + (pv1->getPoint()-pv0->getPoint()) * newdist);
  pv->setTextureCoords(SbVec2f(newvertex[0], newvertex[1]));
  pv->setNormal(pv0->getNormal() + (pv1->getNormal()-pv0->getNormal()) * newdist);
  pv->setMaterialIndex(pv0->getMaterialIndex());
  return pv;
}



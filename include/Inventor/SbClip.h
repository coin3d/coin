/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2001 by Systems in Motion. All rights reserved.
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

#ifndef COIN_SBCLIP_H
#define COIN_SBCLIP_H
 
#include <Inventor/lists/SbList.h>
#include <Inventor/SbVec3f.h>
#include <stddef.h>

class SbPlane;
class SbVec2f;

typedef void * SbClipCallback(const SbVec3f & v0, void * vdata0, 
                              const SbVec3f & v1, void * vdata1,
                              const SbVec3f & newvertex,
                              void * userdata);

class COIN_DLL_API SbClip {
public:
  SbClip(SbClipCallback * callback = NULL, void * userdata = NULL);
  
  void addVertex(const SbVec3f &v, void * vdata = NULL);  
  void reset(void);

  void clip(const SbPlane & plane);

  int getNumVertices(void) const;
  void getVertex(const int idx, SbVec3f & v, void ** vdata = NULL) const; 
  void * getVertexData(const int idx) const;
  
private:
  class SbClipData {
  public:
    SbClipData(void) {}
    SbClipData(const SbVec3f & v, void * data) 
      : vertex(v),
        data(data) {}
  public:
    void get(SbVec3f &v, void *& data) {
      v = this->vertex;
      data = this->data;
    }

    SbVec3f vertex;
    void * data;
  };

  SbClipCallback * callback;
  void * cbdata;
  SbList <SbClipData> array[2];
  int curr;
  void outputVertex(const SbVec3f &v, void * data);
};


#endif // COIN_SBCLIP_H



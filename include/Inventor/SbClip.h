#ifndef COIN_SBCLIP_H
#define COIN_SBCLIP_H
 
/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2001 by Systems in Motion.  All rights reserved.
 *  
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  version 2 as published by the Free Software Foundation.  See the
 *  file LICENSE.GPL at the root directory of this source distribution
 *  for more details.
 *
 *  If you desire to use Coin with software that is incompatible
 *  licensewise with the GPL, and / or you would like to take
 *  advantage of the additional benefits with regard to our support
 *  services, please contact Systems in Motion about acquiring a Coin
 *  Professional Edition License.  See <URL:http://www.coin3d.org> for
 *  more information.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  <URL:http://www.sim.no>, <mailto:support@sim.no>
 *
\**************************************************************************/

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
  // FIXME: this is to avoid a complaint from Doxygen, which has a bug
  // that makes it spit out a warning on undocumented *private*
  // structs and classes. Bug has been reported to
  // <doxygen-develop@lists.sourceforge.net> at 2002-07-30 by mortene.
#ifndef DOXYGEN_SKIP_THIS
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
#endif // DOXYGEN_SKIP_THIS

  SbClipCallback * callback;
  void * cbdata;
  SbList <SbClipData> array[2];
  int curr;
  void outputVertex(const SbVec3f &v, void * data);
};


#endif // !COIN_SBCLIP_H

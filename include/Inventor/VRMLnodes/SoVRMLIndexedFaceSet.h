/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 2001 by Systems in Motion. All rights reserved.
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

#ifndef COIN_SOVRMLINDEXEDFACESET_H
#define COIN_SOVRMLINDEXEDFACESET_H

#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/VRMLnodes/SoVRMLIndexedShape.h>
#include <Inventor/fields/SoSFBool.h>
#include <Inventor/fields/SoSFFloat.h>

#undef SO_END_FACE_INDEX
#define SO_END_FACE_INDEX -1

class SoVRMLIndexedFaceSetP;

class COIN_DLL_API SoVRMLIndexedFaceSet : public SoVRMLIndexedShape
{
  typedef SoVRMLIndexedShape inherited;
  SO_NODE_HEADER(SoVRMLIndexedFaceSet);

public:
  static void initClass(void);
  SoVRMLIndexedFaceSet(void);

  SoSFBool ccw;
  SoSFBool solid;
  SoSFBool convex;
  SoSFFloat creaseAngle;

  virtual void GLRender( SoGLRenderAction * action );
  virtual void getPrimitiveCount( SoGetPrimitiveCountAction * action );

  virtual SbBool generateDefaultNormals(SoState * s, SoNormalBundle * nb );
  virtual SbBool generateDefaultNormals(SoState * state, SoNormalCache * nc);

protected:
  virtual ~SoVRMLIndexedFaceSet();

  virtual void generatePrimitives( SoAction * action );


private:

  enum Binding {
    OVERALL,
    NONE = OVERALL,
    PER_FACE,
    PER_FACE_INDEXED,
    PER_VERTEX,
    PER_VERTEX_INDEXED
  };

  Binding findMaterialBinding(void) const;
  Binding findNormalBinding(void) const;

  virtual void notify(SoNotList * list);
  
  SbBool useConvexCache(SoAction * action, 
                        const SbVec3f * normals, 
                        const int32_t * nindices, 
                        const SbBool normalsfromcache);
  
  SoVRMLIndexedFaceSetP * pimpl;
};

#endif // ! COIN_SOVRMLINDEXEDFACESET_H

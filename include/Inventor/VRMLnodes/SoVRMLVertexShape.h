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

#ifndef COIN_SOVRMLVERTEXSHAPE_H
#define COIN_SOVRMLVERTEXSHAPE_H

#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/VRMLnodes/SoVRMLGeometry.h>
#include <Inventor/fields/SoSFNode.h>
#include <Inventor/fields/SoSFBool.h>
#include <Inventor/elements/SoMaterialBindingElement.h>

class SoNormalCache;
class SoNormalBundle;
class SoState;
class SoCoordinateElement;
class SoVRMLVertexShapeP;

class COIN_DLL_API SoVRMLVertexShape : public SoVRMLGeometry
{
  typedef SoVRMLGeometry inherited;
  SO_NODE_ABSTRACT_HEADER(SoVRMLVertexShape);

public:
  static void initClass(void);

  SoSFNode coord;
  SoSFNode texCoord;
  SoSFNode normal;
  SoSFNode color;
  SoSFBool colorPerVertex;
  SoSFBool normalPerVertex;

  enum Binding {
    OVERALL             = SoMaterialBindingElement::OVERALL,
    PER_PART            = SoMaterialBindingElement::PER_PART,
    PER_PART_INDEXED    = SoMaterialBindingElement::PER_PART_INDEXED,
    PER_FACE            = SoMaterialBindingElement::PER_FACE,
    PER_FACE_INDEXED    = SoMaterialBindingElement::PER_FACE_INDEXED,
    PER_VERTEX          = SoMaterialBindingElement::PER_VERTEX,
    PER_VERTEX_INDEXED  = SoMaterialBindingElement::PER_VERTEX_INDEXED
  };

  virtual SbBool generateDefaultNormals(SoState * s, SoNormalBundle * nb);
  virtual SbBool generateDefaultNormals(SoState * s, SoNormalCache * nc);

  virtual void doAction(SoAction * action);
  virtual void GLRender(SoGLRenderAction * action);
  virtual void getBoundingBox(SoGetBoundingBoxAction * action);
  virtual void callback(SoCallbackAction * action);
  virtual void pick(SoPickAction * action);

  virtual void notify(SoNotList * list);

protected:
  SoVRMLVertexShape(void);
  virtual ~SoVRMLVertexShape();

  virtual SbBool shouldGLRender(SoGLRenderAction * action);

  void setNormalCache(SoState * s, int numNormals, SbVec3f * normals);
  SoNormalCache * getNormalCache(void) const;

  SoNormalCache * generateAndReadLockNormalCache(SoState * const state);
  void getVertexData(SoState * state,
                     const SoCoordinateElement *& coords,
                     const SbVec3f *& normals,
                     const SbBool neednormals);

  void readLockNormalCache(void);
  void readUnlockNormalCache(void);

private:

  void writeLockNormalCache(void);
  void writeUnlockNormalCache(void);
  
  SoVRMLVertexShapeP * pimpl;
}; // class SoVRMLVertexShape

#endif // ! COIN_SOVRMLVERTEXSHAPE_H

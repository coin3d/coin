#ifndef COIN_SOCALLBACKACTION_H
#define COIN_SOCALLBACKACTION_H

/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2003 by Systems in Motion.  All rights reserved.
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
 *  Systems in Motion, Teknobyen, Abels Gate 5, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no>.
 *
\**************************************************************************/

// FIXME: Support getTextureFilename[s]() ? (kintel 20011205)

#include <Inventor/actions/SoAction.h>
#include <Inventor/actions/SoSubAction.h>
#include <Inventor/elements/SoDecimationTypeElement.h>
#include <Inventor/lists/SbList.h>
#include <Inventor/nodes/SoComplexity.h>
#include <Inventor/nodes/SoDrawStyle.h>
#include <Inventor/nodes/SoLightModel.h>
#include <Inventor/nodes/SoMaterialBinding.h>
#include <Inventor/nodes/SoNormalBinding.h>
#include <Inventor/nodes/SoPickStyle.h>
#include <Inventor/nodes/SoShapeHints.h>
#include <Inventor/nodes/SoTexture2.h>
#include <Inventor/nodes/SoTextureCoordinateBinding.h>
#include <Inventor/nodes/SoUnits.h>

class SbColor;
class SbMatrix;
class SbVec2f;
class SbVec2s;
class SbVec3f;
class SbVec4f;
class SbViewVolume;
class SoCallbackAction;
class SoCallbackData;
class SoPrimitiveVertex;
class SoShape;
class SbViewportRegion;

typedef void SoTriangleCB(void * userdata, SoCallbackAction * action,
                          const SoPrimitiveVertex * v1,
                          const SoPrimitiveVertex * v2,
                          const SoPrimitiveVertex * v3);

typedef void SoLineSegmentCB(void * userdata, SoCallbackAction * action,
                             const SoPrimitiveVertex * v1,
                             const SoPrimitiveVertex * v2);

typedef void SoPointCB(void * userdata, SoCallbackAction * action,
                       const SoPrimitiveVertex * v);


class COIN_DLL_API SoCallbackAction : public SoAction {
  typedef SoAction inherited;

  SO_ACTION_HEADER(SoCallbackAction);

public:
  SoCallbackAction(void);
  SoCallbackAction(const SbViewportRegion & vp);

  virtual ~SoCallbackAction();

  static void initClass(void);

  void setViewportRegion(const SbViewportRegion & vp);

  enum Response { CONTINUE,  ABORT, PRUNE };

  typedef Response SoCallbackActionCB(void * userdata,
                                      SoCallbackAction * action,
                                      const SoNode * node);

  void addPreCallback(const SoType type, SoCallbackActionCB * cb, void * userdata);
  void addPostCallback(const SoType type, SoCallbackActionCB * cb, void * userdata);

  void addPreTailCallback(SoCallbackActionCB * cb, void * userdata);
  void addPostTailCallback(SoCallbackActionCB * cb, void * userdata);

  void addTriangleCallback(const SoType type, SoTriangleCB * cb, void * userdata);
  void addLineSegmentCallback(const SoType type, SoLineSegmentCB * cb, void * userdata);
  void addPointCallback(const SoType type, SoPointCB * cb, void * userdata);

  SoDecimationTypeElement::Type getDecimationType(void) const;
  float getDecimationPercentage(void) const;
  float getComplexity(void) const;
  SoComplexity::Type getComplexityType(void) const;
  int32_t getNumCoordinates(void) const;
  const SbVec3f & getCoordinate3(const int index) const;
  const SbVec4f & getCoordinate4(const int index) const;
  SoDrawStyle::Style getDrawStyle(void) const;
  unsigned short getLinePattern(void) const;
  float getLineWidth(void) const;
  float getPointSize(void) const;
  const SbName & getFontName(void) const;
  float getFontSize(void) const;
  SoLightModel::Model getLightModel(void) const;
  const SbVec3f & getLightAttenuation(void) const;
  void getMaterial(SbColor & ambient, SbColor & diffuse,
                   SbColor & specular, SbColor & emission,
                   float & shininess, float & transparency,
                   const int index = 0) const;
  SoMaterialBinding::Binding getMaterialBinding(void) const;
  uint32_t getNumNormals(void) const;
  const SbVec3f & getNormal(const int index) const;
  SoNormalBinding::Binding getNormalBinding(void) const;
  int32_t getNumProfileCoordinates(void) const;
  const SbVec2f & getProfileCoordinate2(const int index) const;
  const SbVec3f & getProfileCoordinate3(const int index) const;
  const SoNodeList & getProfile(void) const;
  SoShapeHints::VertexOrdering getVertexOrdering(void) const;
  SoShapeHints::ShapeType getShapeType(void) const;
  SoShapeHints::FaceType getFaceType(void) const;
  float getCreaseAngle(void) const;
  int32_t getNumTextureCoordinates(void) const;
  const SbVec2f & getTextureCoordinate2(const int index) const;
  const SbVec3f & getTextureCoordinate3(const int index) const;
  const SbVec4f & getTextureCoordinate4(const int index) const;
  SoTextureCoordinateBinding::Binding getTextureCoordinateBinding(void) const;
  const SbColor & getTextureBlendColor(void) const;
  const unsigned char * getTextureImage(SbVec2s & size, int & numcomps) const;
  const unsigned char * getTextureImage(SbVec3s & size, int & numcomps) const;
  const SbMatrix & getTextureMatrix(void) const;
  SoTexture2::Model getTextureModel(void) const;
  SoTexture2::Wrap getTextureWrapS(void) const;
  SoTexture2::Wrap getTextureWrapT(void) const;
  SoTexture2::Wrap getTextureWrapR(void) const;
  const SbMatrix & getModelMatrix(void) const;
  SoUnits::Units getUnits(void) const;
  float getFocalDistance(void) const;
  const SbMatrix & getProjectionMatrix(void) const;
  const SbMatrix & getViewingMatrix(void) const;
  const SbViewVolume & getViewVolume(void) const;
  const SbViewportRegion & getViewportRegion(void) const;
  SoPickStyle::Style getPickStyle(void) const;
  int32_t getSwitch(void) const;

  Response getCurrentResponse(void) const;
  void invokePreCallbacks(const SoNode * const node);
  void invokePostCallbacks(const SoNode * const node);
  void invokeTriangleCallbacks(const SoShape * const shape,
                               const SoPrimitiveVertex * const v1,
                               const SoPrimitiveVertex * const v2,
                               const SoPrimitiveVertex * const v3);
  void invokeLineSegmentCallbacks(const SoShape * const shape,
                                  const SoPrimitiveVertex * const v1,
                                  const SoPrimitiveVertex * const v2);
  void invokePointCallbacks(const SoShape * const shape,
                            const SoPrimitiveVertex * const v);

  SbBool shouldGeneratePrimitives(const SoShape * shape) const;

  virtual SoNode * getCurPathTail(void);
  void setCurrentNode(SoNode * const node);

protected:
  virtual void beginTraversal(SoNode * node);

private:
  void commonConstructor(void);
  class SoCallbackActionP * pimpl;
  friend class SoCallbackActionP;
};

#endif // !COIN_SOCALLBACKACTION_H

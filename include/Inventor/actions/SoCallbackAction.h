/**************************************************************************\
 *
 *  Copyright (C) 1998-1999 by Systems in Motion.  All rights reserved.
 *
 *  This file is part of the Coin library.
 *
 *  This file may be distributed under the terms of the Q Public License
 *  as defined by Troll Tech AS of Norway and appearing in the file
 *  LICENSE.QPL included in the packaging of this file.
 *
 *  If you want to use Coin in applications not covered by licenses
 *  compatible with the QPL, you can contact SIM to aquire a
 *  Professional Edition license for Coin.
 *
 *  Systems in Motion AS, Prof. Brochs gate 6, N-7030 Trondheim, NORWAY
 *  http://www.sim.no/ sales@sim.no Voice: +47 22114160 Fax: +47 67172912
 *
\**************************************************************************/

#ifndef __SOCALLBACKACTION_H__
#define __SOCALLBACKACTION_H__

#include <Inventor/confdep.h>
#if defined(COIN_EXCLUDE_SOCALLBACKACTION)
#error "This file is excluded from Coin through the configuration system!"
#endif // COIN_EXCLUDE_SOCALLBACKACTION

#include <Inventor/actions/SoAction.h>

#include <Inventor/nodes/SoComplexity.h>
#include <Inventor/nodes/SoShapeHints.h>
#include <Inventor/nodes/SoLightModel.h>
#include <Inventor/nodes/SoMaterialBinding.h>
#include <Inventor/nodes/SoNormalBinding.h>
#include <Inventor/nodes/SoPickStyle.h>
#include <Inventor/nodes/SoDrawStyle.h>
#include <Inventor/nodes/SoTexture2.h>
#include <Inventor/nodes/SoUnits.h>
#include <Inventor/nodes/SoTextureCoordinateBinding.h>
#include <Inventor/lists/SbList.h>
#include <stddef.h> // for NULL

class SbViewVolume;
class SbMatrix;
class SbVec2f;
class SbVec3f;
class SbVec4f;
class SoCallbackAction;
class SoPrimitiveVertex;
class SbColor;
class SoShape;
class SbVec2s;
class SoCallbackData;

typedef void SoTriangleCB(void * userData,
                          SoCallbackAction * action,
                          const SoPrimitiveVertex * v1,
                          const SoPrimitiveVertex * v2,
                          const SoPrimitiveVertex * v3);

typedef void SoLineSegmentCB(void * userData, SoCallbackAction * action,
                             const SoPrimitiveVertex * v1,
                             const SoPrimitiveVertex * v2);

typedef void SoPointCB(void * userData, SoCallbackAction * action,
                       const SoPrimitiveVertex * v);


class SoCallbackAction : public SoAction {
  typedef SoAction inherited;

//$ BEGIN TEMPLATE ActionHeader(SoCallbackAction)
private:
  static SoType classTypeId;

public:
  virtual SoType getTypeId(void) const;
  static SoType getClassTypeId(void);

protected:
  virtual const SoEnabledElementsList & getEnabledElements(void) const;
  static SoEnabledElementsList * enabledElements;
  static SoActionMethodList * methods;

public:
  static void addMethod(const SoType type, SoActionMethod method);
  static void enableElement(const SoType type, const int stackIndex);

  static void initClass(void);

  virtual ~SoCallbackAction();
//$ END TEMPLATE ActionHeader

public:
  SoCallbackAction(void);

  enum Response {
    CONTINUE,
    ABORT,
    PRUNE
  };

  typedef Response SoCallbackActionCB(void * userData,
                                      SoCallbackAction * action,
                                      const SoNode * node);

  // setting callbacks
  void addPreCallback(const SoType type,
                      SoCallbackActionCB * cb,
                      void * userdata);
  void addPostCallback(const SoType type,
                       SoCallbackActionCB * cb,
                       void * userdata);

  void addPreTailCallback(SoCallbackActionCB * cb,
                          void * userdata);
  void addPostTailCallback(SoCallbackActionCB * cb,
                           void * userdata);

  void addTriangleCallback(const SoType type,
                           SoTriangleCB * cb,
                           void * userdata);
  void addLineSegmentCallback(const SoType type,
                              SoLineSegmentCB * cb,
                              void * userdata);
  void addPointCallback(const SoType type,
                        SoPointCB * cb,
                        void * userdata);

  // access elements
  float getComplexity() const;
  SoComplexity::Type getComplexityType() const;
  int32_t getNumCoordinates() const;
  const SbVec3f & getCoordinate3(const int index) const;
  const SbVec4f & getCoordinate4(const int index) const;
  SoDrawStyle::Style getDrawStyle() const;
  unsigned short getLinePattern() const;
  float getLineWidth() const;
  float getPointSize() const;
  SbName getFontName() const;
  float getFontSize() const;
  SoLightModel::Model getLightModel() const;
  const SbVec3f & getLightAttenuation() const;
  void getMaterial(SbColor & ambient, SbColor & diffuse,
                   SbColor & specular, SbColor & emission,
                   float & shininess, float
                   & transparency, const int index = 0) const;
  SoMaterialBinding::Binding getMaterialBinding() const;
  long getNumNormals() const;
  const SbVec3f & getNormal(const int index) const;
  SoNormalBinding::Binding getNormalBinding() const;
  int32_t getNumProfileCoordinates() const;
  const SbVec2f & getProfileCoordinate2(const int index) const;
  const SbVec3f & getProfileCoordinate3(const int index) const;
  const SoNodeList & getProfile() const;
  SoShapeHints::VertexOrdering getVertexOrdering() const;
  SoShapeHints::ShapeType getShapeType() const;
  SoShapeHints::FaceType getFaceType() const;
  float getCreaseAngle() const;
  int32_t getNumTextureCoordinates() const;
  const SbVec2f & getTextureCoordinate2(const int index) const;
  const SbVec4f & getTextureCoordinate4(const int index) const;
  SoTextureCoordinateBinding::Binding getTextureCoordinateBinding() const;
  const SbColor & getTextureBlendColor() const;
  const unsigned char * getTextureImage(SbVec2s & size, int & numComps) const;
  const SbMatrix & getTextureMatrix() const;
  SoTexture2::Model getTextureModel() const;
  SoTexture2::Wrap getTextureWrapS() const;
  SoTexture2::Wrap getTextureWrapT() const;
  const SbMatrix & getModelMatrix() const;
  SoUnits::Units getUnits() const;
  float getFocalDistance() const;
  const SbMatrix & getProjectionMatrix() const;
  const SbMatrix & getViewingMatrix() const;
  const SbViewVolume & getViewVolume() const;
  SoPickStyle::Style getPickStyle() const;
  int32_t getSwitch() const;


public: // extender

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

public: // internal
  virtual SoNode * getCurPathTail();
  void setCurrentNode(SoNode * const node);

protected:
  virtual void beginTraversal(SoNode * node);


private:
  Response response;
  SoNode * currentNode;

  SbList <SoCallbackData *> preCB;
  SbList <SoCallbackData *> postCB;

  SoCallbackData * preTailCB;
  SoCallbackData * postTailCB;

  SbList <SoCallbackData *> triangleCB;
  SbList <SoCallbackData *> lineSegmentCB;
  SbList <SoCallbackData *> pointCB;
};



#endif // !__SOCALLBACKACTION_H__

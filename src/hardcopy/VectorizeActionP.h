#ifndef COIN_SOVECTORIZEACTIONP_H
#define COIN_SOVECTORIZEACTIONP_H

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

#include <Inventor/actions/SoCallbackAction.h>
#include <Inventor/nodes/SoShape.h>
#include <Inventor/lists/SbList.h>
#include <Inventor/SbBSPTree.h>
#include <Inventor/SoPrimitiveVertex.h>
#include <Inventor/SbColor4f.h>
#include <Inventor/SbPlane.h>
#include <Inventor/annex/HardCopy/SoVectorizeAction.h>
#include <Inventor/annex/HardCopy/SoVectorOutput.h>
#include <Inventor/elements/SoDrawStyleElement.h>
#include <Inventor/SbVec2s.h>
#include <Inventor/SbImage.h>
#include "VectorizeItems.h"

class SbClip;
class SoPointDetail;

class SoVectorizeActionP {
public:

  SoVectorizeActionP(SoVectorizeAction * p);
  ~SoVectorizeActionP();

public:
  SbBSPTree bsp;
  SbList <SoVectorizeItem*> itemlist;
  SbList <SoVectorizeItem*> annotationlist;
  SoVectorOutput * output;

  int annotationidx;

  struct Page {
    SbVec2f startpos;
    SbVec2f size;
  } page;

  struct Viewport {
    SbVec2f startpos;
    SbVec2f size;
  } viewport;

  SoVectorizeAction::Orientation orientation;

  struct Background {
    SbBool on;
    SbColor color;
  } background;

  float nominalwidth;
  float pixelimagesize;
  SoVectorizeAction::PointStyle pointstyle;

  SbBool testInside(SoState * state,
                    const SbVec3f & p0, 
                    const SbVec3f & p1,
                    const SbVec3f & p2) const;


  void addTriangle(SoVectorizeTriangle * tri);
  void addLine(SoVectorizeLine * line);
  void addPoint(SoVectorizePoint * point);
  void addText(SoVectorizeText * text);
  void addImage(SoVectorizeImage * image);
  
  void outputItems(void);
  void reset(void);

private:
  
  typedef struct {
    SbVec3f point;
    SbVec3f normal;
    uint32_t diffuse;
  } vertexdata;

  SbList <vertexdata*> vertexdatalist;
  int curr_vertexdata_index;
  vertexdata * alloc_vertexdata(void);
  vertexdata * create_vertexdata(const SoPrimitiveVertex * pv, SoState * state);
  vertexdata * create_vertexdata(const SoPointDetail * pd, SoState * state);
  void add_line(vertexdata * vd0, vertexdata * vd1, SoState * state);
  void add_point(vertexdata * vd, SoState * state);
  
  SbBool clip_line(vertexdata * v0, vertexdata * v1, const SbPlane & plane);

  struct ShapeMaterial {
    SbColor ambient_light_model;
    SbColor ambient;
    SbColor emissive;
    SbColor specular;
    float shininess;
  } shapematerial;

  struct Environment {
    float ambientintensity;
    SbColor ambientcolor;
    SbVec3f attenuation;
    int32_t fogtype;
    SbColor fogcolor;
    float fogvisibility;
    float fogstart;
  } environment;

  SbColor4f shade_vertex(SoState * state,
                         const SbVec3f & v,
                         const SbColor4f & vcolor,
                         const SbVec3f & normal);

  SoVectorizeAction * publ;
  SbMatrix shapeprojmatrix;
  SbMatrix shapetoworldmatrix;
  SbMatrix shapetovrc;
  SbPlane cameraplane;
  SbBool docull;
  SbBool twoside;
  SbBool ccw;
  SbBool phong;
  int faceidx;
  int lineidx;
  SoDrawStyleElement::Style drawstyle;
  SbClip * clipper;
  SbBool completelyinside;
  SbPlane vvplane[6];
  SbList <SbPlane> clipplanes;
  int prevfaceindex;
  float linewidth;
  uint16_t linepattern;
  float pointsize;

  static SoCallbackAction::Response pre_shape_cb(void * userdata,
                                                 SoCallbackAction * action,
                                                 const SoNode * node);
  static SoCallbackAction::Response post_shape_cb(void * userdata,
                                                  SoCallbackAction * action,
                                                  const SoNode * node);
  
  static SoCallbackAction::Response pre_text2_cb(void * userdata,
                                                 SoCallbackAction * action,
                                                 const SoNode * node);
  static SoCallbackAction::Response pre_image_cb(void * userdata,
                                                 SoCallbackAction * action,
                                                 const SoNode * node);
  static SoCallbackAction::Response pre_anno_cb(void * userdata,
                                                SoCallbackAction * action,
                                                const SoNode * node);
  static SoCallbackAction::Response post_anno_cb(void * userdata,
                                                 SoCallbackAction * action,
                                                 const SoNode * node);


  static void triangle_cb(void * userdata,
                          SoCallbackAction * action,
                          const SoPrimitiveVertex * v1,
                          const SoPrimitiveVertex * v2,
                          const SoPrimitiveVertex * v3);
  
  static void line_segment_cb(void * userdata, SoCallbackAction * action,
                              const SoPrimitiveVertex * v1,
                              const SoPrimitiveVertex * v2);

  static void point_cb(void * userdata, SoCallbackAction * action,
                       const SoPrimitiveVertex * v);

  static void calc_new_vertexdata(vertexdata * v,
                                  const SbVec3f & pos,
                                  const vertexdata * v0,
                                  const vertexdata * v1);
  
  static void * clip_cb(const SbVec3f & v0, void * vdata0, 
                        const SbVec3f & v1, void * vdata1,
                        const SbVec3f & newvertex,
                        void * userdata);
  static SoCallbackAction::Response camera_cb(void * data, 
                                              SoCallbackAction * action, 
                                              const SoNode * node);

};

// some convenience functions
inline SbVec2f
to_mm(const SbVec2f & val, const SoVectorizeAction::DimensionUnit unit)
{
  switch (unit) {
  default:
  case SoVectorizeAction::MM:
    return val;
  case SoVectorizeAction::INCH:
    return val * 25.4f;
  case SoVectorizeAction::METER:
    return val * 1000.0f;
  }
}

inline SbVec2f
from_mm(const SbVec2f & val, const SoVectorizeAction::DimensionUnit unit)
{
  switch (unit) {
  default:
  case SoVectorizeAction::MM:
    return val;
  case SoVectorizeAction::INCH:
    return val / 25.4f;
  case SoVectorizeAction::METER:
    return val / 1000.0f;
  }
}

inline float
to_mm(const float val, const SoVectorizeAction::DimensionUnit unit)
{
  switch (unit) {
  default:
  case SoVectorizeAction::MM:
    return val;
  case SoVectorizeAction::INCH:
    return val * 25.4f;
  case SoVectorizeAction::METER:
    return val * 1000.0f;
  }
}


inline float
from_mm(const float val, const SoVectorizeAction::DimensionUnit unit)
{
  switch (unit) {
  default:
  case SoVectorizeAction::MM:
    return val;
  case SoVectorizeAction::INCH:
    return val / 25.4f;
  case SoVectorizeAction::METER:
    return val / 1000.0f;
  }
}

#endif // COIN_SOVECTORIZEACTIONP_H

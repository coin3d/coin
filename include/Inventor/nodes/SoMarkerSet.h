/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2000 by Systems in Motion. All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public License
 *  version 2.1 as published by the Free Software Foundation. See the
 *  file LICENSE.LGPL at the root directory of the distribution for
 *  more details.
 *
 *  If you want to use Coin for applications not compatible with the
 *  LGPL, please contact SIM to aquire a Professional Edition license.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  http://www.sim.no support@sim.no Voice: +47 22114160 Fax: +47 22207097
 *
\**************************************************************************/

#ifndef COIN_SOMARKERSET_H
#define COIN_SOMARKERSET_H

#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/nodes/SoPointSet.h>
#include <Inventor/fields/SoMFInt32.h>


class COIN_DLL_EXPORT SoMarkerSet : public SoPointSet {
  typedef SoPointSet inherited;

  SO_NODE_HEADER(SoMarkerSet);

public:
  static void initClass(void);
  SoMarkerSet(void);

  enum MarkerType {
    CROSS_5_5, PLUS_5_5, MINUS_5_5, SLASH_5_5, BACKSLASH_5_5, BAR_5_5,
    STAR_5_5, Y_5_5, LIGHTNING_5_5, WELL_5_5,

    CIRCLE_LINE_5_5, SQUARE_LINE_5_5, DIAMOND_LINE_5_5, TRIANGLE_LINE_5_5,
    RHOMBUS_LINE_5_5, HOURGLASS_LINE_5_5, SATELLITE_LINE_5_5,
    PINE_TREE_LINE_5_5, CAUTION_LINE_5_5, SHIP_LINE_5_5,

    CIRCLE_FILLED_5_5, SQUARE_FILLED_5_5, DIAMOND_FILLED_5_5,
    TRIANGLE_FILLED_5_5, RHOMBUS_FILLED_5_5, HOURGLASS_FILLED_5_5,
    SATELLITE_FILLED_5_5, PINE_TREE_FILLED_5_5, CAUTION_FILLED_5_5,
    SHIP_FILLED_5_5,

    CROSS_7_7, PLUS_7_7, MINUS_7_7, SLASH_7_7, BACKSLASH_7_7, BAR_7_7,
    STAR_7_7, Y_7_7, LIGHTNING_7_7, WELL_7_7,

    CIRCLE_LINE_7_7, SQUARE_LINE_7_7, DIAMOND_LINE_7_7, TRIANGLE_LINE_7_7,
    RHOMBUS_LINE_7_7, HOURGLASS_LINE_7_7, SATELLITE_LINE_7_7,
    PINE_TREE_LINE_7_7, CAUTION_LINE_7_7, SHIP_LINE_7_7,

    CIRCLE_FILLED_7_7, SQUARE_FILLED_7_7, DIAMOND_FILLED_7_7,
    TRIANGLE_FILLED_7_7, RHOMBUS_FILLED_7_7, HOURGLASS_FILLED_7_7,
    SATELLITE_FILLED_7_7, PINE_TREE_FILLED_7_7, CAUTION_FILLED_7_7,
    SHIP_FILLED_7_7,

    CROSS_9_9, PLUS_9_9, MINUS_9_9, SLASH_9_9, BACKSLASH_9_9, BAR_9_9,
    STAR_9_9, Y_9_9, LIGHTNING_9_9, WELL_9_9,

    CIRCLE_LINE_9_9, SQUARE_LINE_9_9, DIAMOND_LINE_9_9, TRIANGLE_LINE_9_9,
    RHOMBUS_LINE_9_9, HOURGLASS_LINE_9_9, SATELLITE_LINE_9_9,
    PINE_TREE_LINE_9_9, CAUTION_LINE_9_9, SHIP_LINE_9_9,

    CIRCLE_FILLED_9_9, SQUARE_FILLED_9_9, DIAMOND_FILLED_9_9,
    TRIANGLE_FILLED_9_9, RHOMBUS_FILLED_9_9, HOURGLASS_FILLED_9_9,
    SATELLITE_FILLED_9_9, PINE_TREE_FILLED_9_9, CAUTION_FILLED_9_9,
    SHIP_FILLED_9_9
  };

  SoMFInt32 markerIndex;

  virtual void GLRender(SoGLRenderAction * action);
  virtual void getPrimitiveCount(SoGetPrimitiveCountAction * action);

  static int getNumDefinedMarkers(void);
  static void addMarker(int markerIndex, const SbVec2s & size,
                        const unsigned char * bytes, SbBool isLSBFirst = TRUE,
                        SbBool isUpToDown = TRUE);
  static SbBool getMarker(int markerIndex, SbVec2s & size,
                          const unsigned char *& bytes, SbBool & isLSBFirst);
  static SbBool removeMarker(int markerIndex);
  static SbBool isMarkerBitSet(int markerIndex, int bitNumber);

protected:
  virtual ~SoMarkerSet();
};

#endif // !COIN_SOMARKERSET_H

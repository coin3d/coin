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

#ifndef COIN_INTERNAL
#error this is a private header file
#endif /* !COIN_INTERNAL */

#ifndef SO_SCALE2DRAGGER_IV_H
#define SO_SCALE2DRAGGER_IV_H

static const char SCALE2DRAGGER_draggergeometry[] =
  "#Inventor V2.1 ascii\n"
  "\n"
  "\n"
  "DEF SCALE2_INACTIVE_MATERIAL Material { diffuseColor 0.5 0.5 0.5  emissiveColor 0.5 0.5 0.5 }\n"
  "DEF SCALE2_ACTIVE_MATERIAL Material { diffuseColor 0.5 0.5 0  emissiveColor 0.5 0.5 0 }\n"
  "DEF SCALE2_FEEDBACK_MATERIAL Material { diffuseColor 0.5 0 0.5  emissiveColor 0.5 0 0.5 }\n"
  "\n"
  "\n"
  "DEF SCALE2_MARKER Group {\n"
  "   PickStyle { style SHAPE }\n"
  "   Translation { translation -0.025 0 0 }\n"
  "   Cube { width 0.05  height 0.10  depth 0.05 }\n"
  "   Translation { translation 0.05 -0.025 0 }\n"
  "   Cube { width 0.05  height 0.05  depth 0.05 }\n"
  "}\n"
  "\n"
  "\n"
  "DEF SCALE2_SCALING_MARKERS Separator {\n"
  "   Separator {\n"
  "\n"
  "      Translation { translation -1.1 -1.1 0 }\n"
  "      USE SCALE2_MARKER\n"
  "   }\n"
  "   Separator {\n"
  "\n"
  "      Translation { translation 1.1 -1.1 0 }\n"
  "      Rotation { rotation 0 0 1  1.57 }\n"
  "      USE SCALE2_MARKER\n"
  "   }\n"
  "   Separator {\n"
  "\n"
  "      Translation { translation -1.1 1.1 0 }\n"
  "      Rotation { rotation 0 0 1  -1.57 }\n"
  "      USE SCALE2_MARKER\n"
  "   }\n"
  "\n"
  "   Translation { translation 1.1 1.1 0 }\n"
  "   Rotation { rotation 0 0 1  3.14 }\n"
  "   USE SCALE2_MARKER\n"
  "}\n"
  "\n"
  "DEF scale2Scaler Separator {\n"
  "   USE SCALE2_INACTIVE_MATERIAL\n"
  "   USE SCALE2_SCALING_MARKERS\n"
  "}\n"
  "\n"
  "DEF scale2ScalerActive Separator {\n"
  "   USE SCALE2_ACTIVE_MATERIAL\n"
  "   USE SCALE2_SCALING_MARKERS\n"
  "\n"
  "   DrawStyle { style LINES  lineWidth 1 }\n"
  "   PickStyle { style UNPICKABLE }\n"
  "   Cube { width 2.2  height 2.2  depth 2.2 }\n"
  "}\n"
  "\n"
  "DEF SCALE2_FEEDBACK Group {\n"
  "   USE SCALE2_FEEDBACK_MATERIAL\n"
  "   PickStyle { style UNPICKABLE }\n"
  "   DrawStyle { lineWidth 2 }\n"
  "   Coordinate3 { point [ 1.2 0 0, -1.2 0 0, 0 1.2 0, 0 -1.2 0, 0 0 1.2, 0 0 -1.2 ] }\n"
  "   IndexedLineSet { coordIndex [ 0, 1, -1, 2, 3, -1, 4, 5, -1 ] }\n"
  "}\n"
  "\n"
  "DEF scale2Feedback Separator { USE SCALE2_FEEDBACK }\n"
  "DEF scale2FeedbackActive Separator { USE SCALE2_FEEDBACK }\n";

#endif /* ! SO_SCALE2DRAGGER_IV_H */

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

#ifndef SO_SCALE1DRAGGER_IV_H
#define SO_SCALE1DRAGGER_IV_H

static const char SCALE1DRAGGER_draggergeometry[] =
  "#Inventor V2.1 ascii\n"
  "\n"
  "\n"
  "DEF SCALE1_INACTIVE_MATERIAL Material { diffuseColor 0.5 0.5 0.5  emissiveColor 0.5 0.5 0.5 }\n"
  "DEF SCALE1_ACTIVE_MATERIAL Material { diffuseColor 0.5 0.5 0  emissiveColor 0.5 0.5 0 }\n"
  "DEF SCALE1_FEEDBACK_MATERIAL Material { diffuseColor 0.5 0 0.5  emissiveColor 0.5 0 0.5 }\n"
  "\n"
  "\n"
  "DEF SCALE1_AXIS Separator {\n"
  "   PickStyle { style UNPICKABLE }\n"
  "   Coordinate3 { point [ 1 0 0, -1 0 0 ] }\n"
  "   IndexedLineSet { coordIndex [ 0, 1, -1 ] }\n"
  "   Translation { translation -1.05 0 0 }\n"
  "   PickStyle { style SHAPE }\n"
  "   DEF SCALE1_MARKER Cube { width 0.10  height 0.10  depth 0.10 }\n"
  "   Translation { translation 2.10 0 0 }\n"
  "   USE SCALE1_MARKER\n"
  "}\n"
  "\n"
  "DEF scale1Scaler Separator {\n"
  "   DrawStyle { lineWidth 2 }\n"
  "   USE SCALE1_INACTIVE_MATERIAL\n"
  "   USE SCALE1_AXIS\n"
  "}\n"
  "\n"
  "DEF scale1ScalerActive Separator {\n"
  "   USE SCALE1_ACTIVE_MATERIAL\n"
  "   DrawStyle { lineWidth 3 }\n"
  "   USE SCALE1_AXIS\n"
  "   DrawStyle { style LINES  lineWidth 1 }\n"
  "   PickStyle { style SHAPE }\n"
  "   Cube { }\n"
  "}\n"
  "\n"
  "DEF SCALE1_FEEDBACK Separator {\n"
  "   PickStyle { style UNPICKABLE }\n"
  "   USE SCALE1_FEEDBACK_MATERIAL\n"
  "   DrawStyle { lineWidth 2 }\n"
  "   Coordinate3 { point [ 0 1.1 0, 0 -1.1 0, 0 0 1.1, 0 0 -1.1 ] }\n"
  "   IndexedLineSet { coordIndex [ 0, 1, -1, 2, 3, -1 ] }\n"
  "}\n"
  "\n"
  "DEF scale1Feedback Separator { USE SCALE1_FEEDBACK }\n"
  "DEF scale1FeedbackActive Separator { USE SCALE1_FEEDBACK }\n";

#endif /* ! SO_SCALE1DRAGGER_IV_H */

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

#ifndef SO_ROTATEDISCDRAGGER_IV_H
#define SO_ROTATEDISCDRAGGER_IV_H

static const char ROTATEDISCDRAGGER_draggergeometry[] =
  "#Inventor V2.1 ascii\n"
  "\n"
  "\n"
  "DEF ROTATEDISC_INACTIVE_MATERIAL Material { diffuseColor 0.5 0.5 0.5  emissiveColor 0.5 0.5 0.5 }\n"
  "DEF ROTATEDISC_ACTIVE_MATERIAL Material { diffuseColor 0.5 0.5 0  emissiveColor 0.5 0.5 0 }\n"
  "DEF ROTATEDISC_FEEDBACK_MATERIAL Material { diffuseColor 0.5 0 0.5  emissiveColor 0.5 0 0.5 }\n"
  "\n"
  "\n"
  "DEF ROTATEDISC_CYLINDER Separator {\n"
  "   DEF ROTATEDISC_CYLINDER_ROTATION Rotation { rotation 1 0 0  1.57 }\n"
  "   DEF ROTATEDISC_CYLINDER_SCALE Scale { scaleFactor 1.733 1 1.733 }\n"
  "   DEF ROTATEDISC_CYLINDER_SHAPEHINTS ShapeHints { vertexOrdering UNKNOWN_ORDERING }\n"
  "   DrawStyle { style LINES }\n"
  "   Cylinder { parts SIDES height 0.2 }\n"
  "}\n"
  "\n"
  "DEF ROTATEDISC_CYLINDER_PICK Separator {\n"
  "   USE ROTATEDISC_CYLINDER_ROTATION\n"
  "   USE ROTATEDISC_CYLINDER_SCALE\n"
  "   USE ROTATEDISC_CYLINDER_SHAPEHINTS\n"
  "   DrawStyle { style INVISIBLE }\n"
  "   Cylinder { parts ALL height 0.2 }\n"
  "}\n"
  "\n"
  "DEF ROTATEDISC_BOX Cube { width 2.2  height 2.2  depth 2.2 }\n"
  "\n"
  "DEF rotateDiscRotator Separator {\n"
  "   USE ROTATEDISC_INACTIVE_MATERIAL\n"
  "   USE ROTATEDISC_CYLINDER\n"
  "\n"
  "   USE ROTATEDISC_CYLINDER_PICK\n"
  "}\n"
  "\n"
  "DEF rotateDiscRotatorActive Separator {\n"
  "   USE ROTATEDISC_ACTIVE_MATERIAL\n"
  "   USE ROTATEDISC_CYLINDER\n"
  "   DrawStyle { style LINES  lineWidth 1 }\n"
  "   USE ROTATEDISC_BOX\n"
  "}\n"
  "\n"
  "DEF ROTATEDISC_AXIS_LINE Group {\n"
  "   Coordinate3 { point [ 0 0 1.1, 0 0 -1.1 ] }\n"
  "   IndexedLineSet { coordIndex [ 0, 1, -1 ] }\n"
  "}\n"
  "\n"
  "DEF rotateDiscFeedback Separator {\n"
  "   USE ROTATEDISC_FEEDBACK_MATERIAL\n"
  "   USE ROTATEDISC_AXIS_LINE\n"
  "}\n"
  "\n"
  "DEF rotateDiscFeedbackActive Separator {\n"
  "   USE ROTATEDISC_ACTIVE_MATERIAL\n"
  "   USE ROTATEDISC_AXIS_LINE\n"
  "}\n";

#endif /* ! SO_ROTATEDISCDRAGGER_IV_H */

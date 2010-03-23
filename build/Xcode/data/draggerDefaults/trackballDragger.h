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

#ifndef SO_TRACKBALLDRAGGER_IV_H
#define SO_TRACKBALLDRAGGER_IV_H

static const char TRACKBALLDRAGGER_draggergeometry[] =
  "#Inventor V2.1 ascii\n"
  "\n"
  "\n"
  "DEF TRACKBALL_INACTIVE_MATERIAL Material { diffuseColor 0.5 0.5 0.5  emissiveColor 0.5 0.5 0.5 }\n"
  "DEF TRACKBALL_ACTIVE_MATERIAL Material { diffuseColor 0.5 0.5 0  emissiveColor 0.5 0.5 0 }\n"
  "DEF TRACKBALL_FEEDBACK_MATERIAL Material { diffuseColor 0.5 0 0.5  emissiveColor 0.5 0 0.5 }\n"
  "\n"
  "DEF TRACKBALL_USER_INACTIVE_MATERIAL Material { diffuseColor 0 0.7 0.1  emissiveColor 0 0.2 0.1 }\n"
  "DEF TRACKBALL_USER_ACTIVE_MATERIAL Material { diffuseColor 0 0.8 0.1  emissiveColor 0 0.3 0.1 }\n"
  "\n"
  "\n"
  "\n"
  "DEF TRACKBALL_BAND_MARKER Group {\n"
  "   DrawStyle { style LINES }\n"
  "   ShapeHints { vertexOrdering UNKNOWN_ORDERING }\n"
  "   Cylinder { parts SIDES  radius 1.05 height 0.1 }\n"
  "}\n"
  "\n"
  "\n"
  "\n"
  "\n"
  "DEF trackballXRotator Separator {\n"
  "   USE TRACKBALL_INACTIVE_MATERIAL\n"
  "   DEF TRACKBALL_X_BAND Group {\n"
  "      Rotation { rotation 0 0 1 1.57 }\n"
  "      USE TRACKBALL_BAND_MARKER\n"
  "   }\n"
  "}\n"
  "DEF trackballXRotatorActive Separator {\n"
  "   USE TRACKBALL_ACTIVE_MATERIAL\n"
  "   USE TRACKBALL_X_BAND\n"
  "}\n"
  "\n"
  "DEF trackballYRotator Separator {\n"
  "   USE TRACKBALL_INACTIVE_MATERIAL\n"
  "   USE TRACKBALL_BAND_MARKER\n"
  "}\n"
  "DEF trackballYRotatorActive Separator {\n"
  "   USE TRACKBALL_ACTIVE_MATERIAL\n"
  "   USE TRACKBALL_BAND_MARKER\n"
  "}\n"
  "\n"
  "DEF trackballZRotator Separator {\n"
  "   USE TRACKBALL_INACTIVE_MATERIAL\n"
  "   DEF TRACKBALL_Z_BAND Group {\n"
  "      Rotation { rotation 1 0 0 1.57 }\n"
  "      USE TRACKBALL_BAND_MARKER\n"
  "   }\n"
  "}\n"
  "DEF trackballZRotatorActive Separator {\n"
  "   USE TRACKBALL_ACTIVE_MATERIAL\n"
  "   USE TRACKBALL_Z_BAND\n"
  "}\n"
  "\n"
  "\n"
  "\n"
  "\n"
  "DEF TRACKBALL_USER_AXIS Group {\n"
  "   Coordinate3 { point [ 0 1.5 0, 0 -1.5 0 ] }\n"
  "   IndexedLineSet { coordIndex [ 0, 1, -1 ] }\n"
  "}\n"
  "\n"
  "DEF trackballUserAxis Separator {\n"
  "   USE TRACKBALL_FEEDBACK_MATERIAL\n"
  "   USE TRACKBALL_USER_AXIS\n"
  "}\n"
  "DEF trackballUserAxisActive Separator {\n"
  "   USE TRACKBALL_FEEDBACK_MATERIAL\n"
  "   USE TRACKBALL_USER_AXIS\n"
  "}\n"
  "\n"
  "DEF TRACKBALL_DRAGGER_USER_ROTATOR Group {\n"
  "   Scale { scaleFactor 1.05 1.05 1.05 } # outside the x, y and z bands\n"
  "   USE TRACKBALL_BAND_MARKER\n"
  "}\n"
  "\n"
  "DEF trackballUserRotator Separator {\n"
  "   USE TRACKBALL_USER_INACTIVE_MATERIAL\n"
  "   USE TRACKBALL_DRAGGER_USER_ROTATOR\n"
  "}\n"
  "\n"
  "DEF trackballUserRotatorActive Separator {\n"
  "   USE TRACKBALL_USER_ACTIVE_MATERIAL\n"
  "   USE TRACKBALL_DRAGGER_USER_ROTATOR\n"
  "}\n"
  "\n"
  "DEF TRACKBALL_CENTER Separator {\n"
  "   DrawStyle { style INVISIBLE }\n"
  "   Sphere { radius 1.02 }\n"
  "}\n"
  "\n"
  "DEF trackballRotator Separator { USE TRACKBALL_CENTER }\n"
  "DEF trackballRotatorActive Separator { USE TRACKBALL_CENTER }\n";

#endif /* ! SO_TRACKBALLDRAGGER_IV_H */

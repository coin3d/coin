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

#ifndef SO_ROTATESPHERICALDRAGGER_IV_H
#define SO_ROTATESPHERICALDRAGGER_IV_H

static const char ROTATESPHERICALDRAGGER_draggergeometry[] =
  "#Inventor V2.1 ascii\n"
  "\n"
  "\n"
  "\n"
  "DEF ROTATESPHERICAL_INACTIVE_MATERIAL Material { diffuseColor 0.5 0.5 0.5  emissiveColor 0.5 0.5 0.5 }\n"
  "DEF ROTATESPHERICAL_ACTIVE_MATERIAL Material { diffuseColor 0.5 0.5 0  emissiveColor 0.5 0.5 0 }\n"
  "DEF ROTATESPHERICAL_FEEDBACK_MATERIAL Material { diffuseColor 0.5 0 0.5  emissiveColor 0.5 0 0.5 }\n"
  "\n"
  "\n"
  "\n"
  "\n"
  "DEF ROTATESPHERICAL_FEEDBACK Group {\n"
  "   Coordinate3 { point [ 1.3 0 0, -1.3 0 0, 0 1.3 0, 0 -1.3 0, 0 0 1.3, 0 0 -1.3 ] }\n"
  "   IndexedLineSet { coordIndex [ 0, 1, -1, 2, 3, -1, 4, 5, -1 ] }\n"
  "}\n"
  "\n"
  "DEF rotateSphericalFeedback Separator {\n"
  "   USE ROTATESPHERICAL_FEEDBACK_MATERIAL\n"
  "   USE ROTATESPHERICAL_FEEDBACK\n"
  "}\n"
  "DEF rotateSphericalFeedbackActive Separator {\n"
  "   USE ROTATESPHERICAL_FEEDBACK_MATERIAL\n"
  "   USE ROTATESPHERICAL_FEEDBACK\n"
  "}\n"
  "\n"
  "\n"
  "\n"
  "\n"
  "DEF ROTATESPHERICAL_RING Group {\n"
  "\n"
  "\n"
  "   ShapeHints {\n"
  "      shapeType UNKNOWN_SHAPE_TYPE\n"
  "      vertexOrdering UNKNOWN_ORDERING\n"
  "   }\n"
  "\n"
  "   DrawStyle { style LINES lineWidth 2 }\n"
  "   Cylinder { parts SIDES height 0 }\n"
  "}\n"
  "\n"
  "DEF ROTATESPHERICAL_BALL Group {\n"
  "\n"
  "\n"
  "   Scale { scaleFactor 1.733 1.733 1.733 }\n"
  "\n"
  "\n"
  "   DrawStyle { style INVISIBLE }\n"
  "   Sphere { }\n"
  "\n"
  "\n"
  "   USE ROTATESPHERICAL_RING\n"
  "   Rotation { rotation 0 0 1 1.57 }\n"
  "   USE ROTATESPHERICAL_RING\n"
  "   Rotation { rotation 1 0 0  1.57 }\n"
  "   USE ROTATESPHERICAL_RING\n"
  "}\n"
  "\n"
  "DEF rotateSphericalRotator Separator {\n"
  "   USE ROTATESPHERICAL_INACTIVE_MATERIAL\n"
  "   USE ROTATESPHERICAL_BALL\n"
  "}\n"
  "\n"
  "DEF rotateSphericalRotatorActive Separator {\n"
  "   USE ROTATESPHERICAL_ACTIVE_MATERIAL\n"
  "   USE ROTATESPHERICAL_BALL\n"
  "}\n";

#endif /* ! SO_ROTATESPHERICALDRAGGER_IV_H */

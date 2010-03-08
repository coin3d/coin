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

#ifndef SO_ROTATECYLINDRICALDRAGGER_IV_H
#define SO_ROTATECYLINDRICALDRAGGER_IV_H

static const char ROTATECYLINDRICALDRAGGER_draggergeometry[] =
  "#Inventor V2.1 ascii\n"
  "\n"
  "\n"
  "\n"
  "DEF ROTATECYLINDRICAL_INACTIVE_MATERIAL Material { diffuseColor 0.5 0.5 0.5  emissiveColor 0.5 0.5 0.5 }\n"
  "DEF ROTATECYLINDRICAL_ACTIVE_MATERIAL Material { diffuseColor 0.5 0.5 0  emissiveColor 0.5 0.5 0 }\n"
  "DEF ROTATECYLINDRICAL_FEEDBACK_MATERIAL Material { diffuseColor 0.5 0 0.5  emissiveColor 0.5 0 0.5 }\n"
  "\n"
  "\n"
  "\n"
  "DEF ROTATECYLINDRICAL_CYLINDER Group {\n"
  "   Scale { scaleFactor 1.6 1.1 1.6 } # surround volume completely\n"
  "   Complexity { value 0.4 }\n"
  "   ShapeHints { vertexOrdering UNKNOWN_ORDERING }\n"
  "   Cylinder { parts SIDES }\n"
  "}\n"
  "\n"
  "\n"
  "DEF rotateCylindricalRotator Separator {\n"
  "   USE ROTATECYLINDRICAL_INACTIVE_MATERIAL\n"
  "   DrawStyle { style LINES  lineWidth 1 }\n"
  "   USE ROTATECYLINDRICAL_CYLINDER\n"
  "}\n"
  "\n"
  "DEF rotateCylindricalRotatorActive Separator {\n"
  "   USE ROTATECYLINDRICAL_ACTIVE_MATERIAL\n"
  "   DrawStyle { style LINES  lineWidth 2 }\n"
  "   USE ROTATECYLINDRICAL_CYLINDER\n"
  "}\n"
  "\n"
  "\n"
  "\n"
  "DEF ROTATECYLINDRICAL_FEEDBACK_MARKER Separator {\n"
  "   USE ROTATECYLINDRICAL_FEEDBACK_MATERIAL\n"
  "   PickStyle { style UNPICKABLE }\n"
  "   DrawStyle { lineWidth 2 }\n"
  "   Coordinate3 { point [ 0 1.1 0, 0 -1.1 0 ] }\n"
  "   IndexedLineSet { coordIndex [ 0, 1, -1 ] }\n"
  "}\n"
  "\n"
  "DEF rotateCylindricalFeedback Separator { USE ROTATECYLINDRICAL_FEEDBACK_MARKER }\n"
  "DEF rotateCylindricalFeedbackActive Separator { USE ROTATECYLINDRICAL_FEEDBACK_MARKER }\n";

#endif /* ! SO_ROTATECYLINDRICALDRAGGER_IV_H */

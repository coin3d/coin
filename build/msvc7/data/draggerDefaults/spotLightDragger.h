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

#ifndef SO_SPOTLIGHTDRAGGER_IV_H
#define SO_SPOTLIGHTDRAGGER_IV_H

static const char SPOTLIGHTDRAGGER_draggergeometry[] =
  "#Inventor V2.1 ascii\n"
  "\n"
  "\n"
  "DEF SPOTLIGHT_INACTIVE_MATERIAL Material { diffuseColor 0.5 0.5 0.5  emissiveColor 0.5 0.5 0.5 }\n"
  "DEF SPOTLIGHT_ACTIVE_MATERIAL Material { diffuseColor 0.5 0.5 0  emissiveColor 0.5 0.5 0 }\n"
  "\n"
  "\n"
  "\n"
  "\n"
  "DEF spotLightOverallMaterial Material { diffuseColor 0.5 0.5 0.5  emissiveColor 0.5 0.5 0.5 }\n"
  "\n"
  "\n"
  "DEF SPOTLIGHT_ARROW Separator {\n"
  "\n"
  "   Rotation { rotation 1 0 0  -1.57 }\n"
  "\n"
  "   Coordinate3 { point [ 0 0 0, 0 9 0 ] }\n"
  "   IndexedLineSet { coordIndex [ 0, 1, -1 ] }\n"
  "   Translation { translation 0 10 0 }\n"
  "   Cone { }\n"
  "   Translation { translation 0 -11 0 }\n"
  "   Cone { bottomRadius 0.1 }\n"
  "}\n"
  "\n"
  "DEF spotLightRotatorRotator Separator {\n"
  "   USE SPOTLIGHT_INACTIVE_MATERIAL\n"
  "   USE SPOTLIGHT_ARROW\n"
  "}\n"
  "DEF spotLightRotatorRotatorActive Separator {\n"
  "   USE SPOTLIGHT_ACTIVE_MATERIAL\n"
  "   USE SPOTLIGHT_ARROW\n"
  "}\n"
  "\n"
  "DEF spotLightRotatorFeedback Separator { }\n"
  "DEF spotLightRotatorFeedbackActive Separator { }\n"
  "\n"
  "\n"
  "DEF SPOTLIGHT_TRANSLATE_AXIS Group {\n"
  "   Rotation { rotation 0 0 1  1.57 }\n"
  "   Cylinder { height 3.0  radius 0.2 }\n"
  "}\n"
  "\n"
  "DEF spotLightTranslatorLineTranslator Separator {\n"
  "   USE SPOTLIGHT_INACTIVE_MATERIAL\n"
  "   USE SPOTLIGHT_TRANSLATE_AXIS\n"
  "}\n"
  "DEF spotLightTranslatorLineTranslatorActive Separator {\n"
  "   USE SPOTLIGHT_ACTIVE_MATERIAL\n"
  "   USE SPOTLIGHT_TRANSLATE_AXIS\n"
  "}\n"
  "\n"
  "DEF spotLightTranslatorPlaneTranslator Separator {\n"
  "   USE SPOTLIGHT_INACTIVE_MATERIAL\n"
  "   Sphere { }\n"
  "}\n"
  "DEF spotLightTranslatorPlaneTranslatorActive Separator {\n"
  "   USE SPOTLIGHT_ACTIVE_MATERIAL\n"
  "   Sphere { }\n"
  "}\n"
  "\n"
  "\n"
  "DEF SPOTLIGHT_CONEANGLE Separator {\n"
  "   Rotation { rotation 1 0 0  1.57 }\n"
  "   ShapeHints { vertexOrdering UNKNOWN_ORDERING }\n"
  "   Cone { parts SIDES  bottomRadius 2.0 }\n"
  "}\n"
  "\n"
  "DEF spotLightBeam Separator {\n"
  "   USE SPOTLIGHT_INACTIVE_MATERIAL\n"
  "   DrawStyle { style LINES  lineWidth 2 }\n"
  "   USE SPOTLIGHT_CONEANGLE\n"
  "}\n"
  "DEF spotLightBeamActive Separator {\n"
  "   USE SPOTLIGHT_ACTIVE_MATERIAL\n"
  "   DrawStyle { style LINES  lineWidth 3 }\n"
  "   USE SPOTLIGHT_CONEANGLE\n"
  "}\n"
  "\n"
  "DEF spotLightBeamPlacement Translation { translation 0 0 -1.5 }\n";

#endif /* ! SO_SPOTLIGHTDRAGGER_IV_H */

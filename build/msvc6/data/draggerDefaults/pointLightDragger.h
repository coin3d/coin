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

#ifndef SO_POINTLIGHTDRAGGER_IV_H
#define SO_POINTLIGHTDRAGGER_IV_H

static const char POINTLIGHTDRAGGER_draggergeometry[] =
  "#Inventor V2.1 ascii\n"
  "\n"
  "\n"
  "\n"
  "\n"
  "\n"
  "DEF pointLightOverallMaterial Material { diffuseColor 0.5 0.5 0.5  emissiveColor 0.5 0.5 0.5 }\n"
  "DEF POINTLIGHT_ACTIVE_MATERIAL Material { diffuseColor 0.5 0.5 0  emissiveColor 0.5 0.5 0 }\n"
  "\n"
  "\n"
  "\n"
  "DEF POINTLIGHT_AXIS_TRANSLATOR Cube { width 4  height 0.5  depth 0.5 }\n"
  "DEF POINTLIGHT_PLANE_TRANSLATOR Sphere { radius 1.0 }\n"
  "\n"
  "\n"
  "\n"
  "\n"
  "DEF pointLightTranslatorLineTranslator Separator {\n"
  "   USE pointLightOverallMaterial\n"
  "   USE POINTLIGHT_AXIS_TRANSLATOR\n"
  "}\n"
  "\n"
  "DEF pointLightTranslatorLineTranslatorActive Separator {\n"
  "   USE POINTLIGHT_ACTIVE_MATERIAL\n"
  "   USE POINTLIGHT_AXIS_TRANSLATOR\n"
  "}\n"
  "\n"
  "DEF pointLightTranslatorPlaneTranslator Separator {\n"
  "   USE POINTLIGHT_PLANE_TRANSLATOR\n"
  "}\n"
  "\n"
  "DEF pointLightTranslatorPlaneTranslatorActive Separator {\n"
  "   USE POINTLIGHT_ACTIVE_MATERIAL\n"
  "   USE POINTLIGHT_PLANE_TRANSLATOR\n"
  "}\n";

#endif /* ! SO_POINTLIGHTDRAGGER_IV_H */

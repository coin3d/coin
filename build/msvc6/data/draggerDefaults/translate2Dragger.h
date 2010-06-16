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

#ifndef SO_TRANSLATE2DRAGGER_IV_H
#define SO_TRANSLATE2DRAGGER_IV_H

static const char TRANSLATE2DRAGGER_draggergeometry[] =
  "#Inventor V2.1 ascii\n"
  "\n"
  "\n"
  "\n"
  "DEF TRANSLATE2_INACTIVE_MATERIAL Material { diffuseColor 0.5 0.5 0.5  emissiveColor 0.5 0.5 0.5 }\n"
  "DEF TRANSLATE2_ACTIVE_MATERIAL Material { diffuseColor 0.5 0.5 0  emissiveColor 0.5 0.5 0 }\n"
  "\n"
  "\n"
  "DEF TRANSLATE2_GEOM Separator {\n"
  "\n"
  "   Cube { width 2  height 0.1  depth 0.1 } # Horizontal\n"
  "   Cube { width 0.1  height 2  depth 0.1 } # Vertical\n"
  "\n"
  "   Separator {\n"
  "      Translation { translation 1.25 0 0 }\n"
  "      RotationXYZ { axis Z  angle -1.57 }\n"
  "      DEF TRANSLATE2_ARROWHEAD Cone { height 0.5  bottomRadius 0.25 }\n"
  "   }\n"
  "\n"
  "   Separator {\n"
  "      Translation { translation -1.25 0 0 }\n"
  "      RotationXYZ { axis Z  angle 1.57 }\n"
  "      USE TRANSLATE2_ARROWHEAD\n"
  "   }\n"
  "\n"
  "   Separator {\n"
  "      Translation { translation 0 1.25 0 }\n"
  "      USE TRANSLATE2_ARROWHEAD\n"
  "   }\n"
  "\n"
  "   Separator {\n"
  "      Translation { translation 0 -1.25 0 }\n"
  "      RotationXYZ { axis X  angle 3.14 }\n"
  "      USE TRANSLATE2_ARROWHEAD\n"
  "   }\n"
  "}\n"
  "\n"
  "DEF translate2Translator Separator {\n"
  "   USE TRANSLATE2_INACTIVE_MATERIAL\n"
  "   USE TRANSLATE2_GEOM\n"
  "}\n"
  "\n"
  "DEF translate2TranslatorActive Separator {\n"
  "   USE TRANSLATE2_ACTIVE_MATERIAL\n"
  "   USE TRANSLATE2_GEOM\n"
  "}\n"
  "\n"
  "DEF translate2Feedback Separator { }\n"
  "DEF translate2FeedbackActive Separator { }\n"
  "\n"
  "DEF translate2XAxisFeedback Separator {\n"
  "  USE TRANSLATE2_ACTIVE_MATERIAL\n"
  "  DrawStyle { lineWidth 2 }\n"
  "  Coordinate3 { point [ -3 0 0, 3 0 0 ] }\n"
  "  LineSet { }\n"
  "}\n"
  "DEF translate2YAxisFeedback Separator {\n"
  "  USE TRANSLATE2_ACTIVE_MATERIAL\n"
  "  DrawStyle { lineWidth 2 }\n"
  "  Coordinate3 { point [ 0 -3 0, 0 3 0 ] }\n"
  "  LineSet { }\n"
  "}\n";

#endif /* ! SO_TRANSLATE2DRAGGER_IV_H */

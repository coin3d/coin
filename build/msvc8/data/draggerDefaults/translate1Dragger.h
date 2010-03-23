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

#ifndef SO_TRANSLATE1DRAGGER_IV_H
#define SO_TRANSLATE1DRAGGER_IV_H

static const char TRANSLATE1DRAGGER_draggergeometry[] =
  "#Inventor V2.1 ascii\n"
  "\n"
  "\n"
  "\n"
  "DEF TRANSLATE1_INACTIVE_MATERIAL Material { diffuseColor 0.5 0.5 0.5  emissiveColor 0.5 0.5 0.5 }\n"
  "DEF TRANSLATE1_ACTIVE_MATERIAL Material { diffuseColor 0.5 0.5 0  emissiveColor 0.5 0.5 0 }\n"
  "\n"
  "\n"
  "DEF TRANSLATE1_GEOM Group {\n"
  "\n"
  "   Cube { width 2  height 0.1  depth 0.1 } # Horizontal\n"
  "\n"
  "\n"
  "   Separator {\n"
  "      Translation { translation 1.25 0 0 }\n"
  "      RotationXYZ { axis Z  angle -1.5708 }\n"
  "      DEF TRANSLATE1_ARROWHEAD Cone { height 0.5  bottomRadius 0.25 }\n"
  "   }\n"
  "\n"
  "\n"
  "   Separator {\n"
  "      Translation { translation -1.25 0 0 }\n"
  "      RotationXYZ { axis Z  angle 1.5708 }\n"
  "      USE TRANSLATE1_ARROWHEAD\n"
  "   }\n"
  "}\n"
  "\n"
  "DEF translate1Translator Separator {\n"
  "   USE TRANSLATE1_INACTIVE_MATERIAL\n"
  "   USE TRANSLATE1_GEOM\n"
  "}\n"
  "\n"
  "DEF translate1TranslatorActive Separator {\n"
  "   USE TRANSLATE1_ACTIVE_MATERIAL\n"
  "   USE TRANSLATE1_GEOM\n"
  "}\n"
  "\n"
  "DEF translate1Feedback Separator { }\n"
  "DEF translate1FeedbackActive Separator { }\n";

#endif /* ! SO_TRANSLATE1DRAGGER_IV_H */

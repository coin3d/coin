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

#ifndef SO_SCALEUNIFORMDRAGGER_IV_H
#define SO_SCALEUNIFORMDRAGGER_IV_H

static const char SCALEUNIFORMDRAGGER_draggergeometry[] =
  "#Inventor V2.1 ascii\n"
  "\n"
  "\n"
  "\n"
  "DEF SCALEUNIFORM_INACTIVE_MATERIAL Material { diffuseColor 0.5 0.5 0.5  emissiveColor 0.5 0.5 0.5 }\n"
  "DEF SCALEUNIFORM_ACTIVE_MATERIAL Material { diffuseColor 0.5 0.5 0  emissiveColor 0.5 0.5 0 }\n"
  "DEF SCALEUNIFORM_FEEDBACK_MATERIAL Material { diffuseColor 0.5 0 0.5  emissiveColor 0.5 0 0.5 }\n"
  "\n"
  "\n"
  "\n"
  "DEF SCALEUNIFORM_SOLIDMARKER Cube { width 0.1  height 0.1  depth 0.1 }\n"
  "\n"
  "\n"
  "\n"
  "\n"
  "DEF SCALEUNIFORM_DRAGPOINTS Group {\n"
  "   Separator {\n"
  "      Translation { translation -1.1 1.1 1.1 }\n"
  "      USE SCALEUNIFORM_SOLIDMARKER\n"
  "   }\n"
  "   Separator {\n"
  "      Translation { translation 1.1 1.1 1.1 }\n"
  "      USE SCALEUNIFORM_SOLIDMARKER\n"
  "   }\n"
  "   Separator {\n"
  "      Translation { translation 1.1 -1.1 1.1 }\n"
  "      USE SCALEUNIFORM_SOLIDMARKER\n"
  "   }\n"
  "   Separator {\n"
  "      Translation { translation -1.1 -1.1 1.1 }\n"
  "      USE SCALEUNIFORM_SOLIDMARKER\n"
  "   }\n"
  "   Separator {\n"
  "      Translation { translation -1.1 1.1 -1.1 }\n"
  "      USE SCALEUNIFORM_SOLIDMARKER\n"
  "   }\n"
  "   Separator {\n"
  "      Translation { translation 1.1 1.1 -1.1 }\n"
  "      USE SCALEUNIFORM_SOLIDMARKER\n"
  "   }\n"
  "   Separator {\n"
  "      Translation { translation 1.1 -1.1 -1.1 }\n"
  "      USE SCALEUNIFORM_SOLIDMARKER\n"
  "   }\n"
  "   Separator {\n"
  "      Translation { translation -1.1 -1.1 -1.1 }\n"
  "      USE SCALEUNIFORM_SOLIDMARKER\n"
  "   }\n"
  "}\n"
  "\n"
  "DEF scaleUniformScaler Separator {\n"
  "   USE SCALEUNIFORM_INACTIVE_MATERIAL\n"
  "   USE SCALEUNIFORM_DRAGPOINTS\n"
  "}\n"
  "\n"
  "DEF scaleUniformScalerActive Separator {\n"
  "   USE SCALEUNIFORM_ACTIVE_MATERIAL\n"
  "   USE SCALEUNIFORM_DRAGPOINTS\n"
  "\n"
  "\n"
  "   DrawStyle { style LINES  lineWidth 1 }\n"
  "   Cube { width 2.25  height 2.25  depth 2.25 }\n"
  "}\n"
  "\n"
  "\n"
  "\n"
  "\n"
  "\n"
  "DEF SCALEUNIFORM_FEEDBACK_AXES Group {\n"
  "   USE SCALEUNIFORM_FEEDBACK_MATERIAL\n"
  "   DrawStyle { lineWidth 2 }\n"
  "   Coordinate3 { point [ 1 0 0, -1 0 0, 0 1 0, 0 -1 0, 0 0 1, 0 0 -1 ] }\n"
  "   IndexedLineSet { coordIndex [ 0, 1, -1, 2, 3, -1, 4, 5, -1 ] }\n"
  "}\n"
  "\n"
  "DEF scaleUniformFeedback Separator {\n"
  "   USE SCALEUNIFORM_FEEDBACK_AXES\n"
  "}\n"
  "DEF scaleUniformFeedbackActive Separator {\n"
  "   USE SCALEUNIFORM_FEEDBACK_AXES\n"
  "}\n";

#endif /* ! SO_SCALEUNIFORMDRAGGER_IV_H */

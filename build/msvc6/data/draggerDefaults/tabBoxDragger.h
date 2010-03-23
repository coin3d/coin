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

#ifndef SO_TABBOXDRAGGER_IV_H
#define SO_TABBOXDRAGGER_IV_H

static const char TABBOXDRAGGER_draggergeometry[] =
  "#Inventor V2.1 ascii\n"
  "\n"
  "\n"
  "DEF TABBOX_INACTIVE_MATERIAL Material { diffuseColor 0.5 0.5 0.5  emissiveColor 0.5 0.5 0.5 }\n"
  "\n"
  "\n"
  "\n"
  "\n"
  "DEF tabBoxScaleTabMaterial Material { diffuseColor 0 0.5 0  emissiveColor 0 0.5 0 }\n"
  "DEF tabBoxScaleTabHints ShapeHints {\n"
  "\n"
  "   vertexOrdering COUNTERCLOCKWISE\n"
  "   shapeType SOLID\n"
  "}\n"
  "\n"
  "\n"
  "DEF tabBoxBoxGeom Separator {\n"
  "   USE TABBOX_INACTIVE_MATERIAL\n"
  "   DrawStyle { style LINES }\n"
  "   PickStyle { style UNPICKABLE }\n"
  "   Cube { }\n"
  "}\n"
  "\n"
  "\n"
  "DEF tabBoxTranslator Separator {\n"
  "   DrawStyle { style INVISIBLE }\n"
  "   Coordinate3 { point [ -1 -1 0, 1 -1 0, 1 1 0, -1 1 0 ] }\n"
  "   IndexedFaceSet { coordIndex [ 0, 1, 2, 3, -1 ] }\n"
  "}\n";

#endif /* ! SO_TABBOXDRAGGER_IV_H */

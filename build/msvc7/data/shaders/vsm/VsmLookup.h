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

#ifndef SO_VSMLOOKUP_GLSL_H
#define SO_VSMLOOKUP_GLSL_H

static const char VSMLOOKUP_shadersource[] =
  "float VsmLookup(in vec4 map, in float dist, in float epsilon, float bleedthreshold)\n"
  "{\n"
  "  float mapdist = map.x;\n"
  "\n"
  "  // replace 0.0 with some factor > 0.0 to make the light affect even parts in shadow\n"
  "  float lit_factor = dist <= mapdist ? 1.0 : 0.0;\n"
  "  float E_x2 = map.y;\n"
  "  float Ex_2 = mapdist * mapdist;\n"
  "  float variance = min(max(E_x2 - Ex_2, 0.0) + epsilon, 1.0);\n"
  "\n"
  "  float m_d = mapdist - dist;\n"
  "  float p_max = variance / (variance + m_d * m_d);\n"
  "\n"
  "  p_max *= smoothstep(bleedthreshold, 1.0, p_max);\n"
  "\n"
  "  return max(lit_factor, p_max);\n"
  "}\n";

#endif /* ! SO_VSMLOOKUP_GLSL_H */

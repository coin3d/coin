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

#ifndef SO_DIRECTIONALLIGHT_GLSL_H
#define SO_DIRECTIONALLIGHT_GLSL_H

static const char DIRECTIONALLIGHT_shadersource[] =
  "\n"
  "void DirectionalLight(in int i,\n"
  "                      in vec3 normal,\n"
  "                      inout vec4 ambient,\n"
  "                      inout vec4 diffuse,\n"
  "                      inout vec4 specular)\n"
  "{\n"
  "  float nDotVP; // normal . light direction\n"
  "  float nDotHV; // normal . light half vector\n"
  "  float pf;     // power factor\n"
  "  \n"
  "  nDotVP = max(0.0, dot(normal, normalize(vec3(gl_LightSource[i].position))));\n"
  "  nDotHV = max(0.0, dot(normal, vec3(gl_LightSource[i].halfVector)));\n"
  "\n"
  "  float shininess = gl_FrontMaterial.shininess;   \n"
  "  if (nDotVP == 0.0) \n"
  "    pf = 0.0;\n"
  "  else\n"
  "    pf = pow(nDotHV, shininess);\n"
  "  \n"
  "  ambient += gl_LightSource[i].ambient;\n"
  "  diffuse += gl_LightSource[i].diffuse * nDotVP;\n"
  "  specular += gl_LightSource[i].specular * pf;\n"
  "}\n"
  "\n"
  "\n";

#endif /* ! SO_DIRECTIONALLIGHT_GLSL_H */

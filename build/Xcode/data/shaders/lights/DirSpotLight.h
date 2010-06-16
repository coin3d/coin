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

#ifndef SO_DIRSPOTLIGHT_GLSL_H
#define SO_DIRSPOTLIGHT_GLSL_H

static const char DIRSPOTLIGHT_shadersource[] =
  "float DirSpotLight(in int i, \n"
  "      		      	     in vec3 eye, \n"
  "			     in vec3 ecPosition3,\n"
  "                       	     in vec3 normal,\n"
  "                       	     inout vec4 ambient,\n"
  "                       	     inout vec4 diffuse,\n"
  "                       	     inout vec4 specular)\n"
  "{\n"
  "  float nDotVP;\n"
  "  float nDotHV;\n"
  "  float pf;\n"
  "  vec3 dir = -normalize(vec3(gl_LightSource[i].spotDirection));\n"
  "  vec3 hv = normalize(eye + dir);\n"
  "  nDotVP = max(0.0, dot(normal, dir));\n"
  "  nDotHV = max(0.0, dot(normal, hv));\n"
  "  float shininess = gl_FrontMaterial.shininess;\n"
  "  if (nDotVP == 0.0)\n"
  "    pf = 0.0;\n"
  "  else\n"
  "    pf = pow(nDotHV, shininess);\n"
  " \n"
  "  ambient += gl_LightSource[i].ambient;\n"
  "  diffuse += gl_LightSource[i].diffuse * nDotVP;\n"
  "  specular += gl_LightSource[i].specular * pf;\n"
  "  return length(vec3(gl_LightSource[i].position) - ecPosition3);\n"
  "}\n";

#endif /* ! SO_DIRSPOTLIGHT_GLSL_H */

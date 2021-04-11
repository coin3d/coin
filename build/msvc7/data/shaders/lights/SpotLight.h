/**************************************************************************\
 * Copyright (c) Kongsberg Oil & Gas Technologies AS
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 * 
 * Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 * 
 * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 * 
 * Neither the name of the copyright holder nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
\**************************************************************************/

#ifndef COIN_INTERNAL
#error this is a private header file
#endif /* !COIN_INTERNAL */

#ifndef SO_SPOTLIGHT_GLSL_H
#define SO_SPOTLIGHT_GLSL_H

static const char SPOTLIGHT_shadersource[] =
  "\n"
  "float SpotLight(in vec3 light_position,\n"
  "                in vec3 light_attenuation,\n"
  "                in vec3 light_spotDirection,\n"
  "                in float light_spotExponent,\n"
  "                in float light_spotCosCutOff,\n"
  "                in vec3 eye,\n"
  "                in vec3 ecPosition3,\n"
  "                in vec3 normal,\n"
  "                inout vec4 ambient,\n"
  "                inout vec4 diffuse,\n"
  "                inout vec4 specular)\n"
  "{\n"
  "  float nDotVP;\n"
  "  float nDotHV;\n"
  "  float pf;\n"
  "  float att;\n"
  "  float spotDot;\n"
  "  float spotAtt;\n"
  "  float d;\n"
  "  vec3 VP;\n"
  "  vec3 halfvec;\n"
  "\n"
  "  VP = light_position - ecPosition3;\n"
  "  d = length(VP);\n"
  "  VP = normalize(VP);\n"
  "\n"
  "  att = 1.0 / (light_attenuation.x +\n"
  "               light_attenuation.y * d +\n"
  "               light_attenuation.z * d * d);\n"
  "\n"
  "  spotDot = dot(-VP, light_spotDirection);\n"
  "\n"
  "  // need to read this variable outside the if statement to work around ATi driver issues\n"
  "  float spotexp = light_spotExponent;\n"
  "\n"
  "  if (spotDot < light_spotCosCutOff)\n"
  "    spotAtt = 0.0;\n"
  "  else\n"
  "    spotAtt = pow(spotDot, spotexp);\n"
  "\n"
  "  att *= spotAtt;\n"
  "\n"
  "  halfvec = normalize(VP + eye);\n"
  "  nDotVP = max(0.0, dot(normal, VP));\n"
  "  nDotHV = max(0.0, dot(normal, halfvec));\n"
  "\n"
  "  // need to read this variable outside the if statement to work around ATi driver issues\n"
  "  float shininess =  gl_FrontMaterial.shininess;\n"
  "\n"
  "  if (nDotVP == 0.0)\n"
  "    pf = 0.0;\n"
  "  else\n"
  "    pf = pow(nDotHV, shininess);\n"
  "\n"
  "  ambient *= att;\n"
  "  diffuse *= nDotVP * att;\n"
  "  specular *= pf * att;\n"
  "\n"
  "  return d;\n"
  "}\n"
  "\n";

#endif /* ! SO_SPOTLIGHT_GLSL_H */

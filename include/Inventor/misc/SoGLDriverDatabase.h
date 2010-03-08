#ifndef SOGLDRIVERDATABASE_H
#define SOGLDRIVERDATABASE_H

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

#include <Inventor/C/glue/gl.h>
#include <Inventor/lists/SbList.h>

class SoGLDriver;
class SoGLDriverDatabaseP;
class SbName;

class COIN_DLL_API SoGLDriverDatabase {
public:
  static SbBool isSupported(const cc_glglue * context, const SbName & feature);
  static SbBool isBroken(const cc_glglue * context, const SbName & feature);
  static SbBool isSlow(const cc_glglue * context, const SbName & feature);
  static SbBool isFast(const cc_glglue * context, const SbName & feature);

  static SbName getComment(const cc_glglue * context, const SbName & feature);

  static void loadFromBuffer(const char * buffer);
  static void loadFromFile(const SbName & filename);

  static void addBuffer(const char * buffer);
  static void addFile(const SbName & filename);

  static void addFeature(const SbName & feature, const SbName & comment);

public:
  static void init(void);

private:
  static SoGLDriverDatabaseP * pimpl(void);
};

/**************************************************************************/

// OpenGL features that can't be checked with a single GL_ARB/EXT extension test
#define SO_GL_MULTIDRAW_ELEMENTS    "COIN_multidraw_elements"
#define SO_GL_POLYGON_OFFSET        "COIN_polygon_offset"
#define SO_GL_TEXTURE_OBJECT        "COIN_texture_object"
#define SO_GL_3D_TEXTURES           "COIN_3d_textures"
#define SO_GL_MULTITEXTURE          "COIN_multitexture"
#define SO_GL_TEXSUBIMAGE           "COIN_texsubimage"
#define SO_GL_2D_PROXY_TEXTURES     "COIN_2d_proxy_textures"
#define SO_GL_TEXTURE_EDGE_CLAMP    "COIN_texture_edge_clamp"
#define SO_GL_TEXTURE_COMPRESSION   "COIN_texture_compression"
#define SO_GL_COLOR_TABLES          "COIN_color_tables"
#define SO_GL_COLOR_SUBTABLES       "COIN_color_subtables"
#define SO_GL_PALETTED_TEXTURES     "COIN_paletted_textures"
#define SO_GL_BLEND_EQUATION        "COIN_blend_equation"
#define SO_GL_VERTEX_ARRAY          "COIN_vertex_array"
#define SO_GL_NV_VERTEX_ARRAY_RANGE "COIN_nv_vertex_array_range"
#define SO_GL_VERTEX_BUFFER_OBJECT  "COIN_vertex_buffer_object"
#define SO_GL_ARB_FRAGMENT_PROGRAM  "COIN_arb_fragment_program"
#define SO_GL_ARB_VERTEX_PROGRAM    "COIN_arb_vertex_program"
#define SO_GL_ARB_VERTEX_SHADER     "COIN_arb_vertex_shader"
#define SO_GL_ARB_SHADER_OBJECT     "COIN_arb_shader_object"
#define SO_GL_OCCLUSION_QUERY       "COIN_occlusion_query"
#define SO_GL_FRAMEBUFFER_OBJECT    "COIN_framebuffer_object"
#define SO_GL_ANISOTROPIC_FILTERING "COIN_anisotropic_filtering"
#define SO_GL_SORTED_LAYERS_BLEND   "COIN_sorted_layers_blend"
#define SO_GL_BUMPMAPPING           "COIN_bumpmapping"
#define SO_GL_VBO_IN_DISPLAYLIST    "COIN_vbo_in_displaylist"
#define SO_GL_NON_POWER_OF_TWO_TEXTURES "COIN_non_power_of_two_textures"
#define SO_GL_GENERATE_MIPMAP       "COIN_generate_mipmap"
#define SO_GL_GLSL_CLIP_VERTEX_HW   "COIN_GLSL_clip_vertex_hw"
#endif // SOGLDATABASE_H

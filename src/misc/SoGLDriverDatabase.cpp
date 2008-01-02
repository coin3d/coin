/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2008 by Kongsberg SIM.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Kongsberg SIM about acquiring
 *  a Coin Professional Edition License.
 *
 *  See http://www.coin3d.org/ for more information.
 *
 *  Kongsberg SIM, Postboks 1283, Pirsenteret, 7462 Trondheim, NORWAY.
 *  http://www.sim.no/  sales@sim.no  coin-support@coin3d.org
 *
\**************************************************************************/

/*!
  \class SoGLDriverDatabase SoGLDriverDatabase.h Inventor/misc/SoGLDriverDatabase.h
  \brief The SoGLDriverDatabase class is used for looking up broken/slow features in OpenGL drivers.

  Coin will maintain a database of drivers where we have found
  problems with certain features, even if the OpenGL driver claims to
  support it. This is an effort to avoid application or operating
  system crashes when Coin attempts to use a specific feature. Using
  this database we can either disable this feature, or find another
  way to handle it for broken drivers.

*/

#include "misc/SoGLDriverDatabase.h"

#include <string.h>

#include <Inventor/C/tidbits.h>
#include <Inventor/SbName.h>
#include <Inventor/C/glue/gl.h>
#include <Inventor/errors/SoDebugError.h>
#include <Inventor/C/tidbits.h>

#include "misc/SbHash.h"
#include "glue/glp.h"

class SoGLDriverDatabaseP {

  static SbBool multidraw_elements_wrapper(const cc_glglue * glue) {
    // FIXME: I'm not able to make glMultiDrawElement work under OS
    // X. It just crashes inside GL when I try to use it. Investigate
    // why this happens. For now we just avoid using
    // glMultiDrawElements() under OS X.  pederb, 2005-02-14
    SbBool ismac = (coin_runtime_os() == COIN_OS_X);
    if (!ismac) return cc_glglue_has_multidraw_vertex_arrays(glue); 
    return FALSE;
  }
  
  void initFunctions(void) {
    // define some reserved feature names for features that cannot be
    // tested directly as a single OpenGL extension test.
    this->featuremap.put(SbName(SO_GL_MULTIDRAW_ELEMENTS).getString(),
                         (glglue_feature_test_f *) &multidraw_elements_wrapper);
    this->featuremap.put(SbName(SO_GL_POLYGON_OFFSET).getString(),
                         (glglue_feature_test_f *) &cc_glglue_has_polygon_offset);
    this->featuremap.put(SbName(SO_GL_TEXTURE_OBJECT).getString(),
                         (glglue_feature_test_f *) &cc_glglue_has_texture_objects);
    this->featuremap.put(SbName(SO_GL_3D_TEXTURES).getString(),
                         (glglue_feature_test_f *) &cc_glglue_has_3d_textures);
    this->featuremap.put(SbName(SO_GL_MULTITEXTURE).getString(),
                         (glglue_feature_test_f *) &cc_glglue_has_multitexture);
    this->featuremap.put(SbName(SO_GL_TEXSUBIMAGE).getString(),
                         (glglue_feature_test_f *) &cc_glglue_has_texsubimage);
    this->featuremap.put(SbName(SO_GL_2D_PROXY_TEXTURES).getString(),
                         (glglue_feature_test_f *) &cc_glglue_has_2d_proxy_textures);
    this->featuremap.put(SbName(SO_GL_TEXTURE_EDGE_CLAMP).getString(),
                         (glglue_feature_test_f *) &cc_glglue_has_texture_edge_clamp);
    this->featuremap.put(SbName(SO_GL_TEXTURE_COMPRESSION).getString(),
                         (glglue_feature_test_f *) &cc_glue_has_texture_compression);
    this->featuremap.put(SbName(SO_GL_COLOR_TABLES).getString(),
                         (glglue_feature_test_f *) &cc_glglue_has_color_tables);
    this->featuremap.put(SbName(SO_GL_COLOR_SUBTABLES).getString(),
                         (glglue_feature_test_f *) &cc_glglue_has_color_subtables);
    this->featuremap.put(SbName(SO_GL_PALETTED_TEXTURES).getString(),
                         (glglue_feature_test_f *) &cc_glglue_has_paletted_textures);
    this->featuremap.put(SbName(SO_GL_BLEND_EQUATION).getString(),
                         (glglue_feature_test_f *) &cc_glglue_has_blendequation);
    this->featuremap.put(SbName(SO_GL_VERTEX_ARRAY).getString(),
                         (glglue_feature_test_f *) &cc_glglue_has_vertex_array);
    this->featuremap.put(SbName(SO_GL_NV_VERTEX_ARRAY_RANGE).getString(),
                         (glglue_feature_test_f *) &cc_glglue_has_nv_vertex_array_range);
    this->featuremap.put(SbName(SO_GL_VERTEX_BUFFER_OBJECT).getString(),
                         (glglue_feature_test_f *) &cc_glglue_has_vertex_buffer_object);
    this->featuremap.put(SbName(SO_GL_ARB_FRAGMENT_PROGRAM).getString(),
                         (glglue_feature_test_f *) &cc_glglue_has_arb_fragment_program);
    this->featuremap.put(SbName(SO_GL_ARB_VERTEX_PROGRAM).getString(),
                         (glglue_feature_test_f *) &cc_glglue_has_arb_vertex_program);
    this->featuremap.put(SbName(SO_GL_ARB_SHADER_OBJECT).getString(),
                         (glglue_feature_test_f *) &cc_glglue_has_arb_shader_objects);
    this->featuremap.put(SbName(SO_GL_OCCLUSION_QUERY).getString(),
                         (glglue_feature_test_f *) &cc_glglue_has_occlusion_query);
    this->featuremap.put(SbName(SO_GL_FRAMEBUFFER_OBJECT).getString(),
                         (glglue_feature_test_f *) &cc_glglue_has_framebuffer_objects);
    this->featuremap.put(SbName(SO_GL_ANISOTROPIC_FILTERING).getString(),
                         (glglue_feature_test_f *) &cc_glglue_can_do_anisotropic_filtering);
    this->featuremap.put(SbName(SO_GL_SORTED_LAYERS_BLEND).getString(),
                         (glglue_feature_test_f *) &cc_glglue_can_do_sortedlayersblend);
    this->featuremap.put(SbName(SO_GL_BUMPMAPPING).getString(),
                         (glglue_feature_test_f *) &cc_glglue_can_do_bumpmapping);
  }

  class SoGLDriver {
  public:
    typedef struct {
      int maxmajor, maxminor, maxmicro;
      int minmajor, minminor, minmicro;
    } versionrange;

    SbList <SbName> platform;
    SbList <SbName> vendor;
    SbList <SbName> renderer;
    SbList <versionrange> version;

    SbList <SbName> broken;
    SbList <SbName> slow;
    SbList <SbName> fast;
  };

  class FeatureID {
  public:
    uint32_t contextid;
    SbName feature;

    // needed for SbHash
    operator unsigned long(void) const {
      unsigned long bitmask = (unsigned long) ((uintptr_t) this->feature.getString());
      bitmask ^= contextid;
      return bitmask;
    }
    int operator==(const FeatureID & v) {
      return (this->contextid == v.contextid) && (this->feature == v.feature);
    }
  };

public:
  SoGLDriverDatabaseP() {
    this->initFunctions();
  }

  SbBool isSupported(const cc_glglue * context, const SbName & feature) {
    // check if we're asking about an actual GL extension
    const char * str = feature.getString();
    if ((feature.getLength() > 3) && (str[0] == 'G') && (str[1] == 'L') && (str[2] == '_')) {
      if (!cc_glglue_glext_supported(context, feature)) return FALSE;
    }
    else { // check our lookup table
      glglue_feature_test_f * testfunc;
      if (this->featuremap.get(feature.getString(), testfunc)) {
        if (!testfunc(context)) return FALSE;
      }
      else {
        SoDebugError::post("SoGLDriverDatabase::isSupported",
                           "Unknown feature '%s'.", feature.getString());
      }
    }
    return !this->isBroken(context, feature);
  }
  SbBool isBroken(const cc_glglue * context, const SbName & feature) {
    FeatureID f;
    f.contextid = context->contextid;
    f.feature = feature;

    SbBool broken = FALSE;
    if (!this->brokencache.get(f, broken)) {
      SoGLDriver * driver = this->findGLDriver(context);
      if (driver) {
        if (driver->broken.find(feature)) broken = TRUE;
      }
      this->brokencache.put(f, broken);
    }
    return broken;
  }
  SbBool isSlow(const cc_glglue * context, const SbName & feature) {
    if (!this->isSupported(context, feature)) {
      SoDebugError::post("SoGLDriverDatabase::isSlow",
                         "Feature '%s' is not supported for the specified context.",
                         feature.getString());
      return TRUE;
    }
    FeatureID f;
    f.contextid = context->contextid;
    f.feature = feature;

    SbBool slow = FALSE;
    if (!this->slowcache.get(f, slow)) {
      SoGLDriver * driver = this->findGLDriver(context);
      if (driver) {
        if (driver->slow.find(feature)) slow = TRUE;
      }
      this->slowcache.put(f, slow);
    }
    return slow;
  }
  SbBool isFast(const cc_glglue * context, const SbName & feature) {
    if (!this->isSupported(context, feature)) {
      SoDebugError::post("SoGLDriverDatabase::isFast",
                         "Feature '%s' is not supported for the specified context.",
                         feature.getString());
      return FALSE;
    }

    FeatureID f;
    f.contextid = context->contextid;
    f.feature = feature;

    SbBool fast = FALSE;
    if (!this->fastcache.get(f, fast)) {
      SoGLDriver * driver = this->findGLDriver(context);
      if (driver) {
        if (driver->fast.find(feature)) fast = TRUE;
      }
      this->fastcache.put(f, fast);
    }
    return fast;
  }

private:

  SoGLDriver * findGLDriver(const cc_glglue * context) {
    int major, minor, micro;
    SbName platform("");
    SbName vendor(context->vendorstr);
    SbName renderer(context->rendererstr);

    major = context->version.major;
    minor = context->version.minor;
    micro = context->version.release;

    // FIXME: implement

    return NULL;
  }
  SbList <SoGLDriver*> driverlist;
  SbHash <SbBool, FeatureID> brokencache;
  SbHash <SbBool, FeatureID> slowcache;
  SbHash <SbBool, FeatureID> fastcache;

  typedef SbBool glglue_feature_test_f(const cc_glglue * glue);
  SbHash <glglue_feature_test_f *, const char *> featuremap;
};


/*!

  Convenience function which checks whether \a feature is suppported
  for \a context.  If \a feature is an OpenGL extension, it checks if
  it's actually supported by the driver, and then calls
  SoGLDriverDatabase::isBroken() to check if the feature is broken for
  \a context.

 */
SbBool
SoGLDriverDatabase::isSupported(const cc_glglue * context, const SbName & feature)
{
  return pimpl()->isSupported(context, feature);
}

/*!
  Checks the driver database to see if \a feature is tagged as broken.
*/
SbBool
SoGLDriverDatabase::isBroken(const cc_glglue * context, const SbName & feature)
{
  return pimpl()->isBroken(context, feature);
}

/*!
  Checks the driver database to see if \a feature is tagged as being slow.
*/
SbBool
SoGLDriverDatabase::isSlow(const cc_glglue * context, const SbName & feature)
{
  return pimpl()->isSlow(context, feature);
}

/*!
  Checks the driver database to see if \a feature is tagged as fast.
*/
SbBool
SoGLDriverDatabase::isFast(const cc_glglue * context, const SbName & feature)
{
  return pimpl()->isFast(context, feature);
}

static SoGLDriverDatabaseP * pimpl_instance = NULL;

static void sogldriverdatabase_atexit(void)
{
  delete pimpl_instance;
  pimpl_instance = NULL;
}

SoGLDriverDatabaseP *
SoGLDriverDatabase::pimpl(void)
{
  if (pimpl_instance == NULL) {
    pimpl_instance = new SoGLDriverDatabaseP;
    cc_coin_atexit((coin_atexit_f*) sogldriverdatabase_atexit);

  }
  return pimpl_instance;
}

/**************************************************************************/

void
SoGLDriverDatabase::init(void)
{
  // make sure the private static class is created to avoid race conditions
  (void) pimpl();
  
  // FIXME: parse the xml database file(s) here
}

/**************************************************************************/

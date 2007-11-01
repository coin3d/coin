/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2007 by Systems in Motion.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Systems in Motion about acquiring
 *  a Coin Professional Edition License.
 *
 *  See http://www.coin3d.org/ for more information.
 *
 *  Systems in Motion, Postboks 1283, Pirsenteret, 7462 Trondheim, NORWAY.
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

#include <Inventor/misc/SoGLDriverDatabase.h>
#include <Inventor/misc/SbHash.h>
#include <Inventor/C/tidbits.h>
#include <Inventor/SbName.h>
#include <Inventor/C/glue/gl.h>
#include <Inventor/C/glue/glp.h>
#include <Inventor/errors/SoDebugError.h>
#include <string.h>

class SoGLDriverDatabaseP {
  void initFunctions(void) {
    // define some reserved feature names for features that cannot be
    // tested directly as a single OpenGL extension test.
    this->gluefunctions.put(SbName("COIN_multidraw_vertex_arrays").getString(),
                            cc_glglue_has_multidraw_vertex_arrays);
    this->gluefunctions.put(SbName("COIN_polygon_offset").getString(),
                            cc_glglue_has_polygon_offset);
    this->gluefunctions.put(SbName("COIN_texture_objects").getString(),
                            cc_glglue_has_texture_objects);
    this->gluefunctions.put(SbName("COIN_3d_textures").getString(),
                            cc_glglue_has_3d_textures);
    this->gluefunctions.put(SbName("COIN_multitexture").getString(),
                            cc_glglue_has_multitexture);
    this->gluefunctions.put(SbName("COIN_texsubimage").getString(),
                            cc_glglue_has_texsubimage);
    this->gluefunctions.put(SbName("COIN_2d_proxy_textures").getString(),
                            cc_glglue_has_2d_proxy_textures);
    this->gluefunctions.put(SbName("COIN_texture_edge_clamp").getString(),
                            cc_glglue_has_texture_edge_clamp);
    this->gluefunctions.put(SbName("COIN_texture_compression").getString(),
                            cc_glue_has_texture_compression);
    this->gluefunctions.put(SbName("COIN_color_tables").getString(),
                            cc_glglue_has_color_tables);
    this->gluefunctions.put(SbName("COIN_color_subtables").getString(),
                            cc_glglue_has_color_subtables);
    this->gluefunctions.put(SbName("COIN_paletted_textures").getString(),
                            cc_glglue_has_paletted_textures);
    this->gluefunctions.put(SbName("COIN_blend_equation").getString(),
                            cc_glglue_has_blendequation);
    this->gluefunctions.put(SbName("COIN_vertex_array").getString(),
                            cc_glglue_has_vertex_array);
    this->gluefunctions.put(SbName("COIN_nv_vertex_array_range").getString(),
                            cc_glglue_has_nv_vertex_array_range);
    this->gluefunctions.put(SbName("COIN_vertex_buffer_object").getString(),
                            cc_glglue_has_vertex_buffer_object);
    this->gluefunctions.put(SbName("COIN_arb_fragment_program").getString(),
                            cc_glglue_has_arb_fragment_program);
    this->gluefunctions.put(SbName("COIN_arb_vertex_program").getString(),
                            cc_glglue_has_arb_vertex_program);
    this->gluefunctions.put(SbName("COIN_occlusion_query").getString(),
                            cc_glglue_has_occlusion_query);
    this->gluefunctions.put(SbName("COIN_framebuffer_object").getString(),
                            cc_glglue_has_framebuffer_objects);
    this->gluefunctions.put(SbName("COIN_anisotropic_filtering").getString(),
                            cc_glglue_can_do_anisotropic_filtering);
    this->gluefunctions.put(SbName("COIN_sorted_layers_blend").getString(),
                            cc_glglue_can_do_sortedlayersblend);
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
    if (feature.getLength() > 3) {
      const char * str = feature.getString();
      if ((str[0] == 'G') && (str[1] == 'L') && (str[2] == '_')) {
        if (!cc_glglue_glext_supported(context, feature)) return FALSE;
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
  SbHash <glglue_feature_test_f *, const char *> gluefunctions;
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

#ifndef COIN_GLPERFORMANCE_H
#define COIN_GLPERFORMANCE_H

/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2005 by Systems in Motion.  All rights reserved.
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
 *  See <URL:http://www.coin3d.org/> for more information.
 *
 *  Systems in Motion, Postboks 1283, Pirsenteret, 7462 Trondheim, NORWAY.
 *  <URL:http://www.sim.no/>.
 *
\**************************************************************************/

#include <Inventor/C/glue/gl.h>
#include <Inventor/SbTime.h>

// *************************************************************************

typedef void cc_perf_pre_cb(const cc_glglue * g, void * userdata);
typedef void cc_perf_render_cb(const cc_glglue * g, void * userdata);
typedef void cc_perf_post_cb(const cc_glglue * g, void * userdata);

// *************************************************************************

/*
  Usage example from SIM Voleon:

  \code
      const cc_perf_render_cb * perfchkfuncs[2] = {
        SoVolumeRenderP::render2DTexturedTriangles,
        SoVolumeRenderP::render3DTexturedTriangles
      };
      double timings[2];
    
      const SbTime t =
        cc_perf_gl_timer(glglue, 2, perfchkfuncs, timings,
                         SoVolumeRenderP::setupPerformanceTest,
                         SoVolumeRenderP::cleanupPerformanceTest);
    
      const double rating = timings[1] / timings[0]; // 3d / 2d
      if (rating < 10.0f) { // 2D should at least be this many times
                            // faster before 3D texturing is dropped.
        do3dtextures = 1;
        return TRUE;
      }
  \endcode

  Render the stuff approximately at origo, as the camera is set up
  before invoking the user-supplied render callback as a perspective
  camera at <0, 0, -0.5>, 45° vertical field-of-view, near plane at
  0.1 and far plane at 10.0.

  An important note about the usage: be aware that this is not meant
  to be used for measuring cases where there are small differences in
  performance between competing techiques due to e.g. better pipeline
  parallelization or other such, rather marginal, effects.

  This is supposed to be used to smoke out systems/drivers where there
  are *major* performance hits suffered for OpenGL techniques that are
  available, but not optimal for use. This can for instance happen if
  a technique is available in the driver, but is not hardware
  accelerated with the particular hardware.  Case in point: 3D
  texturing is available in all OpenGL 1.2+ drivers, but often not
  hardware accelerated on many systems.  Or perhaps the GL feature we
  want to measure / check is just known to be really badly implemented
  on certain systems, or to have one or more bugs which causes major
  slowdowns on other systems.
*/

const SbTime cc_perf_gl_timer(const cc_glglue * glue,
                              const unsigned int nrrendercbs,
                              cc_perf_render_cb * rendercbs[],
                              double averagerendertime[],

                              cc_perf_pre_cb * precb = NULL,
                              cc_perf_post_cb * postcb = NULL,
                              const unsigned int maxruns = 10,
                              const SbTime maxtime = SbTime(0.5),
                              void * userdata = NULL);

// *************************************************************************

#endif // ! COIN_GLPERFORMANCE_H

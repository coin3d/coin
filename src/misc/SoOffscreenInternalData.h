#ifndef COIN_SOOFFSCREENINTERNALDATA_H
#define COIN_SOOFFSCREENINTERNALDATA_H

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
#include <Inventor/SbVec2s.h>
#include <Inventor/errors/SoDebugError.h>
#include <Inventor/lists/SbList.h>
#include <Inventor/misc/SoContextHandler.h>


class SoOffscreenInternalData {
public:
  SoOffscreenInternalData(void) { this->buffersize = SbVec2s(0, 0); }
  virtual ~SoOffscreenInternalData() { }

  // Return FALSE if the necessary resources for rendering are not
  // available.
  virtual SbBool makeContextCurrent(uint32_t contextid) = 0;
  virtual void unmakeContextCurrent(void) = 0;

  virtual unsigned char * getBuffer(void) = 0;

  virtual void setBufferSize(const SbVec2s & size) {
    if ((size[0] <= 0) || (size[1] <= 0)) {
      SoDebugError::post("SoOffscreenInternalData::setBufferSize",
                         "invalid dimensions attempted set: <%d, %d> -- ignored",
                         size[0], size[1]);
      return;
    }

    this->buffersize = size;
  }

  SbVec2s getBufferSize(void) const { return this->buffersize; }

  // add an id to the list of id used for the current context
  void addContextId(const uint32_t id) {
    if (this->contextidused.find(id) == -1) {
      this->contextidused.append(id);
    }
  }
  // notify SoContextHandler about destruction
  void destructingContext(void) {
    if (this->contextidused.getLength()) {
      // just use one of the context ids.
      this->makeContextCurrent(this->contextidused[0]);
      for (int i = 0; i < this->contextidused.getLength(); i++) {
        SoContextHandler::destructingContext(this->contextidused[i]);
      }
      this->contextidused.truncate(0);
      this->unmakeContextCurrent();
    }
  }

  void postRender(void) {
    SbVec2s size = this->getBufferSize();

    glPushAttrib(GL_ALL_ATTRIB_BITS);

    // First reset all settings that can influence the result of a
    // glReadPixels() call, to make sure we get the actual contents of
    // the buffer, unmodified.
    //
    // The values set up below matches the default settings of an
    // OpenGL driver.

    glPixelStorei(GL_PACK_SWAP_BYTES, 0);
    glPixelStorei(GL_PACK_LSB_FIRST, 0);
    glPixelStorei(GL_PACK_ROW_LENGTH, 0);
    glPixelStorei(GL_PACK_SKIP_ROWS, 0);
    glPixelStorei(GL_PACK_SKIP_PIXELS, 0);
    glPixelStorei(GL_PACK_ALIGNMENT, 4);

    glPixelTransferi(GL_MAP_COLOR, 0);
    glPixelTransferi(GL_MAP_STENCIL, 0);
    glPixelTransferi(GL_INDEX_SHIFT, 0);
    glPixelTransferi(GL_INDEX_OFFSET, 0);
    glPixelTransferf(GL_RED_SCALE, 1);
    glPixelTransferf(GL_RED_BIAS, 0);
    glPixelTransferf(GL_GREEN_SCALE, 1);
    glPixelTransferf(GL_GREEN_BIAS, 0);
    glPixelTransferf(GL_BLUE_SCALE, 1);
    glPixelTransferf(GL_BLUE_BIAS, 0);
    glPixelTransferf(GL_ALPHA_SCALE, 1);
    glPixelTransferf(GL_ALPHA_BIAS, 0);
    glPixelTransferf(GL_DEPTH_SCALE, 1);
    glPixelTransferf(GL_DEPTH_BIAS, 0);

    GLuint i = 0;
    GLfloat f = 0.0f;
    glPixelMapfv(GL_PIXEL_MAP_I_TO_I, 1, &f);
    glPixelMapuiv(GL_PIXEL_MAP_S_TO_S, 1, &i);
    glPixelMapfv(GL_PIXEL_MAP_I_TO_R, 1, &f);
    glPixelMapfv(GL_PIXEL_MAP_I_TO_G, 1, &f);
    glPixelMapfv(GL_PIXEL_MAP_I_TO_B, 1, &f);
    glPixelMapfv(GL_PIXEL_MAP_I_TO_A, 1, &f);
    glPixelMapfv(GL_PIXEL_MAP_R_TO_R, 1, &f);
    glPixelMapfv(GL_PIXEL_MAP_G_TO_G, 1, &f);
    glPixelMapfv(GL_PIXEL_MAP_B_TO_B, 1, &f);
    glPixelMapfv(GL_PIXEL_MAP_A_TO_A, 1, &f);

    // The flushing of the OpenGL pipeline before and after the
    // glReadPixels() call is done as a work-around for a reported
    // OpenGL driver bug: on a Win2000 system with ATI Radeon graphics
    // card, the system would hang hard if the flushing was not
    // done.
    //
    // This is obviously an OpenGL driver bug, but the workaround of
    // doing excessive flushing has no real ill effects, so we just do
    // it unconditionally for all drivers. Note that it might not be
    // necessary to flush both before and after glReadPixels() to work
    // around the bug (this was not established with the external
    // reporter), but again it shouldn't matter if we do.
    //
    // For reference, the specific driver which was reported to fail:
    //
    // GL_VENDOR="ATI Technologies Inc."
    // GL_RENDERER="Radeon 9000 DDR x86/SSE2"
    // GL_VERSION="1.3.3446 Win2000 Release"
    //
    // mortene.

    glFlush(); glFinish();
    glReadPixels(0, 0, size[0], size[1], GL_RGBA, GL_UNSIGNED_BYTE,
                 this->getBuffer());
    glPixelStorei(GL_PACK_ALIGNMENT, 4);
    glFlush(); glFinish();

    glPopAttrib();
  }

protected:
  SbVec2s buffersize;

private:
  SbList<uint32_t> contextidused;
};

#endif // !COIN_SOOFFSCREENINTERNALDATA_H

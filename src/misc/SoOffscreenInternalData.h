#ifndef COIN_SOOFFSCREENINTERNALDATA_H
#define COIN_SOOFFSCREENINTERNALDATA_H

/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2003 by Systems in Motion.  All rights reserved.
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
 *  See <URL:http://www.coin3d.org> for  more information.
 *
 *  Systems in Motion, Teknobyen, Abels Gate 5, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no>.
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
    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    glReadPixels(0, 0, size[0], size[1], GL_RGBA, GL_UNSIGNED_BYTE,
                 this->getBuffer());
    glPixelStorei(GL_PACK_ALIGNMENT, 4);
  }

protected:
  SbVec2s buffersize;

private:
  SbList<uint32_t> contextidused;
};

#endif // !COIN_SOOFFSCREENINTERNALDATA_H

/**************************************************************************\
 * 
 *  Copyright (C) 1998-1999 by Systems in Motion.  All rights reserved.
 *
 *  This file is part of the Coin library.
 *
 *  This file may be distributed under the terms of the Q Public License
 *  as defined by Troll Tech AS of Norway and appearing in the file
 *  LICENSE.QPL included in the packaging of this file.
 *
 *  If you want to use Coin in applications not covered by licenses
 *  compatible with the QPL, you can contact SIM to aquire a
 *  Professional Edition license for Coin.
 *
 *  Systems in Motion AS, Prof. Brochs gate 6, N-7030 Trondheim, NORWAY
 *  http://www.sim.no/ sales@sim.no Voice: +47 73540378 Fax: +47 73943861
 *
\**************************************************************************/

#ifndef _SO_GL_IMAGE_H_
#define _SO_GL_IMAGE_H_

// FIXME: couldn't some of this code be used independently of OpenGL?
// And shouldn't its inclusion be based on the presence of texture
// handling instead?  19990408 mortene.
#include <Inventor/confdep.h>
#if defined(COIN_EXCLUDE_SOGLRENDERACTION)
#error This file is excluded from Coin through the configuration system!
#endif // COIN_EXCLUDE_SOGLRENDERACTION

#include <Inventor/SbBasic.h>
#include <Inventor/SbString.h>
#include <Inventor/SbVec2s.h>

class SoImageInterface;

class SoGLImage {
public:

  // to create a SoGLImage from data already in memory
  SoGLImage(const SbVec2s size,
	    const int numComponents,
	    const unsigned char * bytes);
  
  void unref();

  
  static SoGLImage *getGLImage(const char * const texname, 
			       void * const context);
  
  SbBool init(const SbBool clamps, const SbBool clampt);
  SbBool isInitialized() const;
  void apply() const;
  int getHandle() const;
  SbBool hasAlphaComponent() const;
  const SoImageInterface *getImage() const;
  SbBool shouldClampS() const;
  SbBool shouldClampT() const;

private:
  friend class SoTexHandler;

  SoGLImage(SoImageInterface * const img, 
	    const char * const filename, 
	    const char * const org_name, 
	    void * const context);
  ~SoGLImage();

  void replaceImage(SoImageInterface * const newimage, 
		    const char * const fname,
		    const char * const oname);
  
  void checkResize();

  SoImageInterface *image;
  SbString orgname;
  SbString filename;
  
  unsigned int alpha : 1;
  unsigned int clampS : 1;
  unsigned int clampT : 1;

  void *context;
  int refCount;
  int handle;
};

#endif // _SO_GL_IMAGE_H_

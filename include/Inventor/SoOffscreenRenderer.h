#ifndef COIN_SOOFFSCREENRENDERER_H
#define COIN_SOOFFSCREENRENDERER_H

/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2001 by Systems in Motion.  All rights reserved.
 *  
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  version 2 as published by the Free Software Foundation.  See the
 *  file LICENSE.GPL at the root directory of this source distribution
 *  for more details.
 *
 *  If you desire to use Coin with software that is incompatible
 *  licensewise with the GPL, and / or you would like to take
 *  advantage of the additional benefits with regard to our support
 *  services, please contact Systems in Motion about acquiring a Coin
 *  Professional Edition License.  See <URL:http://www.coin3d.org> for
 *  more information.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  <URL:http://www.sim.no>, <mailto:support@sim.no>
 *
\**************************************************************************/

#include <Inventor/SbViewportRegion.h>
#include <Inventor/SbColor.h>
#include <Inventor/lists/SbList.h>
#include <Inventor/SbString.h>
#include <Inventor/SbName.h>

#include <stdio.h>

class SoBase;
class SoGLRenderAction;
class SoNode;
class SoPath;

// This shouldn't strictly be necessary, but the OSF1/cxx compiler
// complains if this is left out, while using the "friend class
// SoExtSelectionP" statement in the class definition.
class SoOffscreenRendererP;


class COIN_DLL_API SoOffscreenRenderer {
public:
  enum Components {
    LUMINANCE = 1,
    LUMINANCE_TRANSPARENCY = 2,
    RGB = 3,
    RGB_TRANSPARENCY = 4
  };

  SoOffscreenRenderer(const SbViewportRegion & viewportregion);
  SoOffscreenRenderer(SoGLRenderAction * action);
  ~SoOffscreenRenderer();

  static float getScreenPixelsPerInch(void);
  static SbVec2s getMaximumResolution(void);
  void setComponents(const Components components);
  Components getComponents(void) const;
  void setViewportRegion(const SbViewportRegion & region);
  const SbViewportRegion & getViewportRegion(void) const;
  void setBackgroundColor(const SbColor & color);
  const SbColor & getBackgroundColor(void) const;
  void setGLRenderAction(SoGLRenderAction * action);
  SoGLRenderAction * getGLRenderAction(void) const;
  SbBool render(SoNode * scene);
  SbBool render(SoPath * scene);
  unsigned char * getBuffer(void) const;

  SbBool writeToRGB(FILE * fp) const;
  SbBool writeToPostScript(FILE * fp) const;
  SbBool writeToPostScript(FILE * fp, const SbVec2f & printsize) const;

  SbBool writeToRGB(const char * filename) const;
  SbBool writeToPostScript(const char * filename) const;
  SbBool writeToPostScript(const char * filename, const SbVec2f & printsize) const;
  
  SbBool isWriteSupported(const SbName & filetypeextension) const;
  int getNumWriteFiletypes(void) const;
  void getWriteFiletypeInfo(const int idx,
                            SbList <SbName> & extlist,
                            SbString & fullname,
                            SbString & description);
  SbBool writeToFile(const SbString & filename, const SbName & filetypeextension) const; 

private:
  friend class SoOffscreenRendererP;
  class SoOffscreenRendererP * pimpl;
};

#endif // !COIN_SOOFFSCREENRENDERER_H

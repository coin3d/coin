#ifndef COIN_SOPOINTLIGHT_H
#define COIN_SOPOINTLIGHT_H

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

#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/fields/SoSFVec3f.h>
#include <Inventor/nodes/SoLight.h>

class COIN_DLL_API SoPointLight : public SoLight {
  typedef SoLight inherited;

  SO_NODE_HEADER(SoPointLight);

public:
  static void initClass(void);
  SoPointLight(void);

  SoSFVec3f location;

  virtual void GLRender(SoGLRenderAction * action);

protected:
  virtual ~SoPointLight();
};

#endif // !COIN_SOPOINTLIGHT_H

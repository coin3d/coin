/**************************************************************************\
 *
 *  Copyright (C) 1998-2000 by Systems in Motion.  All rights reserved.
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
 *  http://www.sim.no/ sales@sim.no Voice: +47 22114160 Fax: +47 67172912
 *
\**************************************************************************/

#ifndef COIN_SOANNOTATION_H
#define COIN_SOANNOTATION_H

#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/nodes/SoSeparator.h>


class COIN_DLL_EXPORT SoAnnotation : public SoSeparator {
  typedef SoSeparator inherited;

  SO_NODE_HEADER(SoAnnotation);

public:
  static void initClass(void);
  SoAnnotation(void);

  virtual void GLRender(SoGLRenderAction * action);
  virtual void GLRenderBelowPath(SoGLRenderAction * action);
  virtual void GLRenderInPath(SoGLRenderAction * action);
  virtual void GLRenderOffPath(SoGLRenderAction * action);

protected:
  virtual ~SoAnnotation();
};

#endif // !COIN_SOANNOTATION_H

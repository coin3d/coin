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

#ifndef COIN_SOGLCLIPPLANEELEMENT_H
#define COIN_SOGLCLIPPLANEELEMENT_H

#include <Inventor/elements/SoClipPlaneElement.h>


class COIN_DLL_EXPORT SoGLClipPlaneElement : public SoClipPlaneElement {
  typedef SoClipPlaneElement inherited;

  SO_ELEMENT_HEADER(SoGLClipPlaneElement);
public:
  static void initClass(void);
protected:
  virtual ~SoGLClipPlaneElement();

public:
  virtual void init(SoState * state);
  virtual void pop(SoState * state,
                   const SoElement * prevTopElement);
  static  int getMaxGLPlanes(void);

protected:
  virtual void addToElt(const SbPlane & plane,
                        const SbMatrix & modelMatrix);

private:
  static int maxGLPlanes;

};

#endif // !COIN_SOGLCLIPPLANEELEMENT_H

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
 *  http://www.sim.no/ sales@sim.no Voice: +47 22114160 Fax: +47 67172912
 *
\**************************************************************************/

#ifndef __SOGLPOINTSIZEELEMENT_H__
#define __SOGLPOINTSIZEELEMENT_H__

#include <Inventor/elements/SoPointSizeElement.h>


class SoGLPointSizeElement : public SoPointSizeElement {
  typedef SoPointSizeElement inherited;

  SO_ELEMENT_HEADER(SoGLPointSizeElement);
public:
  static void initClass(void);
protected:
  virtual ~SoGLPointSizeElement();

public:
  virtual void init(SoState * state);

  virtual void push(SoState * state);
  virtual void pop(SoState * state,
                   const SoElement * prevTopElement);

  void evaluate() const;

protected:
  virtual void setElt(float size);

private:
  static float sizerange[2];
  float current;
  void updategl();

};

#endif // !__SOGLPOINTSIZEELEMENT_H__

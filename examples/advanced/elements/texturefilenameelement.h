/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2000 by Systems in Motion. All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public License
 *  version 2.1 as published by the Free Software Foundation. See the
 *  file LICENSE.LGPL at the root directory of the distribution for
 *  more details.
 *
 *  If you want to use Coin for applications not compatible with the
 *  LGPL, please contact SIM to acquire a Professional Edition license.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  http://www.sim.no support@sim.no Voice: +47 22114160 Fax: +47 22207097
 *
\**************************************************************************/

#ifndef TEXTUREFILENAMEELEMENT_H_
#define TEXTUREFILENAMEELEMENT_H_

#include <Inventor/elements/SoReplacedElement.h>
#include <Inventor/SbString.h>
#include <assert.h>

class TextureFilenameElement : public SoReplacedElement {
  typedef SoReplacedElement inherited;

  SO_ELEMENT_HEADER(TextureFilenameElement);
public:
  static void initClass(void);
protected:
  virtual ~TextureFilenameElement();

public:
  virtual void init(SoState * state);
  static void set(SoState * const state, SoNode * const node,
                  const SbString & filename);
  static const SbString & get(SoState * const state);
  static const TextureFilenameElement * getInstance(SoState * state);

protected:
  virtual void setElt(const SbString & filename);

  SbString filename;
};

#endif // !TEXTUREFILENAMEELEMENT_H_

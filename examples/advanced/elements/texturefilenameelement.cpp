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

/*
  The purpose of the code in this file is to demonstrate how you
  can make your own elements for scene graph traversals.

  See the description and source code in lstextures.cpp for an
  explanation on how new, user-defined extensions elements can
  be put to use.

  Code by Peder Blekken <pederb@sim.no>, 1999-12-09.
 */

#include "texturefilenameelement.h"


SO_ELEMENT_SOURCE(TextureFilenameElement);


void
TextureFilenameElement::initClass(void)
{
  SO_ELEMENT_INIT_CLASS(TextureFilenameElement, inherited);
}

void
TextureFilenameElement::init(SoState * /* state */)
{
  this->filename.makeEmpty();
}

TextureFilenameElement::~TextureFilenameElement()
{
}

void
TextureFilenameElement::set(SoState * const state, SoNode * const node,
                            const SbString & filename)
{
  TextureFilenameElement * elem = (TextureFilenameElement *)
    SoReplacedElement::getElement(state, classStackIndex, node);
  elem->setElt(filename);
}

const SbString &
TextureFilenameElement::get(SoState * const state)
{
  return TextureFilenameElement::getInstance(state)->filename;
}

void
TextureFilenameElement::setElt(const SbString & filename)
{
  this->filename = filename;
}

const TextureFilenameElement *
TextureFilenameElement::getInstance(SoState * state)
{
  return (const TextureFilenameElement *)
    SoElement::getConstElement(state, classStackIndex);
}

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

TextureFilenameElement::TextureFilenameElement(void)
{
  this->setTypeId(TextureFilenameElement::classTypeId);
  this->setStackIndex(TextureFilenameElement::classStackIndex);
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

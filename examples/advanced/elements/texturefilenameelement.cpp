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

#include "texturefilenameelement.h"

SO_ELEMENT_SOURCE(SoTextureFilenameElement);

void
SoTextureFilenameElement::initClass()
{
  SO_ELEMENT_INIT_CLASS(SoTextureFilenameElement, inherited);
}

void
SoTextureFilenameElement::init(SoState * /* state */)
{
  this->filename.makeEmpty();
}

SoTextureFilenameElement::SoTextureFilenameElement()
{
  setTypeId(SoTextureFilenameElement::classTypeId);
  setStackIndex(SoTextureFilenameElement::classStackIndex);
}

SoTextureFilenameElement::~SoTextureFilenameElement()
{
}

void
SoTextureFilenameElement::set(SoState * const state, SoNode * const node,
                              const SbString & filename)
{
  SoTextureFilenameElement * elem = (SoTextureFilenameElement *)
    SoReplacedElement::getElement(state, classStackIndex, node);
  elem->setElt(filename);
}

const SbString &
SoTextureFilenameElement::get(SoState * const state)
{
  return SoTextureFilenameElement::getInstance(state)->filename;
}

void
SoTextureFilenameElement::setElt(const SbString & filename)
{
  this->filename = filename;
}

const SoTextureFilenameElement *
SoTextureFilenameElement::getInstance(SoState * state)
{
  return (const SoTextureFilenameElement *)
    SoElement::getConstElement(state, classStackIndex);
}

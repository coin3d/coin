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

/*!
  \class SoFieldConverter SoFieldConverter.h Inventor/engines/SoFieldConverter.h 
  \brief The SoFieldConverter class is the abstract base class for field converters.
  \ingroup engines

  When fields of different types are attempted connected, the Coin
  library tries to find a field converter class which can be inserted
  between them, acting as a filter converting values from the master
  field to values matching the type of the slave field.

  If a value type conversion is possible (like between an SoSFFloat
  field and an SoSFInt32 field, for instance, where we could do a
  simple typecast for converting the value of one to the other), an
  SoFieldConverter derived object is inserted.

  This class is the abstract base superclass which all such field
  converters needs to inherit.

  Coin comes with one built-in field converter class which takes care
  of all possible field-to-field conversions we know about. This means
  applications programmers should seldom need to care about this
  class, it will probably only be useful if you expand the Coin
  library with your own field type classes. (Doing so is considered
  advanced use of the library.)

  \sa SoDB::addConverter()
 */

#include <Inventor/engines/SoConvertAll.h>
#include <coindefs.h> // COIN_STUB()

SO_ENGINE_ABSTRACT_SOURCE(SoFieldConverter);


SoFieldConverter::SoFieldConverter(void)
{
}

SoFieldConverter::~SoFieldConverter()
{
}

void
SoFieldConverter::initClass(void)
{
  SO_ENGINE_INTERNAL_INIT_ABSTRACT_CLASS(SoFieldConverter);
  SoFieldConverter::initClasses();
}

void
SoFieldConverter::initClasses(void)
{
  SoConvertAll::initClass();
}

SoField *
SoFieldConverter::getConnectedInput(void)
{
  COIN_STUB();
  return 0L;
}

int
SoFieldConverter::getForwardConnections(SoFieldList & l) const
{
  COIN_STUB();
  return 0;
}

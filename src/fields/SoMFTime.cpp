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
  \class SoMFTime SoMFTime.h Inventor/fields/SoMFTime.h
  \brief The SoMFTime class is a container for SbTime values.
  \ingroup fields

  This field is used where nodes, engines or other field containers
  needs to store multiple time representations.

  \sa SoSFTime

*/

#include <Inventor/fields/SoMFTime.h>
#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG



SO_MFIELD_SOURCE(SoMFTime, SbTime, const SbTime &);

// Override from parent class.
void
SoMFTime::initClass(void)
{
  SO_MFIELD_INTERNAL_INIT_CLASS(SoMFTime);
}

// No need to document readValue() and writeValue() here, as the
// necessary information is provided by the documentation of the
// parent classes.
#ifndef DOXYGEN_SKIP_THIS

// These are implemented in the SoSFTime class.
extern SbBool sosftime_read_value(SoInput * in, SbTime & val);
extern void sosftime_write_value(SoOutput * out, const SbTime & val);

SbBool
SoMFTime::read1Value(SoInput * in, int idx)
{
  SbTime p;
  if (!sosftime_read_value(in, p)) return FALSE;
  this->set1Value(idx, p);
  return TRUE;
}

void
SoMFTime::write1Value(SoOutput * out, int idx) const
{
  sosftime_write_value(out, (*this)[idx]);
}

#endif // DOXYGEN_SKIP_THIS

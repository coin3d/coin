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
  \class SoMFName SoMFName.h Inventor/fields/SoMFName.h
  \brief The SoMFName class is a container for SbName values.
  \ingroup fields

  This field is used where nodes, engines or other field containers
  needs to store arrays of names.

  \sa SoSFName

*/

#include <Inventor/fields/SoMFName.h>
#include <SoSubFieldPrivate.h>
#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

#include <Inventor/fields/SoSFString.h>


SO_MFIELD_SOURCE(SoMFName, SbName, const SbName &);


// Override from parent class.
void
SoMFName::initClass(void)
{
  SO_MFIELD_INTERNAL_INIT_CLASS(SoMFName);
}

// No need to document readValue() and writeValue() here, as the
// necessary information is provided by the documentation of the
// parent classes.
#ifndef DOXYGEN_SKIP_THIS

// These are implemented in the SoSFName class.
extern SbBool sosfname_read_value(SoInput * in, SbName & val);
extern void sosfname_write_value(SoOutput * out, const SbName & val);

SbBool
SoMFName::read1Value(SoInput * in, int idx)
{
  SbName n;
  if (!sosfname_read_value(in, n)) return FALSE;
  this->set1Value(idx, n);
  return TRUE;
}

void
SoMFName::write1Value(SoOutput * out, int idx) const
{
  sosfname_write_value(out, (*this)[idx]);
}

#endif // DOXYGEN_SKIP_THIS


/*!
  Set \a num \a strings from index \a start in this multiple-value
  field instance.
*/
void
SoMFName::setValues(const int start, const int num, const char * strings[])
{
  if(start+num > this->maxNum) this->allocValues(start+num);
  else if(start+num > this->num) this->num = start+num;

  for(int i=0; i < num; i++) this->values[i+start] = SbName(strings[i]);
  this->valueChanged();
}

/*!
  Set this field to contain only a single name, given by \a str.
*/
void
SoMFName::setValue(const char * str)
{
  this->setValue(SbName(str));
}

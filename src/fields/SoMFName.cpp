/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2002 by Systems in Motion.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Systems in Motion about acquiring
 *  a Coin Professional Edition License.
 *
 *  See <URL:http://www.coin3d.org> for  more information.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no>.
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
#include <Inventor/fields/SoSubFieldP.h>
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

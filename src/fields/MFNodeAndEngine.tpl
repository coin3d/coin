//$ TEMPLATE MFNodeAndEngine(_Typename_, _typename_)

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

/*!
  \class SoMF_Typename_ SoMF_Typename_.h Inventor/fields/SoMF_Typename_.h
  \brief The SoMF_Typename_ class is a container for _typename_s.
  \ingroup fields

  This field container stores an array of pointers to _typename_s. It takes
  care of the necessary functionality for handling copy, import and
  export operations.

  Note that _typename_ pointers stored in field instances of this type may
  be \c NULL pointers.

  \sa So_Typename_, SoSF_Typename_

*/

#include <Inventor/fields/SoMF_Typename_.h>
#include <Inventor/fields/SoSubFieldP.h>
#include <Inventor/fields/SoSF_Typename_.h>
#include <Inventor/_typename_s/So_Typename_.h>
#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG


SO_MFIELD_REQUIRED_SOURCE(SoMF_Typename_);
SO_MFIELD_CONSTRUCTOR_SOURCE(SoMF_Typename_);
SO_MFIELD_MALLOC_SOURCE(SoMF_Typename_, So_Typename_ *);



// Override from parent class.
void
SoMF_Typename_::initClass(void)
{
  SO_MFIELD_INTERNAL_INIT_CLASS(SoMF_Typename_);
}


// No need to document readValue() and writeValue() here, as the
// necessary information is provided by the documentation of the
// parent classes.
#ifndef DOXYGEN_SKIP_THIS

//// From the SO_MFIELD_VALUE_SOURCE macro, start. ///////////////////////////

// We can't use the macro invocation, as we need to take care of doing
// ref() and unref() on the _typename_s in the array.

int
SoMF_Typename_::fieldSizeof(void) const
{
  return sizeof(So_Typename_ *);
}

void *
SoMF_Typename_::valuesPtr(void)
{
  return (void *)this->values;
}

void
SoMF_Typename_::setValuesPtr(void * ptr)
{
#if COIN_DEBUG
  // We don't get any ref()'ing done here, or any notification
  // mechanisms set up.
  SoDebugError::postWarning("SoMF_Typename_::setValuesPtr", "**dangerous code**");
#endif // COIN_DEBUG

  this->values = (So_Typename_ **)ptr;
}

int
SoMF_Typename_::find(So_Typename_ * value, SbBool addifnotfound)
{
  for (int i=0; i < this->num; i++) if ((*this)[i] == value) return i;

  if (addifnotfound) this->set1Value(this->num, value);
  return -1;
}

void
SoMF_Typename_::setValues(const int start, const int num, const So_Typename_ ** newvals)
{
  // Disable temporarily, so we under any circumstances will not send
  // more than one notification about the changes.
  SbBool notificstate = this->enableNotify(FALSE);

  // ref() new _typename_s before unref()-ing old ones, in case there are
  // common _typename_s (we don't want any premature destruction to happen).
  { for (int i=0; i < num; i++) if (newvals[i]) newvals[i]->ref(); }

  // We favor simplicity of code over performance here.
  { for (int i=0; i < num; i++)
    this->set1Value(start+i, (So_Typename_ *)newvals[i]); }

  // unref() to match the initial ref().
  { for (int i=0; i < num; i++) if (newvals[i]) newvals[i]->unref(); }

  // Finally, send notification.
  (void)this->enableNotify(notificstate);
  if (notificstate) this->valueChanged();
}

void
SoMF_Typename_::set1Value(const int idx, So_Typename_ * newval)
{
  // Disable temporarily, so we under no circumstances will send more
  // than one notification about the change.
  SbBool notificstate = this->enableNotify(FALSE);

  // Expand array if necessary.
  if (idx >= this->getNum()) this->setNum(idx + 1);

  So_Typename_ * oldptr = (*this)[idx];
  if (oldptr == newval) return;

  if (oldptr) {
    oldptr->removeAuditor(this, SoNotRec::FIELD);
    oldptr->unref();
  }

  if (newval) {
    newval->addAuditor(this, SoNotRec::FIELD);
    newval->ref();
  }

  this->values[idx] = newval;

  // Finally, send notification.
  (void)this->enableNotify(notificstate);
  if (notificstate) this->valueChanged();
}

void
SoMF_Typename_::setValue(So_Typename_ * value)
{
  this->deleteAllValues();
  this->set1Value(0, value);
}

SbBool
SoMF_Typename_::operator==(const SoMF_Typename_ & field) const
{
  if (this == &field) return TRUE;
  if (this->getNum() != field.getNum()) return FALSE;

  const So_Typename_ ** const lhs = this->getValues(0);
  const So_Typename_ ** const rhs = field.getValues(0);
  for (int i = 0; i < num; i++) if (lhs[i] != rhs[i]) return FALSE;
  return TRUE;
}

void
SoMF_Typename_::deleteAllValues(void)
{
  if (this->getNum()) this->deleteValues(0);
}

// Overloaded to handle unref() and removeAuditor().
void
SoMF_Typename_::deleteValues(int start, int num)
{
  if (num == -1) num = this->getNum() - 1 - start;
  for (int i=start; i < start+num; i++) {
    So_Typename_ * n = this->values[i];
    if (n) {
      n->removeAuditor(this, SoNotRec::FIELD);
      n->unref();
    }
  }

  inherited::deleteValues(start, num);
}

// Overloaded to insert NULL pointers in new array slots.
void
SoMF_Typename_::insertSpace(int start, int num)
{
  // Disable temporarily so we don't send notification prematurely
  // from inherited::insertSpace().
  SbBool notificstate = this->enableNotify(FALSE);

  inherited::insertSpace(start, num);
  for (int i=start; i < start+num; i++) this->values[i] = NULL;

  // Initialization done, now send notification.
  (void)this->enableNotify(notificstate);
  if (notificstate) this->valueChanged();
}

void
SoMF_Typename_::copyValue(int to, int from)
{
  if (to == from) return;
  this->set1Value(to, this->values[from]);
}

//// From the SO_MFIELD_VALUE_SOURCE macro, end. /////////////////////////////


// Import a single _typename_.
SbBool
SoMF_Typename_::read1Value(SoInput * in, int index)
{
  SoSF_Typename_ sf_typename_;
  SbBool result = sf_typename_.readValue(in);
  if (result) this->set1Value(index, sf_typename_.getValue());
  return result;
}

// Export a single _typename_.
void
SoMF_Typename_::write1Value(SoOutput * out, int idx) const
{
  SoSF_Typename_ sf_typename_;
  sf_typename_.setValue((*this)[idx]);
  sf_typename_.writeValue(out);
}

#endif // DOXYGEN_SKIP_THIS


// Overridden from parent to propagate write reference counting to
// _typename_.
void
SoMF_Typename_::countWriteRefs(SoOutput * out) const
{
  inherited::countWriteRefs(out);

  for (int i=0; i < this->getNum(); i++) {
    So_Typename_ * n = (*this)[i];
    // Set the "from field" flag as FALSE, as that flag is meant to be
    // used for references through field-to-field connections.
    if (n) n->addWriteReference(out, FALSE);
  }
}

// Override from parent to update our _typename_ pointer references. This is
// necessary so 1) we're added as an auditor to the copied _typename_s (they
// have so far only been copied as pointer bits), and 2) so we
// increase the reference count.
void
SoMF_Typename_::fixCopy(SbBool copyconnections)
{
  for (int i=0; i < this->getNum(); i++) {
    So_Typename_ * n = (*this)[i];
    if (n) {
      SoFieldContainer * fc = SoFieldContainer::findCopy(n, copyconnections);
      this->set1Value(i, NULL); // Fool the set-as-same-value detection.
      this->set1Value(i, (So_Typename_ *)fc);
    }
  }
}

// Override from SoField to check _typename_ pointer.
SbBool
SoMF_Typename_::referencesCopy(void) const
{
  if (inherited::referencesCopy()) return TRUE;

  for (int i=0; i < this->getNum(); i++) {
    So_Typename_ * n = (*this)[i];
    if (n && SoFieldContainer::checkCopy(n)) return TRUE;
  }

  return FALSE;
}

//$ TEMPLATE SFNodeAndEngine(_Typename_, _typename_)

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
  \class SoSF_Typename_ SoSF_Typename_.h Inventor/fields/SoSF_Typename_.h
  \brief The SoSF_Typename_ class is a container for a single _typename_.
  \ingroup fields

  This field container stores a pointer to a Coin _typename_. It takes care
  of the necessary functionality for handling copy, import and export
  operations.

  Note that the _typename_ pointer stored in a field instance of this type
  may be a \c NULL pointer.

  \sa So_Typename_, SoMF_Typename_

*/

#include <Inventor/fields/SoSF_Typename_.h>
#include <Inventor/fields/SoSubFieldP.h>

#include <Inventor/SoInput.h>
#include <Inventor/SoOutput.h>
#include <Inventor/actions/SoWriteAction.h>
#include <Inventor/errors/SoReadError.h>
#include <Inventor/nodes/SoNode.h>
#include <Inventor/engines/SoEngine.h>
#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG


// Can't use SO_SFIELD_SOURCE() because we need to modify setValue()
// to ref and unref the passed _typename_.
SO_SFIELD_REQUIRED_SOURCE(SoSF_Typename_);


// Override from parent class.
void
SoSF_Typename_::initClass(void)
{
  SO_SFIELD_INTERNAL_INIT_CLASS(SoSF_Typename_);
}

// (Declarations hidden in SO_[S|M]FIELD_HEADER macro in header file,
// so don't use Doxygen commenting.)
#ifndef DOXYGEN_SKIP_THIS

/* Constructor, sets initial _typename_ pointer to a \c NULL pointer. */
SoSF_Typename_::SoSF_Typename_(void)
{
  this->value = NULL;
}

/* Destructor, dereferences the current _typename_ pointer if necessary. */
SoSF_Typename_::~SoSF_Typename_(void)
{
  this->enableNotify(FALSE);
  this->setValue(NULL);
}

#endif // DOXYGEN_SKIP_THIS


// No need to document readValue() and writeValue() here, as the
// necessary information is provided by the documentation of the
// parent classes.
#ifndef DOXYGEN_SKIP_THIS

// Store the \a newval _typename_ pointer in this field. If \a newval is not
// \c NULL, will add 1 to the reference count of the _typename_.
void
SoSF_Typename_::setValue(So_Typename_ * newval)
{
  So_Typename_ * oldptr = this->getValue();
  if (oldptr == newval) return;

  if (oldptr) {
    oldptr->removeAuditor(this, SoNotRec::FIELD);
    oldptr->unref();
  }

  if (newval) {
    newval->addAuditor(this, SoNotRec::FIELD);
    newval->ref();
  }

  this->value = newval;
  this->valueChanged();
}

// Compares to see if the \a field points to the same _typename_ as this
// field does, and returns \c TRUE if this is the case.
//
// Be aware that this method does \e not check for _typename_/subgraph
// equality if the pointers are not the same, so \c FALSE is returned
// even though the contents of the _typename_/subgraph are equal.
SbBool
SoSF_Typename_::operator==(const SoSF_Typename_ & field) const
{
  return (this->getValue() == field.getValue());
}

// Import _typename_.
SbBool
SoSF_Typename_::readValue(SoInput * in)
{
  SoBase * baseptr;
  if (!SoBase::read(in, baseptr, So_Typename_::getClassTypeId())) return FALSE;

  if (in->eof()) {
    SoReadError::post(in, "Premature end of file");
    return FALSE;
  }

  this->setValue((So_Typename_ *)baseptr);
  return TRUE;
}

// Export _typename_.
void
SoSF_Typename_::writeValue(SoOutput * out) const
{
  SoBase * base = this->getValue();
  if (base) {
    if (base->isOfType(SoNode::getClassTypeId())) {
      SoWriteAction wa(out);
      wa.continueToApply((SoNode *)base);
    }
    else if (base->isOfType(SoEngine::getClassTypeId())) {
      ((SoEngine *)base)->writeInstance(out);
    }
    else {
      assert(0 && "strange internal error");
    }
  }
  else {
    // This actually works for both ASCII and binary formats.
    out->write("NULL");
  }
}

#endif // DOXYGEN_SKIP_THIS


// Overridden from parent to propagate write reference counting to
// _typename_.
void
SoSF_Typename_::countWriteRefs(SoOutput * out) const
{
  inherited::countWriteRefs(out);

  So_Typename_ * n = this->getValue();
  // Set the "from field" flag as FALSE, is that flag is meant to be
  // used for references through field-to-field connections.
  if (n) n->addWriteReference(out, FALSE);
}

// Override from parent to update our _typename_ pointer reference.
void
SoSF_Typename_::fixCopy(SbBool copyconnections)
{
  So_Typename_ * n = this->getValue();
  if (!n) return;

  // There's only been a bitwise copy of the pointer; no auditing has
  // been set up, no increase in the reference count. So we do that by
  // hand.
  n->addAuditor(this, SoNotRec::FIELD);
  n->ref();

  // Make sure copyContents() is run.
  (void)SoFieldContainer::findCopy(n, copyconnections);
}

// Override from SoField to check _typename_ pointer.
SbBool
SoSF_Typename_::referencesCopy(void) const
{
  if (inherited::referencesCopy()) return TRUE;

  So_Typename_ * n = this->getValue();
  if (n && SoFieldContainer::checkCopy(n)) return TRUE;

  return FALSE;
}

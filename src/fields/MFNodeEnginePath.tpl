//$ TEMPLATE MFNodeEnginePath(_Typename_, _typename_)

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

#include <Inventor/SoPath.h>
#include <Inventor/engines/SoEngine.h>
#include <Inventor/nodes/SoNode.h>
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
  if (idx >= this->getNum()) {
#ifdef COIN_SOMFPATH_H
    for (int i = this->getNum(); i <= idx; i++) this->pathheads.append(NULL);
#endif // COIN_SOMFPATH_H
    this->setNum(idx + 1);
  }

  So_Typename_ * oldptr = (*this)[idx];
  if (oldptr == newval) return;

  if (oldptr) {
    oldptr->removeAuditor(this, SoNotRec::FIELD);
    oldptr->unref();
#ifdef COIN_SOMFPATH_H
    SoNode * h = oldptr->getHead();
    // The path should be audited by us at all times. So don't use
    // SoMFPath to wrap SoTempPath or SoLightPath, for instance.
    assert(h==this->pathheads[idx] &&
	   "Path head changed without notification!");
    if (h) {
      h->removeAuditor(this, SoNotRec::FIELD);
      h->unref();
    }
#endif // COIN_SOMFPATH_H
  }

  if (newval) {
    newval->addAuditor(this, SoNotRec::FIELD);
    newval->ref();
#ifdef COIN_SOMFPATH_H
    SoNode * h = newval->getHead();
    if (h) {
      h->addAuditor(this, SoNotRec::FIELD);
      h->ref();
    }
#endif // COIN_SOMFPATH_H
  }

  this->values[idx] = newval;
#ifdef COIN_SOMFPATH_H
  this->pathheads[idx] = newval ? newval->getHead() : NULL;
#endif // COIN_SOMFPATH_H

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
#ifdef COIN_SOMFPATH_H
    SoNode * h = this->pathheads[start];
    this->pathheads.remove(start);
    if (h) {
      h->removeAuditor(this, SoNotRec::FIELD);
      h->unref();
    }
#endif // COIN_SOMFPATH_H
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
  for (int i=start; i < start+num; i++) {
#ifdef COIN_SOMFPATH_H
    this->pathheads.insert(NULL, start);
#endif // COIN_SOMFPATH_H
    this->values[i] = NULL;
  }

  // Initialization done, now send notification.
  (void)this->enableNotify(notificstate);
  if (notificstate) this->valueChanged();
}

// This is the "memmove replacement" to use copy constructors where
// they are defined.
void
SoMF_Typename_::copyValue(int to, int from)
{
  this->values[from] = this->values[to];
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
#ifdef COIN_SOMFPATH_H
    if (this->pathheads[i]) this->pathheads[i]->addWriteReference(out, FALSE);
#endif // COIN_SOMFPATH_H
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
      // There's only been a bitwise copy of the pointer; no auditing
      // has been set up, no increase in the reference count. So we do
      // that by setting the value to NULL and then re-setting with
      // setValue().
      this->values[i] = NULL;
#if defined(COIN_SOMFNODE_H) || defined(COIN_SOMFENGINE_H)
      SoFieldContainer * fc = SoFieldContainer::findCopy(n, copyconnections);
      this->set1Value(i, (So_Typename_ *)fc);
#endif // COIN_SOMFNODE_H || COIN_SOMFENGINE_H

#ifdef COIN_SOMFPATH_H
      this->set1Value(i, n->copy());
#endif // COIN_SOMFPATH_H
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
    if (n) {
      if (n->isOfType(SoNode::getClassTypeId()) ||
	  n->isOfType(SoEngine::getClassTypeId())) {
	if (SoFieldContainer::checkCopy((SoFieldContainer *)n)) return TRUE;
      }
      else if (n->isOfType(SoPath::getClassTypeId())) {
	SoPath * p = (SoPath *)n;
	if (p->getHead() && SoFieldContainer::checkCopy(p->getHead()))
	  return TRUE;
      }
      else {
	assert(0 && "strange internal error");
      }
    }
  }

  return FALSE;
}

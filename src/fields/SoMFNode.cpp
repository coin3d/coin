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
  \class SoMFNode SoMFNode.h Inventor/fields/SoMFNode.h
  \brief The SoMFNode class is a container for nodes.
  \ingroup fields

  This field container stores an array of pointers to nodes. It takes
  care of the necessary functionality for handling copy, import and
  export operations.

  Note that node pointers stored in field instances of this type may
  be \c NULL pointers.

  \sa SoNode, SoGroup, SoSFNode

*/

#include <Inventor/fields/SoMFNode.h>
#include <Inventor/fields/SoSubFieldP.h>
#include <Inventor/fields/SoSFNode.h>
#include <Inventor/nodes/SoNode.h>
#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG


SO_MFIELD_REQUIRED_SOURCE(SoMFNode);
SO_MFIELD_CONSTRUCTOR_SOURCE(SoMFNode);
SO_MFIELD_MALLOC_SOURCE(SoMFNode, SoNode *);



// Override from parent class.
void
SoMFNode::initClass(void)
{
  SO_MFIELD_INTERNAL_INIT_CLASS(SoMFNode);
}


// No need to document readValue() and writeValue() here, as the
// necessary information is provided by the documentation of the
// parent classes.
#ifndef DOXYGEN_SKIP_THIS

//// From the SO_MFIELD_VALUE_SOURCE macro, start. ///////////////////////////

// We can't use the macro invocation, as we need to take care of doing
// ref() and unref() on the nodes in the array.

int
SoMFNode::fieldSizeof(void) const
{
  return sizeof(SoNode *);
}

void *
SoMFNode::valuesPtr(void)
{
  return (void *)this->values;
}

void
SoMFNode::setValuesPtr(void * ptr)
{
  this->values = (SoNode * *)ptr;
}

int
SoMFNode::find(SoNode * value, SbBool addifnotfound)
{
  for (int i=0; i < this->num; i++) if ((*this)[i] == value) return i;

  if (addifnotfound) this->set1Value(this->num, value);
  return -1;
}

void
SoMFNode::setValues(const int start, const int num, const SoNode * * newvals)
{
  // ref() new nodes before unref()-ing old ones, in case there are
  // common nodes (we don't want any premature destruction to happen).
  for (int k=0; k < num; k++) if (newvals[k]) newvals[k]->ref();

  for (int j=start; (j < start+num) && (j < this->num); j++) {
    SoNode * n = (*this)[j];
    if (n) n->unref();
  }

  if (start+num > this->maxNum) this->allocValues(start+num);
  else if (start+num > this->num) this->num = start+num;

  for (int i=0; i < num; i++) this->values[i+start] = (SoNode *)newvals[i];

  this->valueChanged();
}

void
SoMFNode::set1Value(const int idx, SoNode * value)
{
  value->ref();

  if ((idx < this->num) && (*this)[idx]) (*this)[idx]->unref();

  if (idx+1 > this->maxNum) this->allocValues(idx+1);
  else if (idx+1 > this->num) this->num = idx+1;
  this->values[idx] = value;
  this->valueChanged();
}

void
SoMFNode::setValue(SoNode * value)
{
  this->deleteAllValues();

  this->values[0] = value;
  value->ref();
  this->valueChanged();
}

SbBool
SoMFNode::operator==(const SoMFNode & field) const
{
  if (this == &field) return TRUE;
  if (this->getNum() != field.getNum()) return FALSE;

  const SoNode * * const lhs = this->getValues(0);
  const SoNode * * const rhs = field.getValues(0);
  for (int i = 0; i < num; i++) if (lhs[i] != rhs[i]) return FALSE;
  return TRUE;
}

void
SoMFNode::deleteAllValues(void)
{
  for (int i=0; i < this->getNum(); i++) {
    SoNode * n = (*this)[i];
    if (n) n->unref();
  }

  this->allocValues(0);
}

void
SoMFNode::copyValue(int to, int from)
{
  assert(this->values && SbMax(to, from) < num);

  if ((*this)[to]) (*this)[to]->unref();
  this->values[to] = this->values[from];
  if ((*this)[to]) (*this)[to]->ref();

  this->valueChanged();
}

//// From the SO_MFIELD_VALUE_SOURCE macro, end. /////////////////////////////


// Import a single node.
SbBool
SoMFNode::read1Value(SoInput * in, int index)
{
  SoSFNode sfnode;
  SbBool result = sfnode.readValue(in);
  if (result) this->set1Value(index, sfnode.getValue());
  return result;
}

// Export a single node.
void
SoMFNode::write1Value(SoOutput * out, int idx) const
{
  SoSFNode sfnode;
  sfnode.setValue((*this)[idx]);
  sfnode.writeValue(out);
}

#endif // DOXYGEN_SKIP_THIS


// Overridden from parent to propagate write reference counting to
// node.
void
SoMFNode::countWriteRefs(SoOutput * out) const
{
  inherited::countWriteRefs(out);

  for (int i=0; i < this->getNum(); i++) {
    SoNode * n = (*this)[i];
    // Set the "from field" flag as FALSE, is that flag is meant to be
    // used for references through field-to-field connections.
    if (n) n->addWriteReference(out, FALSE);
  }
}

// Override from parent to update our node pointer reference, if
// necessary.
void
SoMFNode::fixCopy(SbBool copyconnections)
{
  for (int i=0; i < this->getNum(); i++) {
    SoNode * n = (*this)[i];
    if (n) {
      SoFieldContainer * fc = SoFieldContainer::findCopy(n, copyconnections);
      this->setValue((SoNode *)fc);
    }
  }
}

// Override from SoField to check node pointer.
SbBool
SoMFNode::referencesCopy(void) const
{
  if (inherited::referencesCopy()) return TRUE;

  for (int i=0; i < this->getNum(); i++) {
    SoNode * n = (*this)[i];
    if (n && SoFieldContainer::checkCopy(n)) return TRUE;
  }

  return FALSE;
}

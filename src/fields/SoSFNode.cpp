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
  \class SoSFNode SoSFNode.h Inventor/fields/SoSFNode.h
  \brief The SoSFNode class is a container for a single node.
  \ingroup fields

  This field container stores a pointer to a Coin node. It takes care
  of the necessary functionality for handling copy, import and export
  operations.

  Note that the node pointer stored in a field instance of this type
  may be a \c NULL pointer.

  \sa SoNode, SoMFNode

*/

#include <Inventor/fields/SoSFNode.h>

#include <Inventor/SoInput.h>
#include <Inventor/SoOutput.h>
#include <Inventor/actions/SoWriteAction.h>
#include <Inventor/errors/SoReadError.h>
#include <Inventor/fields/SoMFNode.h>
#include <Inventor/nodes/SoNode.h>
#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG


// Can't use SO_SFIELD_SOURCE() because we need to modify setValue()
// to ref and unref the passed node.
SO_SFIELD_REQUIRED_SOURCE(SoSFNode);


// Override from parent class.
void
SoSFNode::initClass(void)
{
  SO_SFIELD_INTERNAL_INIT_CLASS(SoSFNode);
}

/*!
  Constructor, sets initial node pointer to a \c NULL pointer.
*/
SoSFNode::SoSFNode(void)
{
  this->value = NULL;
}

/*!
  Destructor, dereferences the current node pointer if necessary.
*/
SoSFNode::~SoSFNode(void)
{
  this->setValue(NULL);
}


// No need to document readValue() and writeValue() here, as the
// necessary information is provided by the documentation of the
// parent classes.
#ifndef DOXYGEN_SKIP_THIS

// Store the \a newval node pointer in this field. If \a newval is not
// \c NULL, will add 1 to the reference count of the node.
void
SoSFNode::setValue(SoNode * newval)
{
  SoNode * oldptr = this->getValue();
  if (oldptr) oldptr->unref();

  this->value = newval;
  if (this->value) this->value->ref();

  this->valueChanged();
}

// Compares to see if the \a field points to the same node as this
// field does, and returns \c TRUE if this is the case.
//
// Be aware that this method does \e not check for node/subgraph
// equality if the pointers are not the same, so \c FALSE is returned
// even though the contents of the node/subgraph are equal.
SbBool
SoSFNode::operator==(const SoSFNode & field) const
{
  return (this->getValue() == field.getValue());
}

// Import node.
SbBool
SoSFNode::readValue(SoInput * in)
{
  SoBase * baseptr;
  if (!SoBase::read(in, baseptr, SoNode::getClassTypeId())) return FALSE;

  if (in->eof()) {
    SoReadError::post(in, "Premature end of file");
    return FALSE;
  }

  this->setValue((SoNode *)baseptr);
  return TRUE;
}

// Export node.
void
SoSFNode::writeValue(SoOutput * out) const
{
  SoNode * node = this->getValue();
  if (node) {
    SoWriteAction wa(out);
    wa.continueToApply(node);
  }
  else {
    // This actually works for both ASCII and binary formats.
    out->write("NULL");
  }
}

#endif // DOXYGEN_SKIP_THIS


// Overridden from parent to propagate write reference counting to
// node.
void
SoSFNode::countWriteRefs(SoOutput * out) const
{
  inherited::countWriteRefs(out);
  SoNode * n = this->getValue();
  // Set the "from field" flag as FALSE, is that flag is meant to be
  // used for references through field-to-field connections.
  if (n) n->addWriteReference(out, FALSE);
}

// Override from parent to update our node pointer reference, if
// necessary.
void
SoSFNode::fixCopy(SbBool copyconnections)
{
  SoNode * n = this->getValue();
  if (!n) return;

  SoFieldContainer * fc = SoFieldContainer::findCopy(n, copyconnections);
  this->setValue((SoNode *)fc);
}

// Override from SoField to check node pointer.
SbBool
SoSFNode::referencesCopy(void) const
{
  if (inherited::referencesCopy()) return TRUE;

  SoNode * n = this->getValue();
  if (n && SoFieldContainer::checkCopy(n)) return TRUE;

  return FALSE;
}

void
SoSFNode::convertTo(SoField * dest) const
{
  if (dest->getTypeId()==SoMFNode::getClassTypeId()) {
    ((SoMFNode *)dest)->setValue(this->getValue());
  }
#if COIN_DEBUG
  else {
    SoDebugError::post("SoSFNode::convertTo",
                       "Can't convert from %s to %s",
                       this->getTypeId().getName().getString(),
                       dest->getTypeId().getName().getString());
  }
#endif // COIN_DEBUG
}

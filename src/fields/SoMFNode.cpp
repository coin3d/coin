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
  \brief The SoMFNode class ...
  \ingroup fields

  FIXME: write class doc
*/

// FIXME:
/*¡
  SoMFNode::setValue() and SoMFNode::set1Value() should ref() and unref()
  nodes. I think. 19991113 mortene.
 */

#include <Inventor/errors/SoReadError.h>
#include <Inventor/fields/SoMFNode.h>
#include <Inventor/fields/SoSFNode.h>
#include <Inventor/nodes/SoNode.h>
#include <Inventor/SoInput.h>
#include <Inventor/SoOutput.h>
#include <Inventor/SbName.h>
#include <Inventor/misc/SoBasic.h> // COIN_STUB()
#include <assert.h>
#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG



SO_MFIELD_SOURCE_MALLOC(SoMFNode, SoNode *, SoNode *);


/*!
  Does initialization common for all objects of the
  SoMFNode class. This includes setting up the
  type system, among other things.
*/
void
SoMFNode::initClass(void)
{
  SO_MFIELD_INTERNAL_INIT_CLASS(SoMFNode);
}

/*!
  FIXME: write function documentation
*/
SbBool
SoMFNode::read1Value(SoInput * in, int index)
{
  SoSFNode sfnode;
  SbBool result = sfnode.readValue(in);
  if (result) this->set1Value(index, sfnode.getValue());
  return result;
}

/*!
  FIXME: write function documentation
*/
void
SoMFNode::write1Value(SoOutput * out, int idx) const
{
  SoSFNode sfnode;
  sfnode.setValue((*this)[idx]);
  sfnode.writeValue(out);
}

/*!
  FIXME: write function documentation
*/
void
SoMFNode::fixCopy(SbBool /* copyConnections */)
{
  COIN_STUB();
}

/*!
  FIXME: write function documentation
*/
SbBool
SoMFNode::referencesCopy(void) const
{
  COIN_STUB();
  return FALSE;
}

void
SoMFNode::convertTo(SoField * dest) const
{
  if (dest->getTypeId()==SoSFNode::getClassTypeId()) {
    if (this->getNum()>0)
      ((SoSFNode *)dest)->setValue((*this)[0]);
  }
#if COIN_DEBUG
  else {
    SoDebugError::post("SoMFNode::convertTo",
                       "Can't convert from %s to %s",
                       this->getTypeId().getName().getString(),
                       dest->getTypeId().getName().getString());
  }
#endif // COIN_DEBUG
}

/**************************************************************************\
 * 
 *  Copyright (C) 1998-1999 by Systems in Motion.  All rights reserved.
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
  \class SoReplacedElement Inventor/elements/SoReplacedElement.h
  \brief The SoReplacedElement class is yet to be documented.

  FIXME: write doc.
*/

#include <Inventor/elements/SoReplacedElement.h>

#include <Inventor/SbName.h>
#include <Inventor/nodes/SoNode.h>
#include <Inventor/SoType.h>

#include <assert.h>

/*!
  \fn SoReplacedElement::nodeId

  FIXME: write doc.
*/

//$ BEGIN TEMPLATE AbstractElementSource( SoReplacedElement )

/*!
  \var SoReplacedElement::classTypeId

  This is the static class type identifier for
  SoReplacedElement.
*/

SoType SoReplacedElement::classTypeId = SoType::badType(); // static

/*!
  This static method returns the SoType type for SoReplacedElement.
*/

SoType
SoReplacedElement::getClassTypeId( // static
  void )
{
  return SoReplacedElement::classTypeId;
} // getClassTypeId()

/*!
  \var SoReplacedElement::classStackIndex

  This is the static state stack index for the
  SoReplacedElement class.
*/

int SoReplacedElement::classStackIndex; // static

/*!
  This static method returns the state stack index for the SoReplacedElement class.
*/

int
SoReplacedElement::getClassStackIndex( // static
  void )
{
  return SoReplacedElement::classStackIndex;
} // getClassStackIndex()
//$ END TEMPLATE AbstractElementSource

/*!
  This static method initializes static data for the SoReplacedElement class.
*/

void
SoReplacedElement::initClass( // static
    void )
{
//$ BEGIN TEMPLATE InitAbstractElementSource( SoReplacedElement )
  assert(SoReplacedElement::classTypeId == SoType::badType());
  assert(inherited::getClassTypeId() != SoType::badType());

  SoReplacedElement::classTypeId = SoType::createType(
    inherited::getClassTypeId(),
    "SoReplacedElement", NULL );

  if ( inherited::classStackIndex < 0 ) {
    SoReplacedElement::classStackIndex =
      createStackIndex( SoReplacedElement::classTypeId );
  } else {
    SoReplacedElement::classStackIndex =
      inherited::classStackIndex;
  }
//$ END TEMPLATE InitAbstractElementSource
} // initClass()

/*!
  This static method cleans up static data for the SoReplacedElement class.
*/

void
SoReplacedElement::cleanClass( // static
    void )
{
//$ BEGIN TEMPLATE CleanAbstractElementSource( SoReplacedElement )
//$ END TEMPLATE CleanAbstractElementSource
} // cleanClass()

/*!
  The constructor.  Can't be used directly.  This is an abstract base class.
*/

SoReplacedElement::SoReplacedElement(
    void )
  : nodeId( 0 )
{
  setTypeId( SoReplacedElement::classTypeId );
  setStackIndex( SoReplacedElement::classStackIndex );
} // SoReplacedElement()

/*!
  The destructor.
*/

SoReplacedElement::~SoReplacedElement( // virtual
    void )
{
} // ~SoReplacedElement()

//! FIXME: write doc.

void
SoReplacedElement::init( // virtual
    SoState * state )
{
  inherited::init( state );
} // init()

//! FIXME: write doc.

SbBool
SoReplacedElement::matches( // virtual
    const SoElement * element ) const
{
  if (((const SoReplacedElement *)(element))->nodeId ==
      this->nodeId)
    return TRUE;
  return FALSE;
} // matches()

//! FIXME: write doc.

SoElement *
SoReplacedElement::copyMatchInfo( // virtual
    void ) const
{
  assert( getTypeId().canCreateInstance() );
  SoReplacedElement * element =
    (SoReplacedElement *)(getTypeId().createInstance());
  element->nodeId = this->nodeId;
  return element;
} // copyMatchInfo()

//! FIXME: write doc.

void
SoReplacedElement::print(FILE * file) const
{
  fprintf(file, "%s[%p]\n", getTypeId().getName().getString(),
	  this );
} // print()

/*!
  This function returns the top element on the stack.

  Originally, this function overrides SoElement::getElement() to set the
  nodeId in the instance before returning it, but I don't think that is
  necessary (should happen automatically) in push().
*/

SoElement *
SoReplacedElement::getElement(SoState * const state,
			      const int stackIndex,
			      SoNode * const node)
{
  SoReplacedElement *elem = 
    (SoReplacedElement*) SoElement::getElement( state, stackIndex );
  if (node)
    elem->nodeId = node->getNodeId();
  else elem->nodeId = 0;
  return elem;
} // getElement()

/*!
  \fn uint32_t SoReplacedElement::getNodeId( void ) const

  This function returns the node identifier for the node that previously
  updated the SoReplacedElement.
*/

//$ EXPORT INLINE
uint32_t 
SoReplacedElement::getNodeId() const
{ 
  return this->nodeId; 
}


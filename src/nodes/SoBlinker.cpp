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
  \class SoBlinker SoBlinker.h Inventor/nodes/SoBlinker.h
  \brief The SoBlinker class ...
  \ingroup nodes

  FIXME: write class doc
*/



#include <Inventor/nodes/SoBlinker.h>

/*!
  \var SoSFFloat SoBlinker::speed
  FIXME: write documentation for field
*/
/*!
  \var SoSFBool SoBlinker::on
  FIXME: write documentation for field
*/


// *************************************************************************

SO_NODE_SOURCE(SoBlinker);

/*!
  Constructor.
*/
SoBlinker::SoBlinker()
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoBlinker);

  SO_NODE_ADD_FIELD(speed, (1));
  SO_NODE_ADD_FIELD(on, (TRUE));
}

/*!
  Destructor.
*/
SoBlinker::~SoBlinker()
{
}

/*!
  Does initialization common for all objects of the
  SoBlinker class. This includes setting up the
  type system, among other things.
*/
void
SoBlinker::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoBlinker);
}

/*!
  FIXME: write doc
*/
void
SoBlinker::getBoundingBox(SoGetBoundingBoxAction * /* action */)
{
  COIN_STUB();
}

/*!
  FIXME: write doc
*/
void
SoBlinker::write(SoWriteAction * /* action */)
{
  COIN_STUB();
}

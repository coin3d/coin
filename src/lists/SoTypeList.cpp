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
  \class SoTypeList Inventor/lists/SoTypeList.h
  \brief The SoTypeList class is a container class for arrays of SoType
  objects.

  FIXME: write doc.
*/

// SoTypeList was moved from being a subclass of SbPList to being a
// subclass of SbList. This removed the need to do lots of ugly casts
// in overloaded methods, with the subsequent removal of all code in
// this file. 19991025 mortene.

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

#ifndef COIN_SOACCUMULATEDELEMENT_H
#define COIN_SOACCUMULATEDELEMENT_H

#include <Inventor/elements/SoSubElement.h>
#include <Inventor/lists/SbList.h>


class COIN_DLL_EXPORT SoAccumulatedElement : public SoElement {
  typedef SoElement inherited;

  SO_ELEMENT_ABSTRACT_HEADER(SoAccumulatedElement);
public:
  static void initClass(void);
protected:
  virtual ~SoAccumulatedElement();

public:
  virtual SbBool matches(const SoElement * element) const;
  virtual void print(FILE * file) const;

protected:
  void clearNodeIds(void);
  void addNodeId(const SoNode * const node);
  void setNodeId(const SoNode * const node);
  void copyNodeIds(const SoAccumulatedElement * copyfrom);
  virtual SoElement * copyMatchInfo(void) const;
  virtual void captureThis(SoState * state) const;

  SbList <uint32_t> nodeids;
  uint32_t checksum;
};

#endif // !COIN_SOACCUMULATEDELEMENT_H

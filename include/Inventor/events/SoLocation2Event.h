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
 *  LGPL, please contact SIM to aquire a Professional Edition license.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  http://www.sim.no support@sim.no Voice: +47 22114160 Fax: +47 22207097
 *
\**************************************************************************/

#ifndef COIN_SOLOCATION2EVENT_H
#define COIN_SOLOCATION2EVENT_H

#include <Inventor/events/SoSubEvent.h>


class COIN_DLL_EXPORT SoLocation2Event : public SoEvent {
  typedef SoEvent inherited;

  SO_EVENT_HEADER();

public:
  virtual ~SoLocation2Event();

  static void initClass(void);
};

#endif // !COIN_SOLOCATION2EVENT_H

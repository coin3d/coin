#ifndef COIN_SCXMLHISTORY_H
#define COIN_SCXMLHISTORY_H

/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2008 by Kongsberg SIM.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Kongsberg SIM about acquiring
 *  a Coin Professional Edition License.
 *
 *  See http://www.coin3d.org/ for more information.
 *
 *  Kongsberg SIM, Postboks 1283, Pirsenteret, 7462 Trondheim, NORWAY.
 *  http://www.sim.no/  sales@sim.no  coin-support@coin3d.org
 *
\**************************************************************************/

#include <Inventor/scxml/ScXMLStateBase.h>

class ScXMLTransition;

class COIN_DLL_API ScXMLHistory : public ScXMLStateBase {
  typedef ScXMLStateBase inherited;
  SCXML_OBJECT_HEADER(ScXMLHistory);

public:
  static void initClass(void);

  ScXMLHistory(void);
  virtual ~ScXMLHistory(void);

  virtual void setTypeXMLAttr(const char * type);
  const char * getTypeXMLAttr(void) const { return this->type; }

  virtual SbBool handleXMLAttributes(void);

  // transition
  virtual void setTransition(ScXMLTransition * transition);
  virtual ScXMLTransition * getTransition(void) const;

protected:
  char * type;

  ScXMLTransition * transitionptr;

}; // ScXMLHistory

#endif // !COIN_SCXMLHISTORY_H

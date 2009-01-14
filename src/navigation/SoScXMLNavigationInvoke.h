#ifndef COIN_SOSCXMLNAVIGATIONINVOKE_H
#define COIN_SOSCXMLNAVIGATIONINVOKE_H

/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2009 by Kongsberg SIM.  All rights reserved.
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

class SoCamera;
class SbRotation;
class SbVec2f;
class SbPlane;
class SoScXMLStateMachine;
class SoEvent;

#include <Inventor/scxml/ScXMLInvoke.h>

class SoScXMLNavigationInvoke : public ScXMLInvoke {
  typedef ScXMLInvoke inherited;
  SCXML_OBJECT_ABSTRACT_HEADER(SoScXMLNavigationInvoke);
public:
  static void initClass(void);

protected:
  static void reorientCamera(SoCamera * camera,
                             const SbRotation & rot);

  static void panCamera(SoCamera * camera,
                        float vpaspect,
                        const SbPlane & panplane,
                        const SbVec2f & previous,
                        const SbVec2f & current);

  static void zoom(SoCamera * camera,
                   float diffvalue);

  SoScXMLStateMachine * castToSo(ScXMLStateMachine * sm) const;
  const SoEvent * getSoEvent(SoScXMLStateMachine * sm) const;
  
}; // SoScXMLNavigationInvoke

#endif // !COIN_SOSCXMLNAVIGATIONINVOKE_H

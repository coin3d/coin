#ifndef COIN_SOSCXMLSEEKTARGET_H
#define COIN_SOSCXMLSEEKTARGET_H

/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) by Kongsberg Oil & Gas Technologies.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Kongsberg Oil & Gas Technologies
 *  about acquiring a Coin Professional Edition License.
 *
 *  See http://www.coin3d.org/ for more information.
 *
 *  Kongsberg Oil & Gas Technologies, Bygdoy Alle 5, 0257 Oslo, NORWAY.
 *  http://www.sim.no/  sales@sim.no  coin-support@coin3d.org
 *
\**************************************************************************/

#include <Inventor/navigation/SoScXMLNavigationTarget.h>

class SoSearchAction;
class SoGetMatrixAction;
class SoRayPickAction;

#define COIN_NAVIGATION_SEEK_EVENT_PREFIX SOSCXML_NAVIGATION_TARGETTYPE ".Seek"

class COIN_DLL_API SoScXMLSeekTarget : public SoScXMLNavigationTarget {
  typedef SoScXMLNavigationTarget inherited;
  SCXML_OBJECT_HEADER(SoScXMLSeekTarget)

public:
  static void initClass(void);
  static void cleanClass(void);

  static SoScXMLSeekTarget * constructSingleton(void);
  static void destructSingleton(void);
  static SoScXMLSeekTarget * singleton(void);

  static const SbName & BEGIN(void);
  static const SbName & UPDATE(void);
  static const SbName & END(void);

protected:
  SoScXMLSeekTarget(void);
  virtual ~SoScXMLSeekTarget(void);

  static const SbName & MISS(void);
  static const SbName & DONE(void);

  virtual SbBool processOneEvent(const ScXMLEvent * event);

private:
  SoSearchAction * searchaction;
  SoGetMatrixAction * getmatrixaction;
  SoRayPickAction * raypickaction;

  class PImpl;
  static SoScXMLSeekTarget * theSingleton;

}; // SoScXMLSeekTarget

#endif // !COIN_SOSCXMLSEEKTARGET_H

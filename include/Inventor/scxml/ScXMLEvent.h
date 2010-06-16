#ifndef COIN_SCXMLEVENT_H
#define COIN_SCXMLEVENT_H

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

#include <Inventor/scxml/ScXMLObject.h>
#include <Inventor/tools/SbLazyPimplPtr.h>
#include <Inventor/lists/SbList.h>

class COIN_DLL_API ScXMLEvent : public ScXMLObject {
  typedef ScXMLObject inherited;
  SCXML_OBJECT_HEADER(ScXMLEvent)

public:
  static void initClass(void);
  static void cleanClass(void);

  ScXMLEvent(void);
  virtual ~ScXMLEvent(void);

  virtual void setEventName(const SbName & name);
  const SbName & getEventName(void) const { return this->name; }

  ScXMLEvent * clone(void) const;

  // associations
  void setAssociation(const char * key, const char * value);
  const char * getAssociation(const char * key) const;

  size_t getNumAssociations(void) const;
  size_t getAssociationKeys(SbList<const char *> & keys) const;

protected:
  SbName name;

  virtual void copyContents(const ScXMLEvent * rhs);

private:
  ScXMLEvent(const ScXMLEvent & rhs); // N/A
  ScXMLEvent & operator = (const ScXMLEvent & rhs); // N/A

  class PImpl;
  SbLazyPimplPtr<PImpl> pimpl;
}; // ScXMLEvent

#endif // !COIN_SCXMLEVENT_H

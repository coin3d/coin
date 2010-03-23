#ifndef COIN_SCXMLANCHORELT_H
#define COIN_SCXMLANCHORELT_H

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

#include <Inventor/scxml/ScXMLElt.h>

#include <Inventor/tools/SbLazyPimplPtr.h>

class COIN_DLL_API ScXMLAnchorElt : public ScXMLElt {
  typedef ScXMLElt inherited;
  SCXML_ELEMENT_HEADER(ScXMLAnchorElt)

public:
  static void initClass(void);
  static void cleanClass(void);

  ScXMLAnchorElt(void);
  virtual ~ScXMLAnchorElt(void);

  virtual void setTypeAttribute(const char * type);
  const char * getTypeAttribute(void) const { return this->type; }

  virtual void setSnapshotAttribute(const char * snapshot);
  const char * getSnapshotAttribute(void) const { return this->snapshot; }

  virtual SbBool handleXMLAttributes(void);

  virtual void copyContents(const ScXMLElt * rhs);

  virtual const ScXMLElt * search(const char * attrname, const char * attrvalue) const;

protected:
  char * type;
  char * snapshot;

private:
  ScXMLAnchorElt(const ScXMLAnchorElt & rhs); // N/A
  ScXMLAnchorElt & operator = (const ScXMLAnchorElt & rhs); // N/A

  class PImpl;
  SbLazyPimplPtr<PImpl> pimpl;

}; // ScXMLAnchorElt

#endif // !COIN_SCXMLANCHORELT_H

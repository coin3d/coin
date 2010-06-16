#ifndef COIN_SCXMLDATAMODELELT_H
#define COIN_SCXMLDATAMODELELT_H

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

#include <Inventor/tools/SbPimplPtr.h>

class ScXMLDataElt;

class COIN_DLL_API ScXMLDataModelElt : public ScXMLElt {
  typedef ScXMLElt inherited;
  SCXML_ELEMENT_HEADER(ScXMLDataModelElt)

public:
  static void initClass(void);
  static void cleanClass(void);

  ScXMLDataModelElt(void);
  virtual ~ScXMLDataModelElt(void);

  virtual void setSchemaAttribute(const char * schema);
  virtual const char * getSchemaAttribute(void) const { return this->schema; }

  virtual SbBool handleXMLAttributes(void);

  virtual void copyContents(const ScXMLElt * rhs);

  virtual int getNumData(void) const;
  virtual ScXMLDataElt * getData(int idx) const;
  virtual void addData(ScXMLDataElt * data);
  virtual void removeData(ScXMLDataElt * data);
  virtual void clearAllData(void);

  virtual const ScXMLElt * search(const char * attrname, const char * attrvalue) const;

protected:
  char * schema;

private:
  ScXMLDataModelElt(const ScXMLDataModelElt & rhs); // N/A
  ScXMLDataModelElt & operator = (const ScXMLDataModelElt & rhs); // N/A

  class PImpl;
  SbPimplPtr<PImpl> pimpl;

}; // ScXMLDataModelElt

#endif // !COIN_SCXMLDATAMODELELT_H

#ifndef COIN_SCXMLDATAELT_H
#define COIN_SCXMLDATAELT_H

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

class ScXMLDataObj;

class COIN_DLL_API ScXMLDataElt : public ScXMLElt {
  typedef ScXMLElt inherited;
  SCXML_ELEMENT_HEADER(ScXMLDataElt)

public:
  static void initClass(void);
  static void cleanClass(void);

  ScXMLDataElt(void);
  virtual ~ScXMLDataElt(void);

  virtual void setIDAttribute(const char * ID);
  virtual const char * getIDAttribute(void) const { return this->ID; }

  virtual void setSrcAttribute(const char * src);
  virtual const char * getSrcAttribute(void) const { return this->src; }

  virtual void setExprAttribute(const char * expr);
  virtual const char * getExprAttribute(void) const { return this->expr; }

  virtual SbBool handleXMLAttributes(void);

  virtual void copyContents(const ScXMLElt * rhs);

  virtual const ScXMLElt * search(const char * attrname, const char * attrvalue) const;

  virtual void setValueObj(ScXMLDataObj * value);
  virtual ScXMLDataObj * getValueObj(void) const { return this->value; }

protected:
  char * ID;
  char * src;
  char * expr;
  ScXMLDataObj * value;

private:
  ScXMLDataElt(const ScXMLDataElt & rhs); // N/A
  ScXMLDataElt & operator = (const ScXMLDataElt & rhs); // N/A

  class PImpl;
  SbLazyPimplPtr<PImpl> pimpl;

}; // ScXMLDataElt

#endif // !COIN_SCXMLDATAELT_H

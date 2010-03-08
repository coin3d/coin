#ifndef COIN_SCXMLELT_H
#define COIN_SCXMLELT_H

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

#include <Inventor/SbBasic.h>
#include <Inventor/SoType.h>
#include <Inventor/tools/SbPimplPtr.h>
#include <Inventor/scxml/ScXMLObject.h>
#include <Inventor/scxml/ScXMLSubObject.h>

class ScXMLStateMachine;

class COIN_DLL_API ScXMLElt : public ScXMLObject {
  typedef ScXMLObject inherited;
  SCXML_ELEMENT_ABSTRACT_HEADER(ScXMLElt)

public:
  static void initClass(void);
  static void cleanClass(void);

  ScXMLElt(void);
  virtual ~ScXMLElt(void);

  void setXMLAttribute(const char * attribute, const char * value);
  const char * getXMLAttribute(const char * attribute) const;

  virtual SbBool handleXMLAttributes(void);

  virtual const ScXMLElt * search(const char * attrname, const char * attrvalue) const;

  ScXMLElt * clone(void) const;
  virtual void copyContents(const ScXMLElt * rhs);

  virtual void setContainer(ScXMLElt * container);
  ScXMLElt * getContainer(void) const { return this->containerptr; }

  SbBool isContainedIn(const ScXMLElt * object) const;

protected:
  static void registerClassType(const char * xmlns,
                                const char * classname, SoType type);
  static void registerInvokeClassType(const char * xmlns,
                                      const char * targettype,
                                      const char * source, SoType type);

private:
  ScXMLElt * containerptr;

private:
  ScXMLElt(const ScXMLElt & rhs); // N/A
  ScXMLElt & operator = (const ScXMLElt & rhs); // N/A

  class PImpl;
  SbPimplPtr<PImpl> pimpl;

}; // ScXMLElt

// *************************************************************************

class ScXMLDocument;
class ScXMLStateMachine;
struct cc_xml_elt;

class COIN_DLL_API ScXMLEltReader {
public:
  ScXMLEltReader(const char * eltname);
  virtual ~ScXMLEltReader(void);

  virtual ScXMLElt * read(ScXMLElt * container, cc_xml_elt * elt, 
                          ScXMLDocument * doc, ScXMLStateMachine * sm) = 0;

protected:
  const char * element;

  void setXMLAttributes(ScXMLElt * scxmlelt, cc_xml_elt * xmlelt);

}; // ScXMLEltReader

// *************************************************************************

#endif // !COIN_SCXMLELT_H

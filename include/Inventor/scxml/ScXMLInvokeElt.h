#ifndef COIN_SCXMLINVOKEELT_H
#define COIN_SCXMLINVOKEELT_H

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

#include <Inventor/scxml/ScXMLExecutableElt.h>

#include <Inventor/tools/SbLazyPimplPtr.h>

class ScXMLEvent;
class ScXMLStateMachine;
class ScXMLParamElt;
class ScXMLFinalizeElt;
class ScXMLContentElt;

class COIN_DLL_API ScXMLInvokeElt : public ScXMLExecutableElt {
  typedef ScXMLExecutableElt inherited;
  SCXML_ELEMENT_HEADER(ScXMLInvokeElt)

public:
  static void initClass(void);
  static void cleanClass(void);

  ScXMLInvokeElt(void);
  virtual ~ScXMLInvokeElt(void);

  // XML attributes
  virtual void setTargetTypeAttribute(const char * id);
  const char * getTargetTypeAttribute(void) const { return this->targettype; }

  virtual void setSrcAttribute(const char * id);
  const char * getSrcAttribute(void) const { return this->src; }

  virtual void setSrcExprAttribute(const char * id);
  const char * getSrcExprAttribute(void) const { return this->srcexpr; }

  virtual SbBool handleXMLAttributes(void);

  virtual void copyContents(const ScXMLElt * rhs);

  virtual const ScXMLElt * search(const char * attrname, const char * attrvalue) const;

  virtual int getNumParams(void) const;
  virtual ScXMLParamElt * getParam(int idx) const;
  virtual void addParam(ScXMLParamElt * param);
  virtual void removeParam(ScXMLParamElt * param);
  virtual void clearAllParams(void);

  virtual void setFinalize(ScXMLFinalizeElt * finalize);
  virtual ScXMLFinalizeElt * getFinalize(void) const;

  virtual void setContent(ScXMLContentElt * content);
  virtual ScXMLContentElt * getContent(void) const;

protected:
  char * targettype;
  char * src;
  char * srcexpr;

private:
  ScXMLInvokeElt(const ScXMLInvokeElt & rhs); // N/A
  ScXMLInvokeElt & operator = (const ScXMLInvokeElt & rhs); // N/A

  class PImpl;
  SbLazyPimplPtr<PImpl> pimpl;

}; // ScXMLInvoke

#endif // !COIN_SCXMLINVOKEELT_H

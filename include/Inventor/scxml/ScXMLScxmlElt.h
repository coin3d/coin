#ifndef COIN_SCXMLSCXMLELT_H
#define COIN_SCXMLSCXMLELT_H

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

class ScXMLInitialElt;
class ScXMLStateElt;
class ScXMLParallelElt;
class ScXMLFinalElt;
class ScXMLDataModelElt;
class ScXMLScriptElt;

class COIN_DLL_API ScXMLScxmlElt : public ScXMLElt {
  typedef ScXMLElt inherited;
  SCXML_ELEMENT_HEADER(ScXMLScxmlElt)

public:
  static void initClass(void);
  static void cleanClass(void);

  ScXMLScxmlElt(void);
  virtual ~ScXMLScxmlElt(void);

  void setIsReferenced(SbBool referenced);
  SbBool isReferenced(void) const;

  // Specified XML attributes
  virtual void setInitialAttribute(const char * initial);
  const char * getInitialAttribute(void) const { return this->initial; }

  virtual void setNameAttribute(const char * name);
  const char * getNameAttribute(void) const { return this->name; }

  virtual void setXMLNSAttribute(const char * xmlns);
  const char * getXMLNSAttribute(void) const { return this->xmlns; }

  virtual void setVersionAttribute(const char * version);
  const char * getVersionAttribute(void) const { return this->version; }

  virtual void setProfileAttribute(const char * profile);
  const char * getProfileAttribute(void) const { return this->profile; }

  virtual void setExModeAttribute(const char * exmode);
  const char * getExModeAttribute(void) const { return this->exmode; }

  virtual SbBool handleXMLAttributes(void);

  virtual void copyContents(const ScXMLElt * rhs);

  virtual const ScXMLElt * search(const char * attrname, const char * attrvalue) const;

  // initial
  virtual void setInitial(ScXMLInitialElt * initial);
  virtual ScXMLInitialElt * getInitial(void) const;

  // state storage
  virtual int getNumStates(void) const;
  virtual ScXMLStateElt * getState(int idx) const;
  virtual void addState(ScXMLStateElt * state);
  virtual void removeState(ScXMLStateElt * state);
  virtual void clearAllStates(void);

  // parallel storage
  virtual int getNumParallels(void) const;
  virtual ScXMLParallelElt * getParallel(int idx) const;
  virtual void addParallel(ScXMLParallelElt * state);
  virtual void removeParallel(ScXMLParallelElt * state);
  virtual void clearAllParallels(void);

  // final storage
  virtual int getNumFinals(void) const;
  virtual ScXMLFinalElt * getFinal(int idx) const;
  virtual void addFinal(ScXMLFinalElt * state);
  virtual void removeFinal(ScXMLFinalElt * state);
  virtual void clearAllFinals(void);

  // datamodel
  virtual void setDataModel(ScXMLDataModelElt * datamodel);
  virtual ScXMLDataModelElt * getDataModel(void) const;

  // script storage
  virtual int getNumScripts(void) const;
  virtual ScXMLScriptElt * getScript(int idx) const;
  virtual void addScript(ScXMLScriptElt * state);
  virtual void removeScript(ScXMLScriptElt * state);
  virtual void clearAllScripts(void);

  virtual void execute(ScXMLStateMachine * statemachine) const;

protected:
  SbBool referenced;

  char * initial;
  char * name;
  char * xmlns;
  char * version;
  char * profile;
  char * exmode;

private:
  ScXMLScxmlElt(const ScXMLScxmlElt & rhs); // N/A
  ScXMLScxmlElt & operator = (const ScXMLScxmlElt & rhs); // N/A

  class PImpl;
  SbPimplPtr<PImpl> pimpl;

}; // ScXMLScxmlElt

#endif // COIN_SCXMLSCXMLELT_H

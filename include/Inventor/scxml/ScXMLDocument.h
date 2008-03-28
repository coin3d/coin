#ifndef COIN_SCXMLDOCUMENT_H
#define COIN_SCXMLDOCUMENT_H

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

#include <Inventor/scxml/ScXMLObject.h>

#include <vector>

#include <Inventor/tools/SbLazyPimplPtr.h>

class ScXMLState;
class ScXMLFinal;
class ScXMLDocumentP;

class COIN_DLL_API ScXMLDocument : public ScXMLObject {
  typedef ScXMLObject inherited;
  SCXML_OBJECT_HEADER(ScXMLDocument);

public:
  static void initClass(void);

  ScXMLDocument(void);
  virtual ~ScXMLDocument(void);

  void setIsReferenced(SbBool referenced);
  SbBool isReferenced(void) const;

  // XML attributes
  virtual void setXMLNSXMLAttr(const char * xmlns);
  const char * getXMLNSXMLAttr(void) const { return this->xmlns; }

  virtual void setVersionXMLAttr(const char * version);
  const char * getVersionXMLAttr(void) const { return this->version; }
  
  virtual void setInitialStateXMLAttr(const char * initialstate);
  const char * getInitialStateXMLAttr(void) const { return this->initialstate; }

  virtual SbBool handleXMLAttributes(void);

  // state storage
  virtual int getNumStates(void) const;
  virtual ScXMLState * getState(int idx) const;
  virtual void addState(ScXMLState * state);
  virtual void removeState(ScXMLState * state);
  virtual void clearAllStates(void);

  // parallel storage
  virtual int getNumParallels(void) const;
  virtual ScXMLState * getParallel(int idx) const;
  virtual void addParallel(ScXMLState * state);
  virtual void removeParallel(ScXMLState * state);
  virtual void clearAllParallels(void);

  // final storage
  virtual int getNumFinals(void) const;
  virtual ScXMLFinal * getFinal(int idx) const;
  virtual void addFinal(ScXMLFinal * state);
  virtual void removeFinal(ScXMLFinal * state);
  virtual void clearAllFinals(void);

protected:
  SbBool referenced;

  char * xmlns;
  char * version;
  char * initialstate;

  std::vector<ScXMLState *> statelist;
  std::vector<ScXMLState *> parallellist;
  std::vector<ScXMLFinal *> finallist;
  // datamodel

private:
  ScXMLDocument(const ScXMLDocument & rhs); // N/A
  ScXMLDocument & operator = (const ScXMLDocument & rhs); // N/A

  SbLazyPimplPtr<ScXMLDocumentP> pimpl;

}; // ScXMLDocument

#endif // COIN_SCXMLDOCUMENT_H

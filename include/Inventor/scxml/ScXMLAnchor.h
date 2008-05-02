#ifndef COIN_SCXMLANCHOR_H
#define COIN_SCXMLANCHOR_H

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

#include <Inventor/tools/SbLazyPimplPtr.h>

class ScXMLAnchorP;

class COIN_DLL_API ScXMLAnchor : public ScXMLObject {
  typedef ScXMLObject inherited;
  SCXML_OBJECT_HEADER(ScXMLAnchor);

public:
  static void initClass(void);

  ScXMLAnchor(void);
  virtual ~ScXMLAnchor(void);

  virtual void setTypeAttribute(const char * type);
  const char * getTypeAttribute(void) const { return this->type; }

  virtual void setSnapshotAttribute(const char * snapshot);
  const char * getSnapshotAttribute(void) const { return this->snapshot; }

  virtual SbBool handleXMLAttributes(void);

protected:
  char * type;
  char * snapshot;

private:
  ScXMLAnchor(const ScXMLAnchor & rhs); // N/A
  ScXMLAnchor & operator = (const ScXMLAnchor & rhs); // N/A

  SbLazyPimplPtr<ScXMLAnchorP> pimpl;

}; // ScXMLAnchor

#endif // !COIN_SCXMLANCHOR_H

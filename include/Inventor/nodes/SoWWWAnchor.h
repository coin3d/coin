/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2000 by Systems in Motion. All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public License
 *  version 2.1 as published by the Free Software Foundation. See the
 *  file LICENSE.LGPL at the root directory of the distribution for
 *  more details.
 *
 *  If you want to use Coin for applications not compatible with the
 *  LGPL, please contact SIM to acquire a Professional Edition license.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  http://www.sim.no support@sim.no Voice: +47 22114160 Fax: +47 22207097
 *
\**************************************************************************/

#ifndef COIN_SOWWWANCHOR_H
#define COIN_SOWWWANCHOR_H

#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/nodes/SoLocateHighlight.h>
#include <Inventor/fields/SoSFString.h>
#include <Inventor/fields/SoSFEnum.h>


class SoWWWAnchor;

typedef void SoWWWAnchorCB(const SbString & url, void * data,
                           SoWWWAnchor * node);

// *************************************************************************

class COIN_DLL_EXPORT SoWWWAnchor : public SoLocateHighlight {
  typedef SoLocateHighlight inherited;

  SO_NODE_HEADER(SoWWWAnchor);

public:
  static void initClass(void);
  SoWWWAnchor(void);

  enum Mapping {
    NONE, POINT
  };

  SoSFString name;
  SoSFString description;
  SoSFEnum map;

  void setFullURLName(const SbString & url);
  const SbString & getFullURLName(void);

  virtual void handleEvent(SoHandleEventAction * action);

  static void setFetchURLCallBack(SoWWWAnchorCB * f, void * userData);
  static void setHighlightURLCallBack(SoWWWAnchorCB * f, void * userData);

protected:
  virtual ~SoWWWAnchor();

  virtual void redrawHighlighted(SoAction * act, SbBool isNowHighlighting);
};

#endif // !COIN_SOWWWANCHOR_H

#ifndef COIN_SOWWWANCHOR_H
#define COIN_SOWWWANCHOR_H

/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2001 by Systems in Motion.  All rights reserved.
 *  
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  version 2 as published by the Free Software Foundation.  See the
 *  file LICENSE.GPL at the root directory of this source distribution
 *  for more details.
 *
 *  If you desire to use Coin with software that is incompatible
 *  licensewise with the GPL, and / or you would like to take
 *  advantage of the additional benefits with regard to our support
 *  services, please contact Systems in Motion about acquiring a Coin
 *  Professional Edition License.  See <URL:http://www.coin3d.org> for
 *  more information.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  <URL:http://www.sim.no>, <mailto:support@sim.no>
 *
\**************************************************************************/

#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/nodes/SoLocateHighlight.h>
#include <Inventor/fields/SoSFString.h>
#include <Inventor/fields/SoSFEnum.h>

class SoWWWAnchor;
class SoWWWAnchorP;

typedef void SoWWWAnchorCB(const SbString & url, void * data,
                           SoWWWAnchor * node);

// *************************************************************************

class COIN_DLL_API SoWWWAnchor : public SoLocateHighlight {
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

private:
  SoWWWAnchorP * pimpl;
  static SoWWWAnchorCB * fetchfunc;
  static void * fetchdata;
  static SoWWWAnchorCB * highlightfunc;
  static void * highlightdata;
};

#endif // !COIN_SOWWWANCHOR_H

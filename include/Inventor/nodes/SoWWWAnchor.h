/**************************************************************************\
 * 
 *  Copyright (C) 1998-1999 by Systems in Motion.  All rights reserved.
 *
 *  This file is part of the Coin library.
 *
 *  This file may be distributed under the terms of the Q Public License
 *  as defined by Troll Tech AS of Norway and appearing in the file
 *  LICENSE.QPL included in the packaging of this file.
 *
 *  If you want to use Coin in applications not covered by licenses
 *  compatible with the QPL, you can contact SIM to aquire a
 *  Professional Edition license for Coin.
 *
 *  Systems in Motion AS, Prof. Brochs gate 6, N-7030 Trondheim, NORWAY
 *  http://www.sim.no/ sales@sim.no Voice: +47 73540378 Fax: +47 73943861
 *
\**************************************************************************/

#ifndef __SOWWWANCHOR_H__
#define __SOWWWANCHOR_H__

#include <Inventor/nodes/SoLocateHighlight.h>
#include <Inventor/fields/SoSFString.h>
#include <Inventor/fields/SoSFEnum.h>

#if defined(COIN_EXCLUDE_SOWWWANCHOR)
#error Configuration settings disrespected -- do not include this file!
#endif // COIN_EXCLUDE_SOWWWANCHOR

class SoWWWAnchor;

typedef void SoWWWAnchorCB(const SbString & url, void * data,
			   SoWWWAnchor * node);

// *************************************************************************

class SoWWWAnchor : public SoLocateHighlight {
  typedef SoLocateHighlight inherited;

//$ BEGIN TEMPLATE NodeHeader( SoWWWAnchor )
private:
  static SoType classTypeId;

public:
  static SoType getClassTypeId(void);
  virtual SoType getTypeId(void) const;
  static void * createInstance(void);
public:
  static void initClass(void);
  static void cleanClass(void);

  SoWWWAnchor(void);
protected:
  virtual ~SoWWWAnchor();
//$ END TEMPLATE NodeHeader

public:
  enum Mapping {
    NONE, POINT
  };

  SoSFString name;
  SoSFString description;
  SoSFEnum map;

  void setFullURLName(const SbString & url);
  const SbString & getFullURLName(void);

#if !defined(COIN_EXCLUDE_SOHANDLEEVENTACTION)
  virtual void handleEvent(SoHandleEventAction * action);
#endif // !COIN_EXCLUDE_SOHANDLEEVENTACTION

  static void setFetchURLCallBack(SoWWWAnchorCB * f, void * userData);
  static void setHighlightURLCallBack(SoWWWAnchorCB * f, void * userData);

protected:
  virtual void redrawHighlighted(SoAction * act, SbBool isNowHighlighting);
};

#endif // !__SOWWWANCHOR_H__

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
 *  http://www.sim.no/ sales@sim.no Voice: +47 22114160 Fax: +47 67172912
 *
\**************************************************************************/

#ifndef __SOINTERACTIONKIT_H__
#define __SOINTERACTIONKIT_H__

#include <Inventor/nodekits/SoSubKit.h>
#include <Inventor/nodekits/SoBaseKit.h>


class SoInteractionKit : public SoBaseKit
{
  typedef SoBaseKit inherited;

  SO_KIT_HEADER(SoInteractionKit);

  SO_KIT_CATALOG_ENTRY_HEADER(geomSeparator);
  SO_KIT_CATALOG_ENTRY_HEADER(topSeparator);

  // FIXME: lots of stuff missing from the API. 19991107 mortene.

public:
  SoInteractionKit(void);

  static void initClass(void);

protected:
  virtual ~SoInteractionKit();
};

#endif // !__SOINTERACTIONKIT_H__

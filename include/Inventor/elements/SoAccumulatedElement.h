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

#ifndef _SO_ACCUMULATED_ELEMENT_H_
#define _SO_ACCUMULATED_ELEMENT_H_

#include <Inventor/confdep.h>
#if defined(COIN_EXCLUDE_SOACCUMULATEDELEMENT)
#error "Configuration settings disable this class!"
#endif // COIN_EXCLUDE_SOACCUMULATEDELEMENT

#include <Inventor/elements/SoElement.h>
#include <Inventor/lists/SbPList.h>

class SoAccumulatedElement : public SoElement {
  typedef SoElement inherited;
//$ BEGIN TEMPLATE AbstractElementHeader( SoAccumulatedElement )
public:
  static  SoType classTypeId;
  static  SoType getClassTypeId(void);
public:
  virtual ~SoAccumulatedElement(void);
  static  int getClassStackIndex(void);

  static  void initClass(void);
  static  void cleanClass(void);

protected:
          SoAccumulatedElement(void);
  static  int classStackIndex;
//$ END TEMPLATE AbstractElementHeader

public:
  virtual SbBool matches(const SoElement * element) const;
  virtual void print(FILE * file) const;
    
protected:
  void clearNodeIds();
  void addNodeId(const SoNode * const node);
  void setNodeId(const SoNode * const node);
  virtual SoElement *copyMatchInfo() const;
  virtual void captureThis(SoState * state) const;
  
  SbPList nodeIds;

}; // class SoAccumulatedElement

#endif // ! _SO_ACCUMULATED_ELEMENT_H_

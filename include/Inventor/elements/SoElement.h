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

#ifndef __SOELEMENT_H__
#define __SOELEMENT_H__

#include <stdio.h>
#include <Inventor/SbBasic.h>
#include <Inventor/SoType.h>

#if defined(COIN_EXCLUDE_SOELEMENT)
#error "Configuration settings disable this class!"
#endif // COIN_EXCLUDE_SOELEMENT

// Remove all these dependencies in SoAction when compiling a minimal
// version of the Coin library. This is for debugging purposes only,
// see the Coin/build/base.pro file for further information.
// 19990629 mortene.

#if defined(__SOLIB_MAKEASBASE__)
// All elements disabled, only the abstract SoElement is "in".
#define COIN_EXCLUDE_SOACCUMULATEDELEMENT 1
#define COIN_EXCLUDE_SOCLIPPLANEELEMENT 1
#define COIN_EXCLUDE_SOGLCLIPPLANEELEMENT 1
#define COIN_EXCLUDE_SOLIGHTELEMENT 1
#define COIN_EXCLUDE_SOMODELMATRIXELEMENT 1
#define COIN_EXCLUDE_SOBBOXMODELMATRIXELEMENT 1
#define COIN_EXCLUDE_SOGLMODELMATRIXELEMENT 1
#define COIN_EXCLUDE_SOPROFILEELEMENT 1
#define COIN_EXCLUDE_SOTEXTUREMATRIXELEMENT 1
#define COIN_EXCLUDE_SOGLTEXTUREMATRIXELEMENT 1
#define COIN_EXCLUDE_SOCACHEELEMENT 1
#define COIN_EXCLUDE_SOINT32ELEMENT 1
#define COIN_EXCLUDE_SOANNOTEXT3CHARORIENTELEMENT 1
#define COIN_EXCLUDE_SOANNOTEXT3FONTSIZEHINTELEMENT 1
#define COIN_EXCLUDE_SOANNOTEXT3RENDERPRINTELEMENT 1
#define COIN_EXCLUDE_SOCOMPLEXITYTYPEELEMENT 1
#define COIN_EXCLUDE_SODECIMATIONTYPEELEMENT 1
#define COIN_EXCLUDE_SODRAWSTYLEELEMENT 1
#define COIN_EXCLUDE_SOGLDRAWSTYLEELEMENT 1
#define COIN_EXCLUDE_SOGLLIGHTIDELEMENT 1
#define COIN_EXCLUDE_SOGLTEXTUREENABLEDELEMENT 1
#define COIN_EXCLUDE_SOLINEPATTERNELEMENT 1
#define COIN_EXCLUDE_SOGLLINEPATTERNELEMENT 1
#define COIN_EXCLUDE_SOMATERIALBINDINGELEMENT 1
#define COIN_EXCLUDE_SONORMALBINDINGELEMENT 1
#define COIN_EXCLUDE_SOPICKSTYLEELEMENT 1
#define COIN_EXCLUDE_SOSWITCHELEMENT 1
#define COIN_EXCLUDE_SOTEXTOUTLINEENABLEDELEMENT 1
#define COIN_EXCLUDE_SOTEXTURECOORDINATEBINDINGELEMENT 1
#define COIN_EXCLUDE_SOUNITSELEMENT 1
#define COIN_EXCLUDE_SOFLOATELEMENT 1
#define COIN_EXCLUDE_SOCOMPLEXITYELEMENT 1
#define COIN_EXCLUDE_SOCREASEANGLEELEMENT 1
#define COIN_EXCLUDE_SODECIMATIONPERCENTAGEELEMENT 1
#define COIN_EXCLUDE_SOFOCALDISTANCEELEMENT 1
#define COIN_EXCLUDE_SOFONTSIZEELEMENT 1
#define COIN_EXCLUDE_SOLINEWIDTHELEMENT 1
#define COIN_EXCLUDE_SOGLLINEWIDTHELEMENT 1
#define COIN_EXCLUDE_SOPOINTSIZEELEMENT 1
#define COIN_EXCLUDE_SOGLPOINTSIZEELEMENT 1
#define COIN_EXCLUDE_SOTEXTUREQUALITYELEMENT 1
#define COIN_EXCLUDE_SOTEXTUREOVERRIDEELEMENT 1
#define COIN_EXCLUDE_SOGLCACHECONTEXTELEMENT 1
#define COIN_EXCLUDE_SOGLRENDERPASSELEMENT 1
#define COIN_EXCLUDE_SOGLUPDATEAREAELEMENT 1
#define COIN_EXCLUDE_SOLAZYELEMENT 1
#define COIN_EXCLUDE_SOGLLAZYELEMENT 1
#define COIN_EXCLUDE_SOLOCALBBOXMATRIXELEMENT 1
#define COIN_EXCLUDE_SOPICKRAYELEMENT 1
#define COIN_EXCLUDE_SOREPLACEDELEMENT 1
#define COIN_EXCLUDE_SOCOORDINATEELEMENT 1
#define COIN_EXCLUDE_SOGLCOORDINATEELEMENT 1
#define COIN_EXCLUDE_SOENVIRONMENTELEMENT 1
#define COIN_EXCLUDE_SOFONTNAMEELEMENT 1
#define COIN_EXCLUDE_SOLIGHTATTENUATIONELEMENT 1
#define COIN_EXCLUDE_SONORMALELEMENT 1
#define COIN_EXCLUDE_SOGLNORMALELEMENT 1
#define COIN_EXCLUDE_SOPOLYGONOFFSETELEMENT 1
#define COIN_EXCLUDE_SOGLPOLYGONOFFSETELEMENT 1
#define COIN_EXCLUDE_SOPROJECTIONMATRIXELEMENT 1
#define COIN_EXCLUDE_SOGLPROJECTIONMATRIXELEMENT 1
#define COIN_EXCLUDE_SOPROFILECOORDINATEELEMENT 1
#define COIN_EXCLUDE_SOTEXTURECOORDINATEELEMENT 1
#define COIN_EXCLUDE_SOGLTEXTURECOORDINATEELEMENT 1
#define COIN_EXCLUDE_SOTEXTUREIMAGEELEMENT 1
#define COIN_EXCLUDE_SOGLTEXTUREIMAGEELEMENT 1
#define COIN_EXCLUDE_SOVIEWINGMATRIXELEMENT 1
#define COIN_EXCLUDE_SOGLVIEWINGMATRIXELEMENT 1
#define COIN_EXCLUDE_SOVIEWVOLUMEELEMENT 1
#define COIN_EXCLUDE_SOSHAPEHINTSELEMENT 1
#define COIN_EXCLUDE_SOGLSHAPEHINTSELEMENT 1
#define COIN_EXCLUDE_SOSHAPESTYLEELEMENT 1
#define COIN_EXCLUDE_SOVIEWPORTREGIONELEMENT 1
#define COIN_EXCLUDE_SOGLVIEWPORTREGIONELEMENT 1
#define COIN_EXCLUDE_SOWINDOWELEMENT 1
#define COIN_EXCLUDE_SOAMBIENTCOLORELEMENT 1
#define COIN_EXCLUDE_SOEMISSIVECOLORELEMENT 1
#define COIN_EXCLUDE_SOLIGHTMODELELEMENT 1
#define COIN_EXCLUDE_SOGLLIGHTMODELELEMENT 1
#define COIN_EXCLUDE_SOSHININESSELEMENT 1
#define COIN_EXCLUDE_SOSPECULARCOLORELEMENT 1
#define COIN_EXCLUDE_SOTRANSPARENCYELEMENT 1
#define COIN_EXCLUDE_SODIFFUSECOLORELEMENT 1
#define COIN_EXCLUDE_SOGLAMBIENTCOLORELEMENT 1
#define COIN_EXCLUDE_SOGLPOLYGONSTIPPLEELEMENT 1
#define COIN_EXCLUDE_SOGLCOLORINDEXELEMENT 1
#define COIN_EXCLUDE_SOGLDIFFUSECOLORELEMENT 1
#define COIN_EXCLUDE_SOGLDISPLAYLIST 1
#define COIN_EXCLUDE_SOGLSHININESSELEMENT 1
#define COIN_EXCLUDE_SOGLSPECULARCOLORELEMENT 1
#define COIN_EXCLUDE_SOGLEMISSIVECOLORELEMENT 1
#define COIN_EXCLUDE_SOGLSHADEMODELELEMENT 1
#define COIN_EXCLUDE_SOGLENVIRONMENTELEMENT 1
#define COIN_EXCLUDE_SOGLNORMALIZEELEMENT 1
#endif // !__SOLIB_MAKEASBASE__


class SoNode;
class SoState;

class SoElement {
//$ BEGIN TEMPLATE AbstractElementHeader( SoElement )
public:
  static  SoType classTypeId;
  static  SoType getClassTypeId(void);
public:
  virtual ~SoElement(void);
  static  int getClassStackIndex(void);

  static  void initClass(void);
  static  void cleanClass(void);

protected:
          SoElement(void);
  static  int classStackIndex;
//$ END TEMPLATE AbstractElementHeader

public:
  const SoType getTypeId(void) const;
  int getStackIndex(void) const;

  virtual void init(SoState * state);

  virtual void push(SoState * state);
  virtual void pop(SoState * state,
		    const SoElement * prevTopElement);
  
  virtual SbBool matches(const SoElement * element) const = 0;
  virtual SoElement * copyMatchInfo(void) const = 0;
  
  static void initElements(void); // only for base class (SoElement)
  static void cleanElements(void); // only for base class (SoElement)

  static int getNumStackIndices(void);
  static SoType getIdFromStackIndex(const int stackIndex);

  void setDepth(const int depth);

  int getDepth(void) const;

  virtual void print(FILE * file = stdout) const;

protected:
  static SoElement * getElement(SoState * const state,
				const int stackIndex);
  static const SoElement * getConstElement(SoState * const state,
					   const int stackIndex);

  void capture(SoState * const state) const;
  virtual void captureThis(SoState * state) const;

  void setTypeId(const SoType typeId);
  void setStackIndex(const int index);

  SoType typeId;
  int stackIndex;

  static int nextStackIndex;
  static int createStackIndex(const SoType id);
  static SoTypeList * stackToType;

  int depth;

  SoElement * next; 
  SoElement * prev;

  friend class SoState; // FIXME: bad design. 19990629 mortene.
};

#endif // !__SOELEMENT_H__

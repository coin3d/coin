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

#ifndef _SO_SEARCH_ACTION_H_
#define _SO_SEARCH_ACTION_H_

#include <Inventor/confdep.h>
#if defined(COIN_EXCLUDE_SOSEARCHACTION)
#error "This file is excluded from Coin through the configuration system!"
#endif // COIN_EXCLUDE_SOSEARCHACTION

#include <Inventor/actions/SoAction.h>
#include <Inventor/SbName.h>
#include <Inventor/lists/SoPathList.h>

class SoSearchAction : public SoAction {
  typedef SoAction inherited;

//$ BEGIN TEMPLATE ActionHeader( SoSearchAction )
private:
  static SoType classTypeId;

public:
  virtual SoType getTypeId(void) const;
  static SoType getClassTypeId(void);

protected:
  virtual const SoEnabledElementsList & getEnabledElements(void) const;
  static SoEnabledElementsList * enabledElements;
  static SoActionMethodList * methods;

public:
  static void addMethod(const SoType type, SoActionMethod method);
  static void enableElement(const SoType type, const int stackIndex);

  static void initClass(void);
  static void cleanClass(void);

  virtual ~SoSearchAction();
//$ END TEMPLATE ActionHeader

public:
  SoSearchAction(void);

  enum LookFor { NODE = 1, TYPE = 2, NAME = 4 };
  enum Interest { FIRST, LAST, ALL };
  
  void setNode( SoNode * const node );
  SoNode * getNode( void ) const;
  void setType( const SoType type,
                const SbBool includeDerived = TRUE );
  SoType getType( SbBool & includeDerived ) const;
  void setName( const SbName name );
  SbName getName( void ) const;
  void setFind( const int what );
  int getFind( void ) const;
  void setInterest( const Interest interest );
  Interest getInterest( void ) const;
  void setSearchingAll( const SbBool searchAll );
  SbBool isSearchingAll( void ) const;
  SoPath * getPath( void ) const;
  SoPathList & getPaths( void );
  void reset( void );

  // extender
  void setFound( void );
  SbBool isFound( void ) const;
  void addPath( SoPath * const path );

protected:
  virtual void beginTraversal( SoNode * node );

private:
  int lookFor; // mask of LookFor flags
  Interest interest;

  SbBool searchAll;
  SbBool includeDerived;

  SoNode * node;
  SoType type;
  SbName name;

  SoPath * path;
  SoPathList paths;

}; // class SoSearchAction

#endif // ! _SO_SEARCH_ACTION_H_

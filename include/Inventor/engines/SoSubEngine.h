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

#ifndef __SOSUBENGINE_H__
#define __SOSUBENGINE_H__

#include <Inventor/engines/SoEngine.h>
#include <Inventor/misc/SoBasic.h>
#include <Inventor/SbName.h>
#include <Inventor/SoType.h>
#include <Inventor/errors/SoDebugError.h>

// FIXME: document macro for Doxygen. 19990925 mortene.

#define SO_ENGINE_ABSTRACT_HEADER(__classname__) \
  private: \
    static SoType classTypeId; \
  public: \
    static SoType getClassTypeId(void) \
      {return __classname__::classTypeId;} \
    virtual SoType getTypeId(void) const \
      {return __classname__::getClassTypeId();}


// FIXME: document macro for Doxygen. 19990925 mortene.

#define SO_ENGINE_HEADER(__classname__) \
    SO_ENGINE_ABSTRACT_HEADER(__classname__) \
  public: \
    static void * createInstance(void)


// FIXME: document macro for Doxygen. 19990925 mortene.

#define SO_COMPOSE__HEADER(__classname__) \
    typedef SoEngine inherited; \
    SO_ENGINE_HEADER(__classname__); \
  public: \
    __classname__(void); \
    static void initClass(void); \
  protected: \
    ~__classname__(); \
  private: \
    virtual void evaluate(void)


// FIXME: document macro for Doxygen. 19990925 mortene.

#define SO_INTERPOLATE_HEADER(__classname__) \
    typedef SoInterpolate inherited; \
    SO_ENGINE_HEADER(__classname__); \
  public: \
    __classname__(void); \
    static void initClass(void); \
  protected: \
    ~__classname__(); \
  private: \
    virtual void evaluate(void)


// FIXME: document macro for Doxygen. 19990925 mortene.

#define SO_ENGINE_ABSTRACT_SOURCE(__classname__) \
  SoType __classname__::classTypeId;


// FIXME: document macro for Doxygen. 19990925 mortene.

#define SO_ENGINE_SOURCE(__classname__) \
  SO_ENGINE_ABSTRACT_SOURCE(__classname__) \
  void *__classname__::createInstance() \
  { \
    return new __classname__; \
  } 


// FIXME: document macro for Doxygen. 19990925 mortene.

#define SO_ENGINE_CONSTRUCTOR(__classname__) \
  this->outputList = new SoEngineOutputList;


// FIXME: document macro for Doxygen. 19990925 mortene.

#define SO_ENGINE_ADD_INPUT(__membername__,__defaultval__) \
  this->__membername__.setValue __defaultval__; \
  this->__membername__.setContainer(this);


// FIXME: document macro for Doxygen. 19990925 mortene.

#define SO_ENGINE_ADD_OUTPUT(__membername__,__outtype__) \
  this->__membername__.setType(__outtype__::getClassTypeId()); \
  this->__membername__.setContainer(this); \
  this->outputList->append(&this->__membername__);


// FIXME: document macro for Doxygen. 19990925 mortene.

#define SO_ENGINE_INIT_ABSTRACT_CLASS(__classname__,__parent__,__parentname__) \
  classTypeId = SoType::createType(__parent__::getClassTypeId(), \
                                   SO__QUOTE(__classname__));


// FIXME: document macro for Doxygen. 19990925 mortene.

#define SO_ENGINE_INIT_CLASS(__classname__,__parent__,__parentname__) \
  classTypeId = SoType::createType(__parent__::getClassTypeId(), \
                                   SO__QUOTE(__classname__), \
                                   &__classname__::createInstance)


// FIXME: document macro for Doxygen. 19990925 mortene.

#define SO_ENGINE_OUTPUT(__outmember__,__outtype__,__outval__) \
  { \
    if (__outmember__.isEnabled()) \
      for (int _i=0;_i<__outmember__.getNumConnections();_i++) \
        ((__outtype__ *)__outmember__[_i])->__outval__; \
  }

//FIXME: check read-only

#endif // !__SOSUBENGINE_H__

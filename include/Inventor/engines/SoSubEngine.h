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

#define SO_ENGINE_ABSTRACT_HEADER(_classname_) \
  private: \
    static SoType classTypeId; \
  public: \
    static SoType getClassTypeId(void) \
      {return _classname_::classTypeId;} \
    virtual SoType getTypeId(void) const \
      {return _classname_::getClassTypeId();}


// FIXME: document macro for Doxygen. 19990925 mortene.

#define SO_ENGINE_HEADER(_classname_) \
    SO_ENGINE_ABSTRACT_HEADER(_classname_) \
  public: \
    static void * createInstance(void)


// FIXME: document macro for Doxygen. 19990925 mortene.

#define SO_COMPOSE__HEADER(_classname_) \
    typedef SoEngine inherited; \
    SO_ENGINE_HEADER(_classname_); \
  public: \
    _classname_(void); \
    static void initClass(void); \
  protected: \
    ~_classname_(); \
  private: \
    virtual void evaluate(void)


// FIXME: document macro for Doxygen. 19990925 mortene.

#define SO_INTERPOLATE_HEADER(_classname_) \
    typedef SoInterpolate inherited; \
    SO_ENGINE_HEADER(_classname_); \
  public: \
    _classname_(void); \
    static void initClass(void); \
  protected: \
    ~_classname_(); \
  private: \
    virtual void evaluate(void)


// FIXME: document macro for Doxygen. 19990925 mortene.

#define SO_ENGINE_ABSTRACT_SOURCE(_classname_) \
  SoType _classname_::classTypeId;


// FIXME: document macro for Doxygen. 19990925 mortene.

#define SO_ENGINE_SOURCE(_classname_) \
  SO_ENGINE_ABSTRACT_SOURCE(_classname_) \
  void *_classname_::createInstance() \
  { \
    return new _classname_; \
  } 


// FIXME: document macro for Doxygen. 19990925 mortene.

#define SO_ENGINE_CONSTRUCTOR(_classname_) \
  this->outputList = new SoEngineOutputList;


// FIXME: document macro for Doxygen. 19990925 mortene.

#define SO_ENGINE_ADD_INPUT(_membername_,_defaultval_) \
  this->_membername_.setValue _defaultval_; \
  this->_membername_.setContainer(this);


// FIXME: document macro for Doxygen. 19990925 mortene.

#define SO_ENGINE_ADD_OUTPUT(_membername_,_outtype_) \
  this->_membername_.setType(_outtype_::getClassTypeId()); \
  this->_membername_.setContainer(this); \
  this->outputList->append(&this->_membername_);


// FIXME: document macro for Doxygen. 19990925 mortene.

#define SO_ENGINE_INIT_ABSTRACT_CLASS(_class_,_parent_,_parentname_) \
  do { \
    /* Make sure we only initialize once. */ \
    assert(_class_::classTypeId == SoType::badType()); \
    /* Make sure superclass gets initialized before subclass. */ \
    assert(_parent_::getClassTypeId() != SoType::badType()); \
 \
    _class_::classTypeId = \
                SoType::createType(_parent_::getClassTypeId(), \
                                   SO__QUOTE(_class_)); \
  } while (0)

#if defined(__SOLIB_INTERNAL__)
#define SO_ENGINE_INTERNAL_INIT_ABSTRACT_CLASS(_class_) \
  do { \
    /* Make sure we only initialize once. */ \
    assert(_class_::classTypeId == SoType::badType()); \
    /* Make sure superclass gets initialized before subclass. */ \
    assert(inherited::getClassTypeId() != SoType::badType()); \
 \
    const char * classname = SO__QUOTE(_class_); \
    _class_::classTypeId = \
                SoType::createType(inherited::getClassTypeId(), \
                                   &classname[2]); \
  } while (0)
#endif // INTERNAL makro


// FIXME: document macro for Doxygen. 19990925 mortene.

#define SO_ENGINE_INIT_CLASS(_class_,_parent_,_parentname_) \
  do { \
    /* Make sure we only initialize once. */ \
    assert(_class_::classTypeId == SoType::badType()); \
    /* Make sure superclass gets initialized before subclass. */ \
    assert(_parent_::getClassTypeId() != SoType::badType()); \
 \
    _class_::classTypeId = \
                SoType::createType(_parent_::getClassTypeId(), \
                                   SO__QUOTE(_class_), \
                                   &_class_::createInstance); \
  } while (0)

#if defined(__SOLIB_INTERNAL__)
#define SO_ENGINE_INTERNAL_INIT_CLASS(_class_) \
  do { \
    /* Make sure we only initialize once. */ \
    assert(_class_::classTypeId == SoType::badType()); \
    /* Make sure superclass gets initialized before subclass. */ \
    assert(inherited::getClassTypeId() != SoType::badType()); \
 \
    const char * classname = SO__QUOTE(_class_); \
    _class_::classTypeId = \
                SoType::createType(inherited::getClassTypeId(), \
                                   &classname[2], \
                                   &_class_::createInstance); \
  } while (0)
#endif // INTERNAL makro

// FIXME: document macro for Doxygen. 19990925 mortene.

#define SO_ENGINE_OUTPUT(_outmember_,_outtype_,_outval_) \
  { \
    if (_outmember_.isEnabled()) \
      for (int _i=0;_i<_outmember_.getNumConnections();_i++) \
        ((_outtype_ *)_outmember_[_i])->_outval_; \
  }

//FIXME: check read-only

#endif // !__SOSUBENGINE_H__

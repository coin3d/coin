/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2003 by Systems in Motion.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Systems in Motion about acquiring
 *  a Coin Professional Edition License.
 *
 *  See <URL:http://www.coin3d.org> for  more information.
 *
 *  Systems in Motion, Teknobyen, Abels Gate 5, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no>.
 *
\**************************************************************************/

/*!
  \class SoType Inventor/SoType.h
  \brief The SoType class is the basis for the run-time type system in Coin.
  \ingroup general

  Many of the classes in the Coin library must have their type
  information registered before any instances are created (including,
  but not limited to: engines, nodes, fields, actions, nodekits and
  manipulators). The use of SoType to store this information provides
  lots of various functionality for working with class hierarchies,
  comparing class types, instantiating objects from classnames, etc
  etc.

  It is for instance possible to do things like this:

  \code
  void cleanLens(SoNode * anode)
  {
    assert(anode->getTypeId().isDerivedFrom(SoCamera::getClassTypeId()));

    if (anode->getTypeId() == SoPerspectiveCamera::getClassTypeId()) {
      // do something..
    }
    else if (anode->getTypeId() == SoOrthographicCamera::getClassTypeId()) {
      // do something..
    }
    else {
      SoDebugError::postWarning("cleanLens", "Unknown camera type %s!\n",
                                anode->getTypeId().getName());
    }
  }
  \endcode

  A notable feature of the SoType class is that it is only 16 bits
  long and therefore should be passed around by value for efficiency
  reasons.

  One important note about the use of SoType to register class
  information: super classes must be registered before any of their
  derived classes are.
*/


#include <Inventor/errors/SoDebugError.h>

#include <Inventor/lists/SoTypeList.h>
// MSVC++ needs 'SbName.h' and 'SbString.h' to compile
#include <Inventor/SbName.h>
#include <Inventor/SbString.h>
#include <Inventor/SoDB.h>
#include <Inventor/lists/SbList.h>

#include <Inventor/C/tidbits.h>
#include <Inventor/C/tidbitsp.h>
#include <Inventor/C/glue/dl.h>
#include <assert.h>
#include <stdlib.h> // NULL
#include <string.h> // strcmp()

#include "cppmangle.icc"

struct SoTypeData {
  SoTypeData(const SbName theName,
             const SoType type = SoType::badType(),
             const SbBool ispublic = FALSE,
             const uint16_t theData = 0,
             const SoType theParent = SoType::badType(),
             const SoType::instantiationMethod createMethod = NULL)
    : name(theName), type(type), isPublic(ispublic), data(theData),
      parent(theParent), method(createMethod) { };

  SbName name;
  SoType type;
  SbBool isPublic;
  uint16_t data;
  SoType parent;
  SoType::instantiationMethod method;
};

// OBSOLETED: this code was only active for GCC 2.7.x, and I don't
// think we support that old compiler version anyhow. Do look into if
// this is what the old SGI MIPSpro CC compiler for IRIX6.2 needs to
// stop spitting out all those linker warnings, though. 20000208 mortene.
#if 0 // obsoleted
// #if defined(NEED_TEMPLATE_DEFINITION)
template class SbList<SoTypeData *>;
// [...]
#endif // obsoleted


SbList<SoTypeData *> * SoType::typedatalist = NULL;
SbDict * SoType::typedict = NULL;
SbDict * SoType::moduledict = NULL;

/*!
  \typedef SoType::instantiationMethod

  This is a convenience typedef for the function signature of a typed
  class' instantiation method. It is an extension on the original
  Inventor API.  Mostly only useful for internal purposes.

  An instantation method will take no arguments and returns a
  void-pointer to a newly allocated and initialized object of the
  class type.
*/

/*!
  This static method initializes the type system.

*/

void
SoType::init(void)
{
#if COIN_DEBUG
  // Debugging for memory leaks will be easier if we can clean up the
  // resource usage.
  coin_atexit((coin_atexit_f *)SoType::clean, 0);
#endif // COIN_DEBUG

  // If any of these assert fails, it is probably because
  // SoType::init() has been called for a second time. --mortene
  assert(SoType::typedatalist == NULL);

  SoType::typedatalist = new SbList<SoTypeData *>;
  SoType::typedict = new SbDict;

  SoType::typedatalist->append(new SoTypeData(SbName("BadType")));
  SoType::typedict->enter((unsigned long)SbName("BadType").getString(), 0);
}

// Clean up internal resource usage.
void
SoType::clean(void)
{
#if COIN_DEBUG
  // clean SoType::typedatalist (first delete structures)
  const int num = SoType::typedatalist->getLength();
  for (int i = 0; i < num; i++) delete (*SoType::typedatalist)[i];
  delete SoType::typedatalist;

  delete SoType::typedict;
#endif // COIN_DEBUG
}

/*!
  This method creates and registers a new class type.

  Classes that do not inherit any other class should use
  SoType::badType() for the first argument. Abstract classes should
  use \c NULL for the \a method argument.

  The value passed in for the \a data parameter can be retrieved with
  SoType::getData().
*/

const SoType
SoType::createType(const SoType parent, const SbName name,
                   const instantiationMethod method,
                   const uint16_t data)
{
#if COIN_DEBUG
  // We don't use SoType::fromName() to test if a type with this name
  // already exists to avoid loading extension nodes in this context.
  // You should be able to "override" dynamically loadable nodes in program
  // code.
  void * discard;
  if (SoType::typedict->find((unsigned long)name.getString(), discard)) {
    SoDebugError::post("SoType::createType",
                       "a type with name ``%s'' already created",
                       name.getString());
    return SoType::fromName(name.getString());
  }
#endif // COIN_DEBUG

#if COIN_DEBUG && 0 // debug
  SoDebugError::postInfo("SoType::createType", "%s", name.getString());
#endif // debug

  SoType newType;
  newType.index = SoType::typedatalist->getLength();
  SoTypeData * typeData = new SoTypeData(name, newType, TRUE, data, parent, method);
  SoType::typedatalist->append(typeData);

  // add to dictionary for fast lookup

  // Cast needed to silence gcc3, which don't seem to like direct
  // casting from int16_t to void*.
  uint32_t mapval = (uint32_t)newType.getKey();

  SoType::typedict->enter((unsigned long)name.getString(), (void *) ((uintptr_t) mapval));
  return newType;
}

/*!
  This method makes a new class's instantiation method override
  the instantiation method of an existing class.

  The new type should be a C++ subclass of the original class type, but
  this won't be checked though.

  If \c NULL is passed as the second argument, the type will be
  considered uninstantiable -- it does not revert the configuration to
  the default setting as one might think.

  Here's a \e complete code examples which shows how to fully override
  a built-in Coin node class, so that a) your application-specific
  extension class gets instantiated instead of the built-in class upon
  scenegraph import, and b) it gets written out properly upon export:

  \code
  #include <Inventor/SoDB.h>
  #include <Inventor/actions/SoWriteAction.h>
  #include <Inventor/errors/SoDebugError.h>
  #include <Inventor/nodes/SoSeparator.h>
  #include <Inventor/nodes/SoWWWInline.h>

  ////// MyWWWInline ////////////////////////////////////////////////////

  class MyWWWInline : public SoWWWInline {
    SO_NODE_HEADER(MyWWWInline);

  public:
    static void initClass(void);
    MyWWWInline(void);

  protected:
    virtual ~MyWWWInline();
    virtual SbBool readInstance(SoInput * in, unsigned short flags);
    virtual const char * getFileFormatName(void) const;
  };

  SO_NODE_SOURCE(MyWWWInline);

  MyWWWInline::MyWWWInline(void)
  {
    SO_NODE_CONSTRUCTOR(MyWWWInline);

    // Fool the library to believe this is an internal class, so it gets
    // written out in the same manner as the built-in classes, instead
    // of as en extension class. There are slight differences, which you
    // want to avoid when overriding a class like we do with MyWWWInline
    // vs SoWWWInline here.
    this->isBuiltIn = TRUE;
  }

  MyWWWInline::~MyWWWInline()
  {
  }

  void
  MyWWWInline::initClass(void)
  {
    SO_NODE_INIT_CLASS(MyWWWInline, SoWWWInline, "SoWWWInline");

    // Override instantiation method, so we get MyWWWInline instead of
    // SoWWWInline instances upon scenegraph import.
    (void)SoType::overrideType(SoWWWInline::getClassTypeId(),
                               MyWWWInline::createInstance);
  }

  // Override SoBase::getFileFormatName() to make node get written as
  // "WWWInline" instead of "MyWWWInline".
  const char *
  MyWWWInline::getFileFormatName(void) const
  {
    return "WWWInline";
  }

  SbBool
  MyWWWInline::readInstance(SoInput * in, unsigned short flags)
  {
    SoDebugError::postInfo("MyWWWInline::readInstance", "hepp");
    return SoWWWInline::readInstance(in, flags);
  }

  ////// main() /////////////////////////////////////////////////////////

  int
  main(int argc, char ** argv)
  {
    SoDB::init();
    MyWWWInline::initClass();

    const char * ivscene =
      "#Inventor V2.1 ascii\n\n"
      "Separator {"
      "  WWWInline { }"
      "}";

    SoInput in;
    in.setBuffer((void *)ivscene, strlen(ivscene));
    SoSeparator * root = SoDB::readAll(&in);
    root->ref();

    SoOutput out;
    SoWriteAction wa(&out);
    wa.apply(root);
    root->unref();

    return 0;
  }
  \endcode
*/
const SoType
SoType::overrideType(const SoType originalType,
                     const instantiationMethod method)
{
  (*SoType::typedatalist)[(int)originalType.getKey()]->method = method;
  return originalType;
}

// FIXME: when doing multi-threaded programming with Coin, one might want to
// override a type only for the local thread.  A new method in the API seems
// to be needed (overrideThreadLocalType?).  This change will of course have
// far-reaching consequences for the whole SoType class implementation.
// 2002-01-24 larsa

// FIXME: investigate how So-prefix stripping works for non-builtin
// extension nodes.  It probably works as for builtins, given that the
// class in question isn't named with an "So" prefix.  20030223 larsa

#ifdef _MSC_VER
typedef void __cdecl initClassFunction(void);
#else
typedef void initClassFunction(void);
#endif

/*!
  This static function returns the SoType object associated with name \a name.

  Type objects for builtin types can be retreived by name both with and
  without the "So" prefix.  For dynamically loadable extension nodes, the
  name given to this function must match exactly.

  If no node type with the given name has been initialized, a dynamically
  loadable extension node with the given name is searched for.  If one is
  found, it is loaded and initialized, and the SoType object for the
  newly initialized class type returned.  If no module is found, or the
  initialization of the module fails, SoType::badType() is returned.

  Support for dynamically loadable extension nodes varies from
  platform to platform, and from compiler suite to compiler suite.

  So far code built with the following compilers are supported: GNU
  GCC v2, GNU GCC v3, Microsoft Visual C++ v6 (and probably v7, not
  tested), SGI MIPSPro v7.

  Extensions built with compilers that are known to be binary
  compatible with the above compilers are also supported, such as
  e.g. the Intel x86 compiler compatible with MSVC++.

  To support dynamic loading for other compilers, we need to know how
  the compiler mangles the "static void classname::initClass(void)"
  symbol.  If your compiler is not supported, tell us at \c
  coin-support@coin3d.org which it is and send us the output of a
  symbol-dump on the shared object.  Typically you can do

  \code
  $ nm <Node>.so | grep initClass
  \endcode

  to find the relevant mangled symbol.
*/
SoType
SoType::fromName(const SbName name)
{
  assert((SoType::typedict != NULL) && "SoType static class data not yet initialized");

  // It should be possible to specify a type name with the "So" prefix
  // and get the correct type id, even though the types in some type
  // hierarchies are named internally without the prefix.
  SbString tmp(name.getString());
  if ( tmp.compareSubString("So") == 0 ) tmp = tmp.getSubString(2);
  SbName noprefixname(tmp);

  void * temp = NULL;
  // FIXME: we need to split SbDict into SbIntDict and SbPtrDict. 20030223 larsa
  if (!SoType::typedict->find((unsigned long)name.getString(), temp) &&
      !SoType::typedict->find((unsigned long)noprefixname.getString(), temp)) {
    if ( !SoDB::isInitialized() ) {
      return SoType::badType();
    }

    // find out which C++ name mangling scheme the compiler uses
    static mangleFunc * manglefunc = getManglingFunction();
    if ( manglefunc == NULL ) {
      // dynamic loading is not yet supported for this compiler suite
      static long first = 1;
      if ( first ) {
        const char * env = coin_getenv("COIN_DEBUG_DL");
        if (env && (atoi(env) > 0)) {
          SoDebugError::post("SoType::fromName",
                             "unable to figure out the C++ name mangling scheme");
        }
        first = 0;
      }
      return SoType::badType();
    }
    SbString mangled = manglefunc(name.getString());

    if ( SoType::moduledict == NULL ) {
      SoType::moduledict = new SbDict;
      // FIXME: this is a one-off memory leak - add cleanup
      // code to coin_atexit()
    }

    // FIXME: should we search the application code for the initClass()
    // symbol first?  dlopen(NULL) might not be portable enough, but it
    // could be a cool feature.  20030223 larsa

    // FIXME: We probably should use loadable modules (type MH_BUNDLE)
    // instead of shared libraries for dynamic extension nodes, on Mac
    // OS X, since (1) this is the Recommended Way for dynamically
    // loadable code and (2) it allows us to unload them when they are
    // no longer needed. Note that this would require major changes to
    // the Mac cc_dl_open() and cc_dl_sym() code. 20030318 kyrah

    SbString modulename;
    static char * modulenamepatterns[] = {
      "%s.so", "lib%s.so", "%s.dll", "lib%s.dll", "%s.dylib", "lib%s.dylib",
      NULL
    };

    cc_libhandle handle = NULL;
    int i;
    for ( i = 0; (modulenamepatterns[i] != NULL) && (handle == NULL); i++ ) {
      modulename.sprintf(modulenamepatterns[i], name.getString());
      SbName module(modulename.getString());

      void * idx = NULL;
      if ( SoType::moduledict->find((unsigned long) module.getString(), idx) ) {
        // Module has been loaded, but type is not yet finished initializing.
        // SoType::badType() is here the expected return value.  See below.
        return SoType::badType();
      }

      // FIXME: should we maybe use a Coin-specific search path variable
      // instead of the LD_LIBRARY_PATH one?  20020216 larsa

      handle = cc_dl_open(module.getString());
      if ( handle != NULL ) {
        // We register the module so we don't recurse infinitely in the
        // initClass() function which calls SoType::fromName() on itself
        // which expects SoType::badType() in return.  See above.
        SoType::moduledict->enter((unsigned long)module.getString(), handle);
      }
    }

    if ( handle == NULL ) return SoType::badType();

    // find and invoke the initClass() function.
    // FIXME: declspec stuff
    initClassFunction * initClass = (initClassFunction *) cc_dl_sym(handle, mangled.getString());
    if ( initClass == NULL ) {
      // FIXME: if a module is found and opened and initialization
      // fails, the remaining module name patterns are not tried.
      // might trigger as a problem one day...  2030224 larsa
#if COIN_DEBUG
      SoDebugError::postWarning("SoType::fromName",
                                "Mangled symbol %s not found in module %s. "
                                "It might be compiled with the wrong compiler / "
                                "compiler-settings or something similar.",
                                mangled.getString(), modulename.getString());
#endif
      cc_dl_close(handle);
      return SoType::badType();
    }

    initClass();

    // We run these tests to get the index into the temp pointer - that's
    // what typedict stores as userdata in SoType::createType().
    if (!SoType::typedict->find((unsigned long)name.getString(), temp) &&
        !SoType::typedict->find((unsigned long)noprefixname.getString(), temp)) {
      assert(0 && "how did this happen?");
    }
  }
  const intptr_t index = (intptr_t) temp;
  assert(index >= 0 && index < SoType::typedatalist->getLength());
  assert(((*SoType::typedatalist)[index]->name == name) ||
         ((*SoType::typedatalist)[index]->name == noprefixname));
  return (*SoType::typedatalist)[index]->type;
}

/*!
  Find and return a type from the given key ID.
*/

SoType
SoType::fromKey(uint16_t key)
{
  assert(SoType::typedatalist);
  assert(key < SoType::typedatalist->getLength());

  return (*SoType::typedatalist)[(int)key]->type;
}

/*!
  This method returns the name of the SoBase-derived class type the SoType
  object is configured for.
*/

SbName
SoType::getName(void) const
{
  return (*SoType::typedatalist)[(int)this->getKey()]->name;
}

/*!
  \fn uint16_t SoType::getData(void) const

  This method returns a type specific data variable.

*/

uint16_t
SoType::getData(void) const
{
  return (*SoType::typedatalist)[(int)this->getKey()]->data;
}

/*!
  This method returns the SoType type for the parent class of the
  SoBase-derived class the SoType object is configured for.
*/

const SoType
SoType::getParent(void) const
{
  return (*SoType::typedatalist)[(int)this->getKey()]->parent;
}

/*!
  This method returns an illegal type, useful for returning errors.

  \sa SbBool SoType::isBad() const
*/

SoType
SoType::badType(void)
{
  SoType bad;
  // Important note: internally in Coin (in the various initClass()
  // methods for nodes, engines, fields, etc etc), we depend on the
  // bitpattern for SoType::badType() to equal 0x0000.
  bad.index = 0;
  return bad;
}

/*!
  \fn SbBool SoType::isBad(void) const

  This method returns TRUE if the SoType object represents an illegal class
  type.
*/

/*!
  This method returns \c TRUE if the given type is derived from (or \e
  is) the \a parent type, and \c FALSE otherwise.
*/

SbBool
SoType::isDerivedFrom(const SoType parent) const
{
  assert(!this->isBad());

  if (parent.isBad()) {
#if COIN_DEBUG
    SoDebugError::postWarning("SoType::isDerivedFrom",
                              "can't compare type '%s' against an invalid type",
                              this->getName().getString());
#endif // COIN_DEBUG
    return FALSE;
  }

  SoType type = *this;
  do {
#if COIN_DEBUG && 0 // debug
    SoDebugError::postInfo("SoType::isDerivedFrom",
                           "this: '%s' parent: '%s'",
                           type.getName().getString(),
                           parent.getName().getString());
#endif // debug
    if (type == parent) return TRUE;
    type = (*SoType::typedatalist)[(int)type.getKey()]->parent;
  } while (!type.isBad());

  return FALSE;
}

/*!
  This method appends all the class types derived from \a type to \a list,
  and returns the number of types added to the list.  Internal types are not
  included in the list, nor are they counted.

  \a type itself is also added to the list, as a type is seen as a derivation
  of its own type.

  NB: do not write code which depends in any way on the order of the
  elements returned in \a list.
*/
int
SoType::getAllDerivedFrom(const SoType type, SoTypeList & list)
{
  assert(type != SoType::badType() && "argument is badType()");

  int counter = 0;
  int n = SoType::typedatalist->getLength();
  for (int i = 0; i < n; i++) {
    SoType chktype = (*SoType::typedatalist)[i]->type;
    if (!chktype.isInternal() && chktype.isDerivedFrom(type)) {
      list.append(chktype);
      counter++;
    }
  }
  return counter;
}

/*!
  This method returns \c FALSE for abstract base classes, and \c TRUE
  for class types that can be instantiated.
*/

SbBool
SoType::canCreateInstance(void) const
{
  return ((*SoType::typedatalist)[(int)this->getKey()]->method != NULL);
}

/*!
  This method instantiates an object of the current type.

  For types that can not be instantiated, \c NULL is returned.

  \DANGEROUS_ALLOC_RETURN

  This is not harmful if you only call SoType::createInstance() on
  types for reference counted class-types, though. These include all
  nodes, engines, paths, nodekits, draggers and manipulators.
*/
void *
SoType::createInstance(void) const
{
  if (this->canCreateInstance()) {
    return (*((*SoType::typedatalist)[(int)this->getKey()]->method))();
  }
  else {
#if COIN_DEBUG
    SoDebugError::postWarning("SoType::createInstance",
                              "can't create instance of class type '%s', "
                              " use SoType::canCreateInstance()",
                              this->getName().getString());
#endif // COIN_DEBUG
    return NULL;
  }
}

/*!
  This function returns the number of types registered in the run-time type
  system.
*/

int
SoType::getNumTypes(void)
{
  return SoType::typedatalist->getLength();
}

/*!
  Returns a pointer to the method used to instantiate objects of the given
  type.

  \COIN_FUNCTION_EXTENSION

  \since Coin 2.0
*/

SoType::instantiationMethod
SoType::getInstantiationMethod(void) const
{
  return (*SoType::typedatalist)[(int)this->getKey()]->method;
}

/*!
  \fn int16_t SoType::getKey(void) const

  This method returns the type's index in the internal typelist.

*/

/*!
  This method turns the specific type into an internal type.
*/

void
SoType::makeInternal(void)
{
  (*SoType::typedatalist)[(int)this->getKey()]->isPublic = FALSE;
}

/*!
  This function returns TRUE if the type is an internal type.
*/

SbBool
SoType::isInternal(void) const
{
  return (*SoType::typedatalist)[(int)this->getKey()]->isPublic == FALSE;
}

/*!
  \fn SbBool SoType::operator != (const SoType type) const

  Check type inequality.
*/

/*!
  \fn SbBool SoType::operator == (const SoType type) const

  Check type equality.
*/

/*!
  \fn SbBool SoType::operator <  (const SoType type) const

  Comparison operator for sorting type data according to some internal
  criterion.
*/

/*!
  \fn SbBool SoType::operator <= (const SoType type) const

  Comparison operator for sorting type data according to some internal
  criterion.
*/

/*!
  \fn SbBool SoType::operator >= (const SoType type) const

  Comparison operator for sorting type data according to some internal
  criterion.
*/

/*!
  \fn SbBool SoType::operator >  (const SoType type) const

  Comparison operator for sorting type data according to some internal
  criterion.
*/

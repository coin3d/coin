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

#include <../tidbits.h> // coin_atexit()
#include <assert.h>
#include <stdlib.h> // NULL
#include <string.h> // strcmp()

struct SoTypeData {
  SoTypeData(const SbName theName,
             const SbBool ispublic = FALSE,
             const uint16_t theData = 0,
             const SoType theParent = SoType::badType(),
             const SoType::instantiationMethod createMethod = NULL)
    : name(theName), isPublic(ispublic), data(theData),
      parent(theParent), method(createMethod) { };

  SbName name;
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


SoTypeList * SoType::typelist = NULL;
SbList<SoTypeData *> * SoType::typedatalist = NULL;
SbDict * SoType::typedict = NULL;

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
  coin_atexit((coin_atexit_f *)SoType::clean);
#endif // COIN_DEBUG

  // If any of these assert fails, it is probably because
  // SoType::init() has been called for a second time. --mortene
  assert(SoType::typelist == NULL);
  assert(SoType::typedatalist == NULL);

  SoType::typelist = new SoTypeList;
  SoType::typedatalist = new SbList<SoTypeData *>;
  SoType::typedict = new SbDict;

  SoType::typelist->append(SoType::badType()); // bad type at index 0
  SoType::typedatalist->append(new SoTypeData(SbName("BadType")));
  SoType::typedict->enter((unsigned long)SbName("BadType").getString(), 0);
}

// Clean up internal resource usage.
void
SoType::clean(void)
{
#if COIN_DEBUG
  delete SoType::typelist;

  // clean SoType::typedatalist (first delete structures)
  const int num = SoType::typedatalist->getLength();
  for (int i = 0; i < num; i++) delete (*SoType::typedatalist)[i];
  delete SoType::typedatalist;

  delete SoType::typedict;
#endif // COIN_DEBUG
}

/*!
  This method creates and registers a new class type.

  Abstract types should use NULL for the \a method argument.

*/

const SoType
SoType::createType(const SoType parent, const SbName name,
                   const instantiationMethod method,
                   const uint16_t data)
{
#if COIN_DEBUG
  if (SoType::fromName(name.getString()) != SoType::badType()) {
    SoDebugError::post("SoType::createType",
                       "a type with name ``%s'' already created",
                       name.getString());
    return SoType::fromName(name.getString());
  }
#endif // COIN_DEBUG

#if COIN_DEBUG && 0 // debug
  SoDebugError::postInfo("SoType::createType", "%s", name.getString());
#endif // debug

  SoTypeData * typeData = new SoTypeData(name, TRUE, data, parent, method);
  SoType newType;
  newType.index = SoType::typelist->getLength();
  SoType::typelist->append(newType);
  SoType::typedatalist->append(typeData);
  // add to dictionary for fast lookup
  SoType::typedict->enter((unsigned long)name.getString(), (void *)newType.index);
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

/*!
  This static method returns the SoType object associated with name \a name.
  If no known type matches the given name, SoType::badType() is returned.
*/

SoType
SoType::fromName(const SbName name)
{
  // It should be possible to specify a type name with the "So" prefix
  // and get the correct type id, even though the types in some type
  // hierarchies are named internally without the prefix.
  SbString tmp(name.getString());
  if ((tmp.getLength() > 2) && (strcmp(tmp.getSubString(0, 1).getString(), "So") == 0))
    tmp = tmp.getSubString(2);
  SbName noprefixname(tmp);

  void * temp = NULL;
  if (SoType::typedict->find((unsigned long)name.getString(), temp) ||
      SoType::typedict->find((unsigned long)noprefixname.getString(), temp)) {
    // the intermediate casting to long needed for 64-bits IRIX CC
    const int index = (int)((long)temp);
    assert(index >= 0 && index < SoType::typelist->getLength());
    assert(((*SoType::typedatalist)[index]->name == name) ||
           ((*SoType::typedatalist)[index]->name == noprefixname));
    return (*SoType::typelist)[index];
  }
  return SoType::badType();
}

/*!
  Find and return a type from the given key ID.
*/

SoType
SoType::fromKey(uint16_t key)
{
  assert(key < SoType::typelist->getLength());
  return (*SoType::typelist)[(int)key];
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
  int counter = 0;
  int n = SoType::typelist->getLength();
  for (int i = 0; i < n; i++) {
    SoType chktype = (*SoType::typelist)[i];
    if (!chktype.isInternal() && chktype.isDerivedFrom(type)) {
      list.append(chktype);
      counter++;
    }
  }
  return counter;
}

/*!
  This method returns FALSE for abstract base classes, and TRUE for class
  types that can be instanciated.
*/

SbBool
SoType::canCreateInstance(void) const
{
  return ((*SoType::typedatalist)[(int)this->getKey()]->method != NULL);
}

/*!
  This method instantiates an object of the current type.

  For types that can not be instanciated, NULL is returned.
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
  return SoType::typelist->getLength();
}

/*!
  Returns a pointer to the method used to instantiate objects of the given
  type.

  \since 2002-01-22
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

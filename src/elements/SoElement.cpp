/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2001 by Systems in Motion. All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public License
 *  version 2.1 as published by the Free Software Foundation. See the
 *  file LICENSE.LGPL at the root directory of the distribution for
 *  more details.
 *
 *  If you want to use Coin for applications not compatible with the
 *  LGPL, please contact SIM to acquire a Professional Edition license.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  http://www.sim.no support@sim.no Voice: +47 22114160 Fax: +47 22207097
 *
\**************************************************************************/

/*!
  \class SoElement Inventor/elements/SoElement.h
  \brief This is the abstract base class for all elements.  The elements are used by the SoState class.
  \ingroup elements
*/

#include <Inventor/elements/SoElements.h>
#include <Inventor/elements/SoCullElement.h> // internal element
#include <Inventor/elements/SoTextureScalePolicyElement.h> // future element
#include <Inventor/misc/SoState.h>
#include <stdlib.h>
#include <Inventor/lists/SoTypeList.h>

#include <assert.h>


/*!
  \fn SoElement * SoElement::getElement(SoState * const state, const int stackIndex)

  This method returns the top instance (in the state stack) of the element
  class with stack index \a stackIndex.  This instance is writable.  To make
  this instance, some lazy evaluation may have to be perfomed, so use
  getConstElement() instead if the instance shouldn't be modified.
  If no instance can be returned, NULL is returned.

  \sa const SoElement * SoElement::getConstElement(SoState * const state, const int stackIndex)
*/

/*!
  \fn SoElement::typeId

  FIXME: write doc.
*/

/*!
  \fn SoElement::stackIndex

  FIXME: write doc.
*/

/*!
  \fn SoElement::depth

  FIXME: write doc.
*/



// FIXME: do we need to duplicate the following documentation in all
// subclasses as well to keep Doxygen happy? 19991209 mortene.

/*!
  \fn SoType SoElement::getClassTypeId(void)
  This static method returns the class type.
*/
/*!
  \var SoElement::classStackIndex
  This is the static state stack index for the class.
*/
/*!
  \fn int SoElement::getClassStackIndex(void)
  This static method returns the state stack index for the class.
*/


int SoElement::classStackIndex;
// Don't set value explicitly to SoType::badType(), to avoid a bug in
// Sun CC v4.0. (Bitpattern 0x0000 equals SoType::badType()).
SoType SoElement::classTypeId;

SoType SoElement::getClassTypeId(void) { return SoElement::classTypeId; }
int SoElement::getClassStackIndex(void) { return SoElement::classStackIndex; }



/*!
  This function initializes all the standard Inventor element classes.
*/

void
SoElement::initElements(void)
{
  SoAccumulatedElement::initClass();
  SoClipPlaneElement::initClass();
  SoGLClipPlaneElement::initClass();
  SoLightElement::initClass();
  SoModelMatrixElement::initClass();
  SoBBoxModelMatrixElement::initClass();
  SoGLModelMatrixElement::initClass();
  SoProfileElement::initClass();
  SoTextureMatrixElement::initClass();
  SoGLTextureMatrixElement::initClass();
  SoCacheElement::initClass();
  SoInt32Element::initClass();
  SoAnnoText3CharOrientElement::initClass();
  SoAnnoText3FontSizeHintElement::initClass();
  SoAnnoText3RenderPrintElement::initClass();
  SoComplexityTypeElement::initClass();
  SoDecimationTypeElement::initClass();
  SoDrawStyleElement::initClass();
  SoGLDrawStyleElement::initClass();
  SoGLLightIdElement::initClass();
  SoGLTextureEnabledElement::initClass();
  SoLinePatternElement::initClass();
  SoGLLinePatternElement::initClass();
  SoMaterialBindingElement::initClass();
  SoNormalBindingElement::initClass();
  SoPickStyleElement::initClass();
  SoSwitchElement::initClass();
  SoTextOutlineEnabledElement::initClass();
  SoTextureCoordinateBindingElement::initClass();
  SoUnitsElement::initClass();
  SoFloatElement::initClass();
  SoComplexityElement::initClass();
  SoCreaseAngleElement::initClass();
  SoDecimationPercentageElement::initClass();
  SoFocalDistanceElement::initClass();
  SoFontSizeElement::initClass();
  SoLineWidthElement::initClass();
  SoGLLineWidthElement::initClass();
  SoPointSizeElement::initClass();
  SoGLPointSizeElement::initClass();
  SoTextureQualityElement::initClass();
  SoGLRenderPassElement::initClass();
  SoGLUpdateAreaElement::initClass();
  SoLocalBBoxMatrixElement::initClass();
  SoOverrideElement::initClass();
  SoTextureOverrideElement::initClass();
  SoPickRayElement::initClass();
  SoReplacedElement::initClass();
  SoCoordinateElement::initClass();
  SoGLCoordinateElement::initClass();
  SoGLColorIndexElement::initClass();
  SoEnvironmentElement::initClass();
  SoGLEnvironmentElement::initClass();
  SoFontNameElement::initClass();
  SoLightAttenuationElement::initClass();
  SoNormalElement::initClass();
  SoGLNormalElement::initClass();
  SoPolygonOffsetElement::initClass();
  SoGLPolygonOffsetElement::initClass();
  SoProjectionMatrixElement::initClass();
  SoGLProjectionMatrixElement::initClass();
  SoProfileCoordinateElement::initClass();
  SoTextureCoordinateElement::initClass();
  SoGLTextureCoordinateElement::initClass();
  SoTextureImageElement::initClass();
  SoGLTextureImageElement::initClass();
  SoViewingMatrixElement::initClass();
  SoGLViewingMatrixElement::initClass();
  SoViewVolumeElement::initClass();
  SoShapeHintsElement::initClass();
  SoGLShapeHintsElement::initClass();
  SoShapeStyleElement::initClass();
  SoViewportRegionElement::initClass();
  SoGLViewportRegionElement::initClass();
  SoWindowElement::initClass();

  SoTransparencyElement::initClass();
  SoAmbientColorElement::initClass();
  SoGLAmbientColorElement::initClass();
  SoDiffuseColorElement::initClass();
  SoGLDiffuseColorElement::initClass();
  SoEmissiveColorElement::initClass();
  SoGLEmissiveColorElement::initClass();
  SoLightModelElement::initClass();
  SoGLLightModelElement::initClass();
  SoShininessElement::initClass();
  SoGLShininessElement::initClass();
  SoSpecularColorElement::initClass();
  SoGLSpecularColorElement::initClass();
  SoGLPolygonStippleElement::initClass();

  SoGLShadeModelElement::initClass();
  SoGLNormalizeElement::initClass();
  SoLazyElement::initClass();
  SoCullElement::initClass();
  SoGLCacheContextElement::initClass();

  SoTextureScalePolicyElement::initClass();
}

/*!
  This function initializes the SoElement class.
*/
void
SoElement::initClass(void)
{
  SoElement::stackToType = new SoTypeList;

  // Make sure we only initialize once.
  assert(SoElement::classTypeId == SoType::badType());
  SoElement::classTypeId =
    SoType::createType(SoType::badType(), "Element", NULL);

  SoElement::classStackIndex = -1;
  SoElement::initElements();

  // cleanup at exit
  atexit(SoElement::cleanup);
}

// atexit callback
void
SoElement::cleanup(void)
{
  delete SoElement::stackToType;
}

/*!
  The constructor.  To create element instances, use SoType::createInstance()
  for the elements type identifier..
*/
SoElement::SoElement(void)
  : nextup(NULL),
    nextdown(NULL)
{
}

/*!
  The destructor.
*/

SoElement::~SoElement()
{
}

/*!
  This function initializes the element type in the given SoState.  It is
  called for the first element of each enabled element type in SoState
  objects.  The default method does nothing.
*/

void
SoElement::init(SoState *)
{
}

/*!
  This method is called every time a new element is required in one of
  the stacks. This happens when a writable element is requested, using
  SoState::getElement() or indirectly SoElement::getElement(), and the
  depth of the current element is less than the state depth.

  Overload this method if your element needs to copy data from the
  previous top of stack. The push() method is called on the new
  element, and the previous element can be found using
  SoElement::getNextInStack().

  The default method does nothing.
*/
void
SoElement::push(SoState * /* state */)
{
}

/*!
  This method is callled when the state is popped, and the depth
  of the element is bigger than the current state depth. pop()
  is called on the new top of stack, and a pointer to the previous
  top of stack is passed in \a prevTopElement.

  Default method does nothing. Overload this method if you need
  to copy some state information from the previou top of stack.
*/
void
SoElement::pop(SoState * /* state */, const SoElement * /* prevTopElement */)
{
}

/*!
  This function is for printing element information, mostly for debugging
  purposes.
*/

void
SoElement::print(FILE * file) const
{
    fprintf(file, "%s[%p]\n", getTypeId().getName().getString(), this);
}

/*!
  This function returns TRUE is the element matches another element (of the
  same class), with respect to cache validity.  If a matches() function is
  written, one should also write a copyMatchInfo() function.
*/

SbBool
SoElement::matches(const SoElement * /* element */) const
{
  return FALSE;
}

/*!
  \fn virtual SoElement * SoElement::copyMatchInfo(void) const = 0

  This function creates a copy of the element that contains enough information
  to enable the matches() function to work.  Used for caching.
*/

/*!
  This method returns the number of allocated element stack index slots.
*/

int
SoElement::getNumStackIndices()
{
  return SoElement::stackToType->getLength();
}

/*!
  This function returns the SoType identifier for the element class with
  element state stack index \a stackIndex.
*/

SoType
SoElement::getIdFromStackIndex(const int stackIndex)
{
  assert(SoElement::stackToType->getLength() > stackIndex);
  return (*SoElement::stackToType)[ stackIndex ];
}

/*!
  This method sets the depth value of the element instance.
*/

void
SoElement::setDepth(const int depth)
{
  this->depth = depth;
}

/*!
  This method returns the depth value of the element instance.
*/

int
SoElement::getDepth() const
{
  return this->depth;
}

/*!
  This function does whatever is necessary in the state for caching purposes.
  If should be called by subclasses of SoElement whenever any value in the
  element is accessed.
*/

inline void
SoElement::capture(SoState * const state) const
{
  if (state->isCacheOpen()) this->captureThis(state);
}

/*!
  This method returns a const pointer to the top element of the class with
  stack index \a stackIndex.  If this instance is modified, strange things
  will most likely start to happen.
  If no instance can be returned, NULL is returned.

  \sa SoElement * SoElement::getElement(SoState * const state, const int stackIndex)
*/
const SoElement *
SoElement::getConstElement(SoState * const state,
                           const int stackIndex)
{
  const SoElement * element = state->getConstElement(stackIndex);
  element->capture(state);
  return element;
}

/*!
  FIXME: write doc.
*/

void
SoElement::captureThis(SoState * state) const
{
  SoCacheElement::addElement(state, this);
}

/*!
  This method sets the type identifier of an instance.
  This is fundamentally different from the SoNode run-time type system.
*/

void
SoElement::setTypeId(const SoType typeId)
{
  this->typeId = typeId;
}

// FIXME: grab better version of getTypeId() doc from SoBase, SoAction
// and / or SoDetail. 20010913 mortene.
/*!
  This method returns the type identifier for the element instance.
*/
const SoType
SoElement::getTypeId() const
{
  return this->typeId;
}

/*!
  This method returns the stack index for an element instance.
*/

int
SoElement::getStackIndex(void) const
{
  return this->stackIndex;
}

/*!
  This function sets the stack index in an instance.  Used in constructors
  of derived elements.
*/

void
SoElement::setStackIndex(const int stackIndex)
{
  this->stackIndex = stackIndex;
}

/*!
  \fn SoElement::stackToType

  FIXME: write doc.
*/

SoTypeList * SoElement::stackToType;

/*!
  This method returns the value of a new available stack index.
*/

int
SoElement::createStackIndex(const SoType typeId)
{
  if (typeId.canCreateInstance()) {
    SoElement::stackToType->append(typeId);
    return SoElement::stackToType->getLength() - 1;
  }
  return -1;
}

/*!
  Returns the next element down in the stack. Should be used
  in push() to get the previous element. This method has a
  silly name (IMHO), but is kept to keep OIV compatibility.
*/
SoElement *
SoElement::getNextInStack(void) const
{
  return this->nextdown;
}

/*!
  Returns the next free element. The next element up in the stack.
*/
SoElement *
SoElement::getNextFree(void) const
{
  return this->nextup;
}

/*!
  Should return \e TRUE if this element needs lazy GL evaluation.
  Default method returns \e FALSE.
*/
SbBool
SoElement::isLazy(void) const
{
  return FALSE;
}

/*!
  Evaluates lazy GL element. Default method does nothing.
*/
void
SoElement::lazyEvaluate(void) const
{
}

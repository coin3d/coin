/**************************************************************************\
 *
 *  Copyright (C) 1998-2000 by Systems in Motion.  All rights reserved.
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

/*!
  \class SoElement Inventor/elements/SoElement.h
  \brief This is the abstract base class for all elements.  The elements are
  used by the SoState class.
*/

#include <Inventor/elements/SoElements.h>
#include <Inventor/elements/SoCullElement.h> // internal element
#include <Inventor/misc/SoState.h>
#include <stdlib.h>
#include <coindefs.h> // COIN_STUB()
#include <Inventor/lists/SoTypeList.h>

#include <assert.h>

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

/*!
  \fn SoElement::next

  FIXME: write doc.
*/

/*!
  \fn SoElement::prev

  FIXME: write doc
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
{
  this->next = NULL;
  this->prev = NULL;
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
  This method pushes the element.
  It should duplicate the needed internal SoElement instance data
  into the SoElement instance at \e next.

  This method is used for copying the
  internal data to the next element in the stack, and is called by SoState
  after such an element has been created.
*/

void
SoElement::push(SoState * /* state */)
{
  assert(this->next);
}

/*!
  This function pops the element.
  Default method does nothing.
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
  COIN_STUB();
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
  This method returns the top instance (in the state stack) of the element
  class with stack index \a stackIndex.  This instance is writable.  To make
  this instance, some lazy evaluation may have to be perfomed, so use
  getConstElement() instead if the instance shouldn't be modified.
  If no instance can be returned, NULL is returned.

  \sa const SoElement * SoElement::getConstElement(SoState * const state, const int stackIndex)
*/

SoElement *
SoElement::getElement(SoState * const state,
                      const int stackIndex)
{
  return state->getElement(stackIndex);
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

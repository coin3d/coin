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

/*!
  \class SoElement Inventor/elements/SoElement.h
  \brief This is the abstract base class for all elements.  The elements are
  used by the SoState class.
*/

#include <Inventor/elements/SoElements.h>
#include <Inventor/misc/SoState.h>
#include <Inventor/SbName.h>
#include <Inventor/SoType.h>
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

// This class should probably not use any templates...

//$ BEGIN TEMPLATE AbstractElementSource(SoElement)

/*!
  \var SoElement::classTypeId

  This is the static class type identifier for
  SoElement.
*/

SoType SoElement::classTypeId = SoType::badType(); // static

/*!
  This static method returns the SoType type for SoElement.
*/

SoType
SoElement::getClassTypeId( // static
  void )
{
  return SoElement::classTypeId;
}

/*!
  \var SoElement::classStackIndex

  This is the static state stack index for the
  SoElement class.
*/

int SoElement::classStackIndex; // static

/*!
  This static method returns the state stack index for the SoElement class.
*/

int
SoElement::getClassStackIndex( // static
  void )
{
  return SoElement::classStackIndex;
}
//$ END TEMPLATE AbstractElementSource

/*!
  This function initializes all the standard Inventor element classes.
*/

void
SoElement::initElements(void)
{
#if !defined(COIN_EXCLUDE_SOACCUMULATEDELEMENT)
  SoAccumulatedElement::initClass();
#endif // !COIN_EXCLUDE_SOACCUMULATEDELEMENT
#if !defined(COIN_EXCLUDE_SOCLIPPLANEELEMENT)
  SoClipPlaneElement::initClass();
#endif // !COIN_EXCLUDE_SOCLIPPLANEELEMENT
#if !defined(COIN_EXCLUDE_SOGLCLIPPLANEELEMENT)
  SoGLClipPlaneElement::initClass();
#endif // !COIN_EXCLUDE_SOGLCLIPPLANEELEMENT
#if !defined(COIN_EXCLUDE_SOLIGHTELEMENT)
  SoLightElement::initClass();
#endif // !COIN_EXCLUDE_SOLIGHTELEMENT
#if !defined(COIN_EXCLUDE_SOMODELMATRIXELEMENT)
  SoModelMatrixElement::initClass();
#endif // !COIN_EXCLUDE_SOMODELMATRIXELEMENT
#if !defined(COIN_EXCLUDE_SOBBOXMODELMATRIXELEMENT)
  SoBBoxModelMatrixElement::initClass();
#endif // !COIN_EXCLUDE_SOBBOXMODELMATRIXELEMENT
#if !defined(COIN_EXCLUDE_SOGLMODELMATRIXELEMENT)
  SoGLModelMatrixElement::initClass();
#endif // !COIN_EXCLUDE_SOGLMODELMATRIXELEMENT
#if !defined(COIN_EXCLUDE_SOPROFILEELEMENT)
  SoProfileElement::initClass();
#endif // !COIN_EXCLUDE_SOPROFILEELEMENT
#if !defined(COIN_EXCLUDE_SOTEXTUREMATRIXELEMENT)
  SoTextureMatrixElement::initClass();
#endif // !COIN_EXCLUDE_SOTEXTUREMATRIXELEMENT
#if !defined(COIN_EXCLUDE_SOGLTEXTUREMATRIXELEMENT)
  SoGLTextureMatrixElement::initClass();
#endif // !COIN_EXCLUDE_SOGLTEXTUREMATRIXELEMENT
#if !defined(COIN_EXCLUDE_SOCACHEELEMENT)
  SoCacheElement::initClass();
#endif // !COIN_EXCLUDE_SOCACHEELEMENT
#if !defined(COIN_EXCLUDE_SOINT32ELEMENT)
  SoInt32Element::initClass();
#endif // !COIN_EXCLUDE_SOINT32ELEMENT
#if !defined(COIN_EXCLUDE_SOANNOTEXT3CHARORIENTELEMENT)
  SoAnnoText3CharOrientElement::initClass();
#endif // !COIN_EXCLUDE_SOANNOTEXT3CHARORIENTELEMENT
#if !defined(COIN_EXCLUDE_SOANNOTEXT3FONTSIZEHINTELEMENT)
  SoAnnoText3FontSizeHintElement::initClass();
#endif // !COIN_EXCLUDE_SOANNOTEXT3FONTSIZEHINTELEMENT
#if !defined(COIN_EXCLUDE_SOANNOTEXT3RENDERPRINTELEMENT)
  SoAnnoText3RenderPrintElement::initClass();
#endif // !COIN_EXCLUDE_SOANNOTEXT3RENDERPRINTELEMENT
#if !defined(COIN_EXCLUDE_SOCOMPLEXITYTYPEELEMENT)
  SoComplexityTypeElement::initClass();
#endif // !COIN_EXCLUDE_SOCOMPLEXITYTYPEELEMENT
#if !defined(COIN_EXCLUDE_SODECIMATIONTYPEELEMENT)
  SoDecimationTypeElement::initClass();
#endif // !COIN_EXCLUDE_SODECIMATIONTYPEELEMENT
#if !defined(COIN_EXCLUDE_SODRAWSTYLEELEMENT)
  SoDrawStyleElement::initClass();
#endif // !COIN_EXCLUDE_SODRAWSTYLEELEMENT
#if !defined(COIN_EXCLUDE_SOGLDRAWSTYLEELEMENT)
  SoGLDrawStyleElement::initClass();
#endif // !COIN_EXCLUDE_SOGLDRAWSTYLEELEMENT
#if !defined(COIN_EXCLUDE_SOGLLIGHTIDELEMENT)
  SoGLLightIdElement::initClass();
#endif // !COIN_EXCLUDE_SOGLLIGHTIDELEMENT
#if !defined(COIN_EXCLUDE_SOGLTEXTUREENABLEDELEMENT)
  SoGLTextureEnabledElement::initClass();
#endif // !COIN_EXCLUDE_SOGLTEXTUREENABLEDELEMENT
#if !defined(COIN_EXCLUDE_SOLINEPATTERNELEMENT)
  SoLinePatternElement::initClass();
#endif // !COIN_EXCLUDE_SOLINEPATTERNELEMENT
#if !defined(COIN_EXCLUDE_SOGLLINEPATTERNELEMENT)
  SoGLLinePatternElement::initClass();
#endif // !COIN_EXCLUDE_SOGLLINEPATTERNELEMENT
#if !defined(COIN_EXCLUDE_SOMATERIALBINDINGELEMENT)
  SoMaterialBindingElement::initClass();
#endif // !COIN_EXCLUDE_SOMATERIALBINDINGELEMENT
#if !defined(COIN_EXCLUDE_SONORMALBINDINGELEMENT)
  SoNormalBindingElement::initClass();
#endif // !COIN_EXCLUDE_SONORMALBINDINGELEMENT
#if !defined(COIN_EXCLUDE_SOPICKSTYLEELEMENT)
  SoPickStyleElement::initClass();
#endif // !COIN_EXCLUDE_SOPICKSTYLEELEMENT
#if !defined(COIN_EXCLUDE_SOSWITCHELEMENT)
  SoSwitchElement::initClass();
#endif // !COIN_EXCLUDE_SOSWITCHELEMENT
#if !defined(COIN_EXCLUDE_SOTEXTOUTLINEENABLEDELEMENT)
  SoTextOutlineEnabledElement::initClass();
#endif // !COIN_EXCLUDE_SOTEXTOUTLINEENABLEDELEMENT
#if !defined(COIN_EXCLUDE_SOTEXTURECOORDINATEBINDINGELEMENT)
  SoTextureCoordinateBindingElement::initClass();
#endif // !COIN_EXCLUDE_SOTEXTURECOORDINATEBINDINGELEMENT
#if !defined(COIN_EXCLUDE_SOUNITSELEMENT)
  SoUnitsElement::initClass();
#endif // !COIN_EXCLUDE_SOUNITSELEMENT
#if !defined(COIN_EXCLUDE_SOFLOATELEMENT)
  SoFloatElement::initClass();
#endif // !COIN_EXCLUDE_SOFLOATELEMENT
#if !defined(COIN_EXCLUDE_SOCOMPLEXITYELEMENT)
  SoComplexityElement::initClass();
#endif // !COIN_EXCLUDE_SOCOMPLEXITYELEMENT
#if !defined(COIN_EXCLUDE_SOCREASEANGLEELEMENT)
  SoCreaseAngleElement::initClass();
#endif // !COIN_EXCLUDE_SOCREASEANGLEELEMENT
#if !defined(COIN_EXCLUDE_SODECIMATIONPERCENTAGEELEMENT)
  SoDecimationPercentageElement::initClass();
#endif // !COIN_EXCLUDE_SODECIMATIONPERCENTAGEELEMENT
#if !defined(COIN_EXCLUDE_SOFOCALDISTANCEELEMENT)
  SoFocalDistanceElement::initClass();
#endif // !COIN_EXCLUDE_SOFOCALDISTANCEELEMENT
#if !defined(COIN_EXCLUDE_SOFONTSIZEELEMENT)
  SoFontSizeElement::initClass();
#endif // !COIN_EXCLUDE_SOFONTSIZEELEMENT
#if !defined(COIN_EXCLUDE_SOLINEWIDTHELEMENT)
  SoLineWidthElement::initClass();
#endif // !COIN_EXCLUDE_SOLINEWIDTHELEMENT
#if !defined(COIN_EXCLUDE_SOGLLINEWIDTHELEMENT)
  SoGLLineWidthElement::initClass();
#endif // !COIN_EXCLUDE_SOGLLINEWIDTHELEMENT
#if !defined(COIN_EXCLUDE_SOPOINTSIZEELEMENT)
  SoPointSizeElement::initClass();
#endif // !COIN_EXCLUDE_SOPOINTSIZEELEMENT
#if !defined(COIN_EXCLUDE_SOGLPOINTSIZEELEMENT)
  SoGLPointSizeElement::initClass();
#endif // !COIN_EXCLUDE_SOGLPOINTSIZEELEMENT
#if !defined(COIN_EXCLUDE_SOTEXTUREQUALITYELEMENT)
  SoTextureQualityElement::initClass();
#endif // !COIN_EXCLUDE_SOTEXTUREQUALITYELEMENT
#if !defined(COIN_EXCLUDE_SOGLRENDERPASSELEMENT)
  SoGLRenderPassElement::initClass();
#endif // !COIN_EXCLUDE_SOGLRENDERPASSELEMENT
#if !defined(COIN_EXCLUDE_SOGLUPDATEAREAELEMENT)
  SoGLUpdateAreaElement::initClass();
#endif // !COIN_EXCLUDE_SOGLUPDATEAREAELEMENT
#if !defined(COIN_EXCLUDE_SOLOCALBBOXMATRIXELEMENT)
  SoLocalBBoxMatrixElement::initClass();
#endif // !COIN_EXCLUDE_SOLOCALBBOXMATRIXELEMENT
#if !defined(COIN_EXCLUDE_SOOVERRIDEELEMENT)
  SoOverrideElement::initClass();
#endif // !COIN_EXCLUDE_SOOVERRIDEELEMENT
#if !defined(COIN_EXCLUDE_SOPICKRAYELEMENT)
  SoPickRayElement::initClass();
#endif // !COIN_EXCLUDE_SOPICKRAYELEMENT
#if !defined(COIN_EXCLUDE_SOREPLACEDELEMENT)
  SoReplacedElement::initClass();
#endif // !COIN_EXCLUDE_SOREPLACEDELEMENT
#if !defined(COIN_EXCLUDE_SOCOORDINATEELEMENT)
  SoCoordinateElement::initClass();
#endif // !COIN_EXCLUDE_SOCOORDINATEELEMENT
#if !defined(COIN_EXCLUDE_SOGLCOORDINATEELEMENT)
  SoGLCoordinateElement::initClass();
#endif // !COIN_EXCLUDE_SOGLCOORDINATEELEMENT
#if !defined(COIN_EXCLUDE_SOENVIRONMENTELEMENT)
  SoEnvironmentElement::initClass();
#endif // !COIN_EXCLUDE_SOENVIRONMENTELEMENT
#if !defined(COIN_EXCLUDE_SOGLENVIRONMENTELEMENT)
  SoGLEnvironmentElement::initClass();
#endif // !COIN_EXCLUDE_SOGLENVIRONMENTELEMENT
#if !defined(COIN_EXCLUDE_SOFONTNAMEELEMENT)
  SoFontNameElement::initClass();
#endif // !COIN_EXCLUDE_SOFONTNAMEELEMENT
#if !defined(COIN_EXCLUDE_SOLIGHTATTENUATIONELEMENT)
  SoLightAttenuationElement::initClass();
#endif // !COIN_EXCLUDE_SOLIGHTATTENUATIONELEMENT
#if !defined(COIN_EXCLUDE_SONORMALELEMENT)
  SoNormalElement::initClass();
#endif // !COIN_EXCLUDE_SONORMALELEMENT
#if !defined(COIN_EXCLUDE_SOGLNORMALELEMENT)
  SoGLNormalElement::initClass();
#endif // !COIN_EXCLUDE_SOGLNORMALELEMENT
#if !defined(COIN_EXCLUDE_SOPOLYGONOFFSETELEMENT)
  SoPolygonOffsetElement::initClass();
#endif // !COIN_EXCLUDE_SOPOLYGONOFFSETELEMENT
#if !defined(COIN_EXCLUDE_SOGLPOLYGONOFFSETELEMENT)
  SoGLPolygonOffsetElement::initClass();
#endif // !COIN_EXCLUDE_SOGLPOLYGONOFFSETELEMENT
#if !defined(COIN_EXCLUDE_SOPROJECTIONMATRIXELEMENT)
  SoProjectionMatrixElement::initClass();
#endif // !COIN_EXCLUDE_SOPROJECTIONMATRIXELEMENT
#if !defined(COIN_EXCLUDE_SOGLPROJECTIONMATRIXELEMENT)
  SoGLProjectionMatrixElement::initClass();
#endif // !COIN_EXCLUDE_SOGLPROJECTIONMATRIXELEMENT
#if !defined(COIN_EXCLUDE_SOPROFILECOORDINATEELEMENT)
  SoProfileCoordinateElement::initClass();
#endif // !COIN_EXCLUDE_SOPROFILECOORDINATEELEMENT
#if !defined(COIN_EXCLUDE_SOTEXTURECOORDINATEELEMENT)
  SoTextureCoordinateElement::initClass();
#endif // !COIN_EXCLUDE_SOTEXTURECOORDINATEELEMENT
#if !defined(COIN_EXCLUDE_SOGLTEXTURECOORDINATEELEMENT)
  SoGLTextureCoordinateElement::initClass();
#endif // !COIN_EXCLUDE_SOGLTEXTURECOORDINATEELEMENT
#if !defined(COIN_EXCLUDE_SOTEXTUREIMAGEELEMENT)
  SoTextureImageElement::initClass();
#endif // !COIN_EXCLUDE_SOTEXTUREIMAGEELEMENT
#if !defined(COIN_EXCLUDE_SOGLTEXTUREIMAGEELEMENT)
  SoGLTextureImageElement::initClass();
#endif // !COIN_EXCLUDE_SOGLTEXTUREIMAGEELEMENT
#if !defined(COIN_EXCLUDE_SOVIEWINGMATRIXELEMENT)
  SoViewingMatrixElement::initClass();
#endif // !COIN_EXCLUDE_SOVIEWINGMATRIXELEMENT
#if !defined(COIN_EXCLUDE_SOGLVIEWINGMATRIXELEMENT)
  SoGLViewingMatrixElement::initClass();
#endif // !COIN_EXCLUDE_SOGLVIEWINGMATRIXELEMENT
#if !defined(COIN_EXCLUDE_SOVIEWVOLUMEELEMENT)
  SoViewVolumeElement::initClass();
#endif // !COIN_EXCLUDE_SOVIEWVOLUMEELEMENT
#if !defined(COIN_EXCLUDE_SOSHAPEHINTSELEMENT)
  SoShapeHintsElement::initClass();
#endif // !COIN_EXCLUDE_SOSHAPEHINTSELEMENT
#if !defined(COIN_EXCLUDE_SOGLSHAPEHINTSELEMENT)
  SoGLShapeHintsElement::initClass();
#endif // !COIN_EXCLUDE_SOGLSHAPEHINTSELEMENT
#if !defined(COIN_EXCLUDE_SOSHAPESTYLEELEMENT)
  SoShapeStyleElement::initClass();
#endif // !COIN_EXCLUDE_SOSHAPESTYLEELEMENT
#if !defined(COIN_EXCLUDE_SOVIEWPORTREGIONELEMENT)
  SoViewportRegionElement::initClass();
#endif // !COIN_EXCLUDE_SOVIEWPORTREGIONELEMENT
#if !defined(COIN_EXCLUDE_SOGLVIEWPORTREGIONELEMENT)
  SoGLViewportRegionElement::initClass();
#endif // !COIN_EXCLUDE_SOGLVIEWPORTREGIONELEMENT
#if !defined(COIN_EXCLUDE_SOWINDOWELEMENT)
  SoWindowElement::initClass();
#endif // !COIN_EXCLUDE_SOWINDOWELEMENT

#if !defined(COIN_EXCLUDE_SOTRANSPARENCYELEMENT)
  SoTransparencyElement::initClass();
#endif // !COIN_EXCLUDE_SOTRANSPARENCYELEMENT
#if !defined(COIN_EXCLUDE_SOAMBIENTCOLORELEMENT)
  SoAmbientColorElement::initClass();
#endif // !COIN_EXCLUDE_SOAMBIENTCOLORELEMENT
#if !defined(COIN_EXCLUDE_SOGLAMBIENTCOLORELEMENT)
  SoGLAmbientColorElement::initClass();
#endif // !COIN_EXCLUDE_SOGLAMBIENTCOLORELEMENT
#if !defined(COIN_EXCLUDE_SODIFFUSECOLORELEMENT)
  SoDiffuseColorElement::initClass();
#endif // !COIN_EXCLUDE_SODIFFUSECOLORELEMENT
#if !defined(COIN_EXCLUDE_SOGLDIFFUSECOLORELEMENT)
  SoGLDiffuseColorElement::initClass();
#endif // !COIN_EXCLUDE_SOGLDIFFUSECOLORELEMENT
#if !defined(COIN_EXCLUDE_SOEMISSIVECOLORELEMENT)
  SoEmissiveColorElement::initClass();
#endif // !COIN_EXCLUDE_SOEMISSIVECOLORELEMENT
#if !defined(COIN_EXCLUDE_SOGLEMISSIVECOLORELEMENT)
  SoGLEmissiveColorElement::initClass();
#endif // !COIN_EXCLUDE_SOGLEMISSIVECOLORELEMENT
#if !defined(COIN_EXCLUDE_SOLIGHTMODELELEMENT)
  SoLightModelElement::initClass();
#endif // !COIN_EXCLUDE_SOLIGHTMODELELEMENT
#if !defined(COIN_EXCLUDE_SOGLLIGHTMODELELEMENT)
  SoGLLightModelElement::initClass();
#endif // !COIN_EXCLUDE_SOGLLIGHTMODELELEMENT
#if !defined(COIN_EXCLUDE_SOSHININESSELEMENT)
  SoShininessElement::initClass();
#endif // !COIN_EXCLUDE_SOSHININESSELEMENT
#if !defined(COIN_EXCLUDE_SOGLSHININESSELEMENT)
  SoGLShininessElement::initClass();
#endif // !COIN_EXCLUDE_SOGLSHININESSELEMENT
#if !defined(COIN_EXCLUDE_SOSPECULARCOLORELEMENT)
  SoSpecularColorElement::initClass();
#endif // !COIN_EXCLUDE_SOSPECULARCOLORELEMENT
#if !defined(COIN_EXCLUDE_SOGLSPECULARCOLORELEMENT)
  SoGLSpecularColorElement::initClass();
#endif // !COIN_EXCLUDE_SOGLSPECULARCOLORELEMENT
#if !defined(COIN_EXCLUDE_SOGLPOLYGONSTIPPLEELEMENT)
  SoGLPolygonStippleElement::initClass();
#endif // !COIN_EXCLUDE_SOGLPOLYGONSTIPPLEELEMENT

#if !defined(COIN_EXCLUDE_SOGLSHADEMODELELEMENT)
  SoGLShadeModelElement::initClass();
#endif
#if !defined(COIN_EXCLUDE_SOGLNORMALIZEELEMENT)
  SoGLNormalizeElement::initClass();
#endif

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
}

#if 0 // FIXME: re-code to be run automatically upon exit. 19991106 mortene.
/*!
  This function cleans up after the SoElement class.
*/

void
SoElement::cleanClass(void)
{
  SoElement::cleanElements();
  delete SoElement::stackToType;
}
#endif // re-code

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
  assert(0 && "FIXME: not implemented");
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
  //    element->captureThis(state);
  return element;
}

/*!
  This function does whatever is necessary in the state for caching purposes.
  If should be called by subclasses of SoElement whenever any value in the
  element is accessed.
*/

void
SoElement::capture(SoState * const state) const
{
  if (state->isCacheOpen())
    captureThis(state);
}

/*!
  FIXME: write doc.
*/

void
SoElement::captureThis(SoState * /* state */) const
{
  assert(0 && "FIXME: not implemented");
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


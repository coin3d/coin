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
  \class SoCallbackAction Inventor/actions/SoCallbackAction.h
  \brief The SoCallbackAction class is a traversal action that calls
  callbacks at specific nodes.
*/

#include <Inventor/actions/SoCallbackAction.h>
#include <Inventor/actions/SoSubAction.h>
#include <Inventor/nodes/SoShape.h>
#include <Inventor/SbName.h>
#include <Inventor/lists/SoEnabledElementsList.h>

#include <Inventor/elements/SoComplexityElement.h>
#include <Inventor/elements/SoComplexityTypeElement.h>
#include <Inventor/elements/SoCoordinateElement.h>
#include <Inventor/elements/SoDrawStyleElement.h>
#include <Inventor/elements/SoLinePatternElement.h>
#include <Inventor/elements/SoLineWidthElement.h>
#include <Inventor/elements/SoPointSizeElement.h>
#include <Inventor/elements/SoFontNameElement.h>
#include <Inventor/elements/SoFontSizeElement.h>
#include <Inventor/elements/SoLightModelElement.h>
#include <Inventor/elements/SoLightAttenuationElement.h>
#include <Inventor/elements/SoAmbientColorElement.h>
#include <Inventor/elements/SoDiffuseColorElement.h>
#include <Inventor/elements/SoEmissiveColorElement.h>
#include <Inventor/elements/SoSpecularColorElement.h>
#include <Inventor/elements/SoShininessElement.h>
#include <Inventor/elements/SoTransparencyElement.h>
#include <Inventor/elements/SoMaterialBindingElement.h>
#include <Inventor/elements/SoNormalElement.h>
#include <Inventor/elements/SoNormalBindingElement.h>
#include <Inventor/elements/SoProfileCoordinateElement.h>
#include <Inventor/elements/SoProfileElement.h>
#include <Inventor/elements/SoShapeHintsElement.h>
#include <Inventor/elements/SoCreaseAngleElement.h>
#include <Inventor/elements/SoTextureCoordinateElement.h>
#include <Inventor/elements/SoTextureCoordinateBindingElement.h>
#include <Inventor/elements/SoTextureImageElement.h>
#include <Inventor/elements/SoTextureMatrixElement.h>
#include <Inventor/elements/SoModelMatrixElement.h>
#include <Inventor/elements/SoUnitsElement.h>
#include <Inventor/elements/SoFocalDistanceElement.h>
#include <Inventor/elements/SoProjectionMatrixElement.h>
#include <Inventor/elements/SoViewingMatrixElement.h>
#include <Inventor/elements/SoViewVolumeElement.h>
#include <Inventor/elements/SoPickStyleElement.h>
#include <Inventor/elements/SoSwitchElement.h>
#include <Inventor/elements/SoViewportRegionElement.h>
#include <Inventor/elements/SoDecimationTypeElement.h>
#include <Inventor/elements/SoDecimationPercentageElement.h>
#include <Inventor/elements/SoOverrideElement.h>

#include <Inventor/SoPath.h>
#include <Inventor/SbMatrix.h>
#include <Inventor/SbViewVolume.h>
#include <Inventor/SbVec2f.h>
#include <Inventor/SbVec3f.h>
#include <Inventor/SbVec4f.h>
#include <Inventor/SbVec2s.h>
#include <Inventor/misc/SoState.h>
#include <Inventor/lists/SoTypeList.h>


// *************************************************************************

//$ BEGIN TEMPLATE ActionSource(SoCallbackAction)

SoType SoCallbackAction::classTypeId = SoType::badType();

/*!
  Returns the unique type identifier for the classname class.
*/
SoType
SoCallbackAction::getClassTypeId(void)
{
  return classTypeId;
}

/*!
  Returns type identifier for an object.
*/
SoType
SoCallbackAction::getTypeId(void) const
{
  return classTypeId;
}

#include <assert.h>

// static variables
SoEnabledElementsList * SoCallbackAction::enabledElements;
SoActionMethodList * SoCallbackAction::methods;

/*!
  \fn SoCallbackAction::enabledElements
  FIXME: write doc.
*/

/*!
  \fn SoCallbackAction::methods
  FIXME: write doc.
*/

/*!
  This method returns the list of enabled elements for the given action class.
*/
const SoEnabledElementsList &
SoCallbackAction::getEnabledElements(void) const
{
  assert(enabledElements);
  return *enabledElements;
}

/*!
  This method adds a method to be perfomed by the action class on the given
  node type.
*/
void
SoCallbackAction::addMethod(const SoType type, SoActionMethod method)
{
  assert(methods);
  methods->addMethod(type, method);
}

/*!
  This method enables an element in the state stack for the action class.
*/
void
SoCallbackAction::enableElement(const SoType type, const int stackIndex)
{
  assert(enabledElements);
  enabledElements->enable(type, stackIndex);
}
//$ END TEMPLATE ActionSource

// *************************************************************************

/*!
  This static method initializes all the static data for the SoCallbackAction
  class.
*/

void
SoCallbackAction::initClass(void)
{
//$ BEGIN TEMPLATE InitActionSource(SoCallbackAction)
  assert(SoCallbackAction::getClassTypeId() == SoType::badType());
  assert(inherited::getClassTypeId() != SoType::badType());

  SoCallbackAction::classTypeId =
      SoType::createType(inherited::getClassTypeId(),
                         "SoCallbackAction");
  enabledElements = new SoEnabledElementsList(inherited::enabledElements);
  methods = new SoActionMethodList(inherited::methods);
//$ END TEMPLATE InitActionSource

  ENABLE_ELEMENT(SoViewportRegionElement);
  ENABLE_ELEMENT(SoDecimationTypeElement);
  ENABLE_ELEMENT(SoDecimationPercentageElement);
  ENABLE_ELEMENT(SoOverrideElement);
  ENABLE_ELEMENT(SoViewportRegionElement);
}

// *************************************************************************

/*!
  A constructor.
*/

SoCallbackAction::SoCallbackAction(void)
{
  SO_ACTION_CONSTRUCTOR(SoCallbackAction);

  static int first = 1;
  if (first) {
    first = 0;
    SO_ACTION_ADD_METHOD(SoNode, SoNode::callbackS);
  }
  methods->setUp(); // FIXME: not sure if this should be called here...
}

/*!
  The destructor.
*/

SoCallbackAction::~SoCallbackAction(void)
{
}

//
// for setting node callbacks. makes sure NULLs are filled in where not set
//
static void
set_callback_data_idx(SbList <SoCallbackData> &list, const int idx,
                      void *func, void *data)
{
  int n = list.getLength();
  while (n <= idx) {
    list.append(SoCallbackData(NULL, NULL));
    n++;
  }
  list[idx] = SoCallbackData(func, data);
}

void
set_callback_data(SbList <SoCallbackData> &list, const SoType type,
                  void *func, void *data)
{
  SoTypeList derivedtypes;
  int n = SoType::getAllDerivedFrom(type, derivedtypes);
  for (int i = 0; i < n; i++) {
    set_callback_data_idx(list, (int)derivedtypes[i].getData(),
                          func, data);
  }
}

void
SoCallbackAction::addPreCallback(const SoType type,
                                 SoCallbackActionCB *cb,
                                 void *userdata)
{
  set_callback_data(this->preCB, type, (void*)cb, userdata);
}

void
SoCallbackAction::addPostCallback(const SoType type,
                                  SoCallbackActionCB *cb,
                                  void *userdata)
{
  set_callback_data(this->postCB, type, (void*) cb, userdata);
}

void
SoCallbackAction::addPreTailCallback(const SoType type,
                                     SoCallbackActionCB *cb,
                                     void *userdata)
{
  set_callback_data(this->preTailCB, type, (void*) cb, userdata);
}

void
SoCallbackAction::addPostTailCallback(const SoType type,
                                      SoCallbackActionCB *cb,
                                      void *userdata)
{
  set_callback_data(this->postTailCB, type, (void*) cb, userdata);
}

void
SoCallbackAction::addTriangleCallback(const SoType type,
                                      SoTriangleCB *cb,
                                      void *userdata)
{
  set_callback_data(this->triangleCB, type, (void*) cb, userdata);
}

void
SoCallbackAction::addLineSegmentCallback(const SoType type,
                                         SoLineSegmentCB *cb,
                                         void *userdata)
{
  set_callback_data(this->lineSegmentCB, type, (void*) cb, userdata);
}

void
SoCallbackAction::addPointCallback(const SoType type,
                                   SoPointCB *cb,
                                   void *userdata)
{
  set_callback_data(this->pointCB, type, (void*) cb, userdata);
}

/************************************************************************************/

float
SoCallbackAction::getComplexity() const
{
  return SoComplexityElement::get(this->state);
}

SoComplexity::Type
SoCallbackAction::getComplexityType() const
{
  return (SoComplexity::Type) SoComplexityTypeElement::get(this->state);
}

int32_t
SoCallbackAction::getNumCoordinates() const
{
  return SoCoordinateElement::getInstance(this->state)->getNum();
}

const SbVec3f &
SoCallbackAction::getCoordinate3(const int index) const
{
  return SoCoordinateElement::getInstance(this->state)->get3(index);
}

const SbVec4f &
SoCallbackAction::getCoordinate4(const int index) const
{
  return SoCoordinateElement::getInstance(this->state)->get4(index);
}

SoDrawStyle::Style
SoCallbackAction::getDrawStyle() const
{
  return (SoDrawStyle::Style) SoDrawStyleElement::get(this->state);
}

unsigned short
SoCallbackAction::getLinePattern() const
{
  return (unsigned short) SoLinePatternElement::get(this->state);
}

float
SoCallbackAction::getLineWidth() const
{
  return SoLineWidthElement::get(this->state);
}

float
SoCallbackAction::getPointSize() const
{
  return SoPointSizeElement::get(this->state);
}

SbName
SoCallbackAction::getFontName() const
{
  return SoFontNameElement::get(this->state);
}

float
SoCallbackAction::getFontSize() const
{
  return SoFontSizeElement::get(this->state);
}

SoLightModel::Model
SoCallbackAction::getLightModel() const
{
  return (SoLightModel::Model) SoLightModelElement::get(this->state);
}

const SbVec3f &
SoCallbackAction::getLightAttenuation() const
{
  return SoLightAttenuationElement::get(this->state);
}


void
SoCallbackAction::getMaterial(SbColor &ambient, SbColor &diffuse,
                              SbColor &specular, SbColor &emission,
                              float &shininess, float &transparency,
                              const int index) const
{
  ambient = SoAmbientColorElement::getInstance(this->state)->get(index);
  diffuse = SoDiffuseColorElement::getInstance(this->state)->get(index);
  emission = SoEmissiveColorElement::getInstance(this->state)->get(index);
  specular = SoSpecularColorElement::getInstance(this->state)->get(index);
  shininess = SoShininessElement::getInstance(this->state)->get(index);
  transparency = SoTransparencyElement::getInstance(this->state)->get(index);
}

SoMaterialBinding::Binding
SoCallbackAction::getMaterialBinding() const
{
  return (SoMaterialBinding::Binding)
    SoMaterialBindingElement::get(this->state);
}

long
SoCallbackAction::getNumNormals() const
{
  return SoNormalElement::getInstance(this->state)->getNum();
}

const SbVec3f &
SoCallbackAction::getNormal(const int index) const
{
  return SoNormalElement::getInstance(this->state)->get(index);
}

SoNormalBinding::Binding
SoCallbackAction::getNormalBinding() const
{
  return (SoNormalBinding::Binding)
    SoNormalBindingElement::get(this->state);
}

int32_t
SoCallbackAction::getNumProfileCoordinates() const
{
  return SoProfileCoordinateElement::getInstance(this->state)->getNum();
}

const SbVec2f &
SoCallbackAction::getProfileCoordinate2(const int index) const
{
  return SoProfileCoordinateElement::getInstance(this->state)->get2(index);
}

const SbVec3f &
SoCallbackAction::getProfileCoordinate3(const int index) const
{
  return SoProfileCoordinateElement::getInstance(this->state)->get3(index);
}

const SoNodeList &
SoCallbackAction::getProfile() const
{
  return SoProfileElement::get(this->state);
}

SoShapeHints::VertexOrdering
SoCallbackAction::getVertexOrdering() const
{
  return (SoShapeHints::VertexOrdering)
    SoShapeHintsElement::getVertexOrdering(this->state);
}

SoShapeHints::ShapeType
SoCallbackAction::getShapeType() const
{
  return (SoShapeHints::ShapeType)
    SoShapeHintsElement::getShapeType(this->state);
}

SoShapeHints::FaceType
SoCallbackAction::getFaceType() const
{
  return (SoShapeHints::FaceType)
    SoShapeHintsElement::getFaceType(this->state);
}

float
SoCallbackAction::getCreaseAngle() const
{
  return SoCreaseAngleElement::get(this->state);
}

int32_t
SoCallbackAction::getNumTextureCoordinates() const
{
  return SoTextureCoordinateElement::getInstance(this->state)->getNum();
}

const SbVec2f &
SoCallbackAction::getTextureCoordinate2(const int index) const
{
  return SoTextureCoordinateElement::getInstance(this->state)->get2(index);
}

const SbVec4f &
SoCallbackAction::getTextureCoordinate4(int index) const
{
  return SoTextureCoordinateElement::getInstance(this->state)->get4(index);
}

SoTextureCoordinateBinding::Binding
SoCallbackAction::getTextureCoordinateBinding() const
{
  return (SoTextureCoordinateBinding::Binding)
    SoTextureCoordinateBindingElement::get(this->state);
}

const SbColor &
SoCallbackAction::getTextureBlendColor() const
{
  return SoTextureImageElement::getBlendColor(this->state);
}

const unsigned char *
SoCallbackAction::getTextureImage(SbVec2s &size, int &numComps) const
{
  return SoTextureImageElement::getImage(state, size, numComps);
}

const SbMatrix &
SoCallbackAction::getTextureMatrix() const
{
  return SoTextureMatrixElement::get(this->state);
}

SoTexture2::Model
SoCallbackAction::getTextureModel() const
{
  return (SoTexture2::Model) SoTextureImageElement::getModel(this->state);
}

SoTexture2::Wrap
SoCallbackAction::getTextureWrapS() const
{
  return (SoTexture2::Wrap) SoTextureImageElement::getWrapS(this->state);
}

SoTexture2::Wrap
SoCallbackAction::getTextureWrapT() const
{
  return (SoTexture2::Wrap) SoTextureImageElement::getWrapT(this->state);
}

const SbMatrix &
SoCallbackAction::getModelMatrix() const
{
  return SoModelMatrixElement::get(this->state);
}

SoUnits::Units
SoCallbackAction::getUnits() const
{
  return (SoUnits::Units) SoUnitsElement::get(this->state);
}

float
SoCallbackAction::getFocalDistance() const
{
  return SoFocalDistanceElement::get(this->state);
}

const SbMatrix &
SoCallbackAction::getProjectionMatrix() const
{
  return SoProjectionMatrixElement::get(this->state);
}

const SbMatrix &
SoCallbackAction::getViewingMatrix() const
{
  return SoViewingMatrixElement::get(this->state);
}

const SbViewVolume &
SoCallbackAction::getViewVolume() const
{
  return SoViewVolumeElement::get(this->state);
}

SoPickStyle::Style
SoCallbackAction::getPickStyle() const
{
  return (SoPickStyle::Style) SoPickStyleElement::get(this->state);
}

int32_t
SoCallbackAction::getSwitch() const
{
  return SoSwitchElement::get(this->state);
}

/************************************************************************************/

SoCallbackAction::Response
SoCallbackAction::getCurrentResponse(void) const
{
  return this->response;
}

void
SoCallbackAction::invokePreCallbacks(const SoNode *const node)
{
  int idx = (int) node->getTypeId().getData();
  if (idx < this->preCB.getLength() && this->preCB[idx].hasCallback()) {
    this->response = (Response) this->preCB[idx].doNodeCallback(this, node);
    if (this->response == SoCallbackAction::ABORT)
      this->setTerminated(TRUE);
  }
}

void
SoCallbackAction::invokePostCallbacks(const SoNode * const node)
{
  int idx = (int) node->getTypeId().getData();
  if (idx < this->postCB.getLength() && this->postCB[idx].hasCallback()) {
    this->response = (Response) this->postCB[idx].doNodeCallback(this, node);
    if (this->response == SoCallbackAction::ABORT)
      this->setTerminated(TRUE);
  }
}

void
SoCallbackAction::invokeTriangleCallbacks(const SoShape * const shape,
                                          const SoPrimitiveVertex * const v1,
                                          const SoPrimitiveVertex * const v2,
                                          const SoPrimitiveVertex * const v3)
{
  int idx = (int) shape->getTypeId().getData();
  if (idx < this->triangleCB.getLength() && this->triangleCB[idx].hasCallback())
    this->triangleCB[idx].doTriangleCallback(this, v1, v2, v3);
}

void
SoCallbackAction::invokeLineSegmentCallbacks(const SoShape * const shape,
                                             const SoPrimitiveVertex * const v1,
                                             const SoPrimitiveVertex * const v2)
{
  int idx = (int) shape->getTypeId().getData();
  if (idx < this->lineSegmentCB.getLength() && this->lineSegmentCB[idx].hasCallback())
    this->lineSegmentCB[idx].doLineSegmentCallback(this, v1, v2);
}

void
SoCallbackAction::invokePointCallbacks(const SoShape * const shape,
                                       const SoPrimitiveVertex * const v)
{
  int idx = (int) shape->getTypeId().getData();
  if (idx < this->pointCB.getLength() && this->pointCB[idx].hasCallback())
    this->pointCB[idx].doPointCallback(this, v);
}

SbBool
SoCallbackAction::shouldGeneratePrimitives(const SoShape *shape) const
{
  int idx = (int) shape->getTypeId().getData();
  if (idx < this->triangleCB.getLength() && this->triangleCB[idx].hasCallback())
    return TRUE;
  if (idx < this->lineSegmentCB.getLength() && this->lineSegmentCB[idx].hasCallback())
    return TRUE;
  if (idx < this->pointCB.getLength() && this->pointCB[idx].hasCallback())
    return TRUE;
  return FALSE;
}

SoNode *
SoCallbackAction::getCurPathTail()
{
  return this->getCurPath()->getTail();
}

void
SoCallbackAction::setCurrentNode(SoNode * const node)
{
  this->currentNode = node;
}

void
SoCallbackAction::beginTraversal(SoNode *node)
{
  this->response = CONTINUE;
  this->traverse(node);
}

int
SoCallbackData::doNodeCallback(SoCallbackAction *action,
                               const SoNode *node)
{
  assert(this->func != NULL);
  SoCallbackAction::SoCallbackActionCB *cbfunc =
    (SoCallbackAction::SoCallbackActionCB*) this->func;

  return (int) cbfunc(this->data, action, node);
}

void
SoCallbackData::doTriangleCallback(SoCallbackAction *action,
                                   const SoPrimitiveVertex * const v1,
                                   const SoPrimitiveVertex * const v2,
                                   const SoPrimitiveVertex * const v3)
{
  assert(this->func != NULL);
  SoTriangleCB *tricb = (SoTriangleCB*) this->func;
  tricb(this->data, action, v1, v2, v3);
}

void
SoCallbackData::doLineSegmentCallback(SoCallbackAction *action,
                                      const SoPrimitiveVertex * const v1,
                                      const SoPrimitiveVertex * const v2)
{
  assert(this->func != NULL);
  SoLineSegmentCB *linecb = (SoLineSegmentCB*) this->func;
  linecb(this->data, action, v1, v2);
}

void
SoCallbackData::doPointCallback(SoCallbackAction *action,
                                const SoPrimitiveVertex *v)
{
  assert(this->func != NULL);
  SoPointCB *ptcb = (SoPointCB*) this->func;
  ptcb(this->data, action, v);
}

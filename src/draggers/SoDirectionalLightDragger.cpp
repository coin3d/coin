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
  \class SoDirectionalLightDragger SoDirectionalLightDragger.h Inventor/draggers/SoDirectionalLightDragger.h
  \brief The SoDirectionalLightDragger class provides interactive geometry for manipulating a directional light source.
  \ingroup draggers

  This dragger is well suited to use for setting up the fields of a
  SoDirectionalLight node, as it provides geometry for the end-user to
  interact with a directional vector.

  For convenience, this dragger also by default contains interaction
  geometry for placing the dragger itself. (SoDirectionalLight nodes
  don't have a position field, so this was strictly not needed.)

  The Coin library also includes a manipulator class,
  SoDirectionalLightManip, which wraps the functionality provided by
  this class inside the necessary mechanisms for connecting it to
  SoDirectionalLight node instances in a scenegraph.

  \sa SoDirectionalLightManip
*/
// FIXME: a bitmap snapshot of the default dragger appearance would be
// nice. 20011023 mortene.

#include <Inventor/draggers/SoDirectionalLightDragger.h>
#include <Inventor/nodekits/SoSubKitP.h>
#include <Inventor/draggers/SoDragPointDragger.h>
#include <Inventor/draggers/SoRotateSphericalDragger.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoRotation.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/sensors/SoFieldSensor.h>

#include <data/draggerDefaults/directionalLightDragger.h>

/*!
  \var SoSFRotation SoDirectionalLightDragger::rotation

  This field is continuously updated to contain the rotation of the
  current direction vector. The application programmer will typically
  connect this to the rotation field of a SoDirectionalLight node
  (unless using the SoDirectionalLightManip class, where this is taken
  care of automatically).

  It may also of course be connected to any other rotation field
  controlling the direction of scenegraph geometry, it does not have
  to part of a SoDirectionalLight node specifically.
*/
/*!
  \var SoSFVec3f SoDirectionalLightDragger::translation

  Continuously updated to contain the current translation from the
  dragger's local origo position.

  This field is not used by the SoDirectionalLightManip, but may be of
  interest for the application programmer wanting to use the
  SoDirectionalLightDragger outside the context of controlling a
  directional light node.
*/

/*!
  \var SoFieldSensor * SoDirectionalLightDragger::rotFieldSensor
  \COININTERNAL
*/
/*!
  \var SoFieldSensor * SoDirectionalLightDragger::translFieldSensor
  \COININTERNAL
*/

SO_KIT_SOURCE(SoDirectionalLightDragger);


// Doc in superclass.
void
SoDirectionalLightDragger::initClass(void)
{
  SO_KIT_INTERNAL_INIT_CLASS(SoDirectionalLightDragger, SO_FROM_INVENTOR_1);
}

// FIXME: document which parts need to be present in the geometry
// scenegraph, and what role they play in the dragger. 20010913 mortene.
/*!
  \DRAGGER_CONSTRUCTOR

  \NODEKIT_PRE_DIAGRAM

  \verbatim
  CLASS SoDirectionalLightDragger
  -->"this"
        "callbackList"
        "topSeparator"
           "motionMatrix"
  -->      "material"
  -->      "translatorSep"
  -->         "translatorRotInv"
  -->         "translator"
  -->      "rotator"
           "geomSeparator"
  \endverbatim

  \NODEKIT_POST_DIAGRAM


  \NODEKIT_PRE_TABLE

  \verbatim
  CLASS SoDirectionalLightDragger
  PVT   "this",  SoDirectionalLightDragger  --- 
        "callbackList",  SoNodeKitListPart [ SoCallback, SoEventCallback ] 
  PVT   "topSeparator",  SoSeparator  --- 
  PVT   "motionMatrix",  SoMatrixTransform  --- 
        "material",  SoMaterial  --- 
  PVT   "translatorSep",  SoSeparator  --- 
        "translatorRotInv",  SoRotation  --- 
        "translator",  SoDragPointDragger  --- 
        "rotator",  SoRotateSphericalDragger  --- 
  PVT   "geomSeparator",  SoSeparator  --- 
  \endverbatim

  \NODEKIT_POST_TABLE
*/
SoDirectionalLightDragger::SoDirectionalLightDragger(void)
{
  SO_KIT_INTERNAL_CONSTRUCTOR(SoDirectionalLightDragger);

  SO_KIT_ADD_CATALOG_ENTRY(material, SoMaterial, TRUE, topSeparator, translatorSep, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(rotator, SoRotateSphericalDragger, TRUE, topSeparator, geomSeparator, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(translator, SoDragPointDragger, TRUE, translatorSep, "", TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(translatorRotInv, SoRotation, TRUE, translatorSep, translator, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(translatorSep, SoSeparator, TRUE, topSeparator, rotator, FALSE);

  if (SO_KIT_IS_FIRST_INSTANCE()) {
    SoInteractionKit::readDefaultParts("directionalLightDragger.iv",
                                       DIRECTIONALLIGHTDRAGGER_draggergeometry,
                                       sizeof(DIRECTIONALLIGHTDRAGGER_draggergeometry));
  }

  SO_KIT_ADD_FIELD(rotation, (SbRotation(SbVec3f(0.0f, 0.0f, 1.0f), 0.0f)));
  SO_KIT_ADD_FIELD(translation, (0.0f, 0.0f, 0.0f));
  SO_KIT_INIT_INSTANCE();

  SoDragger *pdragger = SO_GET_ANY_PART(this, "translator", SoDragPointDragger);
  assert(pdragger);
  SoDragger *sdragger = SO_GET_ANY_PART(this, "rotator", SoDragPointDragger);
  assert(sdragger);

  this->setPartAsDefault("material", "directionalLightOverallMaterial");

  this->addValueChangedCallback(SoDirectionalLightDragger::valueChangedCB);

  this->rotFieldSensor = new SoFieldSensor(SoDirectionalLightDragger::fieldSensorCB, this);
  this->rotFieldSensor->setPriority(0);
  this->translFieldSensor = new SoFieldSensor(SoDirectionalLightDragger::fieldSensorCB, this);
  this->translFieldSensor->setPriority(0);

  this->setUpConnections(TRUE, TRUE);
}

/*!
  Protected destructor.

  (Dragger classes are derived from SoBase, so they are reference
  counted and automatically destroyed when their reference count goes
  to 0.)
 */
SoDirectionalLightDragger::~SoDirectionalLightDragger()
{
  delete this->translFieldSensor;
  delete this->rotFieldSensor;
}

// doc in superclass
SbBool
SoDirectionalLightDragger::setUpConnections(SbBool onoff, SbBool doitalways)
{
  if (!doitalways && this->connectionsSetUp == onoff) return onoff;

  if (onoff) {
    inherited::setUpConnections(onoff, doitalways);
    SoDragger *rotator = (SoDragger*) this->getAnyPart("rotator", FALSE);
    rotator->setPartAsDefault("rotator", "directionalLightRotatorRotator");
    rotator->setPartAsDefault("rotatorActive",
                              "directionalLightRotatorRotatorActive");
    rotator->setPartAsDefault("feedback",
                              "directionalLightRotatorFeedback");
    rotator->setPartAsDefault("feedbackActive",
                              "directionalLightRotatorFeedbackActive");

    SoDragger *translator = (SoDragger*) this->getAnyPart("translator", FALSE);
    translator->setPartAsDefault("yzTranslator.translator",
                                 "directionalLightTranslatorPlaneTranslator");
    translator->setPartAsDefault("xzTranslator.translator",
                                 "directionalLightTranslatorPlaneTranslator");
    translator->setPartAsDefault("xyTranslator.translator",
                                 "directionalLightTranslatorPlaneTranslator");
    translator->setPartAsDefault("yzTranslator.translatorActive",
                                 "directionalLightTranslatorPlaneTranslatorActive");
    translator->setPartAsDefault("xzTranslator.translatorActive",
                                 "directionalLightTranslatorPlaneTranslatorActive");
    translator->setPartAsDefault("xyTranslator.translatorActive",
                                 "directionalLightTranslatorPlaneTranslatorActive");
    translator->setPartAsDefault("xTranslator.translator",
                                 "directionalLightTranslatorLineTranslator");
    translator->setPartAsDefault("yTranslator.translator",
                                 "directionalLightTranslatorLineTranslator");
    translator->setPartAsDefault("zTranslator.translator",
                                 "directionalLightTranslatorLineTranslator");
    translator->setPartAsDefault("xTranslator.translatorActive",
                                 "directionalLightTranslatorLineTranslatorActive");
    translator->setPartAsDefault("yTranslator.translatorActive",
                                 "directionalLightTranslatorLineTranslatorActive");
    translator->setPartAsDefault("zTranslator.translatorActive",
                                 "directionalLightTranslatorLineTranslatorActive");

    this->registerChildDragger(rotator);
    this->registerChildDragger(translator);

    if (this->translFieldSensor->getAttachedField() != &this->translation)
      this->translFieldSensor->attach(&this->translation);
    if (this->rotFieldSensor->getAttachedField() != &this->rotation)
      this->rotFieldSensor->attach(&this->rotation);
  }
  else {
    SoDragger *translator = (SoDragger*) this->getAnyPart("translator", FALSE);
    this->unregisterChildDragger(translator);
    SoDragger *rotator = (SoDragger*) this->getAnyPart("rotator", FALSE);
    this->unregisterChildDragger(rotator);

    if (this->rotFieldSensor->getAttachedField() != NULL)
      this->rotFieldSensor->detach();
    if (this->translFieldSensor->getAttachedField() != NULL)
      this->translFieldSensor->detach();

    inherited::setUpConnections(onoff, doitalways);
  }
  return !(this->connectionsSetUp = onoff);
}

// doc in superclass
void
SoDirectionalLightDragger::setDefaultOnNonWritingFields(void)
{
  this->translator.setDefault(TRUE);
  this->rotator.setDefault(TRUE);
  this->translatorRotInv.setDefault(TRUE);

  inherited::setDefaultOnNonWritingFields();
}

/*! \COININTERNAL */
void
SoDirectionalLightDragger::fieldSensorCB(void * d, SoSensor *)
{
  SoDirectionalLightDragger *thisp = (SoDirectionalLightDragger*)d;
  SbMatrix matrix = thisp->getMotionMatrix();
  thisp->workFieldsIntoTransform(matrix);
  thisp->setMotionMatrix(matrix);
}

/*! \COININTERNAL */
void
SoDirectionalLightDragger::valueChangedCB(void *, SoDragger * d)
{
  SoDirectionalLightDragger *thisp = (SoDirectionalLightDragger*)d;
  SbMatrix matrix = thisp->getMotionMatrix();
  SbVec3f trans, scale;
  SbRotation rot, scaleOrient;
  matrix.getTransform(trans, rot, scale, scaleOrient);

  thisp->translFieldSensor->detach();
  if (thisp->translation.getValue() != trans)
    thisp->translation = trans;
  thisp->translFieldSensor->attach(&thisp->translation);

  thisp->rotFieldSensor->detach();
  if (thisp->rotation.getValue() != rot)
    thisp->rotation = rot;
  thisp->rotFieldSensor->attach(&thisp->rotation);

  SoRotation *invRot = SO_GET_ANY_PART(thisp, "translatorRotInv", SoRotation);
  invRot->rotation = rot.inverse();
}

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
  \class SoSpotLightDragger SoSpotLightDragger.h Inventor/draggers/SoSpotLightDragger.h
  \brief The SoSpotLightDragger class provides interactive geometry for manipulating a spotlight.
  \ingroup draggers

  This dragger is well suited for setting up the fields of a
  SoSpotLight node, as it provides geometry for the end-user to
  interact with a directional vector for the spotlight, to set up it's
  position and to control the cut-off angle for the "lampshade" around
  the lightsource.

  Note that there is one aspect of SoSpotLight nodes that can not be
  controlled with this dragger: the SoSpotLight::dropOffRate field.

  The Coin library includes a manipulator class, SoSpotLightManip,
  which wraps the functionality provided by this class inside the
  necessary mechanisms for connecting it to SoSpotLight node instances
  in a scenegraph.

  \sa SoSpotLightManip, SoSpotLight
  \sa SoDirectionalLightDragger, SoPointLightDragger
*/

#include <Inventor/draggers/SoSpotLightDragger.h>
#include <Inventor/nodekits/SoSubKitP.h>
#include <Inventor/draggers/SoDragPointDragger.h>
#include <Inventor/draggers/SoRotateSphericalDragger.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoRotation.h>
#include <Inventor/nodes/SoScale.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoSwitch.h>
#include <Inventor/nodes/SoTranslation.h>
#include <Inventor/sensors/SoFieldSensor.h>
#include <Inventor/projectors/SbPlaneProjector.h>
#include <math.h>

#include <data/draggerDefaults/spotLightDragger.h>


/*!
  \var SoSFRotation SoSpotLightDragger::rotation

  This field is continuously updated to contain the rotation of the
  current direction vector. The application programmer will typically
  connect this to the rotation field of a SoSpotLight node (unless
  using the SoSpotLightManip class, where this is taken care of
  automatically).

  It may also of course be connected to any other rotation field
  controlling the direction of scenegraph geometry, it does not have
  to part of a SoSpotLight node specifically.
*/

/*!
  \var SoSFVec3f SoSpotLightDragger::translation

  Continuously updated to contain the current translation from the
  dragger's local origo position.
*/

/*!
  \var SoSFFloat SoSpotLightDragger::angle

  The cut-off angle for the "lampshade" around the lightsource.

  Typically connected to a SoSpotLight::cutOffAngle field.
*/

/*!
  \var SoFieldSensor * SoSpotLightDragger::rotFieldSensor
  \COININTERNAL
*/
/*!
  \var SoFieldSensor * SoSpotLightDragger::translFieldSensor
  \COININTERNAL
*/
/*!
  \var SoFieldSensor * SoSpotLightDragger::angleFieldSensor
  \COININTERNAL
*/
/*!
  \var SbPlaneProjector * SoSpotLightDragger::planeProj
  \COININTERNAL
*/

SO_KIT_SOURCE(SoSpotLightDragger);

// doc in superclass
void
SoSpotLightDragger::initClass(void)
{
  SO_KIT_INTERNAL_INIT_CLASS(SoSpotLightDragger, SO_FROM_INVENTOR_1);
}

// FIXME: document which parts need to be present in the geometry
// scenegraph, and what role they play in the dragger. 20010913 mortene.
/*!
  Default constructor, sets up the dragger nodekit catalog with the
  interaction and feedback geometry.
 */
SoSpotLightDragger::SoSpotLightDragger(void)
{
  SO_KIT_INTERNAL_CONSTRUCTOR(SoSpotLightDragger);

  SO_KIT_ADD_CATALOG_ENTRY(material, SoMaterial, TRUE, topSeparator, translatorSep, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(translatorSep, SoSeparator, TRUE, topSeparator, rotator, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(translatorRotInv, SoRotation, TRUE, translatorSep, translator, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(translator, SoDragPointDragger, TRUE, translatorSep, "", TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(rotator, SoRotateSphericalDragger, TRUE, topSeparator, beamSep, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(beamSep, SoSeparator, TRUE, topSeparator, geomSeparator, FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(beamPlacement, SoTranslation, TRUE, beamSep, beamScale, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(beamScale, SoScale, TRUE, beamSep, beamSwitch, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(beamSwitch, SoSwitch, TRUE, beamSep, "", FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(beam, SoSeparator, TRUE, beamSwitch, beamActive, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(beamActive, SoSeparator, TRUE, beamSwitch, "", TRUE);

  if (SO_KIT_IS_FIRST_INSTANCE()) {
    SoInteractionKit::readDefaultParts("spotLightDragger.iv",
                                       SPOTLIGHTDRAGGER_draggergeometry,
                                       sizeof(SPOTLIGHTDRAGGER_draggergeometry));
  }

  SO_KIT_ADD_FIELD(rotation, (SbRotation(SbVec3f(0.0f, 0.0f, 1.0f), 0.0f)));
  SO_KIT_ADD_FIELD(translation, (0.0f, 0.0f, 0.0f));
  SO_KIT_ADD_FIELD(angle, (1.0f));
  SO_KIT_INIT_INSTANCE();

  SoDragger *pdragger = SO_GET_ANY_PART(this, "translator", SoDragPointDragger);
  assert(pdragger);
  SoDragger *sdragger = SO_GET_ANY_PART(this, "rotator", SoDragPointDragger);
  assert(sdragger);

  this->setPartAsDefault("beam", "spotLightBeam");
  this->setPartAsDefault("beamActive", "spotLightBeamActive");
  this->setPartAsDefault("beamPlacement", "spotLightBeamPlacement");
  this->setPartAsDefault("material", "spotLightOverallMaterial");

  SoSwitch *sw;
  sw = SO_GET_ANY_PART(this, "beamSwitch", SoSwitch);
  SoInteractionKit::setSwitchValue(sw, 0);

  this->setBeamScaleFromAngle(1.0f);

  this->planeProj = new SbPlaneProjector();
  this->addStartCallback(SoSpotLightDragger::startCB);
  this->addMotionCallback(SoSpotLightDragger::motionCB);
  this->addFinishCallback(SoSpotLightDragger::doneCB);
  this->addValueChangedCallback(SoSpotLightDragger::valueChangedCB);

  this->rotFieldSensor = new SoFieldSensor(SoSpotLightDragger::fieldSensorCB, this);
  this->rotFieldSensor->setPriority(0);
  this->translFieldSensor = new SoFieldSensor(SoSpotLightDragger::fieldSensorCB, this);
  this->translFieldSensor->setPriority(0);
  this->angleFieldSensor = new SoFieldSensor(SoSpotLightDragger::fieldSensorCB, this);
  this->angleFieldSensor->setPriority(0);

  this->setUpConnections(TRUE, TRUE);
}

/*!
  Protected destructor.

  (Dragger classes are derived from SoBase, so they are reference
  counted and automatically destroyed when their reference count goes
  to 0.)
 */
SoSpotLightDragger::~SoSpotLightDragger()
{
  delete this->angleFieldSensor;
  delete this->translFieldSensor;
  delete this->rotFieldSensor;
  delete this->planeProj;
}

// Doc in superclass.
SbBool
SoSpotLightDragger::setUpConnections(SbBool onoff, SbBool doitalways)
{
  if (!doitalways && this->connectionsSetUp == onoff) return onoff;

  if (onoff) {
    inherited::setUpConnections(onoff, doitalways);
    SoDragger *rotator = (SoDragger*) this->getAnyPart("rotator", FALSE);
    rotator->setPartAsDefault("rotator", "spotLightRotatorRotator");
    rotator->setPartAsDefault("rotatorActive",
                              "spotLightRotatorRotatorActive");
    rotator->setPartAsDefault("feedback",
                              "spotLightRotatorFeedback");
    rotator->setPartAsDefault("feedbackActive",
                              "spotLightRotatorFeedbackActive");

    SoDragger *translator = (SoDragger*) this->getAnyPart("translator", FALSE);
    translator->setPartAsDefault("yzTranslator.translator",
                                 "spotLightTranslatorPlaneTranslator");
    translator->setPartAsDefault("xzTranslator.translator",
                                 "spotLightTranslatorPlaneTranslator");
    translator->setPartAsDefault("xyTranslator.translator",
                                 "spotLightTranslatorPlaneTranslator");
    translator->setPartAsDefault("yzTranslator.translatorActive",
                                 "spotLightTranslatorPlaneTranslatorActive");
    translator->setPartAsDefault("xzTranslator.translatorActive",
                                 "spotLightTranslatorPlaneTranslatorActive");
    translator->setPartAsDefault("xyTranslator.translatorActive",
                                 "spotLightTranslatorPlaneTranslatorActive");
    translator->setPartAsDefault("xTranslator.translator",
                                 "spotLightTranslatorLineTranslator");
    translator->setPartAsDefault("yTranslator.translator",
                                 "spotLightTranslatorLineTranslator");
    translator->setPartAsDefault("zTranslator.translator",
                                 "spotLightTranslatorLineTranslator");
    translator->setPartAsDefault("xTranslator.translatorActive",
                                 "spotLightTranslatorLineTranslatorActive");
    translator->setPartAsDefault("yTranslator.translatorActive",
                                 "spotLightTranslatorLineTranslatorActive");
    translator->setPartAsDefault("zTranslator.translatorActive",
                                 "spotLightTranslatorLineTranslatorActive");

    this->registerChildDragger(rotator);
    this->registerChildDragger(translator);

    if (this->angleFieldSensor->getAttachedField() != &this->angle)
      this->angleFieldSensor->attach(&this->angle);
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

    if (this->angleFieldSensor->getAttachedField() != NULL)
      this->angleFieldSensor->detach();
    if (this->rotFieldSensor->getAttachedField() != NULL)
      this->rotFieldSensor->detach();
    if (this->translFieldSensor->getAttachedField() != NULL)
      this->translFieldSensor->detach();

    inherited::setUpConnections(onoff, doitalways);
  }
  return !(this->connectionsSetUp = onoff);
}

// Doc in superclass.
void
SoSpotLightDragger::setDefaultOnNonWritingFields(void)
{
  if (!(this->angle.isConnectionEnabled() && this->angle.isConnected()) &&
      this->angle.getValue() == 1.0f) this->angle.setDefault(TRUE);
  
  this->translator.setDefault(TRUE);
  this->rotator.setDefault(TRUE);
  
  this->translatorRotInv.setDefault(TRUE);
  this->beamScale.setDefault(TRUE);

  inherited::setDefaultOnNonWritingFields();
}

/*! \COININTERNAL */
void
SoSpotLightDragger::fieldSensorCB(void * d, SoSensor *)
{
  SoSpotLightDragger *thisp = (SoSpotLightDragger*)d;
  SbMatrix matrix = thisp->getMotionMatrix();
  thisp->workFieldsIntoTransform(matrix);
  thisp->setMotionMatrix(matrix);
}

/*! \COININTERNAL */
void
SoSpotLightDragger::valueChangedCB(void *, SoDragger * d)
{
  SoSpotLightDragger *thisp = (SoSpotLightDragger*)d;
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

/*! \COININTERNAL */
void
SoSpotLightDragger::startCB(void *, SoDragger * d)
{
  SoSpotLightDragger *thisp = (SoSpotLightDragger*)d;
  thisp->dragStart();
}

/*! \COININTERNAL */
void
SoSpotLightDragger::motionCB(void *, SoDragger * d)
{
  SoSpotLightDragger *thisp = (SoSpotLightDragger*)d;
  thisp->drag();
}

/*! \COININTERNAL */
void
SoSpotLightDragger::doneCB(void *, SoDragger * d)
{
  SoSpotLightDragger *thisp = (SoSpotLightDragger*)d;
  thisp->dragFinish();
}

/*! \COININTERNAL
  Called when dragger is selected (picked) by the user.
*/
void
SoSpotLightDragger::dragStart(void)
{
  if (this->getActiveChildDragger()) return;
  SoSwitch *sw;
  sw = SO_GET_ANY_PART(this, "beamSwitch", SoSwitch);
  SoInteractionKit::setSwitchValue(sw, 1);

  SbVec3f hitPt = this->getLocalStartingPoint();
  SbVec3f apex = SO_GET_ANY_PART(this, "beamPlacement", SoTranslation)->translation.getValue();

  this->planeProj->setPlane(SbPlane(apex, apex+SbVec3f(0.0f, 0.0f, -1.0f),
                                    hitPt));
}

/*! \COININTERNAL
  Called when user drags the mouse after picking the dragger.
*/
void
SoSpotLightDragger::drag(void)
{
  if (this->getActiveChildDragger()) return;

  SbVec3f apex = SO_GET_ANY_PART(this, "beamPlacement", SoTranslation)->translation.getValue();
  this->planeProj->setViewVolume(this->getViewVolume());
  this->planeProj->setWorkingSpace(this->getLocalToWorldMatrix());
  SbVec3f projPt = planeProj->project(this->getNormalizedLocaterPosition());

  SbVec3f vec = projPt - apex;
  float dot = SbVec3f(0.0f, 0.0f, -1.0f).dot(vec) / vec.length();
  if (dot < 0.0f) dot = 0.01f; // FIXME: pederb, 20000209
  this->setBeamScaleFromAngle((float)acos(dot));
}

/*! \COININTERNAL
  Called when mouse button is released after picking and interacting
  with the dragger.
*/
void
SoSpotLightDragger::dragFinish(void)
{
  if (this->getActiveChildDragger()) return;
  SoSwitch *sw;
  sw = SO_GET_ANY_PART(this, "beamSwitch", SoSwitch);
  SoInteractionKit::setSwitchValue(sw, 0);
}

/*!
  Scales the geometry representing the "lampshade" around the
  lightsource to represent the given \a beamangle.
 */
void
SoSpotLightDragger::setBeamScaleFromAngle(float beamangle)
{
  SoScale *scale = SO_GET_ANY_PART(this, "beamScale", SoScale);
  SbVec3f scaleFactor;
  scaleFactor[0] = scaleFactor[1] = (float)tan(beamangle);
  scaleFactor[2] = 1.0f;
  scale->scaleFactor = scaleFactor;
}

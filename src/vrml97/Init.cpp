/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 2001 by Systems in Motion. All rights reserved.
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

#include <Inventor/VRMLnodes/SoVRML.h>
#include <Inventor/VRMLnodes/SoVRMLNodes.h>

void
so_vrml_init(void)
{
  SoVRMLGeometry::initClass();
  SoVRMLVertexPoint::initClass();
  SoVRMLVertexShape::initClass();
  SoVRMLIndexedShape::initClass();

  SoVRMLParent::initClass();
  SoVRMLGroup::initClass();

  SoVRMLTexture::initClass();

  SoVRMLInterpolator::initClass();

  SoVRMLLight::initClass();
  
  SoVRMLSensor::initClass();
  SoVRMLDragSensor::initClass();

  SoVRMLAnchor::initClass();
  SoVRMLAppearance::initClass();
  SoVRMLAudioClip::initClass();
  SoVRMLBackground::initClass();
  SoVRMLBillboard::initClass();
  SoVRMLBox::initClass();
  SoVRMLCollision::initClass();
  SoVRMLColor::initClass();
  SoVRMLColorInterpolator::initClass();
  SoVRMLCone::initClass();
  SoVRMLCoordinate::initClass();
  SoVRMLCoordinateInterpolator::initClass();
  SoVRMLCylinder::initClass();
  SoVRMLCylinderSensor::initClass();
  SoVRMLDirectionalLight::initClass();
  SoVRMLElevationGrid::initClass();
  SoVRMLExtrusion::initClass();
  SoVRMLFog::initClass();
  SoVRMLFontStyle::initClass();
  SoVRMLImageTexture::initClass();
  SoVRMLIndexedFaceSet::initClass();

  SoVRMLVertexLine::initClass();
  SoVRMLIndexedLine::initClass();
  SoVRMLIndexedLineSet::initClass();
  SoVRMLInline::initClass();
  SoVRMLLOD::initClass();
  SoVRMLShape::initClass();
  SoVRMLMaterial::initClass();
  SoVRMLMovieTexture::initClass();
  SoVRMLNavigationInfo::initClass();
  SoVRMLNormal::initClass();
  SoVRMLNormalInterpolator::initClass();
  SoVRMLOrientationInterpolator::initClass();
  SoVRMLPixelTexture::initClass();
  SoVRMLPlaneSensor::initClass();
  SoVRMLPointLight::initClass();
  SoVRMLPointSet::initClass();
  SoVRMLPositionInterpolator::initClass();
  SoVRMLProximitySensor::initClass();
  SoVRMLScalarInterpolator::initClass();
  SoVRMLScript::initClass();
  SoVRMLSound::initClass();
  SoVRMLSphere::initClass();
  SoVRMLSphereSensor::initClass();
  SoVRMLSpotLight::initClass();
  SoVRMLSwitch::initClass();
  SoVRMLText::initClass();
  SoVRMLTextureCoordinate::initClass();
  SoVRMLTextureTransform::initClass();
  SoVRMLTimeSensor::initClass();
  SoVRMLTouchSensor::initClass();
  SoVRMLTransform::initClass();
  SoVRMLViewpoint::initClass();
  SoVRMLVisibilitySensor::initClass();
  SoVRMLWorldInfo::initClass();
}

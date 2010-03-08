#ifndef COIN_SONODES_H
#define COIN_SONODES_H

/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) by Kongsberg Oil & Gas Technologies.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Kongsberg Oil & Gas Technologies
 *  about acquiring a Coin Professional Edition License.
 *
 *  See http://www.coin3d.org/ for more information.
 *
 *  Kongsberg Oil & Gas Technologies, Bygdoy Alle 5, 0257 Oslo, NORWAY.
 *  http://www.sim.no/  sales@sim.no  coin-support@coin3d.org
 *
\**************************************************************************/

#include <Inventor/nodes/SoNode.h>
#include <Inventor/nodes/SoCamera.h>
#include <Inventor/nodes/SoPerspectiveCamera.h>
#include <Inventor/nodes/SoOrthographicCamera.h>
#include <Inventor/nodes/SoFrustumCamera.h>
#include <Inventor/nodes/SoShape.h>
#include <Inventor/nodes/SoAsciiText.h>
#include <Inventor/nodes/SoCone.h>
#include <Inventor/nodes/SoCube.h>
#include <Inventor/nodes/SoCylinder.h>
#include <Inventor/nodes/SoVertexShape.h>
#include <Inventor/nodes/SoNonIndexedShape.h>
#include <Inventor/nodes/SoFaceSet.h>
#include <Inventor/nodes/SoLineSet.h>
#include <Inventor/nodes/SoPointSet.h>
#include <Inventor/nodes/SoMarkerSet.h>
#include <Inventor/nodes/SoQuadMesh.h>
#include <Inventor/nodes/SoTriangleStripSet.h>
#include <Inventor/nodes/SoIndexedShape.h>
#include <Inventor/nodes/SoIndexedFaceSet.h>
#include <Inventor/nodes/SoIndexedLineSet.h>
#include <Inventor/nodes/SoIndexedMarkerSet.h>
#include <Inventor/nodes/SoIndexedPointSet.h>
#include <Inventor/nodes/SoIndexedTriangleStripSet.h>
#include <Inventor/nodes/SoImage.h>
#include <Inventor/nodes/SoIndexedNurbsCurve.h>
#include <Inventor/nodes/SoIndexedNurbsSurface.h>
#include <Inventor/nodes/SoNurbsCurve.h>
#include <Inventor/nodes/SoNurbsSurface.h>
#include <Inventor/nodes/SoSphere.h>
#include <Inventor/nodes/SoText2.h>
#include <Inventor/nodes/SoText3.h>
#include <Inventor/nodes/SoGroup.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoAnnotation.h>
#include <Inventor/nodes/SoSelection.h>
#include <Inventor/nodes/SoExtSelection.h>
#include <Inventor/nodes/SoLocateHighlight.h>
#include <Inventor/nodes/SoWWWAnchor.h>
#include <Inventor/nodes/SoArray.h>
#include <Inventor/nodes/SoSwitch.h>
#include <Inventor/nodes/SoBlinker.h>
#include <Inventor/nodes/SoLOD.h>
#include <Inventor/nodes/SoLevelOfDetail.h>
#include <Inventor/nodes/SoMultipleCopy.h>
#include <Inventor/nodes/SoPathSwitch.h>
#include <Inventor/nodes/SoTransformSeparator.h>
#include <Inventor/nodes/SoTransformation.h>
#include <Inventor/nodes/SoAntiSquish.h>
#include <Inventor/nodes/SoMatrixTransform.h>
#include <Inventor/nodes/SoRotation.h>
#include <Inventor/nodes/SoPendulum.h>
#include <Inventor/nodes/SoRotor.h>
#include <Inventor/nodes/SoResetTransform.h>
#include <Inventor/nodes/SoRotationXYZ.h>
#include <Inventor/nodes/SoScale.h>
#include <Inventor/nodes/SoTranslation.h>
#include <Inventor/nodes/SoShuttle.h>
#include <Inventor/nodes/SoSurroundScale.h>
#include <Inventor/nodes/SoTransform.h>
#include <Inventor/nodes/SoUnits.h>
#include <Inventor/nodes/SoBaseColor.h>
#include <Inventor/nodes/SoCallback.h>
#include <Inventor/nodes/SoClipPlane.h>
#include <Inventor/nodes/SoColorIndex.h>
#include <Inventor/nodes/SoComplexity.h>
#include <Inventor/nodes/SoCoordinate3.h>
#include <Inventor/nodes/SoCoordinate4.h>
#include <Inventor/nodes/SoLight.h>
#include <Inventor/nodes/SoDirectionalLight.h>
#include <Inventor/nodes/SoSpotLight.h>
#include <Inventor/nodes/SoPointLight.h>
#include <Inventor/nodes/SoDrawStyle.h>
#include <Inventor/nodes/SoEnvironment.h>
#include <Inventor/nodes/SoEventCallback.h>
#include <Inventor/nodes/SoFile.h>
#include <Inventor/nodes/SoFont.h>
#include <Inventor/nodes/SoFontStyle.h>
#include <Inventor/nodes/SoInfo.h>
#include <Inventor/nodes/SoLabel.h>
#include <Inventor/nodes/SoLightModel.h>
#include <Inventor/nodes/SoProfile.h>
#include <Inventor/nodes/SoLinearProfile.h>
#include <Inventor/nodes/SoNurbsProfile.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoMaterialBinding.h>
#include <Inventor/nodes/SoVertexAttribute.h>
#include <Inventor/nodes/SoVertexAttributeBinding.h>
#include <Inventor/nodes/SoNormal.h>
#include <Inventor/nodes/SoNormalBinding.h>
#include <Inventor/nodes/SoPackedColor.h>
#include <Inventor/nodes/SoPickStyle.h>
#include <Inventor/nodes/SoPolygonOffset.h>
#include <Inventor/nodes/SoProfileCoordinate2.h>
#include <Inventor/nodes/SoProfileCoordinate3.h>
#include <Inventor/nodes/SoShapeHints.h>
#include <Inventor/nodes/SoTexture.h>
#include <Inventor/nodes/SoTexture2.h>
#include <Inventor/nodes/SoTexture3.h>
#include <Inventor/nodes/SoTexture2Transform.h>
#include <Inventor/nodes/SoTexture3Transform.h>
#include <Inventor/nodes/SoTextureCoordinate2.h>
#include <Inventor/nodes/SoTextureCoordinate3.h>
#include <Inventor/nodes/SoTextureCoordinateBinding.h>
#include <Inventor/nodes/SoTextureCoordinateFunction.h>
#include <Inventor/nodes/SoTextureCoordinateDefault.h>
#include <Inventor/nodes/SoTextureCoordinateEnvironment.h>
#include <Inventor/nodes/SoTextureCoordinatePlane.h>
#include <Inventor/nodes/SoTextureMatrixTransform.h>
#include <Inventor/nodes/SoVertexProperty.h>
#include <Inventor/nodes/SoWWWInline.h>
#include <Inventor/nodes/SoTransparencyType.h>
#include <Inventor/nodes/SoListener.h>
#include <Inventor/nodes/SoTextureCoordinateCube.h>
#include <Inventor/nodes/SoTextureCoordinateSphere.h>
#include <Inventor/nodes/SoTextureCoordinateCylinder.h>
#include <Inventor/nodes/SoTextureCubeMap.h>
#include <Inventor/nodes/SoShaderObject.h>
#include <Inventor/nodes/SoShaderParameter.h>
#include <Inventor/nodes/SoShaderProgram.h>
#include <Inventor/nodes/SoFragmentShader.h>
#include <Inventor/nodes/SoVertexShader.h>
#include <Inventor/nodes/SoTextureCoordinateNormalMap.h>
#include <Inventor/nodes/SoTextureCoordinateReflectionMap.h>
#include <Inventor/nodes/SoTextureCoordinateObject.h>
#include <Inventor/nodes/SoTextureScalePolicy.h>
#include <Inventor/nodes/SoTextureUnit.h>
#include <Inventor/nodes/SoTextureCombine.h>
#include <Inventor/nodes/SoBumpMap.h>
#include <Inventor/nodes/SoBumpMapCoordinate.h>
#include <Inventor/nodes/SoBumpMapTransform.h>
#include <Inventor/nodes/SoSceneTexture2.h>
#include <Inventor/nodes/SoSceneTextureCubeMap.h>
#include <Inventor/nodes/SoCacheHint.h>
#include <Inventor/nodes/SoDepthBuffer.h>

#endif // !COIN_SONODES_H

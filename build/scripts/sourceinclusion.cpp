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
 *  http://www.sim.no/ sales@sim.no Voice: +47 73540378 Fax: +47 73943861
 *
\**************************************************************************/

/*
  This is a dead simple utility which'll make sure we only try to
  compile the source files necessary under the current configuration
  settings.
  
  19990522 mortene.
*/

#include <iostream.h>
#include <Inventor/confdep.h>

char *sources[] = {
  "SbDict.cpp",
  "SbName.cpp",
  "SbString.cpp",

  "SbVec2f.cpp",
  "SbVec2s.cpp",
  "SbVec3f.cpp",
  "SbVec4f.cpp",
  "SbBox2f.cpp",
  "SbBox2s.cpp",
  "SbBox3f.cpp",
  "SbXfBox3f.cpp",
  "SbColor.cpp",
  "SbColor4f.cpp",
  "SbCylinder.cpp",
  "SbLine.cpp",
  "SbPlane.cpp",
  "SbRotation.cpp",
  "SbSphere.cpp",
  "SbMatrix.cpp",
  "SbTime.cpp",
  "SbViewVolume.cpp",
  "SbBSPTree.cpp",

  "SoInteraction.cpp",

  "projectors/SbProjector.cpp",
  "projectors/SbSphereProjector.cpp",
  "projectors/SbSphereSheetProjector.cpp",
  "projectors/SbSphereSectionProjector.cpp",
  "projectors/SbSpherePlaneProjector.cpp",
  "projectors/SbPlaneProjector.cpp",
  "projectors/SbLineProjector.cpp",
  "projectors/SbCylinderProjector.cpp",
  "projectors/SbCylinderSheetProjector.cpp",
  "projectors/SbCylinderSectionProjector.cpp",
  "projectors/SbCylinderPlaneProjector.cpp",

  "SoDB.cpp",
  "SoInput.cpp",
  "SoOutput.cpp",
#if !defined(COIN_EXCLUDE_SOPATH)
  "SoPath.cpp",
#endif // !COIN_EXCLUDE_SOPATH
#if !defined(COIN_EXCLUDE_SOFULLPATH)
  "SoFullPath.cpp",
#endif // !COIN_EXCLUDE_SOFULLPATH
  "SoPickedPoint.cpp",
#if 0
  "SoLightPath.cpp",
#endif // 0
  "SoType.cpp",
  "SoImageInterface.cpp",
  "SbViewportRegion.cpp",
  "SoSceneManager.cpp",
  "misc/SoBase.cpp",
#if !defined(COIN_EXCLUDE_SOSTATE)
  "misc/SoState.cpp",
#endif // !COIN_EXCLUDE_SOSTATE
  "misc/SoChildList.cpp",
  "misc/SoNotification.cpp",
#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
  "misc/SoGL.cpp",
#endif // !COIN_EXCLUDE_SOGLRENDERACTION
#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
  "misc/SoGLImage.cpp",
#endif // !COIN_EXCLUDE_SOGLRENDERACTION
  "misc/SoTessellator.cpp",
  "misc/SoNormalGenerator.cpp",

  "events/SoEvent.cpp",
  "events/SoButtonEvent.cpp",
  "events/SoMouseButtonEvent.cpp",
  "events/SoKeyboardEvent.cpp",
  "events/SoSpaceballButtonEvent.cpp",
  "events/SoLocation2Event.cpp",
  "events/SoMotion3Event.cpp",

  "nodekits/SoNodeKit.cpp",

  "SoPrimitiveVertex.cpp",

  "caches/SoCache.cpp",
  "caches/SoNormalCache.cpp",
  "caches/SoBoundingBoxCache.cpp",
#if !defined(COIN_EXCLUDE_SOCONVEXDATACACHE)
  "caches/SoConvexDataCache.cpp",
#endif // !COIN_EXCLUDE_SOCONVEXDATACACHE
  "caches/SoTextureCoordinateCache.cpp",

  "bundles/SoBundle.cpp",
#if !defined(COIN_EXCLUDE_SOMATERIALBUNDLE)
  "bundles/SoMaterialBundle.cpp",
#endif // !COIN_EXCLUDE_SOMATERIALBUNDLE
#if !defined(COIN_EXCLUDE_SOTEXTURECOORDINATEBUNDLE)
  "bundles/SoTextureCoordinateBundle.cpp",
#endif // !COIN_EXCLUDE_SOTEXTURECOORDINATEBUNDLE

#if !defined(COIN_EXCLUDE_SOSENSOR)
  "sensors/SoSensor.cpp",
#endif // !COIN_EXCLUDE_SOSENSOR
#if !defined(COIN_EXCLUDE_SODELAYQUEUESENSOR)
  "sensors/SoDelayQueueSensor.cpp",
#endif // !COIN_EXCLUDE_SODELAYQUEUESENSOR
#if !defined(COIN_EXCLUDE_SODATASENSOR)
  "sensors/SoDataSensor.cpp",
#endif // !COIN_EXCLUDE_SODATASENSOR
#if !defined(COIN_EXCLUDE_SOFIELDSENSOR)
  "sensors/SoFieldSensor.cpp",
#endif // !COIN_EXCLUDE_SOFIELDSENSOR
#if !defined(COIN_EXCLUDE_SONODESENSOR)
  "sensors/SoNodeSensor.cpp",
#endif // !COIN_EXCLUDE_SONODESENSOR
#if !defined(COIN_EXCLUDE_SOPATHSENSOR)
  "sensors/SoPathSensor.cpp",
#endif // !COIN_EXCLUDE_SOPATHSENSOR
#if !defined(COIN_EXCLUDE_SOONESHOTSENSOR)
  "sensors/SoOneShotSensor.cpp",
#endif // !COIN_EXCLUDE_SOONESHOTSENSOR
#if !defined(COIN_EXCLUDE_SOIDLESENSOR)
  "sensors/SoIdleSensor.cpp",
#endif // !COIN_EXCLUDE_SOIDLESENSOR
#if !defined(COIN_EXCLUDE_SOTIMERQUEUESENSOR)
  "sensors/SoTimerQueueSensor.cpp",
#endif // !COIN_EXCLUDE_SOTIMERQUEUESENSOR
#if !defined(COIN_EXCLUDE_SOTIMERSENSOR)
  "sensors/SoTimerSensor.cpp",
#endif // !COIN_EXCLUDE_SOTIMERSENSOR
#if !defined(COIN_EXCLUDE_SOALARMSENSOR)
  "sensors/SoAlarmSensor.cpp",
#endif // !COIN_EXCLUDE_SOALARMSENSOR
#if !defined(COIN_EXCLUDE_SOSENSORMANAGER)
  "sensors/SoSensorManager.cpp",
#endif // !COIN_EXCLUDE_SOSENSORMANAGER

#if !defined(COIN_EXCLUDE_SOELEMENT)
  "elements/SoElement.cpp",
#endif // !COIN_EXCLUDE_SOELEMENT
#if !defined(COIN_EXCLUDE_SOACCUMULATEDELEMENT)
  "elements/SoAccumulatedElement.cpp",
#endif // !COIN_EXCLUDE_SOACCUMULATEDELEMENT
#if !defined(COIN_EXCLUDE_SOCLIPPLANEELEMENT)
  "elements/SoClipPlaneElement.cpp",
#endif // !COIN_EXCLUDE_SOCLIPPLANEELEMENT
#if !defined(COIN_EXCLUDE_SOGLCLIPPLANEELEMENT)
  "elements/SoGLClipPlaneElement.cpp",
#endif // !COIN_EXCLUDE_SOGLCLIPPLANEELEMENT
#if !defined(COIN_EXCLUDE_SOLIGHTELEMENT)
  "elements/SoLightElement.cpp",
#endif // !COIN_EXCLUDE_SOLIGHTELEMENT
#if !defined(COIN_EXCLUDE_SOMODELMATRIXELEMENT)
  "elements/SoModelMatrixElement.cpp",
#endif // !COIN_EXCLUDE_SOMODELMATRIXELEMENT
#if !defined(COIN_EXCLUDE_SOBBOXMODELMATRIXELEMENT)
  "elements/SoBBoxModelMatrixElement.cpp",
#endif // !COIN_EXCLUDE_SOBBOXMODELMATRIXELEMENT
#if !defined(COIN_EXCLUDE_SOGLMODELMATRIXELEMENT)
  "elements/SoGLModelMatrixElement.cpp",
#endif // !COIN_EXCLUDE_SOGLMODELMATRIXELEMENT
#if !defined(COIN_EXCLUDE_SOPROFILEELEMENT)
  "elements/SoProfileElement.cpp",
#endif // !COIN_EXCLUDE_SOPROFILEELEMENT
#if !defined(COIN_EXCLUDE_SOTEXTUREMATRIXELEMENT)
  "elements/SoTextureMatrixElement.cpp",
#endif // !COIN_EXCLUDE_SOTEXTUREMATRIXELEMENT
#if !defined(COIN_EXCLUDE_SOGLTEXTUREMATRIXELEMENT)
  "elements/SoGLTextureMatrixElement.cpp",
#endif // !COIN_EXCLUDE_SOGLTEXTUREMATRIXELEMENT
#if !defined(COIN_EXCLUDE_SOCACHEELEMENT)
  "elements/SoCacheElement.cpp",
#endif // !COIN_EXCLUDE_SOCACHEELEMENT
#if !defined(COIN_EXCLUDE_SOINT32ELEMENT)
  "elements/SoInt32Element.cpp",
#endif // !COIN_EXCLUDE_SOINT32ELEMENT
#if !defined(COIN_EXCLUDE_SOANNOTEXT3CHARORIENTELEMENT)
  "elements/SoAnnoText3CharOrientElement.cpp",
#endif // !COIN_EXCLUDE_SOANNOTEXT3CHARORIENTELEMENT
#if !defined(COIN_EXCLUDE_SOANNOTEXT3FONTSIZEHINTELEMENT)
  "elements/SoAnnoText3FontSizeHintElement.cpp",
#endif // !COIN_EXCLUDE_SOANNOTEXT3FONTSIZEHINTELEMENT
#if !defined(COIN_EXCLUDE_SOANNOTEXT3RENDERPRINTELEMENT)
  "elements/SoAnnoText3RenderPrintElement.cpp",
#endif // !COIN_EXCLUDE_SOANNOTEXT3RENDERPRINTELEMENT
#if !defined(COIN_EXCLUDE_SOCOMPLEXITYTYPEELEMENT)
  "elements/SoComplexityTypeElement.cpp",
#endif // !COIN_EXCLUDE_SOCOMPLEXITYTYPEELEMENT
#if !defined(COIN_EXCLUDE_SODECIMATIONTYPEELEMENT)
  "elements/SoDecimationTypeElement.cpp",
#endif // !COIN_EXCLUDE_SODECIMATIONTYPEELEMENT
#if !defined(COIN_EXCLUDE_SODRAWSTYLEELEMENT)
  "elements/SoDrawStyleElement.cpp",
#endif // !COIN_EXCLUDE_SODRAWSTYLEELEMENT
#if !defined(COIN_EXCLUDE_SOGLDRAWSTYLEELEMENT)
  "elements/SoGLDrawStyleElement.cpp",
#endif // !COIN_EXCLUDE_SOGLDRAWSTYLEELEMENT
#if !defined(COIN_EXCLUDE_SOGLLIGHTIDELEMENT)
  "elements/SoGLLightIdElement.cpp",
#endif // !COIN_EXCLUDE_SOGLLIGHTIDELEMENT
#if !defined(COIN_EXCLUDE_SOGLTEXTUREENABLEDELEMENT)
  "elements/SoGLTextureEnabledElement.cpp",
#endif // !COIN_EXCLUDE_SOGLTEXTUREENABLEDELEMENT
#if !defined(COIN_EXCLUDE_SOLINEPATTERNELEMENT)
  "elements/SoLinePatternElement.cpp",
#endif // !COIN_EXCLUDE_SOLINEPATTERNELEMENT
#if !defined(COIN_EXCLUDE_SOGLLINEPATTERNELEMENT)
  "elements/SoGLLinePatternElement.cpp",
#endif // !COIN_EXCLUDE_SOGLLINEPATTERNELEMENT
#if !defined(COIN_EXCLUDE_SOMATERIALBINDINGELEMENT)
  "elements/SoMaterialBindingElement.cpp",
#endif // !COIN_EXCLUDE_SOMATERIALBINDINGELEMENT
#if !defined(COIN_EXCLUDE_SONORMALBINDINGELEMENT)
  "elements/SoNormalBindingElement.cpp",
#endif // !COIN_EXCLUDE_SONORMALBINDINGELEMENT
#if !defined(COIN_EXCLUDE_SOPICKSTYLEELEMENT)
  "elements/SoPickStyleElement.cpp",
#endif // !COIN_EXCLUDE_SOPICKSTYLEELEMENT
#if !defined(COIN_EXCLUDE_SOSWITCHELEMENT)
  "elements/SoSwitchElement.cpp",
#endif // !COIN_EXCLUDE_SOSWITCHELEMENT
#if !defined(COIN_EXCLUDE_SOTEXTOUTLINEENABLEDELEMENT)
  "elements/SoTextOutlineEnabledElement.cpp",
#endif // !COIN_EXCLUDE_SOTEXTOUTLINEENABLEDELEMENT
#if !defined(COIN_EXCLUDE_SOTEXTURECOORDINATEBINDINGELEMENT)
  "elements/SoTextureCoordinateBindingElement.cpp",
#endif // !COIN_EXCLUDE_SOTEXTURECOORDINATEBINDINGELEMENT
#if !defined(COIN_EXCLUDE_SOUNITSELEMENT)
  "elements/SoUnitsElement.cpp",
#endif // !COIN_EXCLUDE_SOUNITSELEMENT
#if !defined(COIN_EXCLUDE_SOFLOATELEMENT)
  "elements/SoFloatElement.cpp",
#endif // !COIN_EXCLUDE_SOFLOATELEMENT
#if !defined(COIN_EXCLUDE_SOCOMPLEXITYELEMENT)
  "elements/SoComplexityElement.cpp",
#endif // !COIN_EXCLUDE_SOCOMPLEXITYELEMENT
#if !defined(COIN_EXCLUDE_SOCREASEANGLEELEMENT)
  "elements/SoCreaseAngleElement.cpp",
#endif // !COIN_EXCLUDE_SOCREASEANGLEELEMENT
#if !defined(COIN_EXCLUDE_SODECIMATIONPERCENTAGEELEMENT)
  "elements/SoDecimationPercentageElement.cpp",
#endif // !COIN_EXCLUDE_SODECIMATIONPERCENTAGEELEMENT
#if !defined(COIN_EXCLUDE_SOFOCALDISTANCEELEMENT)
  "elements/SoFocalDistanceElement.cpp",
#endif // !COIN_EXCLUDE_SOFOCALDISTANCEELEMENT
#if !defined(COIN_EXCLUDE_SOFONTSIZEELEMENT)
  "elements/SoFontSizeElement.cpp",
#endif // !COIN_EXCLUDE_SOFONTSIZEELEMENT
#if !defined(COIN_EXCLUDE_SOLINEWIDTHELEMENT)
  "elements/SoLineWidthElement.cpp",
#endif // !COIN_EXCLUDE_SOLINEWIDTHELEMENT
#if !defined(COIN_EXCLUDE_SOGLLINEWIDTHELEMENT)
  "elements/SoGLLineWidthElement.cpp",
#endif // !COIN_EXCLUDE_SOGLLINEWIDTHELEMENT
#if !defined(COIN_EXCLUDE_SOPOINTSIZEELEMENT)
  "elements/SoPointSizeElement.cpp",
#endif // !COIN_EXCLUDE_SOPOINTSIZEELEMENT
#if !defined(COIN_EXCLUDE_SOGLPOINTSIZEELEMENT)
  "elements/SoGLPointSizeElement.cpp",
#endif // !COIN_EXCLUDE_SOGLPOINTSIZEELEMENT
#if !defined(COIN_EXCLUDE_SOTEXTUREQUALITYELEMENT)
  "elements/SoTextureQualityElement.cpp",
#endif // !COIN_EXCLUDE_SOTEXTUREQUALITYELEMENT
#if !defined(COIN_EXCLUDE_SOGLRENDERPASSELEMENT)
  "elements/SoGLRenderPassElement.cpp",
#endif // !COIN_EXCLUDE_SOGLRENDERPASSELEMENT
#if !defined(COIN_EXCLUDE_SOGLUPDATEAREAELEMENT)
  "elements/SoGLUpdateAreaElement.cpp",
#endif // !COIN_EXCLUDE_SOGLUPDATEAREAELEMENT
#if !defined(COIN_EXCLUDE_SOLOCALBBOXMATRIXELEMENT)
  "elements/SoLocalBBoxMatrixElement.cpp",
#endif // !COIN_EXCLUDE_SOLOCALBBOXMATRIXELEMENT
#if !defined(COIN_EXCLUDE_SOOVERRIDEELEMENT)
  "elements/SoOverrideElement.cpp",
#endif // !COIN_EXCLUDE_SOOVERRIDEELEMENT
#if !defined(COIN_EXCLUDE_SOPICKRAYELEMENT)
  "elements/SoPickRayElement.cpp",
#endif // !COIN_EXCLUDE_SOPICKRAYELEMENT
#if !defined(COIN_EXCLUDE_SOREPLACEDELEMENT)
  "elements/SoReplacedElement.cpp",
#endif // !COIN_EXCLUDE_SOREPLACEDELEMENT
#if !defined(COIN_EXCLUDE_SOCOORDINATEELEMENT)
  "elements/SoCoordinateElement.cpp",
#endif // !COIN_EXCLUDE_SOCOORDINATEELEMENT
#if !defined(COIN_EXCLUDE_SOGLCOORDINATEELEMENT)
  "elements/SoGLCoordinateElement.cpp",
#endif // !COIN_EXCLUDE_SOGLCOORDINATEELEMENT
#if !defined(COIN_EXCLUDE_SOENVIRONMENTELEMENT)
  "elements/SoEnvironmentElement.cpp",
#endif // !COIN_EXCLUDE_SOENVIRONMENTELEMENT
#if !defined(COIN_EXCLUDE_SOGLENVIRONMENTELEMENT)
  "elements/SoGLEnvironmentElement.cpp",
#endif // !COIN_EXCLUDE_SOGLENVIRONMENTELEMENT
#if !defined(COIN_EXCLUDE_SOFONTNAMEELEMENT)
  "elements/SoFontNameElement.cpp",
#endif // !COIN_EXCLUDE_SOFONTNAMEELEMENT
#if !defined(COIN_EXCLUDE_SOLIGHTATTENUATIONELEMENT)
  "elements/SoLightAttenuationElement.cpp",
#endif // !COIN_EXCLUDE_SOLIGHTATTENUATIONELEMENT
#if !defined(COIN_EXCLUDE_SONORMALELEMENT)
  "elements/SoNormalElement.cpp",
#endif // !COIN_EXCLUDE_SONORMALELEMENT
#if !defined(COIN_EXCLUDE_SOGLNORMALELEMENT)
  "elements/SoGLNormalElement.cpp",
#endif // !COIN_EXCLUDE_SOGLNORMALELEMENT
#if !defined(COIN_EXCLUDE_SOPOLYGONOFFSETELEMENT)
  "elements/SoPolygonOffsetElement.cpp",
#endif // !COIN_EXCLUDE_SOPOLYGONOFFSETELEMENT
#if !defined(COIN_EXCLUDE_SOGLPOLYGONOFFSETELEMENT)
  "elements/SoGLPolygonOffsetElement.cpp",
#endif // !COIN_EXCLUDE_SOGLPOLYGONOFFSETELEMENT
#if !defined(COIN_EXCLUDE_SOPROJECTIONMATRIXELEMENT)
  "elements/SoProjectionMatrixElement.cpp",
#endif // !COIN_EXCLUDE_SOPROJECTIONMATRIXELEMENT
#if !defined(COIN_EXCLUDE_SOGLPROJECTIONMATRIXELEMENT)
  "elements/SoGLProjectionMatrixElement.cpp",
#endif // !COIN_EXCLUDE_SOGLPROJECTIONMATRIXELEMENT
#if !defined(COIN_EXCLUDE_SOPROFILECOORDINATEELEMENT)
  "elements/SoProfileCoordinateElement.cpp",
#endif // !COIN_EXCLUDE_SOPROFILECOORDINATEELEMENT
#if !defined(COIN_EXCLUDE_SOTEXTURECOORDINATEELEMENT)
  "elements/SoTextureCoordinateElement.cpp",
#endif // !COIN_EXCLUDE_SOTEXTURECOORDINATEELEMENT
#if !defined(COIN_EXCLUDE_SOGLTEXTURECOORDINATEELEMENT)
  "elements/SoGLTextureCoordinateElement.cpp",
#endif // !COIN_EXCLUDE_SOGLTEXTURECOORDINATEELEMENT
#if !defined(COIN_EXCLUDE_SOTEXTUREIMAGEELEMENT)
  "elements/SoTextureImageElement.cpp",
#endif // !COIN_EXCLUDE_SOTEXTUREIMAGEELEMENT
#if !defined(COIN_EXCLUDE_SOGLTEXTUREIMAGEELEMENT)
  "elements/SoGLTextureImageElement.cpp",
#endif // !COIN_EXCLUDE_SOGLTEXTUREIMAGEELEMENT
#if !defined(COIN_EXCLUDE_SOVIEWINGMATRIXELEMENT)
  "elements/SoViewingMatrixElement.cpp",
#endif // !COIN_EXCLUDE_SOVIEWINGMATRIXELEMENT
#if !defined(COIN_EXCLUDE_SOGLVIEWINGMATRIXELEMENT)
  "elements/SoGLViewingMatrixElement.cpp",
#endif // !COIN_EXCLUDE_SOGLVIEWINGMATRIXELEMENT
#if !defined(COIN_EXCLUDE_SOVIEWVOLUMEELEMENT)
  "elements/SoViewVolumeElement.cpp",
#endif // !COIN_EXCLUDE_SOVIEWVOLUMEELEMENT
#if !defined(COIN_EXCLUDE_SOSHAPEHINTSELEMENT)
  "elements/SoShapeHintsElement.cpp",
#endif // !COIN_EXCLUDE_SOSHAPEHINTSELEMENT
#if !defined(COIN_EXCLUDE_SOGLSHAPEHINTSELEMENT)
  "elements/SoGLShapeHintsElement.cpp",
#endif // !COIN_EXCLUDE_SOGLSHAPEHINTSELEMENT
#if !defined(COIN_EXCLUDE_SOSHAPESTYLEELEMENT)
  "elements/SoShapeStyleElement.cpp",
#endif // !COIN_EXCLUDE_SOSHAPESTYLEELEMENT
#if !defined(COIN_EXCLUDE_SOVIEWPORTREGIONELEMENT)
  "elements/SoViewportRegionElement.cpp",
#endif // !COIN_EXCLUDE_SOVIEWPORTREGIONELEMENT
#if !defined(COIN_EXCLUDE_SOGLVIEWPORTREGIONELEMENT)
  "elements/SoGLViewportRegionElement.cpp",
#endif // !COIN_EXCLUDE_SOGLVIEWPORTREGIONELEMENT
#if !defined(COIN_EXCLUDE_SOWINDOWELEMENT)
  "elements/SoWindowElement.cpp",
#endif // !COIN_EXCLUDE_SOWINDOWELEMENT
#if !defined(COIN_EXCLUDE_SOLIGHTMODELELEMENT)
  "elements/SoLightModelElement.cpp",
#endif // !COIN_EXCLUDE_SOLIGHTMODELELEMENT
#if !defined(COIN_EXCLUDE_SOLIGHTMODELELEMENT)
  "elements/SoGLLightModelElement.cpp",
#endif // !COIN_EXCLUDE_SOLIGHTMODELELEMENT
#if !defined(COIN_EXCLUDE_SOAMBIENTCOLORELEMENT)
  "elements/SoAmbientColorElement.cpp",
#endif // !COIN_EXCLUDE_SOAMBIENTCOLORELEMENT
#if !defined(COIN_EXCLUDE_SODIFFUSECOLORELEMENT)
  "elements/SoDiffuseColorElement.cpp",
#endif // !COIN_EXCLUDE_SODIFFUSECOLORELEMENT
#if !defined(COIN_EXCLUDE_SOEMISSIVECOLORELEMENT)
  "elements/SoEmissiveColorElement.cpp",
#endif // !COIN_EXCLUDE_SOEMISSIVECOLORELEMENT
#if !defined(COIN_EXCLUDE_SOSPECULARCOLORELEMENT)
  "elements/SoSpecularColorElement.cpp",
#endif // !COIN_EXCLUDE_SOSPECULARCOLORELEMENT
#if !defined(COIN_EXCLUDE_SOSHININESSELEMENT)
  "elements/SoShininessElement.cpp",
#endif // !COIN_EXCLUDE_SOSHININESSELEMENT
#if !defined(COIN_EXCLUDE_SOTRANSPARENCYELEMENT)
  "elements/SoTransparencyElement.cpp",
#endif // !COIN_EXCLUDE_SOTRANSPARENCYELEMENT
#if !defined(COIN_EXCLUDE_SOGLAMBIENTCOLORELEMENT)
  "elements/SoGLAmbientColorElement.cpp",
#endif // !COIN_EXCLUDE_SOGLAMBIENTCOLORELEMENT
#if !defined(COIN_EXCLUDE_SOGLDIFFUSECOLORELEMENT)
  "elements/SoGLDiffuseColorElement.cpp",
#endif // !COIN_EXCLUDE_SOGLDIFFUSECOLORELEMENT
#if !defined(COIN_EXCLUDE_SOGLEMISSIVECOLORELEMENT)
  "elements/SoGLEmissiveColorElement.cpp",
#endif // !COIN_EXCLUDE_SOGLEMISSIVECOLORELEMENT
#if !defined(COIN_EXCLUDE_SOGLSPECULARCOLORELEMENT)
  "elements/SoGLSpecularColorElement.cpp",
#endif // !COIN_EXCLUDE_SOGLSPECULARCOLORELEMENT
#if !defined(COIN_EXCLUDE_SOGLSHININESSELEMENT)
  "elements/SoGLShininessElement.cpp",
#endif // !COIN_EXCLUDE_SOGLSHININESSELEMENT
#if !defined(COIN_EXCLUDE_SOGLPOLYGONSTIPPLEELEMENT)
  "elements/SoGLPolygonStippleElement.cpp",
#endif // !COIN_EXCLUDE_SOGLPOLYGONSTIPPLEELEMENT
#if !defined(COIN_EXCLUDE_SOGLSHADEMODELELEMENT)
  "elements/SoGLShadeModelElement.cpp",
#endif // !COIN_EXCLUDE_SOGLSHADEMODELEELEMENT
#if !defined(COIN_EXCLUDE_SOGLNORMALIZEELELEMENT)
  "elements/SoGLNormalizeElement.cpp",
#endif // !COIN_EXCLUDE_SOGLNORMALIZEELEMENT

  "errors/SoError.cpp",
  "errors/SoDebugError.cpp",
  "errors/SoReadError.cpp",
  "errors/SoMemoryError.cpp",

  "fields/SoFieldData.cpp",
  "fields/SoFieldContainer.cpp",
#if !defined(COIN_EXCLUDE_SOFIELD)
  "fields/SoField.cpp",
#endif // !COIN_EXCLUDE_SOFIELD
#if !defined(COIN_EXCLUDE_SOSFIELD)
  "fields/SoSField.cpp",
#endif // !COIN_EXCLUDE_SOSFIELD
#if !defined(COIN_EXCLUDE_SOSFBOOL)
  "fields/SoSFBool.cpp",
#endif // !COIN_EXCLUDE_SOSFBOOL
#if !defined(COIN_EXCLUDE_SOSFCOLOR)
  "fields/SoSFColor.cpp",
#endif // !COIN_EXCLUDE_SOSFCOLOR
#if !defined(COIN_EXCLUDE_SOSFENGINE)
  "fields/SoSFEngine.cpp",
#endif // !COIN_EXCLUDE_SOSFENGINE
#if !defined(COIN_EXCLUDE_SOSFENUM)
  "fields/SoSFEnum.cpp",
#endif // !COIN_EXCLUDE_SOSFENUM
#if !defined(COIN_EXCLUDE_SOSFBITMASK)
  "fields/SoSFBitMask.cpp",
#endif // !COIN_EXCLUDE_SOSFBITMASK
#if !defined(COIN_EXCLUDE_SOSFFLOAT)
  "fields/SoSFFloat.cpp",
#endif // !COIN_EXCLUDE_SOSFFLOAT
#if !defined(COIN_EXCLUDE_SOSFIMAGE)
  "fields/SoSFImage.cpp",
#endif // !COIN_EXCLUDE_SOSFIMAGE
#if !defined(COIN_EXCLUDE_SOSFINT32)
  "fields/SoSFInt32.cpp",
#endif // !COIN_EXCLUDE_SOSFINT32
#if !defined(COIN_EXCLUDE_SOSFMATRIX)
  "fields/SoSFMatrix.cpp",
#endif // !COIN_EXCLUDE_SOSFMATRIX
#if !defined(COIN_EXCLUDE_SOSFNAME)
  "fields/SoSFName.cpp",
#endif // !COIN_EXCLUDE_SOSFNAME
#if !defined(COIN_EXCLUDE_SOSFNODE)
  "fields/SoSFNode.cpp",
#endif // !COIN_EXCLUDE_SOSFNODE
#if !defined(COIN_EXCLUDE_SOSFPATH)
  "fields/SoSFPath.cpp",
#endif // !COIN_EXCLUDE_SOSFPATH
#if !defined(COIN_EXCLUDE_SOSFPLANE)
  "fields/SoSFPlane.cpp",
#endif // !COIN_EXCLUDE_SOSFPLANE
#if !defined(COIN_EXCLUDE_SOSFROTATION)
  "fields/SoSFRotation.cpp",
#endif // !COIN_EXCLUDE_SOSFROTATION
#if !defined(COIN_EXCLUDE_SOSFSHORT)
  "fields/SoSFShort.cpp",
#endif // !COIN_EXCLUDE_SOSFSHORT
#if !defined(COIN_EXCLUDE_SOSFSTRING)
  "fields/SoSFString.cpp",
#endif // !COIN_EXCLUDE_SOSFSTRING
#if !defined(COIN_EXCLUDE_SOSFTIME)
  "fields/SoSFTime.cpp",
#endif // !COIN_EXCLUDE_SOSFTIME
#if !defined(COIN_EXCLUDE_SOSFTRIGGER)
  "fields/SoSFTrigger.cpp",
#endif // !COIN_EXCLUDE_SOSFTRIGGER
#if !defined(COIN_EXCLUDE_SOSFUINT32)
  "fields/SoSFUInt32.cpp",
#endif // !COIN_EXCLUDE_SOSFUINT32
#if !defined(COIN_EXCLUDE_SOSFUSHORT)
  "fields/SoSFUShort.cpp",
#endif // !COIN_EXCLUDE_SOSFUSHORT
#if !defined(COIN_EXCLUDE_SOSFVEC2F)
  "fields/SoSFVec2f.cpp",
#endif // !COIN_EXCLUDE_SOSFVEC2F
#if !defined(COIN_EXCLUDE_SOSFVEC3F)
  "fields/SoSFVec3f.cpp",
#endif // !COIN_EXCLUDE_SOSFVEC3F
#if !defined(COIN_EXCLUDE_SOSFVEC4F)
  "fields/SoSFVec4f.cpp",
#endif // !COIN_EXCLUDE_SOSFVEC4F
#if !defined(COIN_EXCLUDE_SOMFIELD)
  "fields/SoMField.cpp",
#endif // !COIN_EXCLUDE_SOMFIELD
#if !defined(COIN_EXCLUDE_SOMFBOOL)
  "fields/SoMFBool.cpp",
#endif // !COIN_EXCLUDE_SOMFBOOL
#if !defined(COIN_EXCLUDE_SOMFCOLOR)
  "fields/SoMFColor.cpp",
#endif // !COIN_EXCLUDE_SOMFCOLOR
#if !defined(COIN_EXCLUDE_SOMFENGINE)
  "fields/SoMFEngine.cpp",
#endif // !COIN_EXCLUDE_SOMFENGINE
#if !defined(COIN_EXCLUDE_SOMFENUM)
  "fields/SoMFEnum.cpp",
#endif // !COIN_EXCLUDE_SOMFENUM
#if !defined(COIN_EXCLUDE_SOMFBITMASK)
  "fields/SoMFBitMask.cpp",
#endif // !COIN_EXCLUDE_SOMFBITMASK
#if !defined(COIN_EXCLUDE_SOMFFLOAT)
  "fields/SoMFFloat.cpp",
#endif // !COIN_EXCLUDE_SOMFFLOAT
#if !defined(COIN_EXCLUDE_SOMFINT32)
  "fields/SoMFInt32.cpp",
#endif // !COIN_EXCLUDE_SOMFINT32
#if !defined(COIN_EXCLUDE_SOMFMATRIX)
  "fields/SoMFMatrix.cpp",
#endif // !COIN_EXCLUDE_SOMFMATRIX
#if !defined(COIN_EXCLUDE_SOMFNAME)
  "fields/SoMFName.cpp",
#endif // !COIN_EXCLUDE_SOMFNAME
#if !defined(COIN_EXCLUDE_SOMFNODE)
  "fields/SoMFNode.cpp",
#endif // !COIN_EXCLUDE_SOMFNODE
#if !defined(COIN_EXCLUDE_SOMFPATH)
  "fields/SoMFPath.cpp",
#endif // !COIN_EXCLUDE_SOMFPATH
#if !defined(COIN_EXCLUDE_SOMFPLANE)
  "fields/SoMFPlane.cpp",
#endif // !COIN_EXCLUDE_SOMFPLANE
#if !defined(COIN_EXCLUDE_SOMFROTATION)
  "fields/SoMFRotation.cpp",
#endif // !COIN_EXCLUDE_SOMFROTATION
#if !defined(COIN_EXCLUDE_SOMFSHORT)
  "fields/SoMFShort.cpp",
#endif // !COIN_EXCLUDE_SOMFSHORT
#if !defined(COIN_EXCLUDE_SOMFSTRING)
  "fields/SoMFString.cpp",
#endif // !COIN_EXCLUDE_SOMFSTRING
#if !defined(COIN_EXCLUDE_SOMFUINT32)
  "fields/SoMFUInt32.cpp",
#endif // !COIN_EXCLUDE_SOMFUINT32
#if !defined(COIN_EXCLUDE_SOMFUSHORT)
  "fields/SoMFUShort.cpp",
#endif // !COIN_EXCLUDE_SOMFUSHORT
#if !defined(COIN_EXCLUDE_SOMFVEC2F)
  "fields/SoMFVec2f.cpp",
#endif // !COIN_EXCLUDE_SOMFVEC2F
#if !defined(COIN_EXCLUDE_SOMFVEC3F)
  "fields/SoMFVec3f.cpp",
#endif // !COIN_EXCLUDE_SOMFVEC3F
#if !defined(COIN_EXCLUDE_SOMFVEC4F)
  "fields/SoMFVec4f.cpp",
#endif // !COIN_EXCLUDE_SOMFVEC4F
#if !defined(COIN_EXCLUDE_SOMFTIME)
  "fields/SoMFTime.cpp",
#endif // !COIN_EXCLUDE_SOMFTIME

#if !defined(COIN_EXCLUDE_SOACTION)
  "actions/SoAction.cpp",
#endif // !COIN_EXCLUDE_SOACTION
#if !defined(COIN_EXCLUDE_SOCALLBACKACTION)
  "actions/SoCallbackAction.cpp",
#endif // !COIN_EXCLUDE_SOCALLBACKACTION
#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
  "actions/SoGLRenderAction.cpp",
#endif // !COIN_EXCLUDE_SOGLRENDERACTION
#if !defined(COIN_EXCLUDE_SOBOXHIGHLIGHTRENDERACTION)
  "actions/SoBoxHighlightRenderAction.cpp",
#endif // !COIN_EXCLUDE_SOBOXHIGHLIGHTRENDERACTION
#if !defined(COIN_EXCLUDE_SOLINEHIGHLIGHTRENDERACTION)
  "actions/SoLineHighlightRenderAction.cpp",
#endif // !COIN_EXCLUDE_SOLINEHIGHLIGHTRENDERACTION
#if !defined(COIN_EXCLUDE_SOGETBOUNDINGBOXACTION)
  "actions/SoGetBoundingBoxAction.cpp",
#endif // !COIN_EXCLUDE_SOGETBOUNDINGBOXACTION
#if !defined(COIN_EXCLUDE_SOGETMATRIXACTION)
  "actions/SoGetMatrixAction.cpp",
#endif // !COIN_EXCLUDE_SOGETMATRIXACTION
#if !defined(COIN_EXCLUDE_SOGETPRIMITIVECOUNTACTION)
  "actions/SoGetPrimitiveCountAction.cpp",
#endif // !COIN_EXCLUDE_SOGETPRIMITIVECOUNTACTION
#if !defined(COIN_EXCLUDE_SOHANDLEEVENTACTION)
  "actions/SoHandleEventAction.cpp",
#endif // !COIN_EXCLUDE_SOHANDLEEVENTACTION
#if !defined(COIN_EXCLUDE_SOPICKACTION)
  "actions/SoPickAction.cpp",
#endif // !COIN_EXCLUDE_SOPICKACTION
#if !defined(COIN_EXCLUDE_SORAYPICKACTION)
  "actions/SoRayPickAction.cpp",
#endif // !COIN_EXCLUDE_SORAYPICKACTION
#if !defined(COIN_EXCLUDE_SOSEARCHACTION)
  "actions/SoSearchAction.cpp",
#endif // !COIN_EXCLUDE_SOSEARCHACTION
#if !defined(COIN_EXCLUDE_SOSIMPLIFYACTION)
  "actions/SoSimplifyAction.cpp",
#endif // !COIN_EXCLUDE_SOSIMPLIFYACTION
#if !defined(COIN_EXCLUDE_SOGLOBALSIMPLIFYACTION)
  "actions/SoGlobalSimplifyAction.cpp",
#endif // !COIN_EXCLUDE_SOGLOBALSIMPLIFYACTION
#if !defined(COIN_EXCLUDE_SOREORGANIZEACTION)
  "actions/SoReorganizeAction.cpp",
#endif // !COIN_EXCLUDE_SOREORGANIZEACTION
#if !defined(COIN_EXCLUDE_SOSHAPESIMPLIFYACTION)
  "actions/SoShapeSimplifyAction.cpp",
#endif // !COIN_EXCLUDE_SOSHAPESIMPLIFYACTION
#if !defined(COIN_EXCLUDE_SOTOVRMLACTION)
  "actions/SoToVRMLAction.cpp",
#endif // !COIN_EXCLUDE_SOTOVRMLACTION
#if !defined(COIN_EXCLUDE_SOTOVRML2ACTION)
  "actions/SoToVRML2Action.cpp",
#endif // !COIN_EXCLUDE_SOTOVRML2ACTION
#if !defined(COIN_EXCLUDE_SOWRITEACTION)
  "actions/SoWriteAction.cpp",
#endif // !COIN_EXCLUDE_SOWRITEACTION

  "lists/SbPList.cpp",
  "lists/SbStringList.cpp",
  "lists/SbIntList.cpp",
  "lists/SoBaseList.cpp",
  "lists/SoDetailList.cpp",
#if !defined(COIN_EXCLUDE_SOENGINE)
  "lists/SoEngineList.cpp",
#endif // !COIN_EXCLUDE_SOENGINE
#if !defined(COIN_EXCLUDE_SOENGINE)
  "lists/SoEngineOutputList.cpp",
#endif // !COIN_EXCLUDE_SOENGINE
  "lists/SoVRMLInterpOutputList.cpp",
  "lists/SoFieldList.cpp",
  "lists/SoNodeList.cpp",
#if !defined(COIN_EXCLUDE_SOPATH)
  "lists/SoPathList.cpp",
#endif // !COIN_EXCLUDE_SOPATH
  "lists/SoPickedPointList.cpp",
  "lists/SoTypeList.cpp",
  "lists/SoCallbackList.cpp",

  "lists/SoEnabledElementsList.cpp",
  "lists/SoActionMethodList.cpp",
  "lists/SoAuditorList.cpp",

#if !defined(COIN_EXCLUDE_SONODE)
  "nodes/SoNode.cpp",
#endif // !COIN_EXCLUDE_SONODE
#if !defined(COIN_EXCLUDE_SOCAMERA)
  "nodes/SoCamera.cpp",
#endif // !COIN_EXCLUDE_SOCAMERA
#if !defined(COIN_EXCLUDE_SOPERSPECTIVECAMERA)
  "nodes/SoPerspectiveCamera.cpp",
#endif // !COIN_EXCLUDE_SOPERSPECTIVECAMERA
#if !defined(COIN_EXCLUDE_SOORTHOGRAPHICCAMERA)
  "nodes/SoOrthographicCamera.cpp",
#endif // !COIN_EXCLUDE_SOORTHOGRAPHICCAMERA
#if !defined(COIN_EXCLUDE_SOSHAPE)
  "nodes/SoShape.cpp",
#endif // !COIN_EXCLUDE_SOSHAPE
#if !defined(COIN_EXCLUDE_SOANNOTEXT3)
  "nodes/SoAnnoText3.cpp",
#endif // !COIN_EXCLUDE_SOANNOTEXT3
#if !defined(COIN_EXCLUDE_SOASCIITEXT)
  "nodes/SoAsciiText.cpp",
#endif // !COIN_EXCLUDE_SOASCIITEXT
#if !defined(COIN_EXCLUDE_SOCONE)
  "nodes/SoCone.cpp",
#endif // !COIN_EXCLUDE_SOCONE
#if !defined(COIN_EXCLUDE_SOCUBE)
  "nodes/SoCube.cpp",
#endif // !COIN_EXCLUDE_SOCUBE
#if !defined(COIN_EXCLUDE_SOCYLINDER)
  "nodes/SoCylinder.cpp",
#endif // !COIN_EXCLUDE_SOCYLINDER
#if !defined(COIN_EXCLUDE_SOVERTEXSHAPE)
  "nodes/SoVertexShape.cpp",
#endif // !COIN_EXCLUDE_SOVERTEXSHAPE
#if !defined(COIN_EXCLUDE_SONONINDEXEDSHAPE)
  "nodes/SoNonIndexedShape.cpp",
#endif // !COIN_EXCLUDE_SONONINDEXEDSHAPE
#if !defined(COIN_EXCLUDE_SOFACESET)
  "nodes/SoFaceSet.cpp",
#endif // !COIN_EXCLUDE_SOFACESET
#if !defined(COIN_EXCLUDE_SOLINESET)
  "nodes/SoLineSet.cpp",
#endif // !COIN_EXCLUDE_SOLINESET
#if !defined(COIN_EXCLUDE_SOPOINTSET)
  "nodes/SoPointSet.cpp",
#endif // !COIN_EXCLUDE_SOPOINTSET
#if !defined(COIN_EXCLUDE_SOMARKERSET)
  "nodes/SoMarkerSet.cpp",
#endif // !COIN_EXCLUDE_SOMARKERSET
#if !defined(COIN_EXCLUDE_SOQUADMESH)
  "nodes/SoQuadMesh.cpp",
#endif // !COIN_EXCLUDE_SOQUADMESH
#if !defined(COIN_EXCLUDE_SOTRIANGLESTRIPSET)
  "nodes/SoTriangleStripSet.cpp",
#endif // !COIN_EXCLUDE_SOTRIANGLESTRIPSET
#if !defined(COIN_EXCLUDE_SOINDEXEDSHAPE)
  "nodes/SoIndexedShape.cpp",
#endif // !COIN_EXCLUDE_SOINDEXEDSHAPE
#if !defined(COIN_EXCLUDE_SOINDEXEDFACESET)
  "nodes/SoIndexedFaceSet.cpp",
#endif // !COIN_EXCLUDE_SOINDEXEDFACESET
#if !defined(COIN_EXCLUDE_SOINDEXEDLINESET)
  "nodes/SoIndexedLineSet.cpp",
#endif // !COIN_EXCLUDE_SOINDEXEDLINESET
#if !defined(COIN_EXCLUDE_SOINDEXEDTRIANGLESTRIPSET)
  "nodes/SoIndexedTriangleStripSet.cpp",
#endif // !COIN_EXCLUDE_SOINDEXEDTRIANGLESTRIPSET
#if !defined(COIN_EXCLUDE_SOIMAGE)
  "nodes/SoImage.cpp",
#endif // !COIN_EXCLUDE_SOIMAGE
#if !defined(COIN_EXCLUDE_SOINDEXEDNURBSCURVE)
  "nodes/SoIndexedNurbsCurve.cpp",
#endif // !COIN_EXCLUDE_SOINDEXEDNURBSCURVE
#if !defined(COIN_EXCLUDE_SOINDEXEDNURBSSURFACE)
  "nodes/SoIndexedNurbsSurface.cpp",
#endif // !COIN_EXCLUDE_SOINDEXEDNURBSSURFACE
#if !defined(COIN_EXCLUDE_SONURBSCURVE)
  "nodes/SoNurbsCurve.cpp",
#endif // !COIN_EXCLUDE_SONURBSCURVE
#if !defined(COIN_EXCLUDE_SONURBSSURFACE)
  "nodes/SoNurbsSurface.cpp",
#endif // !COIN_EXCLUDE_SONURBSSURFACE
#if !defined(COIN_EXCLUDE_SOSPHERE)
  "nodes/SoSphere.cpp",
#endif // !COIN_EXCLUDE_SOSPHERE
#if !defined(COIN_EXCLUDE_SOTEXT2)
  "nodes/SoText2.cpp",
#endif // !COIN_EXCLUDE_SOTEXT2
#if !defined(COIN_EXCLUDE_SOTEXT3)
  "nodes/SoText3.cpp",
#endif // !COIN_EXCLUDE_SOTEXT3
#if !defined(COIN_EXCLUDE_SOANNOTEXT3PROPERTY)
  "nodes/SoAnnoText3Property.cpp",
#endif // !COIN_EXCLUDE_SOANNOTEXT3PROPERTY
#if !defined(COIN_EXCLUDE_SOGROUP)
  "nodes/SoGroup.cpp",
#endif // !COIN_EXCLUDE_SOGROUP
#if !defined(COIN_EXCLUDE_SOSEPARATOR)
  "nodes/SoSeparator.cpp",
#endif // !COIN_EXCLUDE_SOSEPARATOR
#if !defined(COIN_EXCLUDE_SOANNOTATION)
  "nodes/SoAnnotation.cpp",
#endif // !COIN_EXCLUDE_SOANNOTATION
#if !defined(COIN_EXCLUDE_SOSELECTION)
  "nodes/SoSelection.cpp",
#endif // !COIN_EXCLUDE_SOSELECTION
#if !defined(COIN_EXCLUDE_SOEXTSELECTION)
  "nodes/SoExtSelection.cpp",
#endif // !COIN_EXCLUDE_SOEXTSELECTION
#if !defined(COIN_EXCLUDE_SOLOCATEHIGHLIGHT)
  "nodes/SoLocateHighlight.cpp",
#endif // !COIN_EXCLUDE_SOLOCATEHIGHLIGHT
#if !defined(COIN_EXCLUDE_SOWWWANCHOR)
  "nodes/SoWWWAnchor.cpp",
#endif // !COIN_EXCLUDE_SOWWWANCHOR
#if !defined(COIN_EXCLUDE_SOARRAY)
  "nodes/SoArray.cpp",
#endif // !COIN_EXCLUDE_SOARRAY
#if !defined(COIN_EXCLUDE_SOSWITCH)
  "nodes/SoSwitch.cpp",
#endif // !COIN_EXCLUDE_SOSWITCH
#if !defined(COIN_EXCLUDE_SOBLINKER)
  "nodes/SoBlinker.cpp",
#endif // !COIN_EXCLUDE_SOBLINKER
#if !defined(COIN_EXCLUDE_SOLOD)
  "nodes/SoLOD.cpp",
#endif // !COIN_EXCLUDE_SOLOD
#if !defined(COIN_EXCLUDE_SOLEVELOFSIMPLIFICATION)
  "nodes/SoLevelOfSimplification.cpp",
#endif // !COIN_EXCLUDE_SOLEVELOFSIMPLIFICATION
#if !defined(COIN_EXCLUDE_SOLEVELOFDETAIL)
  "nodes/SoLevelOfDetail.cpp",
#endif // !COIN_EXCLUDE_SOLEVELOFDETAIL
#if !defined(COIN_EXCLUDE_SOMULTIPLECOPY)
  "nodes/SoMultipleCopy.cpp",
#endif // !COIN_EXCLUDE_SOMULTIPLECOPY
#if !defined(COIN_EXCLUDE_SOPATHSWITCH)
  "nodes/SoPathSwitch.cpp",
#endif // !COIN_EXCLUDE_SOPATHSWITCH
#if !defined(COIN_EXCLUDE_SOTRANSFORMSEPARATOR)
  "nodes/SoTransformSeparator.cpp",
#endif // !COIN_EXCLUDE_SOTRANSFORMSEPARATOR
#if !defined(COIN_EXCLUDE_SOTRANSFORMATION)
  "nodes/SoTransformation.cpp",
#endif // !COIN_EXCLUDE_SOTRANSFORMATION
#if !defined(COIN_EXCLUDE_SOANTISQUISH)
  "nodes/SoAntiSquish.cpp",
#endif // !COIN_EXCLUDE_SOANTISQUISH
#if !defined(COIN_EXCLUDE_SOMATRIXTRANSFORM)
  "nodes/SoMatrixTransform.cpp",
#endif // !COIN_EXCLUDE_SOMATRIXTRANSFORM
#if !defined(COIN_EXCLUDE_SOROTATION)
  "nodes/SoRotation.cpp",
#endif // !COIN_EXCLUDE_SOROTATION
#if !defined(COIN_EXCLUDE_SOPENDULUM)
  "nodes/SoPendulum.cpp",
#endif // !COIN_EXCLUDE_SOPENDULUM
#if !defined(COIN_EXCLUDE_SOROTOR)
  "nodes/SoRotor.cpp",
#endif // !COIN_EXCLUDE_SOROTOR
#if !defined(COIN_EXCLUDE_SORESETTRANSFORM)
  "nodes/SoResetTransform.cpp",
#endif // !COIN_EXCLUDE_SORESETTRANSFORM
#if !defined(COIN_EXCLUDE_SOROTATIONXYZ)
  "nodes/SoRotationXYZ.cpp",
#endif // !COIN_EXCLUDE_SOROTATIONXYZ
#if !defined(COIN_EXCLUDE_SOSCALE)
  "nodes/SoScale.cpp",
#endif // !COIN_EXCLUDE_SOSCALE
#if !defined(COIN_EXCLUDE_SOTRANSLATION)
  "nodes/SoTranslation.cpp",
#endif // !COIN_EXCLUDE_SOTRANSLATION
#if !defined(COIN_EXCLUDE_SOSHUTTLE)
  "nodes/SoShuttle.cpp",
#endif // !COIN_EXCLUDE_SOSHUTTLE
#if !defined(COIN_EXCLUDE_SOSURROUNDSCALE)
  "nodes/SoSurroundScale.cpp",
#endif // !COIN_EXCLUDE_SOSURROUNDSCALE
#if !defined(COIN_EXCLUDE_SOTRANSFORM)
  "nodes/SoTransform.cpp",
#endif // !COIN_EXCLUDE_SOTRANSFORM
#if !defined(COIN_EXCLUDE_SOUNITS)
  "nodes/SoUnits.cpp",
#endif // !COIN_EXCLUDE_SOUNITS
#if !defined(COIN_EXCLUDE_SOBASECOLOR)
  "nodes/SoBaseColor.cpp",
#endif // !COIN_EXCLUDE_SOBASECOLOR
#if !defined(COIN_EXCLUDE_SOCALLBACK)
  "nodes/SoCallback.cpp",
#endif // !COIN_EXCLUDE_SOCALLBACK
#if !defined(COIN_EXCLUDE_SOCLIPPLANE)
  "nodes/SoClipPlane.cpp",
#endif // !COIN_EXCLUDE_SOCLIPPLANE
#if !defined(COIN_EXCLUDE_SOCOLORINDEX)
  "nodes/SoColorIndex.cpp",
#endif // !COIN_EXCLUDE_SOCOLORINDEX
#if !defined(COIN_EXCLUDE_SOCOMPLEXITY)
  "nodes/SoComplexity.cpp",
#endif // !COIN_EXCLUDE_SOCOMPLEXITY
#if !defined(COIN_EXCLUDE_SOCOORDINATE3)
  "nodes/SoCoordinate3.cpp",
#endif // !COIN_EXCLUDE_SOCOORDINATE3
#if !defined(COIN_EXCLUDE_SOCOORDINATE4)
  "nodes/SoCoordinate4.cpp",
#endif // !COIN_EXCLUDE_SOCOORDINATE4
#if !defined(COIN_EXCLUDE_SOLIGHT)
  "nodes/SoLight.cpp",
#endif // !COIN_EXCLUDE_SOLIGHT
#if !defined(COIN_EXCLUDE_SODIRECTIONALLIGHT)
  "nodes/SoDirectionalLight.cpp",
#endif // !COIN_EXCLUDE_SODIRECTIONALLIGHT
#if !defined(COIN_EXCLUDE_SOSPOTLIGHT)
  "nodes/SoSpotLight.cpp",
#endif // !COIN_EXCLUDE_SOSPOTLIGHT
#if !defined(COIN_EXCLUDE_SOPOINTLIGHT)
  "nodes/SoPointLight.cpp",
#endif // !COIN_EXCLUDE_SOPOINTLIGHT
#if !defined(COIN_EXCLUDE_SODRAWSTYLE)
  "nodes/SoDrawStyle.cpp",
#endif // !COIN_EXCLUDE_SODRAWSTYLE
#if !defined(COIN_EXCLUDE_SOENVIRONMENT)
  "nodes/SoEnvironment.cpp",
#endif // !COIN_EXCLUDE_SOENVIRONMENT
#if !defined(COIN_EXCLUDE_SOEVENTCALLBACK)
  "nodes/SoEventCallback.cpp",
#endif // !COIN_EXCLUDE_SOEVENTCALLBACK
#if !defined(COIN_EXCLUDE_SOFILE)
  "nodes/SoFile.cpp",
#endif // !COIN_EXCLUDE_SOFILE
#if !defined(COIN_EXCLUDE_SOFONT)
  "nodes/SoFont.cpp",
#endif // !COIN_EXCLUDE_SOFONT
#if !defined(COIN_EXCLUDE_SOFONTSTYLE)
  "nodes/SoFontStyle.cpp",
#endif // !COIN_EXCLUDE_SOFONTSTYLE
#if !defined(COIN_EXCLUDE_SOINFO)
  "nodes/SoInfo.cpp",
#endif // !COIN_EXCLUDE_SOINFO
#if !defined(COIN_EXCLUDE_SOLABEL)
  "nodes/SoLabel.cpp",
#endif // !COIN_EXCLUDE_SOLABEL
#if !defined(COIN_EXCLUDE_SOLIGHTMODEL)
  "nodes/SoLightModel.cpp",
#endif // !COIN_EXCLUDE_SOLIGHTMODEL
#if !defined(COIN_EXCLUDE_SOPROFILE)
  "nodes/SoProfile.cpp",
#endif // !COIN_EXCLUDE_SOPROFILE
#if !defined(COIN_EXCLUDE_SOLINEARPROFILE)
  "nodes/SoLinearProfile.cpp",
#endif // !COIN_EXCLUDE_SOLINEARPROFILE
#if !defined(COIN_EXCLUDE_SONURBSPROFILE)
  "nodes/SoNurbsProfile.cpp",
#endif // !COIN_EXCLUDE_SONURBSPROFILE
#if !defined(COIN_EXCLUDE_SOMATERIAL)
  "nodes/SoMaterial.cpp",
#endif // !COIN_EXCLUDE_SOMATERIAL
#if !defined(COIN_EXCLUDE_SOMATERIALBINDING)
  "nodes/SoMaterialBinding.cpp",
#endif // !COIN_EXCLUDE_SOMATERIALBINDING
#if !defined(COIN_EXCLUDE_SONORMAL)
  "nodes/SoNormal.cpp",
#endif // !COIN_EXCLUDE_SONORMAL
#if !defined(COIN_EXCLUDE_SONORMALBINDING)
  "nodes/SoNormalBinding.cpp",
#endif // !COIN_EXCLUDE_SONORMALBINDING
#if !defined(COIN_EXCLUDE_SOPACKEDCOLOR)
  "nodes/SoPackedColor.cpp",
#endif // !COIN_EXCLUDE_SOPACKEDCOLOR
#if !defined(COIN_EXCLUDE_SOPICKSTYLE)
  "nodes/SoPickStyle.cpp",
#endif // !COIN_EXCLUDE_SOPICKSTYLE
#if !defined(COIN_EXCLUDE_SOPOLYGONOFFSET)
  "nodes/SoPolygonOffset.cpp",
#endif // !COIN_EXCLUDE_SOPOLYGONOFFSET
#if !defined(COIN_EXCLUDE_SOPROFILECOORDINATE2)
  "nodes/SoProfileCoordinate2.cpp",
#endif // !COIN_EXCLUDE_SOPROFILECOORDINATE2
#if !defined(COIN_EXCLUDE_SOPROFILECOORDINATE3)
  "nodes/SoProfileCoordinate3.cpp",
#endif // !COIN_EXCLUDE_SOPROFILECOORDINATE3
#if !defined(COIN_EXCLUDE_SOSHAPEHINTS)
  "nodes/SoShapeHints.cpp",
#endif // !COIN_EXCLUDE_SOSHAPEHINTS
#if !defined(COIN_EXCLUDE_SOTEXTURE2)
  "nodes/SoTexture2.cpp",
#endif // !COIN_EXCLUDE_SOTEXTURE2
#if !defined(COIN_EXCLUDE_SOTEXTURE2TRANSFORM)
  "nodes/SoTexture2Transform.cpp",
#endif // !COIN_EXCLUDE_SOTEXTURE2TRANSFORM
#if !defined(COIN_EXCLUDE_SOTEXTURECOORDINATE2)
  "nodes/SoTextureCoordinate2.cpp",
#endif // !COIN_EXCLUDE_SOTEXTURECOORDINATE2
#if !defined(COIN_EXCLUDE_SOTEXTURECOORDINATEBINDING)
  "nodes/SoTextureCoordinateBinding.cpp",
#endif // !COIN_EXCLUDE_SOTEXTURECOORDINATEBINDING
#if !defined(COIN_EXCLUDE_SOTEXTURECOORDINATEFUNCTION)
  "nodes/SoTextureCoordinateFunction.cpp",
#endif // !COIN_EXCLUDE_SOTEXTURECOORDINATEFUNCTION
#if !defined(COIN_EXCLUDE_SOTEXTURECOORDINATEDEFAULT)
  "nodes/SoTextureCoordinateDefault.cpp",
#endif // !COIN_EXCLUDE_SOTEXTURECOORDINATEDEFAULT
#if !defined(COIN_EXCLUDE_SOTEXTURECOORDINATEENVIRONMENT)
  "nodes/SoTextureCoordinateEnvironment.cpp",
#endif // !COIN_EXCLUDE_SOTEXTURECOORDINATEENVIRONMENT
#if !defined(COIN_EXCLUDE_SOTEXTURECOORDINATEPLANE)
  "nodes/SoTextureCoordinatePlane.cpp",
#endif // !COIN_EXCLUDE_SOTEXTURECOORDINATEPLANE
#if !defined(COIN_EXCLUDE_SOVERTEXPROPERTY)
  "nodes/SoVertexProperty.cpp",
#endif // !COIN_EXCLUDE_SOVERTEXPROPERTY
#if !defined(COIN_EXCLUDE_SOWWWINLINE)
  "nodes/SoWWWInline.cpp",
#endif // !COIN_EXCLUDE_SOWWWINLINE

#if !defined(COIN_EXCLUDE_SOENGINE)
  "engines/SoEngine.cpp",
#endif // !COIN_EXCLUDE_SOENGINE
#if !defined(COIN_EXCLUDE_SOENGINE)
  "engines/SoEngineOutput.cpp",
#endif // !COIN_EXCLUDE_SOENGINE
#if !defined(COIN_EXCLUDE_SOCOMPOSEVEC2F)
  "engines/SoComposeVec2f.cpp",
#endif // !COIN_EXCLUDE_SOCOMPOSEVEC2F
#if !defined(COIN_EXCLUDE_SOCOMPOSEVEC3F)
  "engines/SoComposeVec3f.cpp",
#endif // !COIN_EXCLUDE_SOCOMPOSEVEC3F
#if !defined(COIN_EXCLUDE_SOCOMPOSEVEC4F)
  "engines/SoComposeVec4f.cpp",
#endif // !COIN_EXCLUDE_SOCOMPOSEVEC4F
#if !defined(COIN_EXCLUDE_SODECOMPOSEVEC2F)
  "engines/SoDecomposeVec2f.cpp",
#endif // !COIN_EXCLUDE_SODECOMPOSEVEC2F
#if !defined(COIN_EXCLUDE_SODECOMPOSEVEC3F)
  "engines/SoDecomposeVec3f.cpp",
#endif // !COIN_EXCLUDE_SODECOMPOSEVEC3F
#if !defined(COIN_EXCLUDE_SODECOMPOSEVEC4F)
  "engines/SoDecomposeVec4f.cpp",
#endif // !COIN_EXCLUDE_SODECOMPOSEVEC4F
#if !defined(COIN_EXCLUDE_SOCOMPOSEROTATION)
  "engines/SoComposeRotation.cpp",
#endif // !COIN_EXCLUDE_SOCOMPOSEROTATION
#if !defined(COIN_EXCLUDE_SODECOMPOSEROTATION)
  "engines/SoDecomposeRotation.cpp",
#endif // !COIN_EXCLUDE_SODECOMPOSEROTATION
#if !defined(COIN_EXCLUDE_SOCOMPOSEMATRIX)
  "engines/SoComposeMatrix.cpp",
#endif // !COIN_EXCLUDE_SOCOMPOSEMATRIX
#if !defined(COIN_EXCLUDE_SODECOMPOSEMATRIX)
  "engines/SoDecomposeMatrix.cpp",
#endif // !COIN_EXCLUDE_SODECOMPOSEMATRIX
#if !defined(COIN_EXCLUDE_SOINTERPOLATE)
  "engines/SoInterpolateAbs.cpp",
#endif // !COIN_EXCLUDE_SOINTERPOLATE
#if !defined(COIN_EXCLUDE_SOINTERPOLATEFLOAT)
  "engines/SoInterpolateFloat.cpp",
#endif // !COIN_EXCLUDE_SOINTERPOLATEFLOAT
#if !defined(COIN_EXCLUDE_SOINTERPOLATEVEC2F)
  "engines/SoInterpolateVec2f.cpp",
#endif // !COIN_EXCLUDE_SOINTERPOLATEVEC2F
#if !defined(COIN_EXCLUDE_SOINTERPOLATEVEC3F)
  "engines/SoInterpolateVec3f.cpp",
#endif // !COIN_EXCLUDE_SOINTERPOLATEVEC3F
#if !defined(COIN_EXCLUDE_SOINTERPOLATEVEC4F)
  "engines/SoInterpolateVec4f.cpp",
#endif // !COIN_EXCLUDE_SOINTERPOLATEVEC4F
#if !defined(COIN_EXCLUDE_SOINTERPOLATEROTATION)
  "engines/SoInterpolateRotation.cpp",
#endif // !COIN_EXCLUDE_SOINTERPOLATEROTATION
#if !defined(COIN_EXCLUDE_SOONOFF)
  "engines/SoOnOff.cpp",
#endif // !COIN_EXCLUDE_SOONOFF
#if !defined(COIN_EXCLUDE_SOCOUNTER)
  "engines/SoCounter.cpp",
#endif // !COIN_EXCLUDE_SOCOUNTER
#if !defined(COIN_EXCLUDE_SOTRIGGERANY)
  "engines/SoTriggerAny.cpp",
#endif // !COIN_EXCLUDE_SOTRIGGERANY
#if !defined(COIN_EXCLUDE_SOELAPSEDTIME)
  "engines/SoElapsedTime.cpp",
#endif // !COIN_EXCLUDE_SOELAPSEDTIME
#if !defined(COIN_EXCLUDE_SOONESHOT)
  "engines/SoOneShot.cpp",
#endif // !COIN_EXCLUDE_SOONESHOT
#if !defined(COIN_EXCLUDE_SOCONCATENATE)
  "engines/SoConcatenate.cpp",
#endif // !COIN_EXCLUDE_SOCONCATENATE
#if !defined(COIN_EXCLUDE_SOSELECTONE)
  "engines/SoSelectOne.cpp",
#endif // !COIN_EXCLUDE_SOSELECTONE
#if !defined(COIN_EXCLUDE_SOGATE)
  "engines/SoGate.cpp",
#endif // !COIN_EXCLUDE_SOGATE
#if !defined(COIN_EXCLUDE_SOBOOLOPERATION)
  "engines/SoBoolOperation.cpp",
#endif // !COIN_EXCLUDE_SOBOOLOPERATION
#if !defined(COIN_EXCLUDE_SOFIELDCONVERTER)
  "engines/SoFieldConverter.cpp",
#endif // !COIN_EXCLUDE_SOFIELDCONVERTER
#if !defined(COIN_EXCLUDE_SOCONVERTALL)
  "engines/SoConvertAll.cpp",
#endif // !COIN_EXCLUDE_SOCONVERTALL

  "VRMLnodes/SoVRMLInterpOutput.cpp",
  "VRMLnodes/SoVRMLInterpolator.cpp",

  NULL
};

int
main(void)
{
  cout << "#\n# Automatically generated -- do not edit.\n#\n\n";
  cout << "SOURCES +=";

  int idx = 0;
  while (sources[idx]) {
    cout << " \\" << endl;
    cout << "\t\t../src/" << sources[idx];
    idx ++;
  }
  cout << endl;

  return 0;
}

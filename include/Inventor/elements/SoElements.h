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

#ifndef __SOELEMENTS_H__
#define __SOELEMENTS_H__

#if !defined(COIN_EXCLUDE_SOELEMENT)
#include <Inventor/elements/SoElement.h>
#endif // !COIN_EXCLUDE_SOELEMENT
#if !defined(COIN_EXCLUDE_SOACCUMULATEDELEMENT)
#include <Inventor/elements/SoAccumulatedElement.h>
#endif // !COIN_EXCLUDE_SOACCUMULATEDELEMENT
#if !defined(COIN_EXCLUDE_SOCLIPPLANEELEMENT)
#include <Inventor/elements/SoClipPlaneElement.h>
#endif // !COIN_EXCLUDE_SOCLIPPLANEELEMENT
#if !defined(COIN_EXCLUDE_SOGLCLIPPLANEELEMENT)
#include <Inventor/elements/SoGLClipPlaneElement.h>
#endif // !COIN_EXCLUDE_SOGLCLIPPLANEELEMENT
#if !defined(COIN_EXCLUDE_SOLIGHTELEMENT)
#include <Inventor/elements/SoLightElement.h>
#endif // !COIN_EXCLUDE_SOLIGHTELEMENT
#if !defined(COIN_EXCLUDE_SOMODELMATRIXELEMENT)
#include <Inventor/elements/SoModelMatrixElement.h>
#endif // !COIN_EXCLUDE_SOMODELMATRIXELEMENT
#if !defined(COIN_EXCLUDE_SOBBOXMODELMATRIXELEMENT)
#include <Inventor/elements/SoBBoxModelMatrixElement.h>
#endif // !COIN_EXCLUDE_SOBBOXMODELMATRIXELEMENT
#if !defined(COIN_EXCLUDE_SOGLMODELMATRIXELEMENT)
#include <Inventor/elements/SoGLModelMatrixElement.h>
#endif // !COIN_EXCLUDE_SOGLMODELMATRIXELEMENT
#if !defined(COIN_EXCLUDE_SOPROFILEELEMENT)
#include <Inventor/elements/SoProfileElement.h>
#endif // !COIN_EXCLUDE_SOPROFILEELEMENT
#if !defined(COIN_EXCLUDE_SOTEXTUREMATRIXELEMENT)
#include <Inventor/elements/SoTextureMatrixElement.h>
#endif // !COIN_EXCLUDE_SOTEXTUREMATRIXELEMENT
#if !defined(COIN_EXCLUDE_SOGLTEXTUREMATRIXELEMENT)
#include <Inventor/elements/SoGLTextureMatrixElement.h>
#endif // !COIN_EXCLUDE_SOGLTEXTUREMATRIXELEMENT
#if !defined(COIN_EXCLUDE_SOCACHEELEMENT)
#include <Inventor/elements/SoCacheElement.h>
#endif // !COIN_EXCLUDE_SOCACHEELEMENT
#if !defined(COIN_EXCLUDE_SOINT32ELEMENT)
#include <Inventor/elements/SoInt32Element.h>
#endif // !COIN_EXCLUDE_SOINT32ELEMENT
#if !defined(COIN_EXCLUDE_SOANNOTEXT3CHARORIENTELEMENT)
#include <Inventor/elements/SoAnnoText3CharOrientElement.h>
#endif // !COIN_EXCLUDE_SOANNOTEXT3CHARORIENTELEMENT
#if !defined(COIN_EXCLUDE_SOANNOTEXT3FONTSIZEHINTELEMENT)
#include <Inventor/elements/SoAnnoText3FontSizeHintElement.h>
#endif // !COIN_EXCLUDE_SOANNOTEXT3FONTSIZEHINTELEMENT
#if !defined(COIN_EXCLUDE_SOANNOTEXT3RENDERPRINTELEMENT)
#include <Inventor/elements/SoAnnoText3RenderPrintElement.h>
#endif // !COIN_EXCLUDE_SOANNOTEXT3RENDERPRINTELEMENT
#if !defined(COIN_EXCLUDE_SOCOMPLEXITYTYPEELEMENT)
#include <Inventor/elements/SoComplexityTypeElement.h>
#endif // !COIN_EXCLUDE_SOCOMPLEXITYTYPEELEMENT
#if !defined(COIN_EXCLUDE_SODECIMATIONTYPEELEMENT)
#include <Inventor/elements/SoDecimationTypeElement.h>
#endif // !COIN_EXCLUDE_SODECIMATIONTYPEELEMENT
#if !defined(COIN_EXCLUDE_SODRAWSTYLEELEMENT)
#include <Inventor/elements/SoDrawStyleElement.h>
#endif // !COIN_EXCLUDE_SODRAWSTYLEELEMENT
#if !defined(COIN_EXCLUDE_SOGLDRAWSTYLEELEMENT)
#include <Inventor/elements/SoGLDrawStyleElement.h>
#endif // !COIN_EXCLUDE_SOGLDRAWSTYLEELEMENT
#if !defined(COIN_EXCLUDE_SOGLLIGHTIDELEMENT)
#include <Inventor/elements/SoGLLightIdElement.h>
#endif // !COIN_EXCLUDE_SOGLLIGHTIDELEMENT
#if !defined(COIN_EXCLUDE_SOGLTEXTUREENABLEDELEMENT)
#include <Inventor/elements/SoGLTextureEnabledElement.h>
#endif // !COIN_EXCLUDE_SOGLTEXTUREENABLEDELEMENT
#if !defined(COIN_EXCLUDE_SOLINEPATTERNELEMENT)
#include <Inventor/elements/SoLinePatternElement.h>
#endif // !COIN_EXCLUDE_SOLINEPATTERNELEMENT
#if !defined(COIN_EXCLUDE_SOGLLINEPATTERNELEMENT)
#include <Inventor/elements/SoGLLinePatternElement.h>
#endif // !COIN_EXCLUDE_SOGLLINEPATTERNELEMENT
#if !defined(COIN_EXCLUDE_SOMATERIALBINDINGELEMENT)
#include <Inventor/elements/SoMaterialBindingElement.h>
#endif // !COIN_EXCLUDE_SOMATERIALBINDINGELEMENT
#if !defined(COIN_EXCLUDE_SONORMALBINDINGELEMENT)
#include <Inventor/elements/SoNormalBindingElement.h>
#endif // !COIN_EXCLUDE_SONORMALBINDINGELEMENT
#if !defined(COIN_EXCLUDE_SOPICKSTYLEELEMENT)
#include <Inventor/elements/SoPickStyleElement.h>
#endif // !COIN_EXCLUDE_SOPICKSTYLEELEMENT
#if !defined(COIN_EXCLUDE_SOSWITCHELEMENT)
#include <Inventor/elements/SoSwitchElement.h>
#endif // !COIN_EXCLUDE_SOSWITCHELEMENT
#if !defined(COIN_EXCLUDE_SOTEXTOUTLINEENABLEDELEMENT)
#include <Inventor/elements/SoTextOutlineEnabledElement.h>
#endif // !COIN_EXCLUDE_SOTEXTOUTLINEENABLEDELEMENT
#if !defined(COIN_EXCLUDE_SOTEXTURECOORDINATEBINDINGELEMENT)
#include <Inventor/elements/SoTextureCoordinateBindingElement.h>
#endif // !COIN_EXCLUDE_SOTEXTURECOORDINATEBINDINGELEMENT
#if !defined(COIN_EXCLUDE_SOUNITSELEMENT)
#include <Inventor/elements/SoUnitsElement.h>
#endif // !COIN_EXCLUDE_SOUNITSELEMENT
#if !defined(COIN_EXCLUDE_SOFLOATELEMENT)
#include <Inventor/elements/SoFloatElement.h>
#endif // !COIN_EXCLUDE_SOFLOATELEMENT
#if !defined(COIN_EXCLUDE_SOCOMPLEXITYELEMENT)
#include <Inventor/elements/SoComplexityElement.h>
#endif // !COIN_EXCLUDE_SOCOMPLEXITYELEMENT
#if !defined(COIN_EXCLUDE_SOCREASEANGLEELEMENT)
#include <Inventor/elements/SoCreaseAngleElement.h>
#endif // !COIN_EXCLUDE_SOCREASEANGLEELEMENT
#if !defined(COIN_EXCLUDE_SODECIMATIONPERCENTAGEELEMENT)
#include <Inventor/elements/SoDecimationPercentageElement.h>
#endif // !COIN_EXCLUDE_SODECIMATIONPERCENTAGEELEMENT
#if !defined(COIN_EXCLUDE_SOFOCALDISTANCEELEMENT)
#include <Inventor/elements/SoFocalDistanceElement.h>
#endif // !COIN_EXCLUDE_SOFOCALDISTANCEELEMENT
#if !defined(COIN_EXCLUDE_SOFONTSIZEELEMENT)
#include <Inventor/elements/SoFontSizeElement.h>
#endif // !COIN_EXCLUDE_SOFONTSIZEELEMENT
#if !defined(COIN_EXCLUDE_SOLINEWIDTHELEMENT)
#include <Inventor/elements/SoLineWidthElement.h>
#endif // !COIN_EXCLUDE_SOLINEWIDTHELEMENT
#if !defined(COIN_EXCLUDE_SOGLLINEWIDTHELEMENT)
#include <Inventor/elements/SoGLLineWidthElement.h>
#endif // !COIN_EXCLUDE_SOGLLINEWIDTHELEMENT
#if !defined(COIN_EXCLUDE_SOPOINTSIZEELEMENT)
#include <Inventor/elements/SoPointSizeElement.h>
#endif // !COIN_EXCLUDE_SOPOINTSIZEELEMENT
#if !defined(COIN_EXCLUDE_SOGLPOINTSIZEELEMENT)
#include <Inventor/elements/SoGLPointSizeElement.h>
#endif // !COIN_EXCLUDE_SOGLPOINTSIZEELEMENT
#if !defined(COIN_EXCLUDE_SOTEXTUREQUALITYELEMENT)
#include <Inventor/elements/SoTextureQualityElement.h>
#endif // !COIN_EXCLUDE_SOTEXTUREQUALITYELEMENT
#if !defined(COIN_EXCLUDE_SOGLRENDERPASSELEMENT)
#include <Inventor/elements/SoGLRenderPassElement.h>
#endif // !COIN_EXCLUDE_SOGLRENDERPASSELEMENT
#if !defined(COIN_EXCLUDE_SOGLUPDATEAREAELEMENT)
#include <Inventor/elements/SoGLUpdateAreaElement.h>
#endif // !COIN_EXCLUDE_SOGLUPDATEAREAELEMENT
#if !defined(COIN_EXCLUDE_SOLOCALBBOXMATRIXELEMENT)
#include <Inventor/elements/SoLocalBBoxMatrixElement.h>
#endif // !COIN_EXCLUDE_SOLOCALBBOXMATRIXELEMENT
#if !defined(COIN_EXCLUDE_SOOVERRIDEELEMENT)
#include <Inventor/elements/SoOverrideElement.h>
#endif // !COIN_EXCLUDE_SOOVERRIDEELEMENT
#if !defined(COIN_EXCLUDE_SOPICKRAYELEMENT)
#include <Inventor/elements/SoPickRayElement.h>
#endif // !COIN_EXCLUDE_SOPICKRAYELEMENT
#if !defined(COIN_EXCLUDE_SOREPLACEDELEMENT)
#include <Inventor/elements/SoReplacedElement.h>
#endif // !COIN_EXCLUDE_SOREPLACEDELEMENT
#if !defined(COIN_EXCLUDE_SOCOORDINATEELEMENT)
#include <Inventor/elements/SoCoordinateElement.h>
#endif // !COIN_EXCLUDE_SOCOORDINATEELEMENT
#if !defined(COIN_EXCLUDE_SOGLCOORDINATEELEMENT)
#include <Inventor/elements/SoGLCoordinateElement.h>
#endif // !COIN_EXCLUDE_SOGLCOORDINATEELEMENT
#if !defined(COIN_EXCLUDE_SOENVIRONMENTELEMENT)
#include <Inventor/elements/SoEnvironmentElement.h>
#endif // !COIN_EXCLUDE_SOENVIRONMENTELEMENT
#if !defined(COIN_EXCLUDE_SOGLENVIRONMENTELEMENT)
#include <Inventor/elements/SoGLEnvironmentElement.h>
#endif // !COIN_EXCLUDE_SOGLENVIRONMENTELEMENT
#if !defined(COIN_EXCLUDE_SOFONTNAMEELEMENT)
#include <Inventor/elements/SoFontNameElement.h>
#endif // !COIN_EXCLUDE_SOFONTNAMEELEMENT
#if !defined(COIN_EXCLUDE_SOLIGHTATTENUATIONELEMENT)
#include <Inventor/elements/SoLightAttenuationElement.h>
#endif // !COIN_EXCLUDE_SOLIGHTATTENUATIONELEMENT
#if !defined(COIN_EXCLUDE_SONORMALELEMENT)
#include <Inventor/elements/SoNormalElement.h>
#endif // !COIN_EXCLUDE_SONORMALELEMENT
#if !defined(COIN_EXCLUDE_SOGLNORMALELEMENT)
#include <Inventor/elements/SoGLNormalElement.h>
#endif // !COIN_EXCLUDE_SOGLNORMALELEMENT
#if !defined(COIN_EXCLUDE_SOPOLYGONOFFSETELEMENT)
#include <Inventor/elements/SoPolygonOffsetElement.h>
#endif // !COIN_EXCLUDE_SOPOLYGONOFFSETELEMENT
#if !defined(COIN_EXCLUDE_SOGLPOLYGONOFFSETELEMENT)
#include <Inventor/elements/SoGLPolygonOffsetElement.h>
#endif // !COIN_EXCLUDE_SOGLPOLYGONOFFSETELEMENT
#if !defined(COIN_EXCLUDE_SOPROJECTIONMATRIXELEMENT)
#include <Inventor/elements/SoProjectionMatrixElement.h>
#endif // !COIN_EXCLUDE_SOPROJECTIONMATRIXELEMENT
#if !defined(COIN_EXCLUDE_SOGLPROJECTIONMATRIXELEMENT)
#include <Inventor/elements/SoGLProjectionMatrixElement.h>
#endif // !COIN_EXCLUDE_SOGLPROJECTIONMATRIXELEMENT
#if !defined(COIN_EXCLUDE_SOPROFILECOORDINATEELEMENT)
#include <Inventor/elements/SoProfileCoordinateElement.h>
#endif // !COIN_EXCLUDE_SOPROFILECOORDINATEELEMENT
#if !defined(COIN_EXCLUDE_SOTEXTURECOORDINATEELEMENT)
#include <Inventor/elements/SoTextureCoordinateElement.h>
#endif // !COIN_EXCLUDE_SOTEXTURECOORDINATEELEMENT
#if !defined(COIN_EXCLUDE_SOGLTEXTURECOORDINATEELEMENT)
#include <Inventor/elements/SoGLTextureCoordinateElement.h>
#endif // !COIN_EXCLUDE_SOGLTEXTURECOORDINATEELEMENT
#if !defined(COIN_EXCLUDE_SOTEXTUREIMAGEELEMENT)
#include <Inventor/elements/SoTextureImageElement.h>
#endif // !COIN_EXCLUDE_SOTEXTUREIMAGEELEMENT
#if !defined(COIN_EXCLUDE_SOGLTEXTUREIMAGEELEMENT)
#include <Inventor/elements/SoGLTextureImageElement.h>
#endif // !COIN_EXCLUDE_SOGLTEXTUREIMAGEELEMENT
#if !defined(COIN_EXCLUDE_SOVIEWINGMATRIXELEMENT)
#include <Inventor/elements/SoViewingMatrixElement.h>
#endif // !COIN_EXCLUDE_SOVIEWINGMATRIXELEMENT
#if !defined(COIN_EXCLUDE_SOGLVIEWINGMATRIXELEMENT)
#include <Inventor/elements/SoGLViewingMatrixElement.h>
#endif // !COIN_EXCLUDE_SOGLVIEWINGMATRIXELEMENT
#if !defined(COIN_EXCLUDE_SOVIEWVOLUMEELEMENT)
#include <Inventor/elements/SoViewVolumeElement.h>
#endif // !COIN_EXCLUDE_SOVIEWVOLUMEELEMENT
#if !defined(COIN_EXCLUDE_SOSHAPEHINTSELEMENT)
#include <Inventor/elements/SoShapeHintsElement.h>
#endif // !COIN_EXCLUDE_SOSHAPEHINTSELEMENT
#if !defined(COIN_EXCLUDE_SOGLSHAPEHINTSELEMENT)
#include <Inventor/elements/SoGLShapeHintsElement.h>
#endif // !COIN_EXCLUDE_SOGLSHAPEHINTSELEMENT
#if !defined(COIN_EXCLUDE_SOSHAPESTYLEELEMENT)
#include <Inventor/elements/SoShapeStyleElement.h>
#endif // !COIN_EXCLUDE_SOSHAPESTYLEELEMENT
#if !defined(COIN_EXCLUDE_SOVIEWPORTREGIONELEMENT)
#include <Inventor/elements/SoViewportRegionElement.h>
#endif // !COIN_EXCLUDE_SOVIEWPORTREGIONELEMENT
#if !defined(COIN_EXCLUDE_SOGLVIEWPORTREGIONELEMENT)
#include <Inventor/elements/SoGLViewportRegionElement.h>
#endif // !COIN_EXCLUDE_SOGLVIEWPORTREGIONELEMENT
#if !defined(COIN_EXCLUDE_SOWINDOWELEMENT)
#include <Inventor/elements/SoWindowElement.h>
#endif // !COIN_EXCLUDE_SOWINDOWELEMENT
#if !defined(COIN_EXCLUDE_SOAMBIENTCOLORELEMENT)
#include <Inventor/elements/SoAmbientColorElement.h>
#endif // !COIN_EXCLUDE_SOAMBIENTCOLORELEMENT
#if !defined(COIN_EXCLUDE_SOEMISSIVECOLORELEMENT)
#include <Inventor/elements/SoEmissiveColorElement.h>
#endif // !COIN_EXCLUDE_SOEMISSIVECOLORELEMENT
#if !defined(COIN_EXCLUDE_SOLIGHTMODELELEMENT)
#include <Inventor/elements/SoLightModelElement.h>
#endif // !COIN_EXCLUDE_SOLIGHTMODELELEMENT
#if !defined(COIN_EXCLUDE_SOGLLIGHTMODELELEMENT)
#include <Inventor/elements/SoGLLightModelElement.h>
#endif // !COIN_EXCLUDE_SOGLLIGHTMODELELEMENT
#if !defined(COIN_EXCLUDE_SOSHININESSELEMENT)
#include <Inventor/elements/SoShininessElement.h>
#endif // !COIN_EXCLUDE_SOSHININESSELEMENT
#if !defined(COIN_EXCLUDE_SOSPECULARCOLORELEMENT)
#include <Inventor/elements/SoSpecularColorElement.h>
#endif // !COIN_EXCLUDE_SOSPECULARCOLORELEMENT
#if !defined(COIN_EXCLUDE_SOTRANSPARENCYELEMENT)
#include <Inventor/elements/SoTransparencyElement.h>
#endif // !COIN_EXCLUDE_SOTRANSPARENCYELEMENT
#if !defined(COIN_EXCLUDE_SODIFFUSECOLORELEMENT)
#include <Inventor/elements/SoDiffuseColorElement.h>
#endif // !COIN_EXCLUDE_SODIFFUSECOLORELEMENT
#if !defined(COIN_EXCLUDE_SOGLSPECULARCOLORELEMENT)
#include <Inventor/elements/SoGLSpecularColorElement.h>
#endif // !COIN_EXCLUDE_SOGLSPECULARCOLORELEMENT
#if !defined(COIN_EXCLUDE_SOGLAMBIENTCOLORELEMENT)
#include <Inventor/elements/SoGLAmbientColorElement.h>
#endif // !COIN_EXCLUDE_SOGLAMBIENTCOLORELEMENT
#if !defined(COIN_EXCLUDE_SOGLDIFFUSECOLORELEMENT)
#include <Inventor/elements/SoGLDiffuseColorElement.h>
#endif // !COIN_EXCLUDE_SOGLDIFFUSECOLORELEMENT
#if !defined(COIN_EXCLUDE_SOGLEMISSIVECOLORELEMENT)
#include <Inventor/elements/SoGLEmissiveColorElement.h>
#endif // !COIN_EXCLUDE_SOGLEMISSIVECOLORELEMENT
#if !defined(COIN_EXCLUDE_SOGLSHININESSELEMENT)
#include <Inventor/elements/SoGLShininessElement.h>
#endif // !COIN_EXCLUDE_SOGLSHININESSELEMENT
#if !defined(COIN_EXCLUDE_SOGLPOLYGONSTIPPLEELEMENT)
#include <Inventor/elements/SoGLPolygonStippleElement.h>
#endif // !COIN_EXCLUDE_SOGLPOLYGONSTIPPLEELEMENT

// elements not found in OIV. Added to optimize rendering
#if !defined(COIN_EXCLUDE_SOGLSHADEMODELELEMENT)
#include <Inventor/elements/SoGLShadeModelElement.h>
#endif
#if !defined(COIN_EXCLUDE_SOGLNORMALIZEELEMENT)
#include <Inventor/elements/SoGLNormalizeElement.h>
#endif

#endif // !__SOELEMENTS_H__

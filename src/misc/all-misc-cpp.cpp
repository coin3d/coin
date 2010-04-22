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

// FIXME: for some odd reason, order is important between these
// two. if SoBase.cpp is compiled before SoDebug.cpp, we get a weird
// error from the SbHash usage in SoDebug.cpp. investigate.  -mortene.

// This is caused by the SbHashFunc(void *) being declared only when
// used. Hashing on (void *) is extremely dangerous, since we don't
// have any control over which kind of types are hashed this way.
// Indeed the very occurence of this problem is a symptom of the
// problems of hashing on (void *). Since all the implementations for
// (class *) are identical regardless of class, there shouldn't arise
// any problems from doing this, but if we change the implementations,
// we should make very sure that the correct function is picked as the
// hash function when doing inclusion like this. - BFG 20081117

#include "SoDebug.cpp"
#include "SoBase.cpp"

#include "AudioTools.cpp"
#include "CoinResources.cpp"
#include "CoinStaticObjectInDLL.cpp"
#include "SoAudioDevice.cpp"
#include "SoBaseP.cpp"
#include "SoChildList.cpp"
#include "SoCompactPathList.cpp"
#include "SoConfigSettings.cpp"
#include "SoContextHandler.cpp"
#include "SoDB.cpp"
#include "SoDBP.cpp"
#include "SoEventManager.cpp"
#include "SoFullPath.cpp"
#include "SoGenerate.cpp"
#include "SoGlyph.cpp"
#include "SoInteraction.cpp"
#include "SoJavaScriptEngine.cpp"
#include "SoLightPath.cpp"
#include "SoLockManager.cpp"
#include "SoNormalGenerator.cpp"
#include "SoNotRec.cpp"
#include "SoNotification.cpp"
#include "SoPath.cpp"
#include "SoPick.cpp"
#include "SoPickedPoint.cpp"
#include "SoPrimitiveVertex.cpp"
#include "SoProto.cpp"
#include "SoProtoInstance.cpp"
#include "SoSceneManager.cpp"
#include "SoSceneManagerP.cpp"
#include "SoShaderGenerator.cpp"
#include "SoState.cpp"
#include "SoTempPath.cpp"
#include "SoType.cpp"

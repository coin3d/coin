############################################################################
#
#  Copyright (C) 1998-1999 by Systems in Motion.  All rights reserved.
#
#  This file is part of the Coin library.
#
#  This file may be distributed under the terms of the Q Public License
#  as defined by Troll Tech AS of Norway and appearing in the file
#  LICENSE.QPL included in the packaging of this file.
#
#  If you want to use Coin in applications not covered by licenses
#  compatible with the QPL, you can contact SIM to aquire a
#  Professional Edition license for Coin.
#
#  Systems in Motion AS, Prof. Brochs gate 6, N-7030 Trondheim, NORWAY
#  http://www.sim.no/ sales@sim.no Voice: +47 22114160 Fax: +47 67172912
#
############################################################################

# This projectfile makes it easy to create a library containing only
# the base classes of Coin, which is particularly useful during
# development and debugging. (I.e. quicker turn-around times, as there's
# significantly less time spent compiling and linking).
#
# To aid in the creation of this "mini" library, we need to do some checking
# around in the code to disable certain features. These checks will be made
# against the __SOLIB_MAKEASBASE__ define.
#
#        19990522 mortene.

TEMPLATE    =	lib
TARGET	    =   CoinBase
CONFIG      =	debug sharedlib

# Defines:
# COIN_DEBUG = [0,1]   (compile with/without run-time debug code)
# __SOLIB_INTERNAL__   (used to indicate for include files that we're compiling
#                       the library itself -- must be set)
# __SOLIB_MAKEASBASE__ (temporary defined flag used during development to make
#                       a minimal library of the base classes)
#
DEFINES     = __SOLIB_INTERNAL__ COIN_DEBUG=1 __SOLIB_MAKEASBASE__

INCLUDEPATH  =	../include
DEPENDPATH  =	../include
MOC_DIR     =   base_objects
OBJECTS_DIR =   base_objects

SOURCES       =	\
	../src/SbDict.cpp \
	../src/SbName.cpp \
	../src/SbString.cpp \
	../src/SbVec2f.cpp \
	../src/SbVec2s.cpp \
	../src/SbVec3f.cpp \
	../src/SbVec4f.cpp \
	../src/SbBox2f.cpp \
	../src/SbBox2s.cpp \
	../src/SbBox3f.cpp \
	../src/SbXfBox3f.cpp \
	../src/SbColor.cpp \
	../src/SbCylinder.cpp \
	../src/SbLine.cpp \
	../src/SbPlane.cpp \
	../src/SbRotation.cpp \
	../src/SbSphere.cpp \
	../src/SbMatrix.cpp \
	../src/lists/SbList.cpp \
	../src/lists/SbPList.cpp \
	  ../src/lists/SoTypeList.cpp \
	  ../src/lists/SoAuditorList.cpp \
	  ../src/lists/SoActionMethodList.cpp \
	  ../src/lists/SoBaseList.cpp \
	    ../src/lists/SoNodeList.cpp \
	      ../src/misc/SoChildList.cpp \
	    ../src/lists/SoPathList.cpp \
	  ../src/lists/SoFieldList.cpp \
	  ../src/lists/SoEngineOutputList.cpp \
	  ../src/lists/SoVRMLInterpOutputList.cpp \
	  ../src/lists/SbStringList.cpp \
	  ../src/lists/SbIntList.cpp \
	../src/lists/SoEnabledElementsList.cpp \
	../src/SbTime.cpp \
	../src/SbViewVolume.cpp \
	../src/SbViewportRegion.cpp \
	\
	../src/SoDB.cpp \
	../src/SoType.cpp \
	../src/SoPath.cpp \
	../src/SoInput.cpp \
	../src/SoOutput.cpp \
	\
	../src/projectors/SbProjector.cpp \
	../src/projectors/SbSphereProjector.cpp \
	  ../src/projectors/SbSphereSheetProjector.cpp \
	../src/projectors/SbCylinderProjector.cpp \
	../src/projectors/SbLineProjector.cpp \
	../src/projectors/SbPlaneProjector.cpp \
	\
	../src/errors/SoError.cpp \
	  ../src/errors/SoDebugError.cpp \
	  ../src/errors/SoReadError.cpp \
	  ../src/errors/SoMemoryError.cpp \
	\
	../src/misc/SoBase.cpp \
	../src/misc/SoState.cpp \
	../src/misc/SoNotification.cpp \
	\
	../src/fields/SoFieldContainer.cpp \
	../src/fields/SoFieldData.cpp \
	../src/fields/SoField.cpp \
	  ../src/fields/SoSField.cpp \
	    ../src/fields/SoSFBool.cpp \
	    ../src/fields/SoSFInt32.cpp \
	    ../src/fields/SoSFUInt32.cpp \
	    ../src/fields/SoSFString.cpp \
	    ../src/fields/SoSFEnum.cpp \
	    ../src/fields/SoSFBitMask.cpp \
	    ../src/fields/SoSFFloat.cpp \
	    ../src/fields/SoSFColor.cpp \
	    ../src/fields/SoSFMatrix.cpp \
	    ../src/fields/SoSFName.cpp \
	    ../src/fields/SoSFVec2f.cpp \
	    ../src/fields/SoSFVec3f.cpp \
	    ../src/fields/SoSFVec4f.cpp \
	    ../src/fields/SoSFRotation.cpp \
	    ../src/fields/SoSFPlane.cpp \
	    ../src/fields/SoSFTime.cpp \
	    ../src/fields/SoSFShort.cpp \
	    ../src/fields/SoSFUShort.cpp \
	    ../src/fields/SoSFImage.cpp \
	\
	  ../src/fields/SoMField.cpp \
	    ../src/fields/SoMFInt32.cpp \
	    ../src/fields/SoMFBool.cpp \
	    ../src/fields/SoMFString.cpp \
	    ../src/fields/SoMFEnum.cpp \
	    ../src/fields/SoMFBitMask.cpp \
	    ../src/fields/SoMFFloat.cpp \
	    ../src/fields/SoMFMatrix.cpp \
	    ../src/fields/SoMFName.cpp \
	    ../src/fields/SoMFPlane.cpp \
	    ../src/fields/SoMFRotation.cpp \
	    ../src/fields/SoMFShort.cpp \
	    ../src/fields/SoMFTime.cpp \
	    ../src/fields/SoMFUInt32.cpp \
	    ../src/fields/SoMFUShort.cpp \
	    ../src/fields/SoMFVec2f.cpp \
	    ../src/fields/SoMFVec3f.cpp \
	    ../src/fields/SoMFVec4f.cpp \
	    ../src/fields/SoMFColor.cpp \
	\
	../src/nodes/SoNode.cpp \
	  ../src/nodes/SoGroup.cpp \
	  ../src/nodes/SoInfo.cpp \
	\
	../src/engines/SoEngineOutput.cpp \
	../src/engines/SoEngine.cpp \
	\
	../src/VRMLnodes/SoVRMLInterpOutput.cpp \
	../src/VRMLnodes/SoVRMLInterpolator.cpp \
	\
	../src/actions/SoAction.cpp \
	  ../src/actions/SoWriteAction.cpp \
	\
	../src/elements/SoElement.cpp \
	../src/elements/SoOverrideElement.cpp

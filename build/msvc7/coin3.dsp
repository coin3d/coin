# Microsoft Developer Studio Project File - Name="coin3" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=coin3 - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE
!MESSAGE NMAKE /f "coin3.mak".
!MESSAGE
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE
!MESSAGE NMAKE /f "coin3.mak" CFG="coin3 - Win32 Debug"
!MESSAGE
!MESSAGE Possible choices for configuration are:
!MESSAGE
!MESSAGE "coin3 - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "coin3 - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "coin3 - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "COIN_DEBUG=0" /D "HAVE_CONFIG_H" /D "COIN_MAKE_DLL" /D "COIN_INTERNAL" /I "..\..\\build\msvc7" /I "..\..\\build\msvc7\include" /I "..\..\\include" /I "..\..\\include\Inventor\annex" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O1 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "COIN_DEBUG=0" /D "HAVE_CONFIG_H" /D "COIN_MAKE_DLL" /D "COIN_INTERNAL" /I "..\..\\build\msvc7" /I "..\..\\build\msvc7\include" /I "..\..\\include" /I "..\..\\include\Inventor\annex" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 opengl32.lib gdi32.lib winmm.lib user32.lib /nologo /dll /machine:I386
# ADD LINK32 opengl32.lib gdi32.lib winmm.lib user32.lib /nologo /dll /machine:I386 /out:"coin3.dll" /opt:nowin98
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "COIN_DEBUG=1" /D "HAVE_CONFIG_H" /D "COIN_MAKE_DLL" /D "COIN_INTERNAL" /I "..\..\\build\msvc7" /I "..\..\\build\msvc7\include" /I "..\..\\include" /I "..\..\\include\Inventor\annex" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "COIN_DEBUG=1" /D "HAVE_CONFIG_H" /D "COIN_MAKE_DLL" /D "COIN_INTERNAL" /I "..\..\\build\msvc7" /I "..\..\\build\msvc7\include" /I "..\..\\include" /I "..\..\\include\Inventor\annex" /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 opengl32.lib gdi32.lib winmm.lib user32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 opengl32.lib gdi32.lib winmm.lib user32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept /out:"coin3d.dll"

!ENDIF

# Begin Target

# Name "coin3 - Win32 Release"
# Name "coin3 - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;ic;icc"
# Begin Source File

SOURCE=..\..\\src\actions\SoAction.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\actions"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\actions"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\actions\SoBoxHighlightRenderAction.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\actions"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\actions"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\actions\SoCallbackAction.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\actions"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\actions"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\actions\SoGLRenderAction.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\actions"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\actions"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\actions\SoGetBoundingBoxAction.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\actions"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\actions"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\actions\SoGetMatrixAction.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\actions"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\actions"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\actions\SoGetPrimitiveCountAction.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\actions"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\actions"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\actions\SoHandleEventAction.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\actions"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\actions"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\actions\SoLineHighlightRenderAction.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\actions"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\actions"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\actions\SoPickAction.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\actions"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\actions"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\actions\SoRayPickAction.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\actions"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\actions"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\actions\SoSearchAction.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\actions"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\actions"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\actions\SoToVRMLAction.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\actions"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\actions"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\actions\SoToVRML2Action.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\actions"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\actions"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\actions\SoWriteAction.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\actions"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\actions"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\actions\SoAudioRenderAction.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\actions"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\actions"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\base\hash.c
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\base"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\base"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\base\heap.c
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\base"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\base"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\base\list.c
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\base"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\base"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\base\memalloc.c
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\base"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\base"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\base\rbptree.c
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\base"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\base"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\base\time.c
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\base"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\base"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\base\string.c
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\base"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\base"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\base\dynarray.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\base"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\base"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\base\namemap.c
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\base"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\base"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\base\SbBSPTree.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\base"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\base"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\base\SbBox2s.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\base"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\base"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\base\SbBox3s.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\base"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\base"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\base\SbBox2f.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\base"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\base"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\base\SbBox2d.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\base"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\base"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\base\SbBox3f.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\base"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\base"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\base\SbClip.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\base"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\base"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\base\SbColor.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\base"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\base"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\base\SbColor4f.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\base"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\base"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\base\SbCylinder.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\base"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\base"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\base\SbDict.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\base"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\base"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\base\SbDPLine.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\base"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\base"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\base\SbDPMatrix.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\base"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\base"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\base\SbDPPlane.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\base"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\base"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\base\SbDPRotation.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\base"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\base"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\base\SbHeap.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\base"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\base"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\base\SbImage.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\base"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\base"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\base\SbLine.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\base"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\base"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\base\SbMatrix.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\base"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\base"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\base\SbName.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\base"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\base"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\base\SbOctTree.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\base"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\base"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\base\SbPlane.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\base"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\base"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\base\SbRotation.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\base"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\base"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\base\SbSphere.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\base"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\base"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\base\SbString.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\base"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\base"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\base\SbTesselator.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\base"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\base"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\base\SbTime.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\base"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\base"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\base\SbVec2s.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\base"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\base"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\base\SbVec2f.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\base"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\base"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\base\SbVec2d.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\base"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\base"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\base\SbVec3s.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\base"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\base"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\base\SbVec3f.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\base"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\base"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\base\SbVec3d.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\base"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\base"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\base\SbVec4f.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\base"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\base"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\base\SbVec4d.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\base"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\base"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\base\SbViewVolume.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\base"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\base"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\base\SbDPViewVolume.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\base"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\base"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\base\SbViewportRegion.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\base"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\base"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\base\SbXfBox3f.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\base"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\base"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\bundles\SoBundle.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\bundles"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\bundles"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\bundles\SoMaterialBundle.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\bundles"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\bundles"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\bundles\SoNormalBundle.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\bundles"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\bundles"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\bundles\SoTextureCoordinateBundle.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\bundles"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\bundles"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\caches\SoBoundingBoxCache.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\caches"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\caches"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\caches\SoCache.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\caches"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\caches"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\caches\SoConvexDataCache.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\caches"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\caches"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\caches\SoGLCacheList.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\caches"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\caches"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\caches\SoGLRenderCache.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\caches"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\caches"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\caches\SoNormalCache.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\caches"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\caches"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\caches\SoTextureCoordinateCache.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\caches"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\caches"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\caches\SoPrimitiveVertexCache.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\caches"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\caches"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\caches\SoGlyphCache.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\caches"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\caches"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\details\SoDetail.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\details"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\details"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\details\SoConeDetail.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\details"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\details"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\details\SoCubeDetail.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\details"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\details"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\details\SoCylinderDetail.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\details"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\details"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\details\SoFaceDetail.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\details"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\details"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\details\SoLineDetail.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\details"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\details"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\details\SoNodeKitDetail.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\details"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\details"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\details\SoPointDetail.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\details"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\details"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\details\SoTextDetail.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\details"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\details"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\draggers\SoDragger.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\draggers"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\draggers"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\draggers\SoCenterballDragger.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\draggers"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\draggers"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\draggers\SoDirectionalLightDragger.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\draggers"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\draggers"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\draggers\SoDragPointDragger.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\draggers"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\draggers"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\draggers\SoHandleBoxDragger.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\draggers"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\draggers"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\draggers\SoJackDragger.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\draggers"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\draggers"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\draggers\SoPointLightDragger.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\draggers"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\draggers"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\draggers\SoRotateCylindricalDragger.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\draggers"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\draggers"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\draggers\SoRotateDiscDragger.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\draggers"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\draggers"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\draggers\SoRotateSphericalDragger.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\draggers"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\draggers"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\draggers\SoScale1Dragger.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\draggers"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\draggers"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\draggers\SoScale2Dragger.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\draggers"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\draggers"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\draggers\SoScale2UniformDragger.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\draggers"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\draggers"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\draggers\SoScaleUniformDragger.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\draggers"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\draggers"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\draggers\SoSpotLightDragger.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\draggers"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\draggers"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\draggers\SoTabBoxDragger.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\draggers"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\draggers"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\draggers\SoTabPlaneDragger.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\draggers"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\draggers"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\draggers\SoTrackballDragger.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\draggers"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\draggers"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\draggers\SoTransformBoxDragger.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\draggers"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\draggers"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\draggers\SoTransformerDragger.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\draggers"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\draggers"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\draggers\SoTranslate1Dragger.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\draggers"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\draggers"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\draggers\SoTranslate2Dragger.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\draggers"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\draggers"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\elements\GL\SoGLCacheContextElement.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\GL"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\GL"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\elements\GL\SoGLClipPlaneElement.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\GL"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\GL"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\elements\GL\SoGLColorIndexElement.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\GL"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\GL"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\elements\GL\SoGLCoordinateElement.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\GL"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\GL"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\elements\GL\SoGLDrawStyleElement.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\GL"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\GL"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\elements\GL\SoGLEnvironmentElement.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\GL"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\GL"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\elements\GL\SoGLLazyElement.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\GL"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\GL"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\elements\GL\SoGLLightIdElement.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\GL"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\GL"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\elements\GL\SoGLLinePatternElement.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\GL"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\GL"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\elements\GL\SoGLLineWidthElement.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\GL"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\GL"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\elements\GL\SoGLModelMatrixElement.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\GL"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\GL"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\elements\GL\SoGLMultiTextureCoordinateElement.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\GL"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\GL"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\elements\GL\SoGLMultiTextureEnabledElement.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\GL"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\GL"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\elements\GL\SoGLMultiTextureMatrixElement.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\GL"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\GL"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\elements\GL\SoGLMultiTextureImageElement.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\GL"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\GL"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\elements\GL\SoGLNormalElement.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\GL"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\GL"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\elements\GL\SoGLPointSizeElement.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\GL"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\GL"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\elements\GL\SoGLPolygonOffsetElement.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\GL"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\GL"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\elements\GL\SoGLProjectionMatrixElement.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\GL"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\GL"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\elements\GL\SoGLRenderPassElement.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\GL"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\GL"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\elements\GL\SoGLShapeHintsElement.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\GL"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\GL"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\elements\GL\SoGLTextureCoordinateElement.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\GL"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\GL"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\elements\GL\SoGLTextureEnabledElement.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\GL"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\GL"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\elements\GL\SoGLTexture3EnabledElement.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\GL"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\GL"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\elements\GL\SoGLTextureImageElement.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\GL"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\GL"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\elements\GL\SoGLTextureMatrixElement.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\GL"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\GL"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\elements\GL\SoGLUpdateAreaElement.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\GL"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\GL"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\elements\GL\SoGLViewingMatrixElement.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\GL"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\GL"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\elements\GL\SoGLViewportRegionElement.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\GL"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\GL"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\elements\GL\SoGLDisplayList.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\GL"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\GL"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\elements\SoAccumulatedElement.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\elements"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\elements"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\elements\SoAmbientColorElement.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\elements"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\elements"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\elements\SoAnnoText3CharOrientElement.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\elements"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\elements"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\elements\SoAnnoText3FontSizeHintElement.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\elements"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\elements"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\elements\SoAnnoText3RenderPrintElement.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\elements"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\elements"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\elements\SoBBoxModelMatrixElement.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\elements"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\elements"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\elements\SoBumpMapElement.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\elements"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\elements"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\elements\SoBumpMapCoordinateElement.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\elements"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\elements"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\elements\SoBumpMapMatrixElement.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\elements"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\elements"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\elements\SoBumpMappingPropertyElement.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\elements"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\elements"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\elements\SoCacheElement.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\elements"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\elements"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\elements\SoCacheHintElement.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\elements"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\elements"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\elements\SoClipPlaneElement.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\elements"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\elements"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\elements\SoComplexityElement.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\elements"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\elements"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\elements\SoComplexityTypeElement.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\elements"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\elements"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\elements\SoCoordinateElement.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\elements"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\elements"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\elements\SoCreaseAngleElement.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\elements"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\elements"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\elements\SoCullElement.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\elements"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\elements"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\elements\SoDecimationPercentageElement.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\elements"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\elements"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\elements\SoDecimationTypeElement.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\elements"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\elements"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\elements\SoDiffuseColorElement.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\elements"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\elements"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\elements\SoDrawStyleElement.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\elements"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\elements"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\elements\SoElement.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\elements"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\elements"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\elements\SoEmissiveColorElement.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\elements"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\elements"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\elements\SoEnvironmentElement.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\elements"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\elements"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\elements\SoFloatElement.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\elements"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\elements"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\elements\SoFocalDistanceElement.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\elements"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\elements"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\elements\SoFontNameElement.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\elements"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\elements"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\elements\SoFontSizeElement.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\elements"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\elements"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\elements\SoInt32Element.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\elements"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\elements"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\elements\SoLazyElement.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\elements"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\elements"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\elements\SoLightAttenuationElement.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\elements"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\elements"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\elements\SoLightElement.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\elements"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\elements"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\elements\SoLightModelElement.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\elements"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\elements"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\elements\SoLinePatternElement.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\elements"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\elements"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\elements\SoLineWidthElement.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\elements"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\elements"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\elements\SoLocalBBoxMatrixElement.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\elements"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\elements"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\elements\SoMaterialBindingElement.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\elements"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\elements"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\elements\SoMultiTextureCoordinateElement.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\elements"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\elements"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\elements\SoMultiTextureEnabledElement.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\elements"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\elements"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\elements\SoMultiTextureMatrixElement.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\elements"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\elements"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\elements\SoMultiTextureImageElement.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\elements"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\elements"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\elements\SoModelMatrixElement.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\elements"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\elements"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\elements\SoNormalBindingElement.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\elements"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\elements"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\elements\SoNormalElement.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\elements"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\elements"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\elements\SoOverrideElement.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\elements"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\elements"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\elements\SoPickRayElement.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\elements"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\elements"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\elements\SoPickStyleElement.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\elements"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\elements"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\elements\SoPointSizeElement.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\elements"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\elements"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\elements\SoPolygonOffsetElement.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\elements"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\elements"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\elements\SoProfileCoordinateElement.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\elements"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\elements"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\elements\SoProfileElement.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\elements"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\elements"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\elements\SoProjectionMatrixElement.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\elements"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\elements"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\elements\SoReplacedElement.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\elements"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\elements"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\elements\SoShapeHintsElement.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\elements"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\elements"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\elements\SoShapeStyleElement.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\elements"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\elements"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\elements\SoShininessElement.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\elements"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\elements"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\elements\SoSpecularColorElement.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\elements"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\elements"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\elements\SoSwitchElement.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\elements"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\elements"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\elements\SoTextOutlineEnabledElement.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\elements"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\elements"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\elements\SoTextureCombineElement.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\elements"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\elements"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\elements\SoTextureCoordinateBindingElement.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\elements"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\elements"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\elements\SoTextureCoordinateElement.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\elements"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\elements"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\elements\SoTextureEnabledElement.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\elements"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\elements"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\elements\SoTexture3EnabledElement.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\elements"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\elements"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\elements\SoTextureImageElement.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\elements"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\elements"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\elements\SoTextureMatrixElement.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\elements"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\elements"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\elements\SoTextureOverrideElement.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\elements"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\elements"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\elements\SoTextureScalePolicyElement.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\elements"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\elements"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\elements\SoTextureScaleQualityElement.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\elements"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\elements"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\elements\SoTextureUnitElement.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\elements"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\elements"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\elements\SoTextureQualityElement.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\elements"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\elements"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\elements\SoTransparencyElement.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\elements"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\elements"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\elements\SoUnitsElement.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\elements"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\elements"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\elements\SoViewVolumeElement.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\elements"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\elements"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\elements\SoViewingMatrixElement.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\elements"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\elements"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\elements\SoViewportRegionElement.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\elements"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\elements"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\elements\SoWindowElement.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\elements"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\elements"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\elements\SoListenerPositionElement.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\elements"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\elements"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\elements\SoListenerOrientationElement.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\elements"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\elements"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\elements\SoListenerGainElement.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\elements"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\elements"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\elements\SoListenerDopplerElement.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\elements"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\elements"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\elements\SoSoundElement.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\elements"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\elements"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\engines\SoBoolOperation.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\engines"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\engines"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\engines\SoCalculator.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\engines"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\engines"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\engines\SoComposeMatrix.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\engines"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\engines"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\engines\SoComposeRotation.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\engines"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\engines"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\engines\SoComposeRotationFromTo.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\engines"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\engines"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\engines\SoComposeVec2f.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\engines"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\engines"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\engines\SoComposeVec3f.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\engines"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\engines"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\engines\SoComposeVec4f.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\engines"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\engines"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\engines\SoComputeBoundingBox.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\engines"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\engines"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\engines\SoConcatenate.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\engines"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\engines"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\engines\SoConvertAll.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\engines"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\engines"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\engines\SoCounter.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\engines"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\engines"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\engines\SoDecomposeMatrix.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\engines"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\engines"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\engines\SoDecomposeRotation.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\engines"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\engines"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\engines\SoDecomposeVec2f.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\engines"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\engines"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\engines\SoDecomposeVec3f.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\engines"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\engines"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\engines\SoDecomposeVec4f.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\engines"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\engines"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\engines\SoElapsedTime.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\engines"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\engines"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\engines\SoEngine.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\engines"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\engines"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\engines\SoEngineOutput.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\engines"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\engines"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\engines\SoFieldConverter.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\engines"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\engines"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\engines\SoGate.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\engines"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\engines"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\engines\SoInterpolate.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\engines"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\engines"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\engines\SoInterpolateFloat.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\engines"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\engines"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\engines\SoInterpolateRotation.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\engines"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\engines"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\engines\SoInterpolateVec2f.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\engines"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\engines"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\engines\SoInterpolateVec3f.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\engines"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\engines"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\engines\SoInterpolateVec4f.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\engines"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\engines"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\engines\SoNodeEngine.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\engines"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\engines"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\engines\SoOnOff.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\engines"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\engines"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\engines\SoOneShot.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\engines"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\engines"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\engines\SoOutputData.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\engines"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\engines"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\engines\SoSelectOne.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\engines"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\engines"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\engines\SoTimeCounter.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\engines"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\engines"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\engines\SoTransformVec3f.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\engines"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\engines"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\engines\SoTriggerAny.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\engines"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\engines"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\engines\evaluator.c
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\engines"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\engines"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\engines\evaluator_tab.c
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\engines"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\engines"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\errors\error.c
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\errors"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\errors"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\errors\debugerror.c
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\errors"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\errors"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\errors\SoDebugError.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\errors"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\errors"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\errors\SoError.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\errors"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\errors"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\errors\SoMemoryError.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\errors"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\errors"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\errors\SoReadError.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\errors"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\errors"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\events\SoButtonEvent.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\events"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\events"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\events\SoEvent.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\events"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\events"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\events\SoKeyboardEvent.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\events"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\events"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\events\SoLocation2Event.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\events"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\events"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\events\SoMotion3Event.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\events"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\events"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\events\SoMouseButtonEvent.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\events"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\events"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\events\SoSpaceballButtonEvent.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\events"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\events"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\fields\SoField.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\fields"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\fields"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\fields\SoFieldContainer.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\fields"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\fields"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\fields\SoFieldData.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\fields"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\fields"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\fields\SoMFBitMask.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\fields"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\fields"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\fields\SoMFBool.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\fields"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\fields"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\fields\SoMFColor.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\fields"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\fields"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\fields\SoMFEngine.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\fields"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\fields"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\fields\SoMFEnum.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\fields"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\fields"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\fields\SoMFFloat.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\fields"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\fields"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\fields\SoMFInt32.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\fields"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\fields"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\fields\SoMFMatrix.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\fields"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\fields"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\fields\SoMFName.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\fields"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\fields"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\fields\SoMFNode.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\fields"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\fields"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\fields\SoMFPath.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\fields"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\fields"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\fields\SoMFPlane.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\fields"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\fields"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\fields\SoMFRotation.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\fields"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\fields"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\fields\SoMFShort.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\fields"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\fields"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\fields\SoMFString.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\fields"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\fields"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\fields\SoMFTime.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\fields"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\fields"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\fields\SoMFUInt32.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\fields"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\fields"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\fields\SoMFUShort.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\fields"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\fields"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\fields\SoMFVec2f.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\fields"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\fields"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\fields\SoMFVec3f.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\fields"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\fields"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\fields\SoMFVec3d.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\fields"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\fields"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\fields\SoMFVec4f.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\fields"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\fields"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\fields\SoMField.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\fields"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\fields"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\fields\SoSFBitMask.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\fields"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\fields"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\fields\SoSFBool.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\fields"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\fields"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\fields\SoSFBox3s.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\fields"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\fields"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\fields\SoSFColor.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\fields"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\fields"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\fields\SoSFEngine.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\fields"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\fields"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\fields\SoSFEnum.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\fields"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\fields"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\fields\SoSFFloat.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\fields"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\fields"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\fields\SoSFImage.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\fields"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\fields"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\fields\SoSFImage3.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\fields"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\fields"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\fields\SoSFInt32.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\fields"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\fields"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\fields\SoSFMatrix.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\fields"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\fields"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\fields\SoSFName.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\fields"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\fields"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\fields\SoSFNode.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\fields"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\fields"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\fields\SoSFPath.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\fields"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\fields"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\fields\SoSFPlane.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\fields"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\fields"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\fields\SoSFRotation.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\fields"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\fields"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\fields\SoSFShort.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\fields"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\fields"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\fields\SoSFString.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\fields"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\fields"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\fields\SoSFTime.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\fields"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\fields"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\fields\SoSFTrigger.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\fields"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\fields"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\fields\SoSFUInt32.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\fields"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\fields"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\fields\SoSFUShort.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\fields"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\fields"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\fields\SoSFVec2s.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\fields"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\fields"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\fields\SoSFVec2f.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\fields"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\fields"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\fields\SoSFVec3d.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\fields"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\fields"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\fields\SoSFVec3s.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\fields"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\fields"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\fields\SoSFVec3f.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\fields"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\fields"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\fields\SoSFVec4f.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\fields"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\fields"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\fields\SoSField.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\fields"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\fields"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\fields\SoGlobalField.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\fields"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\fields"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\fields\shared.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\fields"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\fields"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\fonts\fontlib_wrapper.c
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\fonts"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\fonts"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\fonts\win32.c
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\fonts"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\fonts"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\fonts\freetype.c
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\fonts"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\fonts"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\fonts\fontspec.c
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\fonts"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\fonts"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\fonts\glyph2d.c
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\fonts"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\fonts"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\fonts\glyph3d.c
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\fonts"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\fonts"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\fonts\default3dfont.c
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\fonts"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\fonts"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\fonts\default2dfont.c
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\fonts"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\fonts"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\glue\dl.c
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\glue"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\glue"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\glue\gl.c
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\glue"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\glue"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\glue\gl_wgl.c
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\glue"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\glue"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\glue\gl_agl.c
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\glue"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\glue"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\glue\gl_glx.c
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\glue"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\glue"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\glue\GLUWrapper.c
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\glue"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\glue"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\glue\simage_wrapper.c
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\glue"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\glue"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\glue\openal_wrapper.c
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\glue"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\glue"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\glue\win32api.c
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\glue"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\glue"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\glue\zlib.c
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\glue"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\glue"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\glue\bzip2.c
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\glue"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\glue"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\glue\freetype.c
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\glue"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\glue"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\glue\glue-dummy.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\glue"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\glue"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\glue\normalization_cubemap.c
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\glue"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\glue"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\io\SoInput.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\io"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\io"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\io\SoInput_FileInfo.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\io"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\io"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\io\SoInput_Reader.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\io"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\io"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\io\SoOutput.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\io"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\io"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\io\SoOutput_Writer.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\io"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\io"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\io\SoByteStream.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\io"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\io"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\io\SoTranSender.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\io"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\io"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\io\SoTranReceiver.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\io"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\io"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\io\SoWriterefCounter.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\io"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\io"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\io\gzmemio.c
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\io"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\io"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\manips\SoClipPlaneManip.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\manips"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\manips"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\manips\SoDirectionalLightManip.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\manips"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\manips"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\manips\SoPointLightManip.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\manips"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\manips"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\manips\SoSpotLightManip.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\manips"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\manips"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\manips\SoTransformManip.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\manips"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\manips"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\manips\SoCenterballManip.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\manips"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\manips"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\manips\SoHandleBoxManip.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\manips"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\manips"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\manips\SoJackManip.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\manips"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\manips"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\manips\SoTabBoxManip.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\manips"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\manips"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\manips\SoTrackballManip.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\manips"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\manips"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\manips\SoTransformBoxManip.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\manips"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\manips"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\manips\SoTransformerManip.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\manips"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\manips"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\manips\commoncode.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\manips"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\manips"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\misc\SoBase.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\misc"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\misc"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\misc\SoChildList.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\misc"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\misc"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\misc\SoContextHandler.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\misc"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\misc"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\misc\SoGL.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\misc"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\misc"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\misc\SoGLImage.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\misc"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\misc"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\misc\SoGLBigImage.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\misc"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\misc"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\misc\SoGlyph.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\misc"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\misc"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\misc\SoNormalGenerator.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\misc"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\misc"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\misc\SoNotification.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\misc"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\misc"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\misc\SoNotRec.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\misc"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\misc"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\misc\SoState.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\misc"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\misc"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\misc\SoDB.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\misc"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\misc"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\misc\SoFullPath.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\misc"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\misc"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\misc\SoTempPath.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\misc"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\misc"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\misc\SoLightPath.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\misc"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\misc"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\misc\SoInteraction.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\misc"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\misc"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\misc\SoPath.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\misc"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\misc"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\misc\SoPick.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\misc"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\misc"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\misc\SoPickedPoint.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\misc"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\misc"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\misc\SoPrimitiveVertex.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\misc"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\misc"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\misc\SoProto.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\misc"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\misc"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\misc\SoProtoInstance.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\misc"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\misc"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\misc\SoSceneManager.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\misc"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\misc"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\misc\SoType.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\misc"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\misc"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\misc\SoOffscreenRenderer.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\misc"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\misc"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\misc\SoOffscreenAGLData.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\misc"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\misc"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\misc\SoOffscreenGLXData.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\misc"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\misc"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\misc\SoOffscreenWGLData.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\misc"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\misc"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\misc\SoGenerate.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\misc"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\misc"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\misc\SoLockManager.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\misc"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\misc"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\misc\SoAudioDevice.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\misc"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\misc"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\misc\AudioTools.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\misc"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\misc"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\lists\SbList.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\lists"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\lists"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\lists\SbPList.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\lists"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\lists"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\lists\SbIntList.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\lists"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\lists"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\lists\SbVec3fList.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\lists"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\lists"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\lists\SbStringList.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\lists"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\lists"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\lists\SoActionMethodList.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\lists"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\lists"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\lists\SoAuditorList.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\lists"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\lists"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\lists\SoBaseList.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\lists"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\lists"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\lists\SoCallbackList.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\lists"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\lists"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\lists\SoDetailList.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\lists"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\lists"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\lists\SoEnabledElementsList.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\lists"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\lists"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\lists\SoEngineList.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\lists"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\lists"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\lists\SoEngineOutputList.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\lists"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\lists"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\lists\SoFieldList.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\lists"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\lists"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\lists\SoNodeList.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\lists"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\lists"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\lists\SoPathList.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\lists"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\lists"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\lists\SoPickedPointList.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\lists"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\lists"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\lists\SoTypeList.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\lists"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\lists"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\nodekits\SoNodeKit.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\nodekits"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\nodekits"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\nodekits\SoNodeKitPath.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\nodekits"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\nodekits"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\nodekits\SoNodeKitListPart.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\nodekits"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\nodekits"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\nodekits\SoNodekitCatalog.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\nodekits"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\nodekits"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\nodekits\SoBaseKit.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\nodekits"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\nodekits"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\nodekits\SoAppearanceKit.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\nodekits"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\nodekits"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\nodekits\SoCameraKit.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\nodekits"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\nodekits"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\nodekits\SoInteractionKit.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\nodekits"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\nodekits"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\nodekits\SoLightKit.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\nodekits"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\nodekits"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\nodekits\SoSceneKit.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\nodekits"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\nodekits"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\nodekits\SoSeparatorKit.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\nodekits"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\nodekits"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\nodekits\SoShapeKit.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\nodekits"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\nodekits"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\nodekits\SoWrapperKit.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\nodekits"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\nodekits"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\nodes\SoAnnotation.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\nodes"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\nodes"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\nodes\SoAnnoText3.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\nodes"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\nodes"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\nodes\SoAnnoText3Property.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\nodes"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\nodes"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\nodes\SoAntiSquish.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\nodes"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\nodes"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\nodes\SoArray.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\nodes"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\nodes"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\nodes\SoBaseColor.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\nodes"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\nodes"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\nodes\SoBlinker.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\nodes"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\nodes"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\nodes\SoBumpMap.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\nodes"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\nodes"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\nodes\SoBumpMapCoordinate.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\nodes"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\nodes"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\nodes\SoBumpMapTransform.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\nodes"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\nodes"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\nodes\SoBumpMappingProperty.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\nodes"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\nodes"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\nodes\SoCallback.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\nodes"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\nodes"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\nodes\SoCacheHint.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\nodes"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\nodes"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\nodes\SoCamera.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\nodes"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\nodes"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\nodes\SoClipPlane.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\nodes"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\nodes"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\nodes\SoColorIndex.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\nodes"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\nodes"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\nodes\SoComplexity.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\nodes"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\nodes"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\nodes\SoCoordinate3.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\nodes"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\nodes"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\nodes\SoCoordinate4.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\nodes"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\nodes"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\nodes\SoDirectionalLight.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\nodes"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\nodes"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\nodes\SoDrawStyle.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\nodes"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\nodes"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\nodes\SoEnvironment.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\nodes"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\nodes"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\nodes\SoEventCallback.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\nodes"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\nodes"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\nodes\SoExtSelection.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\nodes"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\nodes"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\nodes\SoFile.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\nodes"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\nodes"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\nodes\SoFont.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\nodes"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\nodes"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\nodes\SoFontStyle.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\nodes"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\nodes"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\nodes\SoGroup.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\nodes"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\nodes"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\nodes\SoInfo.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\nodes"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\nodes"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\nodes\SoLOD.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\nodes"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\nodes"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\nodes\SoLabel.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\nodes"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\nodes"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\nodes\SoLevelOfDetail.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\nodes"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\nodes"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\nodes\SoLight.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\nodes"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\nodes"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\nodes\SoLightModel.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\nodes"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\nodes"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\nodes\SoLinearProfile.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\nodes"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\nodes"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\nodes\SoListener.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\nodes"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\nodes"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\nodes\SoLocateHighlight.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\nodes"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\nodes"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\nodes\SoMaterial.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\nodes"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\nodes"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\nodes\SoMaterialBinding.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\nodes"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\nodes"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\nodes\SoMatrixTransform.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\nodes"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\nodes"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\nodes\SoMultipleCopy.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\nodes"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\nodes"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\nodes\SoNode.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\nodes"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\nodes"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\nodes\SoNormal.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\nodes"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\nodes"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\nodes\SoNormalBinding.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\nodes"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\nodes"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\nodes\SoNurbsProfile.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\nodes"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\nodes"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\nodes\SoOrthographicCamera.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\nodes"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\nodes"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\nodes\SoPackedColor.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\nodes"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\nodes"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\nodes\SoPathSwitch.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\nodes"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\nodes"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\nodes\SoPattern.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\nodes"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\nodes"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\nodes\SoPendulum.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\nodes"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\nodes"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\nodes\SoPerspectiveCamera.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\nodes"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\nodes"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\nodes\SoPickStyle.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\nodes"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\nodes"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\nodes\SoPointLight.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\nodes"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\nodes"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\nodes\SoPolygonOffset.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\nodes"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\nodes"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\nodes\SoProfile.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\nodes"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\nodes"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\nodes\SoProfileCoordinate2.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\nodes"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\nodes"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\nodes\SoProfileCoordinate3.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\nodes"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\nodes"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\nodes\SoResetTransform.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\nodes"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\nodes"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\nodes\SoRotation.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\nodes"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\nodes"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\nodes\SoRotationXYZ.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\nodes"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\nodes"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\nodes\SoRotor.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\nodes"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\nodes"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\nodes\SoScale.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\nodes"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\nodes"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\nodes\SoSceneTexture2.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\nodes"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\nodes"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\nodes\SoSelection.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\nodes"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\nodes"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\nodes\SoSeparator.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\nodes"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\nodes"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\nodes\SoShapeHints.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\nodes"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\nodes"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\nodes\SoShuttle.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\nodes"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\nodes"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\nodes\SoSpotLight.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\nodes"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\nodes"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\nodes\SoSurroundScale.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\nodes"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\nodes"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\nodes\SoSwitch.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\nodes"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\nodes"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\nodes\SoTexture2.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\nodes"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\nodes"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\nodes\SoTexture3.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\nodes"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\nodes"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\nodes\SoTexture2Transform.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\nodes"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\nodes"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\nodes\SoTexture3Transform.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\nodes"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\nodes"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\nodes\SoTextureCombine.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\nodes"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\nodes"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\nodes\SoTextureCoordinate2.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\nodes"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\nodes"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\nodes\SoTextureCoordinate3.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\nodes"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\nodes"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\nodes\SoTextureCoordinateBinding.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\nodes"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\nodes"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\nodes\SoTextureCoordinateCube.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\nodes"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\nodes"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\nodes\SoTextureCoordinateCylinder.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\nodes"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\nodes"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\nodes\SoTextureCoordinateSphere.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\nodes"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\nodes"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\nodes\SoTextureCoordinateDefault.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\nodes"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\nodes"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\nodes\SoTextureCoordinateEnvironment.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\nodes"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\nodes"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\nodes\SoTextureCoordinateFunction.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\nodes"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\nodes"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\nodes\SoTextureCoordinatePlane.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\nodes"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\nodes"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\nodes\SoTextureScalePolicy.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\nodes"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\nodes"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\nodes\SoTextureUnit.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\nodes"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\nodes"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\nodes\SoTransform.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\nodes"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\nodes"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\nodes\SoTransparencyType.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\nodes"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\nodes"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\nodes\SoTransformSeparator.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\nodes"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\nodes"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\nodes\SoTransformation.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\nodes"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\nodes"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\nodes\SoTranslation.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\nodes"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\nodes"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\nodes\SoUnits.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\nodes"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\nodes"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\nodes\SoUnknownNode.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\nodes"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\nodes"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\nodes\SoVertexProperty.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\nodes"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\nodes"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\nodes\SoWWWAnchor.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\nodes"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\nodes"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\nodes\SoWWWInline.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\nodes"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\nodes"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\shapenodes\SoAsciiText.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\shapenodes"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\shapenodes"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\shapenodes\SoCone.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\shapenodes"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\shapenodes"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\shapenodes\SoCube.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\shapenodes"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\shapenodes"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\shapenodes\SoCylinder.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\shapenodes"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\shapenodes"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\shapenodes\SoFaceSet.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\shapenodes"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\shapenodes"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\shapenodes\SoImage.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\shapenodes"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\shapenodes"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\shapenodes\SoIndexedFaceSet.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\shapenodes"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\shapenodes"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\shapenodes\SoIndexedLineSet.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\shapenodes"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\shapenodes"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\shapenodes\SoIndexedNurbsCurve.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\shapenodes"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\shapenodes"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\shapenodes\SoIndexedNurbsSurface.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\shapenodes"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\shapenodes"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\shapenodes\SoIndexedShape.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\shapenodes"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\shapenodes"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\shapenodes\SoIndexedTriangleStripSet.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\shapenodes"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\shapenodes"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\shapenodes\SoLineSet.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\shapenodes"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\shapenodes"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\shapenodes\SoMarkerSet.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\shapenodes"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\shapenodes"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\shapenodes\SoNonIndexedShape.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\shapenodes"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\shapenodes"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\shapenodes\SoNurbsCurve.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\shapenodes"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\shapenodes"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\shapenodes\SoNurbsSurface.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\shapenodes"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\shapenodes"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\shapenodes\SoPointSet.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\shapenodes"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\shapenodes"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\shapenodes\SoQuadMesh.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\shapenodes"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\shapenodes"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\shapenodes\SoShape.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\shapenodes"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\shapenodes"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\shapenodes\SoSphere.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\shapenodes"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\shapenodes"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\shapenodes\SoText2.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\shapenodes"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\shapenodes"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\shapenodes\SoText3.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\shapenodes"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\shapenodes"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\shapenodes\SoTriangleStripSet.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\shapenodes"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\shapenodes"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\shapenodes\SoVertexShape.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\shapenodes"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\shapenodes"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\shapenodes\soshape_primdata.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\shapenodes"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\shapenodes"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\shapenodes\soshape_bigtexture.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\shapenodes"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\shapenodes"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\shapenodes\soshape_trianglesort.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\shapenodes"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\shapenodes"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\shapenodes\soshape_bumprender.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\shapenodes"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\shapenodes"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\projectors\SbCylinderPlaneProjector.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\projectors"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\projectors"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\projectors\SbCylinderProjector.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\projectors"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\projectors"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\projectors\SbCylinderSectionProjector.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\projectors"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\projectors"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\projectors\SbCylinderSheetProjector.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\projectors"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\projectors"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\projectors\SbLineProjector.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\projectors"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\projectors"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\projectors\SbPlaneProjector.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\projectors"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\projectors"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\projectors\SbProjector.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\projectors"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\projectors"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\projectors\SbSpherePlaneProjector.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\projectors"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\projectors"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\projectors\SbSphereProjector.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\projectors"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\projectors"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\projectors\SbSphereSectionProjector.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\projectors"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\projectors"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\projectors\SbSphereSheetProjector.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\projectors"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\projectors"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\sensors\SoAlarmSensor.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\sensors"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\sensors"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\sensors\SoDataSensor.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\sensors"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\sensors"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\sensors\SoDelayQueueSensor.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\sensors"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\sensors"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\sensors\SoFieldSensor.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\sensors"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\sensors"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\sensors\SoIdleSensor.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\sensors"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\sensors"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\sensors\SoNodeSensor.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\sensors"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\sensors"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\sensors\SoOneShotSensor.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\sensors"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\sensors"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\sensors\SoPathSensor.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\sensors"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\sensors"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\sensors\SoSensor.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\sensors"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\sensors"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\sensors\SoSensorManager.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\sensors"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\sensors"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\sensors\SoTimerQueueSensor.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\sensors"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\sensors"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\sensors\SoTimerSensor.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\sensors"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\sensors"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\upgraders\SoUpgrader.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\upgraders"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\upgraders"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\upgraders\SoPackedColorV20.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\upgraders"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\upgraders"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\upgraders\SoShapeHintsV10.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\upgraders"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\upgraders"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\3ds\3ds-dummy.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\3ds"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\3ds"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\collision\SbTri3f.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\collision"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\collision"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\collision\SoIntersectionDetectionAction.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\collision"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\collision"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\hardcopy\HardCopy.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\hardcopy"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\hardcopy"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\hardcopy\PSVectorOutput.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\hardcopy"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\hardcopy"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\hardcopy\CGMVectorOutput.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\hardcopy"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\hardcopy"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\hardcopy\HPGLVectorOutput.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\hardcopy"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\hardcopy"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\hardcopy\GDIVectorOutput.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\hardcopy"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\hardcopy"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\hardcopy\VectorOutput.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\hardcopy"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\hardcopy"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\hardcopy\VectorizeAction.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\hardcopy"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\hardcopy"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\hardcopy\VectorizeActionP.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\hardcopy"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\hardcopy"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\hardcopy\VectorizePSAction.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\hardcopy"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\hardcopy"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\hardcopy\VectorizeCGMAction.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\hardcopy"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\hardcopy"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\hardcopy\VectorizeHPGLAction.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\hardcopy"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\hardcopy"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\hardcopy\VectorizeGDIAction.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\hardcopy"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\hardcopy"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\threads\common.c
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\threads"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\threads"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\threads\thread.c
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\threads"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\threads"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\threads\mutex.c
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\threads"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\threads"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\threads\rwmutex.c
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\threads"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\threads"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\threads\storage.c
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\threads"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\threads"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\threads\condvar.c
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\threads"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\threads"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\threads\worker.c
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\threads"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\threads"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\threads\wpool.c
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\threads"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\threads"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\threads\recmutex.c
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\threads"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\threads"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\threads\sched.c
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\threads"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\threads"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\threads\sync.c
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\threads"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\threads"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\threads\fifo.c
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\threads"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\threads"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\threads\barrier.c
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\threads"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\threads"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\threads\threads-dummy.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\threads"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\threads"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\mpeg\SoMPEGRenderer.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\mpeg"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\mpeg"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\mpeg\SoMPEGFrameRenderer.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\mpeg"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\mpeg"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\shaders\shaders-dummy.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\shaders"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\shaders"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\vrml97\Anchor.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\vrml97"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\vrml97"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\vrml97\Appearance.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\vrml97"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\vrml97"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\vrml97\AudioClip.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\vrml97"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\vrml97"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\vrml97\Background.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\vrml97"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\vrml97"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\vrml97\Billboard.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\vrml97"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\vrml97"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\vrml97\Box.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\vrml97"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\vrml97"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\vrml97\Collision.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\vrml97"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\vrml97"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\vrml97\Color.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\vrml97"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\vrml97"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\vrml97\ColorInterpolator.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\vrml97"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\vrml97"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\vrml97\Cone.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\vrml97"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\vrml97"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\vrml97\Coordinate.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\vrml97"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\vrml97"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\vrml97\CoordinateInterpolator.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\vrml97"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\vrml97"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\vrml97\Cylinder.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\vrml97"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\vrml97"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\vrml97\CylinderSensor.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\vrml97"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\vrml97"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\vrml97\DirectionalLight.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\vrml97"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\vrml97"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\vrml97\DragSensor.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\vrml97"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\vrml97"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\vrml97\ElevationGrid.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\vrml97"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\vrml97"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\vrml97\Extrusion.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\vrml97"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\vrml97"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\vrml97\Fog.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\vrml97"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\vrml97"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\vrml97\FontStyle.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\vrml97"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\vrml97"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\vrml97\Geometry.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\vrml97"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\vrml97"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\vrml97\Group.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\vrml97"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\vrml97"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\vrml97\ImageTexture.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\vrml97"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\vrml97"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\vrml97\IndexedFaceSet.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\vrml97"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\vrml97"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\vrml97\IndexedLine.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\vrml97"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\vrml97"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\vrml97\IndexedLineSet.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\vrml97"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\vrml97"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\vrml97\IndexedShape.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\vrml97"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\vrml97"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\vrml97\Init.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\vrml97"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\vrml97"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\vrml97\Inline.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\vrml97"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\vrml97"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\vrml97\Interpolator.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\vrml97"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\vrml97"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\vrml97\LOD.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\vrml97"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\vrml97"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\vrml97\Light.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\vrml97"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\vrml97"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\vrml97\Material.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\vrml97"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\vrml97"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\vrml97\MovieTexture.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\vrml97"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\vrml97"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\vrml97\NavigationInfo.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\vrml97"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\vrml97"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\vrml97\Normal.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\vrml97"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\vrml97"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\vrml97\NormalInterpolator.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\vrml97"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\vrml97"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\vrml97\OrientationInterpolator.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\vrml97"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\vrml97"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\vrml97\Parent.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\vrml97"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\vrml97"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\vrml97\PixelTexture.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\vrml97"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\vrml97"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\vrml97\PlaneSensor.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\vrml97"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\vrml97"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\vrml97\PointLight.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\vrml97"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\vrml97"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\vrml97\PointSet.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\vrml97"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\vrml97"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\vrml97\PositionInterpolator.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\vrml97"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\vrml97"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\vrml97\ProximitySensor.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\vrml97"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\vrml97"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\vrml97\ScalarInterpolator.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\vrml97"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\vrml97"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\vrml97\Script.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\vrml97"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\vrml97"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\vrml97\Sensor.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\vrml97"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\vrml97"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\vrml97\Shape.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\vrml97"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\vrml97"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\vrml97\Sound.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\vrml97"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\vrml97"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\vrml97\Sphere.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\vrml97"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\vrml97"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\vrml97\SphereSensor.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\vrml97"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\vrml97"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\vrml97\SpotLight.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\vrml97"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\vrml97"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\vrml97\Switch.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\vrml97"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\vrml97"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\vrml97\Text.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\vrml97"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\vrml97"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\vrml97\Texture.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\vrml97"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\vrml97"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\vrml97\TextureCoordinate.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\vrml97"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\vrml97"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\vrml97\TextureTransform.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\vrml97"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\vrml97"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\vrml97\TimeSensor.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\vrml97"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\vrml97"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\vrml97\TouchSensor.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\vrml97"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\vrml97"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\vrml97\Transform.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\vrml97"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\vrml97"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\vrml97\VertexLine.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\vrml97"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\vrml97"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\vrml97\VertexPoint.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\vrml97"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\vrml97"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\vrml97\VertexShape.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\vrml97"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\vrml97"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\vrml97\Viewpoint.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\vrml97"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\vrml97"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\vrml97\VisibilitySensor.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\vrml97"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\vrml97"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\vrml97\WorldInfo.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\vrml97"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\vrml97"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\tidbits.c
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\src"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\src"
!ENDIF 
# End Source File
# Begin Source File

SOURCE=..\..\\src\coin-dummy.cpp
!IF  "$(CFG)" == "coin3 - Win32 Release"
# PROP Intermediate_Dir "Release\src"
!ELSEIF  "$(CFG)" == "coin3 - Win32 Debug"
# PROP Intermediate_Dir "Debug\src"
!ENDIF 
# End Source File
# End Group
# Begin Group "Public Headers"

# PROP Default_Filter "h"
# Begin Source File

SOURCE=..\..\\include\Inventor\C\base\hash.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\C\base\heap.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\C\base\memalloc.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\C\base\rbptree.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\C\base\time.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\C\base\string.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\C\base\list.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\C\errors\error.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\C\errors\debugerror.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\C\glue\gl.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\C\glue\dl.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\C\threads\common.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\C\threads\thread.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\C\threads\mutex.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\C\threads\condvar.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\C\threads\recmutex.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\C\threads\rwmutex.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\C\threads\storage.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\C\threads\worker.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\C\threads\wpool.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\C\threads\sched.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\C\threads\sync.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\C\threads\fifo.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\C\threads\barrier.h
# End Source File
# Begin Source File

SOURCE=..\..\\build\msvc7\include\Inventor\C\basic.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\C\tidbits.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\MPEG\SoMPEGRenderer.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\MPEG\SoMPEGFrameRenderer.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\VRMLnodes\SoVRML.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\VRMLnodes\SoVRMLAnchor.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\VRMLnodes\SoVRMLAppearance.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\VRMLnodes\SoVRMLAudioClip.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\VRMLnodes\SoVRMLBackground.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\VRMLnodes\SoVRMLBillboard.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\VRMLnodes\SoVRMLBox.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\VRMLnodes\SoVRMLCollision.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\VRMLnodes\SoVRMLColor.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\VRMLnodes\SoVRMLColorInterpolator.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\VRMLnodes\SoVRMLCone.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\VRMLnodes\SoVRMLCoordinate.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\VRMLnodes\SoVRMLCoordinateInterpolator.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\VRMLnodes\SoVRMLCylinder.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\VRMLnodes\SoVRMLCylinderSensor.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\VRMLnodes\SoVRMLDirectionalLight.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\VRMLnodes\SoVRMLDragSensor.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\VRMLnodes\SoVRMLElevationGrid.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\VRMLnodes\SoVRMLExtrusion.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\VRMLnodes\SoVRMLFog.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\VRMLnodes\SoVRMLFontStyle.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\VRMLnodes\SoVRMLGeometry.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\VRMLnodes\SoVRMLGroup.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\VRMLnodes\SoVRMLImageTexture.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\VRMLnodes\SoVRMLIndexedFaceSet.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\VRMLnodes\SoVRMLIndexedLine.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\VRMLnodes\SoVRMLIndexedLineSet.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\VRMLnodes\SoVRMLIndexedShape.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\VRMLnodes\SoVRMLInline.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\VRMLnodes\SoVRMLInterpolator.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\VRMLnodes\SoVRMLLOD.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\VRMLnodes\SoVRMLLight.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\VRMLnodes\SoVRMLMacros.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\VRMLnodes\SoVRMLMaterial.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\VRMLnodes\SoVRMLMovieTexture.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\VRMLnodes\SoVRMLNavigationInfo.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\VRMLnodes\SoVRMLNodes.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\VRMLnodes\SoVRMLNormal.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\VRMLnodes\SoVRMLNormalInterpolator.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\VRMLnodes\SoVRMLOrientationInterpolator.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\VRMLnodes\SoVRMLParent.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\VRMLnodes\SoVRMLPixelTexture.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\VRMLnodes\SoVRMLPlaneSensor.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\VRMLnodes\SoVRMLPointLight.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\VRMLnodes\SoVRMLPointSet.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\VRMLnodes\SoVRMLPositionInterpolator.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\VRMLnodes\SoVRMLProximitySensor.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\VRMLnodes\SoVRMLScalarInterpolator.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\VRMLnodes\SoVRMLScript.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\VRMLnodes\SoVRMLSensor.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\VRMLnodes\SoVRMLShape.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\VRMLnodes\SoVRMLSound.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\VRMLnodes\SoVRMLSphere.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\VRMLnodes\SoVRMLSphereSensor.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\VRMLnodes\SoVRMLSpotLight.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\VRMLnodes\SoVRMLSubInterpolator.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\VRMLnodes\SoVRMLSwitch.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\VRMLnodes\SoVRMLText.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\VRMLnodes\SoVRMLTexture.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\VRMLnodes\SoVRMLTextureCoordinate.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\VRMLnodes\SoVRMLTextureTransform.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\VRMLnodes\SoVRMLTimeSensor.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\VRMLnodes\SoVRMLTouchSensor.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\VRMLnodes\SoVRMLTransform.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\VRMLnodes\SoVRMLVertexLine.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\VRMLnodes\SoVRMLVertexPoint.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\VRMLnodes\SoVRMLVertexShape.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\VRMLnodes\SoVRMLViewpoint.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\VRMLnodes\SoVRMLVisibilitySensor.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\VRMLnodes\SoVRMLWorldInfo.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\actions\SoSubAction.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\actions\SoActions.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\actions\SoAction.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\actions\SoBoxHighlightRenderAction.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\actions\SoCallbackAction.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\actions\SoGLRenderAction.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\actions\SoGetBoundingBoxAction.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\actions\SoGetMatrixAction.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\actions\SoGetPrimitiveCountAction.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\actions\SoHandleEventAction.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\actions\SoLineHighlightRenderAction.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\actions\SoPickAction.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\actions\SoRayPickAction.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\actions\SoSearchAction.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\actions\SoToVRMLAction.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\actions\SoToVRML2Action.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\actions\SoWriteAction.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\actions\SoAudioRenderAction.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\annex\HardCopy\SoHardCopy.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\annex\HardCopy\SoPSVectorOutput.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\annex\HardCopy\SoCGMVectorOutput.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\annex\HardCopy\SoHPGLVectorOutput.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\annex\HardCopy\SoGDIVectorOutput.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\annex\HardCopy\SoVectorOutput.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\annex\HardCopy\SoVectorizeAction.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\annex\HardCopy\SoVectorizePSAction.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\annex\HardCopy\SoVectorizeCGMAction.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\annex\HardCopy\SoVectorizeHPGLAction.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\annex\HardCopy\SoVectorizeGDIAction.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\bundles\SoBundle.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\bundles\SoMaterialBundle.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\bundles\SoNormalBundle.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\bundles\SoTextureCoordinateBundle.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\caches\SoBoundingBoxCache.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\caches\SoCache.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\caches\SoConvexDataCache.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\caches\SoGLCacheList.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\caches\SoGLRenderCache.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\caches\SoNormalCache.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\caches\SoTextureCoordinateCache.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\collision\SoIntersectionDetectionAction.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\details\SoSubDetail.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\details\SoDetail.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\details\SoDetails.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\details\SoConeDetail.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\details\SoCubeDetail.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\details\SoCylinderDetail.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\details\SoFaceDetail.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\details\SoLineDetail.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\details\SoNodeKitDetail.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\details\SoPointDetail.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\details\SoTextDetail.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\draggers\SoDragger.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\draggers\SoCenterballDragger.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\draggers\SoDirectionalLightDragger.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\draggers\SoDragPointDragger.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\draggers\SoHandleBoxDragger.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\draggers\SoJackDragger.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\draggers\SoPointLightDragger.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\draggers\SoRotateCylindricalDragger.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\draggers\SoRotateDiscDragger.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\draggers\SoRotateSphericalDragger.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\draggers\SoScale1Dragger.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\draggers\SoScale2Dragger.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\draggers\SoScale2UniformDragger.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\draggers\SoScaleUniformDragger.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\draggers\SoSpotLightDragger.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\draggers\SoTabBoxDragger.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\draggers\SoTabPlaneDragger.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\draggers\SoTrackballDragger.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\draggers\SoTransformBoxDragger.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\draggers\SoTransformerDragger.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\draggers\SoTranslate1Dragger.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\draggers\SoTranslate2Dragger.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\elements\SoSubElement.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\elements\SoElements.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\elements\SoAccumulatedElement.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\elements\SoAmbientColorElement.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\elements\SoAnnoText3CharOrientElement.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\elements\SoAnnoText3FontSizeHintElement.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\elements\SoAnnoText3RenderPrintElement.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\elements\SoBBoxModelMatrixElement.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\elements\SoBumpMapElement.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\elements\SoBumpMapCoordinateElement.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\elements\SoBumpMapMatrixElement.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\elements\SoCacheElement.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\elements\SoCacheHintElement.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\elements\SoClipPlaneElement.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\elements\SoComplexityElement.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\elements\SoComplexityTypeElement.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\elements\SoCoordinateElement.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\elements\SoCreaseAngleElement.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\elements\SoCullElement.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\elements\SoDecimationPercentageElement.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\elements\SoDecimationTypeElement.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\elements\SoDiffuseColorElement.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\elements\SoDrawStyleElement.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\elements\SoElement.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\elements\SoEmissiveColorElement.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\elements\SoEnvironmentElement.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\elements\SoFloatElement.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\elements\SoFocalDistanceElement.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\elements\SoFontNameElement.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\elements\SoFontSizeElement.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\elements\SoGLCacheContextElement.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\elements\SoGLClipPlaneElement.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\elements\SoGLColorIndexElement.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\elements\SoGLCoordinateElement.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\elements\SoGLDisplayList.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\elements\SoGLDrawStyleElement.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\elements\SoGLEnvironmentElement.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\elements\SoGLLazyElement.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\elements\SoGLLightIdElement.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\elements\SoGLLinePatternElement.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\elements\SoGLLineWidthElement.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\elements\SoGLModelMatrixElement.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\elements\SoGLMultiTextureCoordinateElement.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\elements\SoGLMultiTextureEnabledElement.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\elements\SoGLMultiTextureMatrixElement.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\elements\SoGLMultiTextureImageElement.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\elements\SoGLNormalElement.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\elements\SoGLNormalizeElement.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\elements\SoGLPointSizeElement.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\elements\SoGLPolygonOffsetElement.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\elements\SoGLProjectionMatrixElement.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\elements\SoGLRenderPassElement.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\elements\SoGLShapeHintsElement.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\elements\SoGLShadeModelElement.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\elements\SoGLTextureCoordinateElement.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\elements\SoGLTextureEnabledElement.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\elements\SoGLTexture3EnabledElement.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\elements\SoGLTextureImageElement.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\elements\SoGLTextureMatrixElement.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\elements\SoGLUpdateAreaElement.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\elements\SoGLViewingMatrixElement.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\elements\SoGLViewportRegionElement.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\elements\SoInt32Element.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\elements\SoLazyElement.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\elements\SoLightAttenuationElement.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\elements\SoLightElement.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\elements\SoLightModelElement.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\elements\SoLinePatternElement.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\elements\SoLineWidthElement.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\elements\SoLocalBBoxMatrixElement.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\elements\SoLongElement.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\elements\SoMaterialBindingElement.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\elements\SoModelMatrixElement.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\elements\SoMultiTextureCoordinateElement.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\elements\SoMultiTextureEnabledElement.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\elements\SoMultiTextureMatrixElement.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\elements\SoMultiTextureImageElement.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\elements\SoNormalBindingElement.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\elements\SoNormalElement.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\elements\SoOverrideElement.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\elements\SoPickRayElement.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\elements\SoPickStyleElement.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\elements\SoPointSizeElement.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\elements\SoPolygonOffsetElement.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\elements\SoProfileCoordinateElement.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\elements\SoProfileElement.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\elements\SoProjectionMatrixElement.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\elements\SoReplacedElement.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\elements\SoShapeHintsElement.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\elements\SoShapeStyleElement.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\elements\SoShininessElement.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\elements\SoSpecularColorElement.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\elements\SoSwitchElement.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\elements\SoTextOutlineEnabledElement.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\elements\SoTextureCombineElement.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\elements\SoTextureCoordinateBindingElement.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\elements\SoTextureCoordinateElement.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\elements\SoTextureEnabledElement.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\elements\SoTexture3EnabledElement.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\elements\SoTextureImageElement.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\elements\SoTextureMatrixElement.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\elements\SoTextureOverrideElement.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\elements\SoTextureQualityElement.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\elements\SoTextureUnitElement.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\elements\SoTransparencyElement.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\elements\SoUnitsElement.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\elements\SoViewVolumeElement.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\elements\SoViewingMatrixElement.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\elements\SoViewportRegionElement.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\elements\SoWindowElement.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\elements\SoListenerPositionElement.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\elements\SoListenerOrientationElement.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\elements\SoListenerGainElement.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\elements\SoListenerDopplerElement.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\elements\SoSoundElement.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\engines\SoSubEngine.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\engines\SoSubNodeEngine.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\engines\SoEngines.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\engines\SoBoolOperation.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\engines\SoCalculator.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\engines\SoCompose.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\engines\SoComposeMatrix.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\engines\SoComposeRotation.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\engines\SoComposeRotationFromTo.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\engines\SoComposeVec2f.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\engines\SoComposeVec3f.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\engines\SoComposeVec4f.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\engines\SoComputeBoundingBox.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\engines\SoConcatenate.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\engines\SoCounter.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\engines\SoDecomposeMatrix.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\engines\SoDecomposeRotation.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\engines\SoDecomposeVec2f.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\engines\SoDecomposeVec3f.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\engines\SoDecomposeVec4f.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\engines\SoElapsedTime.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\engines\SoEngine.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\engines\SoEngineOutput.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\engines\SoFieldConverter.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\engines\SoGate.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\engines\SoInterpolate.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\engines\SoInterpolateFloat.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\engines\SoInterpolateRotation.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\engines\SoInterpolateVec2f.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\engines\SoInterpolateVec3f.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\engines\SoInterpolateVec4f.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\engines\SoNodeEngine.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\engines\SoOnOff.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\engines\SoOneShot.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\engines\SoOutputData.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\engines\SoSelectOne.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\engines\SoTimeCounter.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\engines\SoTransformVec3f.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\engines\SoTriggerAny.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\errors\SoErrors.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\errors\SoDebugError.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\errors\SoError.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\errors\SoMemoryError.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\errors\SoReadError.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\events\SoSubEvent.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\events\SoButtonEvent.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\events\SoEvent.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\events\SoEvents.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\events\SoKeyboardEvent.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\events\SoLocation2Event.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\events\SoMotion3Event.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\events\SoMouseButtonEvent.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\events\SoSpaceballButtonEvent.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\fields\SoSubField.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\fields\SoFields.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\fields\SoField.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\fields\SoFieldContainer.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\fields\SoFieldData.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\fields\SoMFBitMask.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\fields\SoMFBool.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\fields\SoMFColor.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\fields\SoMFEngine.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\fields\SoMFEnum.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\fields\SoMFFloat.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\fields\SoMFInt32.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\fields\SoMFLong.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\fields\SoMFMatrix.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\fields\SoMFName.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\fields\SoMFNode.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\fields\SoMFPath.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\fields\SoMFPlane.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\fields\SoMFRotation.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\fields\SoMFShort.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\fields\SoMFString.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\fields\SoMFTime.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\fields\SoMFUInt32.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\fields\SoMFULong.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\fields\SoMFUShort.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\fields\SoMFVec2f.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\fields\SoMFVec3f.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\fields\SoMFVec3d.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\fields\SoMFVec4f.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\fields\SoMField.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\fields\SoSFBitMask.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\fields\SoSFBool.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\fields\SoSFBox3s.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\fields\SoSFColor.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\fields\SoSFEngine.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\fields\SoSFEnum.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\fields\SoSFFloat.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\fields\SoSFImage.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\fields\SoSFImage3.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\fields\SoSFInt32.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\fields\SoSFLong.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\fields\SoSFMatrix.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\fields\SoSFName.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\fields\SoSFNode.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\fields\SoSFPath.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\fields\SoSFPlane.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\fields\SoSFRotation.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\fields\SoSFShort.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\fields\SoSFString.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\fields\SoSFTime.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\fields\SoSFTrigger.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\fields\SoSFUInt32.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\fields\SoSFULong.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\fields\SoSFUShort.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\fields\SoSFVec2s.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\fields\SoSFVec2f.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\fields\SoSFVec3s.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\fields\SoSFVec3f.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\fields\SoSFVec3d.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\fields\SoSFVec4f.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\fields\SoSField.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\lists\SbList.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\lists\SbPList.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\lists\SbIntList.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\lists\SbVec3fList.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\lists\SbStringList.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\lists\SoActionMethodList.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\lists\SoAuditorList.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\lists\SoBaseList.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\lists\SoCallbackList.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\lists\SoDetailList.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\lists\SoEnabledElementsList.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\lists\SoEngineList.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\lists\SoEngineOutputList.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\lists\SoFieldList.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\lists\SoNodeList.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\lists\SoPathList.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\lists\SoPickedPointList.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\lists\SoTypeList.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\lock\SoLockMgr.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\manips\SoClipPlaneManip.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\manips\SoDirectionalLightManip.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\manips\SoPointLightManip.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\manips\SoSpotLightManip.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\manips\SoTransformManip.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\manips\SoCenterballManip.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\manips\SoHandleBoxManip.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\manips\SoJackManip.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\manips\SoTabBoxManip.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\manips\SoTrackballManip.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\manips\SoTransformBoxManip.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\manips\SoTransformerManip.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\misc\SoAuditorList.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\misc\SoBase.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\misc\SoBasic.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\misc\SoByteStream.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\misc\SoCallbackList.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\misc\SoChildList.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\misc\SoContextHandler.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\misc\SoGLImage.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\misc\SoGLBigImage.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\misc\SoNormalGenerator.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\misc\SoNotification.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\misc\SoNotRec.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\misc\SoProto.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\misc\SoProtoInstance.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\misc\SoTranReceiver.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\misc\SoState.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\misc\SoTranscribe.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\misc\SoTranSender.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\misc\SoLightPath.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\misc\SoTempPath.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\misc\SoGlyph.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\misc\SoAudioDevice.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\nodekits\SoSubKit.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\nodekits\SoNodeKit.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\nodekits\SoNodeKitListPart.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\nodekits\SoNodekitCatalog.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\nodekits\SoBaseKit.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\nodekits\SoAppearanceKit.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\nodekits\SoCameraKit.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\nodekits\SoInteractionKit.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\nodekits\SoLightKit.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\nodekits\SoSceneKit.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\nodekits\SoSeparatorKit.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\nodekits\SoShapeKit.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\nodekits\SoWrapperKit.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\nodes\SoAnnotation.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\nodes\SoAnnoText3.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\nodes\SoAnnoText3Property.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\nodes\SoAntiSquish.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\nodes\SoArray.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\nodes\SoAsciiText.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\nodes\SoBaseColor.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\nodes\SoBlinker.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\nodes\SoBumpMap.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\nodes\SoBumpMapCoordinate.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\nodes\SoBumpMapTransform.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\nodes\SoCacheHint.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\nodes\SoCallback.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\nodes\SoCamera.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\nodes\SoClipPlane.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\nodes\SoColorIndex.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\nodes\SoComplexity.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\nodes\SoCone.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\nodes\SoCoordinate3.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\nodes\SoCoordinate4.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\nodes\SoCube.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\nodes\SoCylinder.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\nodes\SoDirectionalLight.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\nodes\SoDrawStyle.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\nodes\SoEnvironment.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\nodes\SoEventCallback.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\nodes\SoExtSelection.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\nodes\SoFaceSet.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\nodes\SoFaceSet.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\nodes\SoFile.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\nodes\SoFont.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\nodes\SoFontStyle.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\nodes\SoGroup.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\nodes\SoImage.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\nodes\SoIndexedFaceSet.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\nodes\SoIndexedLineSet.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\nodes\SoIndexedNurbsCurve.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\nodes\SoIndexedNurbsSurface.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\nodes\SoIndexedShape.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\nodes\SoIndexedTriangleStripSet.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\nodes\SoInfo.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\nodes\SoLOD.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\nodes\SoLabel.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\nodes\SoLevelOfDetail.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\nodes\SoLight.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\nodes\SoLightModel.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\nodes\SoLineSet.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\nodes\SoLineSet.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\nodes\SoLinearProfile.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\nodes\SoListener.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\nodes\SoLocateHighlight.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\nodes\SoMarkerSet.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\nodes\SoMarkerSet.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\nodes\SoMaterial.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\nodes\SoMaterialBinding.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\nodes\SoMatrixTransform.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\nodes\SoMultipleCopy.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\nodes\SoNode.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\nodes\SoNodes.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\nodes\SoNonIndexedShape.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\nodes\SoNormal.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\nodes\SoNormalBinding.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\nodes\SoNurbsCurve.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\nodes\SoNurbsProfile.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\nodes\SoNurbsSurface.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\nodes\SoOrthographicCamera.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\nodes\SoPackedColor.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\nodes\SoPathSwitch.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\nodes\SoPattern.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\nodes\SoPendulum.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\nodes\SoPerspectiveCamera.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\nodes\SoPickStyle.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\nodes\SoPointLight.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\nodes\SoPointSet.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\nodes\SoPolygonOffset.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\nodes\SoProfile.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\nodes\SoProfileCoordinate2.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\nodes\SoProfileCoordinate3.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\nodes\SoQuadMesh.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\nodes\SoResetTransform.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\nodes\SoRotation.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\nodes\SoRotationXYZ.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\nodes\SoRotor.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\nodes\SoScale.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\nodes\SoSceneTexture2.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\nodes\SoSelection.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\nodes\SoSeparator.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\nodes\SoShape.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\nodes\SoShapeHints.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\nodes\SoShuttle.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\nodes\SoSphere.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\nodes\SoSpotLight.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\nodes\SoSubNode.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\nodes\SoSurroundScale.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\nodes\SoSwitch.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\nodes\SoText2.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\nodes\SoText3.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\nodes\SoTexture2.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\nodes\SoTexture2Transform.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\nodes\SoTexture3.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\nodes\SoTexture3Transform.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\nodes\SoTextureCombine.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\nodes\SoTextureCoordinate2.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\nodes\SoTextureCoordinate3.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\nodes\SoTextureCoordinateBinding.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\nodes\SoTextureCoordinateCube.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\nodes\SoTextureCoordinateCylinder.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\nodes\SoTextureCoordinateSphere.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\nodes\SoTextureCoordinateDefault.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\nodes\SoTextureCoordinateEnvironment.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\nodes\SoTextureCoordinateFunction.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\nodes\SoTextureCoordinatePlane.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\nodes\SoTextureScalePolicy.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\nodes\SoTextureUnit.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\nodes\SoTransform.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\nodes\SoTransformSeparator.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\nodes\SoTransformation.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\nodes\SoTranslation.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\nodes\SoTransparencyType.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\nodes\SoTriangleStripSet.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\nodes\SoUnits.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\nodes\SoVertexProperty.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\nodes\SoVertexShape.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\nodes\SoWWWAnchor.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\nodes\SoWWWInline.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\projectors\SbProjectors.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\projectors\SbCylinderPlaneProjector.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\projectors\SbCylinderProjector.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\projectors\SbCylinderSectionProjector.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\projectors\SbCylinderSheetProjector.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\projectors\SbLineProjector.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\projectors\SbPlaneProjector.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\projectors\SbProjector.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\projectors\SbSpherePlaneProjector.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\projectors\SbSphereProjector.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\projectors\SbSphereSectionProjector.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\projectors\SbSphereSheetProjector.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\sensors\SoSensors.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\sensors\SoAlarmSensor.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\sensors\SoDataSensor.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\sensors\SoDelayQueueSensor.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\sensors\SoFieldSensor.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\sensors\SoIdleSensor.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\sensors\SoNodeSensor.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\sensors\SoOneShotSensor.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\sensors\SoPathSensor.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\sensors\SoSensor.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\sensors\SoSensorManager.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\sensors\SoTimerQueueSensor.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\sensors\SoTimerSensor.h
# End Source File
# Begin Source File

SOURCE=..\..\\build\msvc7\include\Inventor\system\gl.h
# End Source File
# Begin Source File

SOURCE=..\..\\build\msvc7\include\Inventor\system\inttypes.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\threads\SbThread.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\threads\SbMutex.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\threads\SbRWMutex.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\threads\SbCondVar.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\threads\SbStorage.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\threads\SbTypedStorage.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\threads\SbFifo.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\threads\SbBarrier.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\threads\SbThreadAutoLock.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\Sb.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\SbBSPTree.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\SbBasic.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\SbBox.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\SbBox2d.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\SbBox2f.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\SbBox2s.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\SbBox3f.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\SbBox3s.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\SbClip.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\SbColor.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\SbColor4f.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\SbCylinder.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\SbDPLine.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\SbDPLinear.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\SbDPMatrix.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\SbDPPlane.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\SbDPRotation.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\SbDPViewVolume.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\SbDict.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\SbHeap.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\SbImage.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\SbLine.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\SbLinear.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\SbMatrix.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\SbName.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\SbOctTree.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\SbPList.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\SbPlane.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\SbRotation.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\SbSphere.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\SbString.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\SbTesselator.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\SbTime.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\SbVec2d.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\SbVec2f.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\SbVec2s.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\SbVec3d.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\SbVec3f.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\SbVec3s.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\SbVec4d.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\SbVec4f.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\SbViewVolume.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\SbViewportRegion.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\SbXfBox3f.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\So.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\SoDB.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\SoFullPath.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\SoInput.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\SoInteraction.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\SoLists.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\SoNodeKitPath.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\SoOffscreenRenderer.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\SoOutput.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\SoPath.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\SoPickedPoint.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\SoPrimitiveVertex.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\SoSceneManager.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\SoType.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\SoWinEnterScope.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\SoWinLeaveScope.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\SbPList.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\So.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\SoDB.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\SoFullPath.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\SoNodeKitPath.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\SoInput.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\SoInteraction.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\SoOffscreenRenderer.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\SoOutput.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\SoPath.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\SoPickedPoint.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\SoPrimitiveVertex.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\SoSceneManager.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\SoType.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\Sb.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\SbBox.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\SbBSPTree.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\SbLinear.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\SbDPLinear.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\SoLists.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\SbBox2s.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\SbBox3s.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\SbBox2f.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\SbBox2d.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\SbBox3f.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\SbClip.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\SbColor.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\SbColor4f.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\SbCylinder.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\SbDict.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\SbDPLine.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\SbDPLinear.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\SbDPMatrix.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\SbDPPlane.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\SbDPRotation.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\SbHeap.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\SbImage.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\SbLine.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\SbMatrix.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\SbName.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\SbOctTree.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\SbPlane.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\SbRotation.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\SbSphere.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\SbString.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\SbTesselator.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\SbTime.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\SbVec2s.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\SbVec2f.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\SbVec2d.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\SbVec3s.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\SbVec3f.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\SbVec3d.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\SbVec4f.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\SbVec4d.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\SbViewVolume.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\SbDPViewVolume.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\SbViewportRegion.h
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\SbXfBox3f.h
# End Source File
# End Group
# Begin Group "Private Headers"

# PROP Default_Filter "h;ic;icc"
# Begin Source File

SOURCE=..\..\\include\coindefs.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\actions\SoSubActionP.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\C\base\hashp.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\C\base\heapp.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\C\glue\bzip2.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\C\glue\dlp.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\C\glue\freetype.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\C\glue\glp.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\C\glue\GLUWrapper.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\C\glue\gl_agl.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\C\glue\gl_glx.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\C\glue\gl_wgl.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\C\glue\openal_wrapper.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\C\glue\simage_wrapper.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\C\glue\win32api.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\C\glue\zlib.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\C\threads\barrierp.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\C\threads\condvarp.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\C\threads\fifop.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\C\threads\mutexp.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\C\threads\recmutexp.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\C\threads\rwmutexp.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\C\threads\schedp.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\C\threads\storagep.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\C\threads\syncp.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\C\threads\threadp.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\C\threads\threadsutilp.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\C\threads\workerp.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\C\threads\wpoolp.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\C\tidbitsp.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\caches\SoGlyphCache.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\elements\SoTextureScalePolicyElement.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\elements\SoTextureScaleQualityElement.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\engines\SoConvertAll.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\engines\SoSubEngineP.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\engines\SoSubNodeEngineP.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\fields\SoGlobalField.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\fields\SoSubFieldP.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\misc\SoGenerate.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\misc\SoGL.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\misc\SoPick.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\nodekits\SoSubKitP.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\nodes\SoSubNodeP.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\nodes\SoUnknownNode.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\..\\include\Inventor\VRMLnodes\SoVRMLSubInterpolatorP.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\..\\include\unconfig.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\..\\src\engines\evaluator.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\..\\src\fonts\defaultfonts.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\..\\src\io\SoWriterefCounter.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\..\\src\misc\AudioTools.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\..\\src\nodes\SoSoundElementHelper.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\..\\src\shapenodes\soshape_bigtexture.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\..\\src\shapenodes\soshape_primdata.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\..\\src\shapenodes\soshape_trianglesort.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\..\\src\upgraders\SoPackedColorV20.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\..\\src\upgraders\SoShapeHintsV10.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\..\\src\upgraders\SoUpgrader.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\..\\src\engines\so_eval.ic
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\..\\src\misc\cppmangle.icc
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\..\\src\misc\SoGLFaceSetTemplate.icc
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\..\\src\misc\SoGLindexedLineSetTemplate.icc
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\..\\src\misc\SoGLLineSetTemplate.icc
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\..\\src\misc\SoGLnonindexedFaceSetTemplate.icc
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\..\\src\misc\SoGLnonindexedTristripSetTemplate.icc
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\..\\src\misc\SoGLqmeshpreciselightingTemplate.icc
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\..\\src\misc\SoGLqmeshTemplate.icc
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\..\\src\misc\SoGLTristripTemplate.icc
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\..\\src\misc\systemsanity.icc
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\..\\src\threads\condvar_pthread.ic
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\..\\src\threads\condvar_win32.ic
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\..\\src\threads\mutex_pthread.ic
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\..\\src\threads\mutex_win32cs.ic
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\..\\src\threads\mutex_win32mutex.ic
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\..\\src\threads\thread_pthread.ic
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\..\\src\threads\thread_win32.ic
# PROP Exclude_From_Build 1
# End Source File
# End Group
# End Target
# End Project

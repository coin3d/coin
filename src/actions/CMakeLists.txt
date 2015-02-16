# source files
set(COIN_ACTIONS_FILES
	SoAction.cpp
	SoBoxHighlightRenderAction.cpp
	SoCallbackAction.cpp
	SoGLRenderAction.cpp
	SoGetBoundingBoxAction.cpp
	SoGetMatrixAction.cpp
	SoGetPrimitiveCountAction.cpp
	SoHandleEventAction.cpp
	SoLineHighlightRenderAction.cpp
	SoPickAction.cpp
	SoRayPickAction.cpp
	SoReorganizeAction.cpp
	SoSearchAction.cpp
	SoSimplifyAction.cpp
	SoToVRMLAction.cpp
	SoToVRML2Action.cpp
	SoWriteAction.cpp
	SoAudioRenderAction.cpp
)

# build library
if(COIN_BUILD_SINGLE_LIB)
	foreach(SRCFILE ${COIN_ACTIONS_FILES})
		set(COIN_SOURCE ${COIN_SOURCE} actions/${SRCFILE} CACHE INTERNAL "COIN_SOURCE")
	endforeach(SRCFILE)
else()
	add_library(actions OBJECT ${COIN_ACTIONS_FILES})
endif()

# define documentation
foreach(SRCFILE ${COIN_ACTIONS_FILES})
	set(COIN_DOCUMENTATION_FILES ${COIN_DOCUMENTATION_FILES} ${CMAKE_SOURCE_DIR}/src/actions/${SRCFILE} CACHE INTERNAL "COIN_DOCUMENTATION_FILES")
endforeach(SRCFILE)
set(COIN_DOCUMENTATION_FILES
	${COIN_DOCUMENTATION_FILES}
	${CMAKE_SOURCE_DIR}/src/actions/SoActionP.h
	CACHE INTERNAL "COIN_DOCUMENTATION_FILES"
)

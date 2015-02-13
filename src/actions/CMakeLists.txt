if(COIN_BUILD_SINGLE_LIB)
	set(COIN_SOURCE ${COIN_SOURCE}
		actions/SoAction.cpp
		actions/SoBoxHighlightRenderAction.cpp
		actions/SoCallbackAction.cpp
		actions/SoGLRenderAction.cpp
		actions/SoGetBoundingBoxAction.cpp
		actions/SoGetMatrixAction.cpp
		actions/SoGetPrimitiveCountAction.cpp
		actions/SoHandleEventAction.cpp
		actions/SoLineHighlightRenderAction.cpp
		actions/SoPickAction.cpp
		actions/SoRayPickAction.cpp
		actions/SoReorganizeAction.cpp
		actions/SoSearchAction.cpp
		actions/SoSimplifyAction.cpp
		actions/SoToVRMLAction.cpp
		actions/SoToVRML2Action.cpp
		actions/SoWriteAction.cpp
		actions/SoAudioRenderAction.cpp
		CACHE INTERNAL "COIN_SOURCE"
	)
else()
	add_library(actions OBJECT
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
endif()

file(GLOB_RECURSE SOCPP So*.cpp)
set(COIN_DOCUMENTATION_FILES
	${COIN_DOCUMENTATION_FILES}
	${SOCPP}
	${CMAKE_SOURCE_DIR}/src/actions/SoActionP.h
	CACHE INTERNAL "COIN_DOCUMENTATION_FILES"
)

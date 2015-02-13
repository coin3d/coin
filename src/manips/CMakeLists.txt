if(COIN_BUILD_SINGLE_LIB)
	set(COIN_SOURCE ${COIN_SOURCE}
		manips/SoClipPlaneManip.cpp
		manips/SoDirectionalLightManip.cpp
		manips/SoPointLightManip.cpp
		manips/SoSpotLightManip.cpp
		manips/SoTransformManip.cpp
		manips/SoCenterballManip.cpp
		manips/SoHandleBoxManip.cpp
		manips/SoJackManip.cpp
		manips/SoTabBoxManip.cpp
		manips/SoTrackballManip.cpp
		manips/SoTransformBoxManip.cpp
		manips/SoTransformerManip.cpp
		manips/commoncode.cpp
		CACHE INTERNAL "COIN_SOURCE"
	)
else()
	add_library(manips OBJECT
		SoClipPlaneManip.cpp
		SoDirectionalLightManip.cpp
		SoPointLightManip.cpp
		SoSpotLightManip.cpp
		SoTransformManip.cpp
		SoCenterballManip.cpp
		SoHandleBoxManip.cpp
		SoJackManip.cpp
		SoTabBoxManip.cpp
		SoTrackballManip.cpp
		SoTransformBoxManip.cpp
		SoTransformerManip.cpp
		commoncode.cpp
	)
endif()

file(GLOB_RECURSE SOCPP So*.cpp)
set(COIN_DOCUMENTATION_FILES
	${COIN_DOCUMENTATION_FILES}
	${SOCPP}
	${CMAKE_SOURCE_DIR}/src/manips/commoncode.cpp
	CACHE INTERNAL "COIN_DOCUMENTATION_FILES"
)

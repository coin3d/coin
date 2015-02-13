if(COIN_BUILD_SINGLE_LIB)
	set(COIN_SOURCE ${COIN_SOURCE}
		draggers/SoDragger.cpp
		draggers/SoCenterballDragger.cpp
		draggers/SoDirectionalLightDragger.cpp
		draggers/SoDragPointDragger.cpp
		draggers/SoHandleBoxDragger.cpp
		draggers/SoJackDragger.cpp
		draggers/SoPointLightDragger.cpp
		draggers/SoRotateCylindricalDragger.cpp
		draggers/SoRotateDiscDragger.cpp
		draggers/SoRotateSphericalDragger.cpp
		draggers/SoScale1Dragger.cpp
		draggers/SoScale2Dragger.cpp
		draggers/SoScale2UniformDragger.cpp
		draggers/SoScaleUniformDragger.cpp
		draggers/SoSpotLightDragger.cpp
		draggers/SoTabBoxDragger.cpp
		draggers/SoTabPlaneDragger.cpp
		draggers/SoTrackballDragger.cpp
		draggers/SoTransformBoxDragger.cpp
		draggers/SoTransformerDragger.cpp
		draggers/SoTranslate1Dragger.cpp
		draggers/SoTranslate2Dragger.cpp
		CACHE INTERNAL "COIN_SOURCE"
	)
else()
	add_library(draggers OBJECT
		SoDragger.cpp
		SoCenterballDragger.cpp
		SoDirectionalLightDragger.cpp
		SoDragPointDragger.cpp
		SoHandleBoxDragger.cpp
		SoJackDragger.cpp
		SoPointLightDragger.cpp
		SoRotateCylindricalDragger.cpp
		SoRotateDiscDragger.cpp
		SoRotateSphericalDragger.cpp
		SoScale1Dragger.cpp
		SoScale2Dragger.cpp
		SoScale2UniformDragger.cpp
		SoScaleUniformDragger.cpp
		SoSpotLightDragger.cpp
		SoTabBoxDragger.cpp
		SoTabPlaneDragger.cpp
		SoTrackballDragger.cpp
		SoTransformBoxDragger.cpp
		SoTransformerDragger.cpp
		SoTranslate1Dragger.cpp
		SoTranslate2Dragger.cpp
	)
endif()

file(GLOB_RECURSE SOCPP So*.cpp)
set(COIN_DOCUMENTATION_FILES
	${COIN_DOCUMENTATION_FILES}
	${SOCPP}
	CACHE INTERNAL "COIN_DOCUMENTATION_FILES"
)

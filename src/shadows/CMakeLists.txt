if(COIN_BUILD_SINGLE_LIB)
	set(COIN_SOURCE ${COIN_SOURCE}
		shadows/SoShadowGroup.cpp
		shadows/SoShadowStyle.cpp
		shadows/SoShadowSpotLight.cpp
		shadows/SoShadowDirectionalLight.cpp
		shadows/SoShadowStyleElement.cpp
		shadows/SoShadowCulling.cpp
		shadows/SoGLShadowCullingElement.cpp
		CACHE INTERNAL "COIN_SOURCE"
	)
else()
	add_library(shadows OBJECT
		SoShadowGroup.cpp
		SoShadowStyle.cpp
		SoShadowSpotLight.cpp
		SoShadowDirectionalLight.cpp
		SoShadowStyleElement.cpp
		SoShadowCulling.cpp
		SoGLShadowCullingElement.cpp
	)
endif()

file(GLOB_RECURSE SOCPP So*.cpp)
set(COIN_DOCUMENTATION_FILES
	${COIN_DOCUMENTATION_FILES}
	${SOCPP}
	CACHE INTERNAL "COIN_DOCUMENTATION_FILES"
)

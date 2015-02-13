if(COIN_BUILD_SINGLE_LIB)
	set(COIN_SOURCE ${COIN_SOURCE}
		rendering/SoGL.cpp
		rendering/SoGLBigImage.cpp
		rendering/SoGLDriverDatabase.cpp
		rendering/SoGLImage.cpp
		rendering/SoGLCubeMapImage.cpp
		rendering/SoGLNurbs.cpp
		rendering/SoRenderManager.cpp
		rendering/SoRenderManagerP.cpp
		rendering/SoOffscreenRenderer.cpp
		rendering/SoOffscreenCGData.cpp
		rendering/SoOffscreenGLXData.cpp
		rendering/SoOffscreenWGLData.cpp
		rendering/SoVBO.cpp
		rendering/SoVertexArrayIndexer.cpp
		rendering/CoinOffscreenGLCanvas.cpp
		CACHE INTERNAL "COIN_SOURCE"
	)
else()
	add_library(rendering OBJECT
		SoGL.cpp
		SoGLBigImage.cpp
		SoGLDriverDatabase.cpp
		SoGLImage.cpp
		SoGLCubeMapImage.cpp
		SoGLNurbs.cpp
		SoRenderManager.cpp
		SoRenderManagerP.cpp
		SoOffscreenRenderer.cpp
		SoOffscreenCGData.cpp
		SoOffscreenGLXData.cpp
		SoOffscreenWGLData.cpp
		SoVBO.cpp
		SoVertexArrayIndexer.cpp
		CoinOffscreenGLCanvas.cpp
	)
endif()

file(GLOB_RECURSE SOCPP So*.cpp)
set(COIN_DOCUMENTATION_FILES
	${COIN_DOCUMENTATION_FILES}
	${SOCPP}
	${CMAKE_SOURCE_DIR}/src/rendering/SoVBO.h
	CACHE INTERNAL "COIN_DOCUMENTATION_FILES"
)

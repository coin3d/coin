if(COIN_BUILD_SINGLE_LIB)
	set(COIN_SOURCE ${COIN_SOURCE}
		caches/SoBoundingBoxCache.cpp
		caches/SoCache.cpp
		caches/SoConvexDataCache.cpp
		caches/SoGLCacheList.cpp
		caches/SoGLRenderCache.cpp
		caches/SoNormalCache.cpp
		caches/SoTextureCoordinateCache.cpp
		caches/SoPrimitiveVertexCache.cpp
		caches/SoGlyphCache.cpp
		caches/SoShaderProgramCache.cpp
		caches/SoVBOCache.cpp
		CACHE INTERNAL "COIN_SOURCE"
	)
else()
	add_library(caches OBJECT
		SoBoundingBoxCache.cpp
		SoCache.cpp
		SoConvexDataCache.cpp
		SoGLCacheList.cpp
		SoGLRenderCache.cpp
		SoNormalCache.cpp
		SoTextureCoordinateCache.cpp
		SoPrimitiveVertexCache.cpp
		SoGlyphCache.cpp
		SoShaderProgramCache.cpp
		SoVBOCache.cpp
	)
endif()

file(GLOB_RECURSE SOCPP So*.cpp)
set(COIN_DOCUMENTATION_FILES
	${COIN_DOCUMENTATION_FILES}
	${SOCPP}
	${CMAKE_SOURCE_DIR}/src/caches/SoGlyphCache.h
	${CMAKE_SOURCE_DIR}/src/caches/SoVBOCache.h
	CACHE INTERNAL "COIN_DOCUMENTATION_FILES"
)

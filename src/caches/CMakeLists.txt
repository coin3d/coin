# source files
set(COIN_CACHES_FILES
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

# build library
if(COIN_BUILD_SINGLE_LIB)
	foreach(SRCFILE ${COIN_CACHES_FILES})
		set(COIN_SOURCE ${COIN_SOURCE} caches/${SRCFILE} CACHE INTERNAL "COIN_SOURCE")
	endforeach(SRCFILE)
else()
	add_library(caches OBJECT ${COIN_CACHES_FILES})
endif()

# define documentation
foreach(SRCFILE ${COIN_CACHES_FILES})
	set(COIN_DOCUMENTATION_FILES ${COIN_DOCUMENTATION_FILES} ${CMAKE_SOURCE_DIR}/src/caches/${SRCFILE} CACHE INTERNAL "COIN_DOCUMENTATION_FILES")
endforeach(SRCFILE)
set(COIN_DOCUMENTATION_FILES
	${COIN_DOCUMENTATION_FILES}
	${CMAKE_SOURCE_DIR}/src/caches/SoGlyphCache.h
	${CMAKE_SOURCE_DIR}/src/caches/SoVBOCache.h
	CACHE INTERNAL "COIN_DOCUMENTATION_FILES"
)

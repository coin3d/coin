# source files
set(COIN_GLUE_FILES
	cg.cpp
	spidermonkey.cpp
	dl.cpp
	gl.cpp
	gl_wgl.cpp
	gl_agl.cpp
	gl_cgl.cpp
	gl_glx.cpp
	GLUWrapper.cpp
	simage_wrapper.cpp
	openal_wrapper.cpp
	win32api.cpp
	zlib.cpp
	bzip2.cpp
	freetype.cpp
	normalization_cubemap.cpp
)

# build library
if(COIN_BUILD_SINGLE_LIB)
	foreach(SRCFILE ${COIN_GLUE_FILES})
		set(COIN_SOURCE ${COIN_SOURCE} glue/${SRCFILE} CACHE INTERNAL "COIN_SOURCE")
	endforeach(SRCFILE)
else()
	add_library(glue OBJECT ${COIN_GLUE_FILES})
endif()

# define documentation
set(COIN_DOCUMENTATION_FILES
	${COIN_DOCUMENTATION_FILES}
	${CMAKE_SOURCE_DIR}/src/glue/dl.cpp
	${CMAKE_SOURCE_DIR}/src/glue/gl.cpp
	${CMAKE_SOURCE_DIR}/src/glue/spidermonkey.cpp
	CACHE INTERNAL "COIN_DOCUMENTATION_FILES"
)

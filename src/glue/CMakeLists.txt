if(COIN_BUILD_SINGLE_LIB)
	set(COIN_SOURCE ${COIN_SOURCE}
		glue/cg.cpp
		glue/spidermonkey.cpp
		glue/dl.cpp
		glue/gl.cpp
		glue/gl_wgl.cpp
		glue/gl_agl.cpp
		glue/gl_cgl.cpp
		glue/gl_glx.cpp
		glue/GLUWrapper.cpp
		glue/simage_wrapper.cpp
		glue/openal_wrapper.cpp
		glue/win32api.cpp
		glue/zlib.cpp
		glue/bzip2.cpp
		glue/freetype.cpp
		glue/normalization_cubemap.cpp
		CACHE INTERNAL "COIN_SOURCE"
	)
else()
	add_library(glue OBJECT
		cg.cpp
		spidermonkey.cpp
		dl.cpp
		gl.cpp
		gl_wgl.cpp gl_agl.cpp gl_cgl.cpp gl_glx.cpp
		GLUWrapper.cpp
		simage_wrapper.cpp
		openal_wrapper.cpp
		win32api.cpp
		zlib.cpp
		bzip2.cpp
		freetype.cpp
		normalization_cubemap.cpp
	)
endif()

set(COIN_DOCUMENTATION_FILES
	${COIN_DOCUMENTATION_FILES}
	${CMAKE_SOURCE_DIR}/src/glue/dl.cpp
	${CMAKE_SOURCE_DIR}/src/glue/gl.cpp
	${CMAKE_SOURCE_DIR}/src/glue/spidermonkey.cpp
	CACHE INTERNAL "COIN_DOCUMENTATION_FILES"
)

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

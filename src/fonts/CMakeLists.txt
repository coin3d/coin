# source files
set(COIN_FONTS_FILES
	fontlib_wrapper.cpp
	win32.cpp
	freetype.cpp
	coretext.cpp
	fontspec.cpp
	glyph.cpp
	glyph2d.cpp
	glyph3d.cpp
	default3dfont.cpp
	default2dfont.cpp
	common.cpp
)

# build library
if(COIN_BUILD_SINGLE_LIB)
	foreach(SRCFILE ${COIN_FONTS_FILES})
		set(COIN_SOURCE ${COIN_SOURCE} fonts/${SRCFILE} CACHE INTERNAL "COIN_SOURCE")
	endforeach(SRCFILE)
else()
	add_library(fonts OBJECT ${COIN_FONTS_FILES})
endif()

# define documentation
foreach(SRCFILE ${COIN_FONTS_FILES})
	set(COIN_DOCUMENTATION_FILES ${COIN_DOCUMENTATION_FILES} ${CMAKE_SOURCE_DIR}/src/fonts/${SRCFILE} CACHE INTERNAL "COIN_DOCUMENTATION_FILES")
endforeach(SRCFILE)

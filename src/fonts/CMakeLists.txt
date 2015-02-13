if(COIN_BUILD_SINGLE_LIB)
	set(COIN_SOURCE ${COIN_SOURCE}
		fonts/fontlib_wrapper.cpp
		fonts/win32.cpp
		fonts/freetype.cpp
		fonts/coretext.cpp
		fonts/fontspec.cpp
		fonts/glyph.cpp
		fonts/glyph2d.cpp
		fonts/glyph3d.cpp
		fonts/default3dfont.cpp
		fonts/default2dfont.cpp
		fonts/common.cpp
		CACHE INTERNAL "COIN_SOURCE"
	)
else()
	add_library(fonts OBJECT
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
endif()

set(COIN_DOCUMENTATION_FILES
	${COIN_DOCUMENTATION_FILES}
	${CMAKE_SOURCE_DIR}/src/cfonts/fontlib_wrapper.cpp
	CACHE INTERNAL "COIN_DOCUMENTATION_FILES"
)

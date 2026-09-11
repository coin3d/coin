include(CheckCXXSourceCompiles)
include(CMakePushCheckState)

function(_coin_require_core_header_rejection header result_variable description)
	check_cxx_source_compiles("#include <${header}>
#include \"glue/gl-core.h\"
int main() { return 0; }
" ${result_variable})
	if(${result_variable})
		message(FATAL_ERROR "Core-profile mode accepted ${description}")
	endif()
endfunction()

function(coin_check_core_gl_headers)
	cmake_push_check_state(RESET)
	set(CMAKE_REQUIRED_DEFINITIONS
		-DCOIN_INTERNAL
		-DCOIN_BUILDING_COIN
	)
	set(CMAKE_REQUIRED_INCLUDES
		${PROJECT_BINARY_DIR}/include
		${PROJECT_SOURCE_DIR}/include
		${PROJECT_SOURCE_DIR}/src
		${PROJECT_SOURCE_DIR}/src/glue/khronos
	)

	# The private path must compile on its own, expose core declarations, and
	# omit compatibility-only declarations supplied by gl-fallbacks.h.
	check_cxx_source_compiles([[#include "glue/gl-core.h"
#ifndef GL_VERSION_3_0
#error Core OpenGL declarations were not selected.
#endif
#ifdef GL_CLIENT_ACTIVE_TEXTURE
#error Legacy compatibility declarations leaked into core mode.
#endif
int main() { return 0; }
]] COIN_PRIVATE_CORE_GL_HEADER_COMPILES)
	if(NOT COIN_PRIVATE_CORE_GL_HEADER_COMPILES)
		message(FATAL_ERROR "Coin's private core-profile OpenGL header mode does not compile")
	endif()

	# On desktop Unix, prove that compatibility headers included before Coin's
	# abstraction are diagnosed instead of being silently mixed with glcorearb.
	if(NOT APPLE AND NOT WIN32)
		_coin_require_core_header_rejection(
			GL/gl.h
			COIN_PRIVATE_CORE_WITH_LEGACY_GL_H_COMPILES
			"a prior legacy GL header"
		)
		_coin_require_core_header_rejection(
			GL/glext.h
			COIN_PRIVATE_CORE_WITH_COMPAT_GLEXT_H_COMPILES
			"a prior compatibility glext header"
		)
	endif()

	cmake_pop_check_state()
endfunction()

coin_check_core_gl_headers()

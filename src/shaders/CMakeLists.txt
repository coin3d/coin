# source files
set(COIN_SHADERS_FILES
	SoFragmentShader.cpp
	SoGeometryShader.cpp
	SoGLARBShaderParameter.cpp
	SoGLARBShaderObject.cpp
	SoGLARBShaderProgram.cpp
	SoGLCgShaderObject.cpp
	SoGLCgShaderParameter.cpp
	SoGLCgShaderProgram.cpp
	SoGLSLShaderParameter.cpp
	SoGLSLShaderObject.cpp
	SoGLSLShaderProgram.cpp
	SoGLShaderObject.cpp
	SoGLShaderParameter.cpp
	SoGLShaderProgram.cpp
	SoGLShaderProgramElement.cpp
	SoShaderObject.cpp
	SoShaderParameter.cpp
	SoShaderProgram.cpp
	SoShader.cpp
	SoVertexShader.cpp
)

# build library
if(COIN_BUILD_SINGLE_LIB)
	foreach(SRCFILE ${COIN_SHADERS_FILES})
		set(COIN_SOURCE ${COIN_SOURCE} shaders/${SRCFILE} CACHE INTERNAL "COIN_SOURCE")
	endforeach(SRCFILE)
else()
	add_library(shaders OBJECT ${COIN_SHADERS_FILES})
endif()

# define documentation
foreach(SRCFILE ${COIN_SHADERS_FILES})
	set(COIN_DOCUMENTATION_FILES ${COIN_DOCUMENTATION_FILES} ${CMAKE_SOURCE_DIR}/src/shaders/${SRCFILE} CACHE INTERNAL "COIN_DOCUMENTATION_FILES")
endforeach(SRCFILE)
set(COIN_DOCUMENTATION_FILES
	${COIN_DOCUMENTATION_FILES}
	${CMAKE_SOURCE_DIR}/src/shaders/SoShader.h
	CACHE INTERNAL "COIN_DOCUMENTATION_FILES"
)

if(COIN_BUILD_SINGLE_LIB)
	set(COIN_SOURCE ${COIN_SOURCE}
		errors/error.cpp
		errors/debugerror.cpp
		errors/SoDebugError.cpp
		errors/SoError.cpp
		errors/SoMemoryError.cpp
		errors/SoReadError.cpp
		CACHE INTERNAL "COIN_SOURCE"
	)
else()
	add_library(errors OBJECT
		error.cpp
		debugerror.cpp
		SoDebugError.cpp
		SoError.cpp
		SoMemoryError.cpp
		SoReadError.cpp
	)
endif()

file(GLOB_RECURSE SOCPP So*.cpp)
set(COIN_DOCUMENTATION_FILES
	${COIN_DOCUMENTATION_FILES}
	${SOCPP}
	${CMAKE_SOURCE_DIR}/src/errors/debugerror.cpp
	${CMAKE_SOURCE_DIR}/src/errors/error.cpp
	CACHE INTERNAL "COIN_DOCUMENTATION_FILES"
)

if(COIN_BUILD_SINGLE_LIB)
	set(COIN_SOURCE ${COIN_SOURCE}
		collision/SbTri3f.cpp
		collision/SoIntersectionDetectionAction.cpp
		CACHE INTERNAL "COIN_SOURCE"
	)
else()
	add_library(collision OBJECT
		SbTri3f.cpp
		SoIntersectionDetectionAction.cpp
	)
endif()

set(COIN_DOCUMENTATION_FILES
	${COIN_DOCUMENTATION_FILES}
	${CMAKE_SOURCE_DIR}/src/collision/SbTri3f.cpp
	${CMAKE_SOURCE_DIR}/src/collision/SbTri3f.h
	${CMAKE_SOURCE_DIR}/src/collision/SoIntersectionDetectionAction.cpp
	CACHE INTERNAL "COIN_DOCUMENTATION_FILES"
)

if(COIN_BUILD_SINGLE_LIB)
	set(COIN_SOURCE ${COIN_SOURCE}
		hardcopy/HardCopy.cpp
		hardcopy/PSVectorOutput.cpp
		hardcopy/VectorOutput.cpp
		hardcopy/VectorizeAction.cpp
		hardcopy/VectorizeActionP.cpp
		hardcopy/VectorizePSAction.cpp
		CACHE INTERNAL "COIN_SOURCE"
	)
else()
	add_library(hardcopy OBJECT
		HardCopy.cpp
		PSVectorOutput.cpp
		VectorOutput.cpp
		VectorizeAction.cpp
		VectorizeActionP.cpp
		VectorizePSAction.cpp
	)
endif()

set(COIN_DOCUMENTATION_FILES
	${COIN_DOCUMENTATION_FILES}
	${CMAKE_SOURCE_DIR}/src/hardcopy/HardCopy.cpp
	${CMAKE_SOURCE_DIR}/src/hardcopy/PSVectorOutput.cpp
	${CMAKE_SOURCE_DIR}/src/hardcopy/VectorizeAction.cpp
	${CMAKE_SOURCE_DIR}/src/hardcopy/VectorizePSAction.cpp
	${CMAKE_SOURCE_DIR}/src/hardcopy/VectorOutput.cpp
	CACHE INTERNAL "COIN_DOCUMENTATION_FILES"
)

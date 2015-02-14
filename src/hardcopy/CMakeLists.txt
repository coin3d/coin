# source files
set(COIN_HARDCOPY_FILES
	HardCopy.cpp
	PSVectorOutput.cpp
	VectorOutput.cpp
	VectorizeAction.cpp
	VectorizeActionP.cpp
	VectorizePSAction.cpp
)

# build library
if(COIN_BUILD_SINGLE_LIB)
	foreach(SRCFILE ${COIN_HARDCOPY_FILES})
		set(COIN_SOURCE ${COIN_SOURCE} hardcopy/${SRCFILE} CACHE INTERNAL "COIN_SOURCE")
	endforeach(SRCFILE)
else()
	add_library(hardcopy OBJECT ${COIN_HARDCOPY_FILES})
endif()

# define documentation
set(COIN_DOCUMENTATION_FILES
	${COIN_DOCUMENTATION_FILES}
	${CMAKE_SOURCE_DIR}/src/hardcopy/HardCopy.cpp
	${CMAKE_SOURCE_DIR}/src/hardcopy/PSVectorOutput.cpp
	${CMAKE_SOURCE_DIR}/src/hardcopy/VectorizeAction.cpp
	${CMAKE_SOURCE_DIR}/src/hardcopy/VectorizePSAction.cpp
	${CMAKE_SOURCE_DIR}/src/hardcopy/VectorOutput.cpp
	CACHE INTERNAL "COIN_DOCUMENTATION_FILES"
)

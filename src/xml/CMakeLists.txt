add_subdirectory(expat)

# source files
set(COIN_XML_FILES
	document.cpp
	element.cpp
	attribute.cpp
	entity.cpp
	utils.cpp
	path.cpp
)

# build library
if(COIN_BUILD_SINGLE_LIB)
	foreach(SRCFILE ${COIN_XML_FILES})
		set(COIN_SOURCE ${COIN_SOURCE} xml/${SRCFILE} CACHE INTERNAL "COIN_SOURCE")
	endforeach(SRCFILE)
else()
	add_library(xml OBJECT ${COIN_XML_FILES})
endif()

# define documentation
set(COIN_DOCUMENTATION_FILES
	${COIN_DOCUMENTATION_FILES}
	${CMAKE_SOURCE_DIR}/src/xml/attribute.cpp
	${CMAKE_SOURCE_DIR}/src/xml/document.cpp
	${CMAKE_SOURCE_DIR}/src/xml/element.cpp
	${CMAKE_SOURCE_DIR}/src/xml/types.cpp
	CACHE INTERNAL "COIN_DOCUMENTATION_FILES"
)

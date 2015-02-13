add_subdirectory(expat)

if(COIN_BUILD_SINGLE_LIB)
	set(COIN_SOURCE ${COIN_SOURCE}
		xml/document.cpp
		xml/element.cpp
		xml/attribute.cpp
		xml/entity.cpp
		xml/utils.cpp
		xml/path.cpp
		CACHE INTERNAL "COIN_SOURCE"
	)

else()
	add_library(xml OBJECT
        document.cpp
        element.cpp
        attribute.cpp
        entity.cpp
        utils.cpp
        path.cpp
	)
endif()

set(COIN_DOCUMENTATION_FILES
	${COIN_DOCUMENTATION_FILES}
	${CMAKE_SOURCE_DIR}/src/xml/attribute.cpp \\
	${CMAKE_SOURCE_DIR}/src/xml/document.cpp \\
	${CMAKE_SOURCE_DIR}/src/xml/element.cpp \\
	${CMAKE_SOURCE_DIR}/src/xml/types.cpp
	CACHE INTERNAL "COIN_DOCUMENTATION_FILES"
)

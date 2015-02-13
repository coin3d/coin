if(COIN_BUILD_SINGLE_LIB)
	set(COIN_SOURCE ${COIN_SOURCE}
		foreignfiles/SoForeignFileKit.cpp
		foreignfiles/SoSTLFileKit.cpp
		foreignfiles/steel-wrapper.cpp
		CACHE INTERNAL "COIN_SOURCE"
	)
else()
	add_library(foreignfiles OBJECT
		SoForeignFileKit.cpp
		SoSTLFileKit.cpp
		steel-wrapper.cpp
	)
endif()

file(GLOB_RECURSE SOCPP So*.cpp)
set(COIN_DOCUMENTATION_FILES
	${COIN_DOCUMENTATION_FILES}
	${SOCPP}
	CACHE INTERNAL "COIN_DOCUMENTATION_FILES"
)

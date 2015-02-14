# source files
set(COIN_FOREIGNFILES_FILES
	SoForeignFileKit.cpp
	SoSTLFileKit.cpp
	steel-wrapper.cpp
)

# build library
if(COIN_BUILD_SINGLE_LIB)
	foreach(SRCFILE ${COIN_FOREIGNFILES_FILES})
		set(COIN_SOURCE ${COIN_SOURCE} foreignfiles/${SRCFILE} CACHE INTERNAL "COIN_SOURCE")
	endforeach(SRCFILE)
else()
	add_library(foreignfiles OBJECT ${COIN_FOREIGNFILES_FILES})
endif()

# define documentation
file(GLOB_RECURSE SOCPP So*.cpp)
set(COIN_DOCUMENTATION_FILES
	${COIN_DOCUMENTATION_FILES}
	${SOCPP}
	CACHE INTERNAL "COIN_DOCUMENTATION_FILES"
)

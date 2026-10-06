# source files
set(COIN_EXPAT_FILES
	xcs.c
	xmlparse.c
	xmlrole.c
	xmltok.c
)

if(WIN32)
	list(APPEND COIN_EXPAT_FILES random_rand_s.c)
else()
	list(APPEND COIN_EXPAT_FILES random_dev_urandom.c)
endif()

# build library
if(COIN_BUILD_SINGLE_LIB)
	foreach(SRCFILE ${COIN_EXPAT_FILES})
		set(COIN_SOURCE ${COIN_SOURCE} xml/expat/${SRCFILE} CACHE INTERNAL "COIN_SOURCE")
	endforeach(SRCFILE)
else()
	add_library(expat OBJECT ${COIN_EXPAT_FILES})
endif()

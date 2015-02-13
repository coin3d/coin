if(COIN_BUILD_SINGLE_LIB)
	set(COIN_SOURCE ${COIN_SOURCE}
		xml/expat/xmlparse.c
		xml/expat/xmlrole.c
		xml/expat/xmltok.c
		CACHE INTERNAL "COIN_SOURCE"
	)
else()
	add_library(expat OBJECT
		xmlparse.c
		xmlrole.c
		xmltok.c
	)
endif()

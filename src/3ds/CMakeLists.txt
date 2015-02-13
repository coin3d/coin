if(COIN_BUILD_SINGLE_LIB)
	set(COIN_SOURCE ${COIN_SOURCE}
		3ds/3dsLoader.cpp
		3ds/SoStream.cpp
		CACHE INTERNAL "COIN_SOURCE"
	)
else()
	add_library(3ds OBJECT
		3dsLoader.cpp
		SoStream.cpp
	)
endif()

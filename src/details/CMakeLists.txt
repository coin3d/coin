if(COIN_BUILD_SINGLE_LIB)
	set(COIN_SOURCE ${COIN_SOURCE}
		details/SoDetail.cpp
		details/SoConeDetail.cpp
		details/SoCubeDetail.cpp
		details/SoCylinderDetail.cpp
		details/SoFaceDetail.cpp
		details/SoLineDetail.cpp
		details/SoNodeKitDetail.cpp
		details/SoPointDetail.cpp
		details/SoTextDetail.cpp
		CACHE INTERNAL "COIN_SOURCE"
	)
else()
	add_library(details OBJECT
		SoDetail.cpp
		SoConeDetail.cpp
		SoCubeDetail.cpp
		SoCylinderDetail.cpp
		SoFaceDetail.cpp
		SoLineDetail.cpp
		SoNodeKitDetail.cpp
		SoPointDetail.cpp
		SoTextDetail.cpp
	)
endif()

file(GLOB_RECURSE SOCPP So*.cpp)
set(COIN_DOCUMENTATION_FILES
	${COIN_DOCUMENTATION_FILES}
	${SOCPP}
	CACHE INTERNAL "COIN_DOCUMENTATION_FILES"
)

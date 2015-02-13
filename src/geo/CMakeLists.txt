if(COIN_BUILD_SINGLE_LIB)
	set(COIN_SOURCE ${COIN_SOURCE}
		geo/SoGeo.cpp
		geo/SoGeoOrigin.cpp
		geo/SoGeoLocation.cpp
		geo/SoGeoElement.cpp
		geo/SoGeoSeparator.cpp
		geo/SoGeoCoordinate.cpp
		geo/SbGeoAngle.cpp
		geo/SbGeoEllipsoid.cpp
		geo/SbGeoProjection.cpp
		geo/SbPolarStereographic.cpp
		geo/SbUTMProjection.cpp
		CACHE INTERNAL "COIN_SOURCE"
	)
else()
	add_library(geo OBJECT
		SoGeo.cpp
		SoGeoOrigin.cpp
		SoGeoLocation.cpp
		SoGeoElement.cpp
		SoGeoSeparator.cpp
		SoGeoCoordinate.cpp
		SbGeoAngle.cpp
		SbGeoEllipsoid.cpp
		SbGeoProjection.cpp
		SbPolarStereographic.cpp
		SbUTMProjection.cpp
	)
endif()

file(GLOB_RECURSE SOCPP So*.cpp)
set(COIN_DOCUMENTATION_FILES
	${COIN_DOCUMENTATION_FILES}
	${SOCPP}
	CACHE INTERNAL "COIN_DOCUMENTATION_FILES"
)

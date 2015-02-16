# source files
set(COIN_GEO_FILES
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

# build library
if(COIN_BUILD_SINGLE_LIB)
	foreach(SRCFILE ${COIN_GEO_FILES})
		set(COIN_SOURCE ${COIN_SOURCE} geo/${SRCFILE} CACHE INTERNAL "COIN_SOURCE")
	endforeach(SRCFILE)
else()
	add_library(geo OBJECT ${COIN_GEO_FILES})
endif()

# define documentation
foreach(SRCFILE ${COIN_GEO_FILES})
	set(COIN_DOCUMENTATION_FILES ${COIN_DOCUMENTATION_FILES} ${CMAKE_SOURCE_DIR}/src/geo/${SRCFILE} CACHE INTERNAL "COIN_DOCUMENTATION_FILES")
endforeach(SRCFILE)

# source files
set(COIN_NAVIGATION_FILES
	SoCameraUtils.cpp
	SoScXMLNavigation.cpp
	SoScXMLNavigationTarget.cpp
	SoScXMLPanTarget.cpp
	SoScXMLRotateTarget.cpp
	SoScXMLSeekTarget.cpp
	SoScXMLSpinTarget.cpp
	SoScXMLZoomTarget.cpp
	SoScXMLDollyTarget.cpp
	SoScXMLMiscTarget.cpp
	SoScXMLFlightControlTarget.cpp
	SoScXMLMotionTarget.cpp
)

# build library
if(COIN_BUILD_SINGLE_LIB)
	foreach(SRCFILE ${COIN_NAVIGATION_FILES})
		set(COIN_SOURCE ${COIN_SOURCE} navigation/${SRCFILE} CACHE INTERNAL "COIN_SOURCE")
	endforeach(SRCFILE)
else()
	add_library(navigation OBJECT ${COIN_NAVIGATION_FILES})
endif()

# define documentation
file(GLOB_RECURSE SOCPP So*.cpp)
set(COIN_DOCUMENTATION_FILES
	${COIN_DOCUMENTATION_FILES}
	${SOCPP}
	CACHE INTERNAL "COIN_DOCUMENTATION_FILES"
)

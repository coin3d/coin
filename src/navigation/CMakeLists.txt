if(COIN_BUILD_SINGLE_LIB)
	set(COIN_SOURCE ${COIN_SOURCE}
        navigation/SoCameraUtils.cpp
        navigation/SoScXMLNavigation.cpp
        navigation/SoScXMLNavigationTarget.cpp
        navigation/SoScXMLPanTarget.cpp
        navigation/SoScXMLRotateTarget.cpp
        navigation/SoScXMLSeekTarget.cpp
        navigation/SoScXMLSpinTarget.cpp
        navigation/SoScXMLZoomTarget.cpp
        navigation/SoScXMLDollyTarget.cpp
        navigation/SoScXMLMiscTarget.cpp
        navigation/SoScXMLFlightControlTarget.cpp
        navigation/SoScXMLMotionTarget.cpp
		CACHE INTERNAL "COIN_SOURCE"
	)
else()
	add_library(navigation OBJECT
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
endif()

file(GLOB_RECURSE SOCPP So*.cpp)
set(COIN_DOCUMENTATION_FILES
	${COIN_DOCUMENTATION_FILES}
	${SOCPP}
	CACHE INTERNAL "COIN_DOCUMENTATION_FILES"
)

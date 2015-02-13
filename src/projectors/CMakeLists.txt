if(COIN_BUILD_SINGLE_LIB)
	set(COIN_SOURCE ${COIN_SOURCE}
		projectors/SbCylinderPlaneProjector.cpp
		projectors/SbCylinderProjector.cpp
		projectors/SbCylinderSectionProjector.cpp
		projectors/SbCylinderSheetProjector.cpp
		projectors/SbLineProjector.cpp
		projectors/SbPlaneProjector.cpp
		projectors/SbProjector.cpp
		projectors/SbSpherePlaneProjector.cpp
		projectors/SbSphereProjector.cpp
		projectors/SbSphereSectionProjector.cpp
		projectors/SbSphereSheetProjector.cpp
		CACHE INTERNAL "COIN_SOURCE"
	)
else()
	add_library(projectors OBJECT
		SbCylinderPlaneProjector.cpp
		SbCylinderProjector.cpp
		SbCylinderSectionProjector.cpp
		SbCylinderSheetProjector.cpp
		SbLineProjector.cpp
		SbPlaneProjector.cpp
		SbProjector.cpp
		SbSpherePlaneProjector.cpp
		SbSphereProjector.cpp
		SbSphereSectionProjector.cpp
		SbSphereSheetProjector.cpp
	)
endif()

file(GLOB_RECURSE SBCPP Sb*.cpp)
set(COIN_DOCUMENTATION_FILES
	${COIN_DOCUMENTATION_FILES}
	${SBCPP}
	CACHE INTERNAL "COIN_DOCUMENTATION_FILES"
)

if(COIN_BUILD_SINGLE_LIB)
	set(COIN_SOURCE ${COIN_SOURCE}
        upgraders/SoUpgrader.cpp
        upgraders/SoPackedColorV20.cpp
        upgraders/SoShapeHintsV10.cpp
		CACHE INTERNAL "COIN_SOURCE"
	)
else()
	add_library(upgraders OBJECT
        SoUpgrader.cpp
        SoPackedColorV20.cpp
        SoShapeHintsV10.cpp
	)
endif()

file(GLOB_RECURSE SOCPP So*.*)
set(COIN_DOCUMENTATION_FILES
	${COIN_DOCUMENTATION_FILES}
	${SOCPP}
	CACHE INTERNAL "COIN_DOCUMENTATION_FILES"
)

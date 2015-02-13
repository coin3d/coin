if(COIN_BUILD_SINGLE_LIB)
	set(COIN_SOURCE ${COIN_SOURCE}
		bundles/SoBundle.cpp
		bundles/SoMaterialBundle.cpp
		bundles/SoNormalBundle.cpp
		bundles/SoVertexAttributeBundle.cpp
		bundles/SoTextureCoordinateBundle.cpp
		CACHE INTERNAL "COIN_SOURCE"
	)
else()
	add_library(bundles OBJECT
		SoBundle.cpp
		SoMaterialBundle.cpp
		SoNormalBundle.cpp
		SoVertexAttributeBundle.cpp
		SoTextureCoordinateBundle.cpp
	)
endif()

file(GLOB_RECURSE SOCPP So*.cpp)
set(COIN_DOCUMENTATION_FILES
	${COIN_DOCUMENTATION_FILES}
	${SOCPP}
	CACHE INTERNAL "COIN_DOCUMENTATION_FILES"
)

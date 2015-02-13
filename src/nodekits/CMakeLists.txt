if(COIN_BUILD_SINGLE_LIB)
	set(COIN_SOURCE ${COIN_SOURCE}
		nodekits/SoNodeKit.cpp
		nodekits/SoNodeKitPath.cpp
		nodekits/SoNodeKitListPart.cpp
		nodekits/SoNodekitCatalog.cpp
		nodekits/SoBaseKit.cpp
		nodekits/SoAppearanceKit.cpp
		nodekits/SoCameraKit.cpp
		nodekits/SoInteractionKit.cpp
		nodekits/SoLightKit.cpp
		nodekits/SoSceneKit.cpp
		nodekits/SoSeparatorKit.cpp
		nodekits/SoShapeKit.cpp
		nodekits/SoWrapperKit.cpp
		CACHE INTERNAL "COIN_SOURCE"
	)
else()
	add_library(nodekits OBJECT
		SoNodeKit.cpp
		SoNodeKitPath.cpp
		SoNodeKitListPart.cpp
		SoNodekitCatalog.cpp
		SoBaseKit.cpp
		SoAppearanceKit.cpp
		SoCameraKit.cpp
		SoInteractionKit.cpp
		SoLightKit.cpp
		SoSceneKit.cpp
		SoSeparatorKit.cpp
		SoShapeKit.cpp
		SoWrapperKit.cpp
	)
endif()

file(GLOB_RECURSE SOCPP So*.cpp)
set(COIN_DOCUMENTATION_FILES
	${COIN_DOCUMENTATION_FILES}
	${SOCPP}
	CACHE INTERNAL "COIN_DOCUMENTATION_FILES"
)

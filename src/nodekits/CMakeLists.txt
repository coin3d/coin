# source files
set(COIN_NODEKITS_FILES
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

# build library
if(COIN_BUILD_SINGLE_LIB)
	foreach(SRCFILE ${COIN_NODEKITS_FILES})
		set(COIN_SOURCE ${COIN_SOURCE} nodekits/${SRCFILE} CACHE INTERNAL "COIN_SOURCE")
	endforeach(SRCFILE)
else()
	add_library(nodekits OBJECT ${COIN_NODEKITS_FILES})
endif()

# define documentation
foreach(SRCFILE ${COIN_NODEKITS_FILES})
	set(COIN_DOCUMENTATION_FILES ${COIN_DOCUMENTATION_FILES} ${CMAKE_SOURCE_DIR}/src/nodekits/${SRCFILE} CACHE INTERNAL "COIN_DOCUMENTATION_FILES")
endforeach(SRCFILE)

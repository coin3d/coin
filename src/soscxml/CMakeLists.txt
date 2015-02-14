# source files
set(COIN_SOSCXML_FILES
	ScXMLCoinEvaluator.cpp
	SoScXMLEvent.cpp
	SoScXMLStateMachine.cpp
	eval-coin-tab.cpp
	eval-coin.cpp
)

# build library
if(COIN_BUILD_SINGLE_LIB)
	foreach(SRCFILE ${COIN_SOSCXML_FILES})
		set(COIN_SOURCE ${COIN_SOURCE} soscxml/${SRCFILE} CACHE INTERNAL "COIN_SOURCE")
	endforeach(SRCFILE)
else()
	add_library(soscxml OBJECT ${COIN_SOSCXML_FILES})
endif()

# define documentation
file(GLOB_RECURSE SOCPP So*.cpp)
set(COIN_DOCUMENTATION_FILES
	${COIN_DOCUMENTATION_FILES}
	${SOCPP}
	CACHE INTERNAL "COIN_DOCUMENTATION_FILES"
)

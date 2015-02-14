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
foreach(SRCFILE ${COIN_SOSCXML_FILES})
	set(COIN_DOCUMENTATION_FILES ${COIN_DOCUMENTATION_FILES} ${CMAKE_SOURCE_DIR}/src/soscxml/${SRCFILE} CACHE INTERNAL "COIN_DOCUMENTATION_FILES")
endforeach(SRCFILE)

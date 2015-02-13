if(COIN_BUILD_SINGLE_LIB)
	set(COIN_SOURCE ${COIN_SOURCE}
        soscxml/ScXMLCoinEvaluator.cpp
        soscxml/SoScXMLEvent.cpp
        soscxml/SoScXMLStateMachine.cpp
        soscxml/eval-coin-tab.cpp
        soscxml/eval-coin.cpp
		CACHE INTERNAL "COIN_SOURCE"
	)
else()
	add_library(soscxml OBJECT
        ScXMLCoinEvaluator.cpp
        SoScXMLEvent.cpp
        SoScXMLStateMachine.cpp
        eval-coin-tab.cpp
        eval-coin.cpp
	)
endif()

file(GLOB_RECURSE SOCPP So*.cpp)
set(COIN_DOCUMENTATION_FILES
	${COIN_DOCUMENTATION_FILES}
	${SOCPP}
	CACHE INTERNAL "COIN_DOCUMENTATION_FILES"
)

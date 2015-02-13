if(COIN_BUILD_SINGLE_LIB)
	set(COIN_SOURCE ${COIN_SOURCE}
		events/SoButtonEvent.cpp
		events/SoEvent.cpp
		events/SoKeyboardEvent.cpp
		events/SoLocation2Event.cpp
		events/SoMotion3Event.cpp
		events/SoMouseButtonEvent.cpp
		events/SoSpaceballButtonEvent.cpp
		CACHE INTERNAL "COIN_SOURCE"
	)
else()
	add_library(events OBJECT
		SoButtonEvent.cpp
		SoEvent.cpp
		SoKeyboardEvent.cpp
		SoLocation2Event.cpp
		SoMotion3Event.cpp
		SoMouseButtonEvent.cpp
		SoSpaceballButtonEvent.cpp
	)
endif()

file(GLOB_RECURSE SOCPP So*.cpp)
set(COIN_DOCUMENTATION_FILES
	${COIN_DOCUMENTATION_FILES}
	${SOCPP}
	CACHE INTERNAL "COIN_DOCUMENTATION_FILES"
)

if(COIN_BUILD_SINGLE_LIB)
	set(COIN_SOURCE ${COIN_SOURCE}
		sensors/SoAlarmSensor.cpp
		sensors/SoDataSensor.cpp
		sensors/SoDelayQueueSensor.cpp
		sensors/SoFieldSensor.cpp
		sensors/SoIdleSensor.cpp
		sensors/SoNodeSensor.cpp
		sensors/SoOneShotSensor.cpp
		sensors/SoPathSensor.cpp
		sensors/SoSensor.cpp
		sensors/SoSensorManager.cpp
		sensors/SoTimerQueueSensor.cpp
		sensors/SoTimerSensor.cpp
		CACHE INTERNAL "COIN_SOURCE"
	)
else()
	add_library(sensors OBJECT
		SoAlarmSensor.cpp
		SoDataSensor.cpp
		SoDelayQueueSensor.cpp
		SoFieldSensor.cpp
		SoIdleSensor.cpp
		SoNodeSensor.cpp
		SoOneShotSensor.cpp
		SoPathSensor.cpp
		SoSensor.cpp
		SoSensorManager.cpp
		SoTimerQueueSensor.cpp
		SoTimerSensor.cpp
	)
endif()

file(GLOB_RECURSE SOCPP So*.cpp)
set(COIN_DOCUMENTATION_FILES
	${COIN_DOCUMENTATION_FILES}
	${SOCPP}
	CACHE INTERNAL "COIN_DOCUMENTATION_FILES"
)

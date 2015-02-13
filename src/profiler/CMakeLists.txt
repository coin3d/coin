if(COIN_BUILD_SINGLE_LIB)
	set(COIN_SOURCE ${COIN_SOURCE}
        profiler/SoProfiler.cpp
        profiler/SoProfilerElement.cpp
        profiler/SoProfilerOverlayKit.cpp
        profiler/SoProfilerStats.cpp
        profiler/SoProfilingReportGenerator.cpp
        profiler/SoProfilerTopEngine.cpp
        profiler/SoScrollingGraphKit.cpp
        profiler/SoNodeVisualize.cpp
        profiler/SoProfilerTopKit.cpp
        profiler/SoProfilerVisualizeKit.cpp
        profiler/SbProfilingData.cpp
		CACHE INTERNAL "COIN_SOURCE"
	)
else()
	add_library(profiler OBJECT
        SoProfiler.cpp
        SoProfilerElement.cpp
        SoProfilerOverlayKit.cpp
        SoProfilerStats.cpp
        SoProfilingReportGenerator.cpp
        SoProfilerTopEngine.cpp
        SoScrollingGraphKit.cpp
        SoNodeVisualize.cpp
        SoProfilerTopKit.cpp
        SoProfilerVisualizeKit.cpp
        SbProfilingData.cpp
	)
endif()

file(GLOB_RECURSE SOCPP So*.cpp)
set(COIN_DOCUMENTATION_FILES
	${COIN_DOCUMENTATION_FILES}
	${SOCPP}
	${CMAKE_SOURCE_DIR}/src/profiler/SoProfilerP.h
	${CMAKE_SOURCE_DIR}/src/profiler/SbProfilingData.cpp
	CACHE INTERNAL "COIN_DOCUMENTATION_FILES"
)

if(COIN_BUILD_SINGLE_LIB)
	set(COIN_SOURCE ${COIN_SOURCE}
		lists/SbList.cpp
		lists/SbPList.cpp
		lists/SbIntList.cpp
		lists/SbVec3fList.cpp
		lists/SbStringList.cpp
		lists/SoActionMethodList.cpp
		lists/SoAuditorList.cpp
		lists/SoBaseList.cpp
		lists/SoCallbackList.cpp
		lists/SoDetailList.cpp
		lists/SoEnabledElementsList.cpp
		lists/SoEngineList.cpp
		lists/SoEngineOutputList.cpp
		lists/SoFieldList.cpp
		lists/SoNodeList.cpp
		lists/SoPathList.cpp
		lists/SoPickedPointList.cpp
		lists/SoTypeList.cpp
		CACHE INTERNAL "COIN_SOURCE"
	)
else()
	add_library(lists OBJECT
		SbList.cpp
		SbPList.cpp
		SbIntList.cpp
		SbVec3fList.cpp
		SbStringList.cpp
		SoActionMethodList.cpp
		SoAuditorList.cpp
		SoBaseList.cpp
		SoCallbackList.cpp
		SoDetailList.cpp
		SoEnabledElementsList.cpp
		SoEngineList.cpp
		SoEngineOutputList.cpp
		SoFieldList.cpp
		SoNodeList.cpp
		SoPathList.cpp
		SoPickedPointList.cpp
		SoTypeList.cpp
	)
endif()

file(GLOB_RECURSE SOCPP So*.cpp)
file(GLOB_RECURSE SBCPP Sb*.cpp)
set(COIN_DOCUMENTATION_FILES
	${COIN_DOCUMENTATION_FILES}
	${SOCPP}
	${SBCPP}
	CACHE INTERNAL "COIN_DOCUMENTATION_FILES"
)

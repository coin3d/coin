add_subdirectory(Inventor)
install(DIRECTORY Inventor/ DESTINATION include/Inventor FILES_MATCHING PATTERN "*.h")
install(FILES SoWinEnterScope.h SoWinLeaveScope.h SoDebug.h DESTINATION include)
set(COIN_DOCUMENTATION_FILES
	${COIN_DOCUMENTATION_FILES}
	${CMAKE_SOURCE_DIR}/include/SoDebug.h
	CACHE INTERNAL "COIN_DOCUMENTATION_FILES"
)

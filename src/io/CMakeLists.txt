if(COIN_BUILD_SINGLE_LIB)
	set(COIN_SOURCE ${COIN_SOURCE}
		io/SoInput.cpp
		io/SoInput_FileInfo.cpp
		io/SoInput_Reader.cpp
		io/SoOutput.cpp
		io/SoOutput_Writer.cpp
		io/SoByteStream.cpp
		io/SoTranSender.cpp
		io/SoTranReceiver.cpp
		io/SoWriterefCounter.cpp
		io/gzmemio.cpp
		CACHE INTERNAL "COIN_SOURCE"
	)
else()
	add_library(io OBJECT
		SoInput.cpp
		SoInput_FileInfo.cpp
		SoInput_Reader.cpp
		SoOutput.cpp
		SoOutput_Writer.cpp
		SoByteStream.cpp
		SoTranSender.cpp
		SoTranReceiver.cpp
		SoWriterefCounter.cpp
		gzmemio.cpp
	)
endif()

set(COIN_DOCUMENTATION_FILES
	${COIN_DOCUMENTATION_FILES}
	${CMAKE_SOURCE_DIR}/src/io/SoInput.cpp
	${CMAKE_SOURCE_DIR}/src/io/SoInputP.h
	${CMAKE_SOURCE_DIR}/src/io/SoOutput.cpp
	${CMAKE_SOURCE_DIR}/src/io/SoWriterefCounter.cpp
	${CMAKE_SOURCE_DIR}/src/io/SoWriterefCounter.h
	CACHE INTERNAL "COIN_DOCUMENTATION_FILES"
)

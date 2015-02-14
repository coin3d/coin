# source files
set(COIN_THREADS_FILES
	common.cpp
	thread.cpp
	mutex.cpp
	rwmutex.cpp
	storage.cpp
	condvar.cpp
	worker.cpp
	wpool.cpp
	recmutex.cpp
	sched.cpp
	sync.cpp
	fifo.cpp
	barrier.cpp
)

# build library
if(COIN_BUILD_SINGLE_LIB)
	foreach(SRCFILE ${COIN_THREADS_FILES})
		set(COIN_SOURCE ${COIN_SOURCE} threads/${SRCFILE} CACHE INTERNAL "COIN_SOURCE")
	endforeach(SRCFILE)
else()
	add_library(threads OBJECT ${COIN_THREADS_FILES})
endif()

# define documentation
set(COIN_DOCUMENTATION_FILES
	${COIN_DOCUMENTATION_FILES}
	${CMAKE_SOURCE_DIR}/src/threads/barrier.cpp
	${CMAKE_SOURCE_DIR}/src/threads/barrierp.h
	${CMAKE_SOURCE_DIR}/src/threads/common.cpp
	${CMAKE_SOURCE_DIR}/src/threads/condvar.cpp
	${CMAKE_SOURCE_DIR}/src/threads/condvarp.h
	${CMAKE_SOURCE_DIR}/src/threads/fifo.cpp
	${CMAKE_SOURCE_DIR}/src/threads/fifop.h
	${CMAKE_SOURCE_DIR}/src/threads/mutex.cpp
	${CMAKE_SOURCE_DIR}/src/threads/mutexp.h
	${CMAKE_SOURCE_DIR}/src/threads/recmutex.cpp
	${CMAKE_SOURCE_DIR}/src/threads/recmutexp.h
	${CMAKE_SOURCE_DIR}/src/threads/rwmutex.cpp
	${CMAKE_SOURCE_DIR}/src/threads/rwmutexp.h
	${CMAKE_SOURCE_DIR}/src/threads/sched.cpp
	${CMAKE_SOURCE_DIR}/src/threads/schedp.h
	${CMAKE_SOURCE_DIR}/src/threads/storage.cpp
	${CMAKE_SOURCE_DIR}/src/threads/storagep.h
	${CMAKE_SOURCE_DIR}/src/threads/sync.cpp
	${CMAKE_SOURCE_DIR}/src/threads/thread.cpp
	${CMAKE_SOURCE_DIR}/src/threads/threadp.h
	${CMAKE_SOURCE_DIR}/src/threads/worker.cpp
	${CMAKE_SOURCE_DIR}/src/threads/workerp.h
	${CMAKE_SOURCE_DIR}/src/threads/wpool.cpp
	${CMAKE_SOURCE_DIR}/src/threads/wpoolp.h
	CACHE INTERNAL "COIN_DOCUMENTATION_FILES"
)

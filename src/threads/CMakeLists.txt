add_library(threads OBJECT
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

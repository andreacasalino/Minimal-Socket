add_subdirectory(SynchSocket)

option(COMPILE_ASYNCH "Compile the asynchronous cross socket package" ON)
if(COMPILE_ASYNCH)
add_subdirectory(AsynchSocket)
endif()

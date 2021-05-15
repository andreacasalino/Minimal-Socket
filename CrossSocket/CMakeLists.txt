add_subdirectory(SynchSocket)

option(COMPILE_ASYNCH "Compile the asynchronous cross socket package" ON)
if(COMPILE_ASYNCH)
add_subdirectory(AsynchSocket)
endif()

option(COMPILE_TYPED "Compile the typed cross socket package" ON)
if(COMPILE_TYPED)
add_subdirectory(TypedSocket)
endif()

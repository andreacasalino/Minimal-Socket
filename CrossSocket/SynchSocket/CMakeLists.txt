set(PROJECT_SHORTNAME "Synch-Cross-Socket")

MakeLibrary(${PROJECT_SHORTNAME} include)

if(WIN32)
  target_link_libraries(${PROJECT_SHORTNAME}
      PRIVATE
        wsock32 ws2_32
  )
endif()
find_package(Threads)
target_link_libraries(${PROJECT_SHORTNAME}
    PUBLIC
	${CMAKE_THREAD_LIBS_INIT}      
)

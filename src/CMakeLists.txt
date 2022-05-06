set(PROJECT_SHORTNAME "MinimalSocket")

MakeLibrary(${PROJECT_SHORTNAME} header)

if(WIN32)
  target_link_libraries(${PROJECT_SHORTNAME} PRIVATE wsock32 ws2_32)
endif()

find_package(Threads)
target_link_libraries(${PROJECT_SHORTNAME} PUBLIC ${CMAKE_THREAD_LIBS_INIT})

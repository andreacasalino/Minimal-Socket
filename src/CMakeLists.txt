set(LIBRARY_NAME "MinimalSocket")
set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/header)

file(GLOB_RECURSE SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/*.h)

if(LIB_OPT)
	if (WIN32)
		set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE)
	endif ()	
	
	add_library(${LIBRARY_NAME} SHARED ${SRC_FILES})
else()
	add_library(${LIBRARY_NAME} STATIC ${SRC_FILES})
endif()

target_include_directories(${LIBRARY_NAME} PUBLIC ${INCLUDE_DIR})

find_package(Threads)
target_link_libraries(${LIBRARY_NAME} PUBLIC ${CMAKE_THREAD_LIBS_INIT})
if(WIN32)
  target_link_libraries(${LIBRARY_NAME} PRIVATE wsock32 ws2_32)
endif()

install(TARGETS ${LIBRARY_NAME})
install (DIRECTORY ${INCLUDE_DIR}/ DESTINATION include/${LIBRARY_NAME} FILES_MATCHING PATTERN "*.h*")

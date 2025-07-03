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

target_include_directories(${LIBRARY_NAME} PUBLIC
    $<BUILD_INTERFACE:${INCLUDE_DIR}>
    $<INSTALL_INTERFACE:include>
)

find_package(Threads REQUIRED)
target_link_libraries(${LIBRARY_NAME} PUBLIC Threads::Threads)

if(WIN32)
    target_link_libraries(${LIBRARY_NAME} PRIVATE wsock32 ws2_32)
endif()

include(CMakePackageConfigHelpers)
configure_package_config_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/config.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/${LIBRARY_NAME}Config.cmake
    INSTALL_DESTINATION lib/cmake/${LIBRARY_NAME}
)

install(TARGETS ${LIBRARY_NAME}
    EXPORT ${LIBRARY_NAME}Targets
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION include
)

install(EXPORT ${LIBRARY_NAME}Targets
    FILE Targets.cmake
    NAMESPACE ${LIBRARY_NAME}::
    DESTINATION lib/cmake/${LIBRARY_NAME}
)

install(DIRECTORY ${INCLUDE_DIR}/ DESTINATION include FILES_MATCHING PATTERN "*.h*")

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/${LIBRARY_NAME}Config.cmake
    DESTINATION lib/cmake/${LIBRARY_NAME}
)

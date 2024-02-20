# add_executable(README README.cpp)
# target_link_libraries(README PUBLIC MinimalSocket)	

find_package (Python3 COMPONENTS Interpreter)

include(cmake/MakeSample.cmake)

add_subdirectory(utils)

set(LauncherSource ${CMAKE_CURRENT_SOURCE_DIR}/Launcher.cpp)
option(BUILD_MinimalCppSocket_AVOID_CUSTOM_TARGET "" OFF)

add_subdirectory(tcp)
add_subdirectory(udp)

# add_executable(README README.cpp)
# target_link_libraries(README PUBLIC MinimalSocket)	

project(MinimalCppSocket-Samples)

include(cmake/MakeSample.cmake)
include(cmake/MakaLauncher.cmake)

add_subdirectory(utils)

add_subdirectory(tcp)
add_subdirectory(udp)

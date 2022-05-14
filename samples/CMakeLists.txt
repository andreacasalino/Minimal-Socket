project(MinimalCppSocket-Samples)

include(cmake/MakeSample.cmake)
include(cmake/MakaLauncher.cmake)

add_subdirectory(utils)

add_subdirectory(tcp)
# add_subdirectory(udp)

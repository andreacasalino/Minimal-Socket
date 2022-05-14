project(MinimalCppSocket-Samples)

include(cmake/MakeSample.cmake)
MakeSample(Dummy)
find_package(OpenMP REQUIRED)
target_link_libraries(Dummy PUBLIC OpenMP::OpenMP_CXX)


# add_subdirectory(utils)

# add_subdirectory(tcp)
# add_subdirectory(udp)

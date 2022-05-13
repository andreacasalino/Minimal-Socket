cmake_minimum_required(VERSION 3.0 FATAL_ERROR)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(LIB_OPT "Compile shared libraries (ON) or static (OFF)" OFF)
option(BUILD_MinimalCppSocket_SAMPLES "Build the samples showing how to use Minimal Cpp Socket" ON)
option(BUILD_MinimalCppSocket_TESTS "" OFF)

# set macro-directory and find scripts
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/")

# include cmake custom functions
set(WITH_SOURCE_TREE ON)
include(GroupSources)
include(AutoCollect)
include(MakeLibrary)

set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")

project(MinimalCppSocket)
add_subdirectory(src)

if(BUILD_MinimalCppSocket_SAMPLES)
    add_subdirectory(samples)
endif()

if(BUILD_MinimalCppSocket_TESTS)
    add_subdirectory(tests)
endif()

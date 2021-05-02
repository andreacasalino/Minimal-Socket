cmake_minimum_required(VERSION 3.0 FATAL_ERROR)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(LIB_OPT "Compile shared libraries (ON) or static (OFF)" OFF)
option(BUILD_SAMPLES "Build the samples showing how to use EFG" ON)
option(BUILD_TESTS "" OFF)

# set macro-directory and find scripts
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/")

# include cmake custom functions
set(WITH_SOURCE_TREE ON)
include(GroupSources)
include(AutoCollect)
include(MakeLibrary)

project(CrossSocket)
add_subdirectory(CrossSocket)

if(BUILD_SAMPLES OR BUILD_TESTS)
	add_subdirectory(Utils)
endif()

if(BUILD_SAMPLES)
    add_subdirectory(Samples)
endif()

if(BUILD_TESTS)
    add_subdirectory(Tests)
endif()

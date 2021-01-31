cmake_minimum_required(VERSION 3.0 FATAL_ERROR)

project(Cross-Socket-Prj VERSION ${VERSION})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(LIB_OPT "Compile shared libraries (ON) or static (OFF)" OFF)
option(BUILD_SAMPLES "Build the samples showing how to use EFG" ON)

# only windows specific stuff
if(WIN32)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()

# set macro-directory and find scripts
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/")

# include cmake custom functions
set(WITH_SOURCE_TREE ON)
include(GroupSources)
include(AutoCollect)
include(Log)
include(SetInstallRPath)

add_subdirectory(CrossSocket)
if(BUILD_SAMPLES)
    add_subdirectory(Utils)
    add_subdirectory(Samples)
endif()


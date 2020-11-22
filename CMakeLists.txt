cmake_minimum_required(VERSION 3.0 FATAL_ERROR)

# project structure by https://rix0r.nl/blog/2015/08/13/cmake-guide/
# helpful tutorial https://pabloariasal.github.io/2018/02/19/its-time-to-do-cmake-right/
set(VERSION_MAJOR 0)
set(VERSION_MINOR 1)
set(VERSION_PATCH 0)
set(VERSION ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH})

option(LIB_OPT "Compile shared libraries (ON) or static (OFF)" OFF)
option(BUILD_SAMPLES "Build the samples showing how to use EFG" ON)

project(Stream-Socket-Prj VERSION ${VERSION})

# Must use GNUInstallDirs to install libraries into correct
# locations on all platforms.
include(GNUInstallDirs)
# for version.cmake generator
include(CMakePackageConfigHelpers)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
# Avoid linux warnings: Cannot create package registry file
set(CMAKE_EXPORT_NO_PACKAGE_REGISTRY ON)

# system information printout
cmake_host_system_information(RESULT NUMBER_OF_PHYSICAL_CORES QUERY NUMBER_OF_PHYSICAL_CORES)
cmake_host_system_information(RESULT OS_PLATFORM QUERY OS_PLATFORM)
cmake_host_system_information(RESULT IS_64BIT QUERY IS_64BIT)
message(STATUS "OS_PLATFORM:${OS_PLATFORM}, NUMBER_OF_PHYSICAL_CORES:${NUMBER_OF_PHYSICAL_CORES} (64BIT=${IS_64BIT})")
cmake_host_system_information(RESULT OS_NAME QUERY OS_NAME)
cmake_host_system_information(RESULT OS_RELEASE QUERY OS_RELEASE)
cmake_host_system_information(RESULT OS_VERSION QUERY OS_VERSION)
message(STATUS "OS:${OS_NAME} ${OS_RELEASE} ${OS_VERSION}")
# end of system information printout

option(DEBUG "Debug cmake build" ON)
option(DEBUG_LOG "Application Debug Logging" ON)

# only windows specific stuff
if(WIN32)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()

if(DEBUG_LOG)
add_definitions(
    # This will set the spd preprocessor log level to trace
    -DSPDLOG_ACTIVE_LEVEL=0
    # Activate the preprocessor debug and trace logging calls
    -DSPDLOG_DEBUG_ON
    -DSPDLOG_TRACE_ON
)
endif()

# allow creation of custom file sinks
add_definitions(-DSPDLOG_NO_FINAL)

if(NOT WIN32)
    # add compiler flags
    SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wno-unknown-pragmas" CACHE STRING "" FORCE)
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
    add_subdirectory(Samples)
endif()

# write a config file with version information for complete framework
set(FRAMEWORK_NAME "CrossSocket")
write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/${FRAMEWORK_NAME}ConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)
set(INSTALL_CONFIGDIR "${CMAKE_INSTALL_LIBDIR}/cmake/${FRAMEWORK_NAME}")
#Install the config, configversion and custom find modules
install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/${FRAMEWORK_NAME}ConfigVersion.cmake
    DESTINATION ${INSTALL_CONFIGDIR}
)
install(FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/${FRAMEWORK_NAME}Config.cmake.in
    DESTINATION ${INSTALL_CONFIGDIR}
    RENAME ${FRAMEWORK_NAME}Config.cmake
)


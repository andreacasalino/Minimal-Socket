include(GetCompiler)
# include this file after all other files, this uses cpack defined variables
set(BUILD_PROPERTIES_FILENAME "build.properties" CACHE INTERNAL "name of properties file")
get_compiler(COMPILER)
file(WRITE ${CMAKE_BINARY_DIR}/${BUILD_PROPERTIES_FILENAME} "// Automatically generated file by cmake\n")
if(CPACK_PACKAGE_VENDOR)
file(APPEND ${CMAKE_BINARY_DIR}/${BUILD_PROPERTIES_FILENAME} "COMPANY=${CPACK_PACKAGE_VENDOR}\n")
endif()
string(TIMESTAMP TIMESTAMP_FORMATED "%d.%m.%Y %H:%M:%S" )
file(APPEND ${CMAKE_BINARY_DIR}/${BUILD_PROPERTIES_FILENAME} "DATE=${TIMESTAMP_FORMATED}\n")
file(APPEND ${CMAKE_BINARY_DIR}/${BUILD_PROPERTIES_FILENAME} "PROJECT_NAME=${PROJECT_NAME}\n")
file(APPEND ${CMAKE_BINARY_DIR}/${BUILD_PROPERTIES_FILENAME} "VERSION=${VERSION}\n")
if(CPACK_SYSTEM_NAME)
file(APPEND ${CMAKE_BINARY_DIR}/${BUILD_PROPERTIES_FILENAME} "SYSTEM_NAME=${CPACK_SYSTEM_NAME}\n")
endif()
file(APPEND ${CMAKE_BINARY_DIR}/${BUILD_PROPERTIES_FILENAME} "COMPILER=${COMPILER}\n")
message(STATUS "writing properties file of cmake build to ${BUILD_PROPERTIES_FILENAME}")
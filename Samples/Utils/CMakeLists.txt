set(PROJECT_SHORTNAME "Sample-Utils")
project(${PROJECT_SHORTNAME} VERSION ${VERSION} LANGUAGES CXX)
string(REPLACE - _ COMPONENT_NAME ${PROJECT_NAME})

CollectSourceFiles(${CMAKE_CURRENT_SOURCE_DIR} SOURCES)

GroupSources(${CMAKE_CURRENT_SOURCE_DIR})

if(LIB_OPT)
	add_library(${PROJECT_NAME} SHARED ${SOURCES})
else()
	add_library(${PROJECT_NAME} STATIC ${SOURCES})
endif()

target_compile_features(${PROJECT_NAME}
    PUBLIC cxx_auto_type
    PRIVATE cxx_variadic_templates
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_link_libraries(${PROJECT_NAME}
  PUBLIC
	Cross-Socket
)


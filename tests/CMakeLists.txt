include(FetchContent)
FetchContent_Declare(
	catch2
	GIT_REPOSITORY https://github.com/catchorg/Catch2.git
	GIT_TAG        v3.5.0
)
FetchContent_MakeAvailable(catch2)

set(TEST_NAME "MinimalSocket-tests")

file(GLOB SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/*.h)
add_executable(${TEST_NAME} ${SRC_FILES})

target_link_libraries(${TEST_NAME} PUBLIC 
	MinimalSocket
	Catch2::Catch2
	Catch2::Catch2WithMain
)

install(TARGETS ${TEST_NAME})


project(MinimalCppSocket-Tests)

include(FetchContent)
FetchContent_Declare(
	catch2
	GIT_REPOSITORY https://github.com/catchorg/Catch2.git
	GIT_TAG        4ff8b27bb6bed4b8b309e56cd269b4f1fbc24e89
)
FetchContent_MakeAvailable(catch2)

add_subdirectory(UtilsTest)

function(MakeTest)
	set(TEST_NAME "Tests")

	file(GLOB SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
	add_executable(${TEST_NAME} ${SRC_FILES})

	target_link_libraries(${TEST_NAME} PUBLIC UtilsTest)

	install(TARGETS ${TEST_NAME})
endfunction()

MakeTest()

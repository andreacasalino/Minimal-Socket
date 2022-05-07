project(MinimalCppSocket-Tests)

if (WIN32)
	set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
endif (WIN32)
include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        f079775276a99fd4373569bf5761052a01844270
)
FetchContent_MakeAvailable(googletest)

add_subdirectory(UtilsTest)

function(MakeTest NAME)
	add_executable(${NAME} ${NAME}.cpp)

	target_link_libraries(${NAME} PUBLIC UtilsTest)
		
	install(TARGETS ${NAME})
endfunction()

MakeTest(Test00-address)
MakeTest(Test01-tcp)
MakeTest(Test01-udp)

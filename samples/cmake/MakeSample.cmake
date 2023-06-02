function(MakeApp NAME)
	add_executable(${NAME} ${NAME}.cpp)

	target_link_libraries(${NAME} PUBLIC
		Utils
		MinimalSocket
	)	
	
	set_target_properties(${NAME} PROPERTIES VS_DEBUGGER_ENVIRONMENT "PATH=%PATH%;${CMAKE_INSTALL_PREFIX}/bin/")
	
	install(TARGETS ${NAME})
endfunction()

function(MakeSample INPUT PREFIX)
    set(SCRIPT_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/../utils/Monitor.py)
    set(TARGET_NAME "${PREFIX}${INPUT}")
	
	set(BIN_LOCATION ${CMAKE_CURRENT_BINARY_DIR})
	if (CMAKE_GENERATOR MATCHES "Visual Studio")
		set(BIN_LOCATION ${BIN_LOCATION}/$<CONFIG>)
	endif()

    add_custom_target(${TARGET_NAME} ALL 
        COMMAND "${Python3_EXECUTABLE}" ${SCRIPT_LOCATION}
        "--location" "${BIN_LOCATION}"
        "--cmd" "${CMAKE_CURRENT_SOURCE_DIR}/${INPUT}"
        "--dest" "${CMAKE_CURRENT_BINARY_DIR}/report.html"
        "--sleep" "0.5"
    )
endfunction()

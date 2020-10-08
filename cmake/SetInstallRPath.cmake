function(setInstallRPath)

	cmake_parse_arguments(
        PARSED_ARGS # prefix of output variables
        "" # list of names of the boolean arguments (only defined ones will be true)
        "TARGET" # list of names of mono-valued arguments
        "INSTALL_RPATH" # list of names of multi-valued arguments (output variables are lists)
        ${ARGN} # arguments of the function to parse, here we take the all original ones
    )
		if(NOT PARSED_ARGS_TARGET)
        message(FATAL_ERROR "setInstallRPath missing TARGET")
    endif(NOT PARSED_ARGS_TARGET)

		if(NOT PARSED_ARGS_INSTALL_RPATH)
  		message(FATAL_ERROR "setInstallRPath missing INSTALL_RPATH")
    endif(NOT PARSED_ARGS_INSTALL_RPATH)

    if(WIN32)
      set(INSTALL_RPATH "")
    elseif(APPLE)
      list(TRANSFORM PARSED_ARGS_INSTALL_RPATH PREPEND "@loader_path")
      list(JOIN PARSED_ARGS_INSTALL_RPATH ";" PARSED_ARGS_INSTALL_RPATH)
    else()
      list(TRANSFORM PARSED_ARGS_INSTALL_RPATH PREPEND "$ORIGIN")
      list(JOIN PARSED_ARGS_INSTALL_RPATH ":" PARSED_ARGS_INSTALL_RPATH)
    endif()
    #message(STATUS "setting rpath: ${PARSED_ARGS_INSTALL_RPATH}")
    set_target_properties(${PARSED_ARGS_TARGET} PROPERTIES INSTALL_RPATH "${PARSED_ARGS_INSTALL_RPATH}")
endfunction(setInstallRPath)

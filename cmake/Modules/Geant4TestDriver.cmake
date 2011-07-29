#---------------------------------------------------------------------------------------------------
# Geant4 test driver
#   Script arguments: 
#     CMD command to be executed
#     OUT file to collect stdout
#     ERR file to collect stderr
#     ENV evironment VAR1=Value1;VAR2=Value2
#     CWD current working directory
#     DBG debug flag

if(DBG)
  message(STATUS "ENV=${ENV}")
endif()

#---Massage arguments---------------------------------------------------------------------------------
if(CMD)
  string(REPLACE ":" ";" _cmd ${CMD})
  if(DBG)
    message(STATUS "testdriver:CMD=${_cmd}")
  endif()
else()
  return()
endif()

if(OUT)
  set(_out OUTPUT_FILE ${OUT})
  if(DBG)
    message(STATUS "testdriver:OUT=${OUT}")
  endif()
endif()

if(ERR)
  set(_err ERROR_FILE ${ERR})
  if(DBG)
    message(STATUS "testdriver:ERR=${ERR}")
  endif()
else()
  set(_err ERROR_VARIABLE _errvar)
endif()

if(CWD)
  set(_cwd WORKING_DIRECTORY ${CWD})
  if(DBG)
    message(STATUS "testdriver:CWD=${CWD}")
  endif()
endif()

#---Set environment --------------------------------------------------------------------------------
if(ENV)
  string(REPLACE "@" "=" _env ${ENV})
  string(REPLACE ":" ";" _env ${_env})
  foreach(pair ${_env})
    string(REPLACE "=" ";" pair ${pair})
    list(GET pair 0 var)
    list(GET pair 1 val)
    set(ENV{${var}} ${val})
    if(DBG)
      message(STATUS "testdriver[ENV]:${var}==>${val}")
    endif()
  endforeach()
endif()

#---Execute the actual test ------------------------------------------------------------------------
execute_process(COMMAND ${_cmd} ${_out} ${_err} ${_cwd} RESULT_VARIABLE _rc)

#---Return error is test returned an error code of write somthing to the stderr---------------------
if(_errvar)
  message(FATAL_ERROR "output error: ${_errvar}")
endif()
if(_rc)
  message(FATAL_ERROR "error code: ${_rc}")
endif()






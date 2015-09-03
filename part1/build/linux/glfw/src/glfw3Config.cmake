# - Config file for the glfw3 package
# It defines the following variables
#   GLFW3_INCLUDE_DIR, the path where GLFW headers are located
#   GLFW3_LIBRARY_DIR, folder in which the GLFW library is located
#   GLFW3_LIBRARY, library to link against to use GLFW

set(GLFW3_VERSION "3.1.1")


####### Expanded from @PACKAGE_INIT@ by configure_package_config_file() #######
####### Any changes to this file will be overwritten by the next CMake run ####
####### The input file was glfw3Config.cmake.in                            ########

get_filename_component(PACKAGE_PREFIX_DIR "${CMAKE_CURRENT_LIST_DIR}/../../../" ABSOLUTE)

macro(set_and_check _var _file)
  set(${_var} "${_file}")
  if(NOT EXISTS "${_file}")
    message(FATAL_ERROR "File or directory ${_file} referenced by variable ${_var} does not exist !")
  endif()
endmacro()

macro(find_dependency dep)
  if (NOT ${dep}_FOUND)
    if (${ARGV1})
      set(version ${ARGV1})
    endif()
    set(exact_arg)
    if(${CMAKE_FIND_PACKAGE_NAME}_FIND_VERSION_EXACT)
      set(exact_arg EXACT)
    endif()
    set(quiet_arg)
    if(${CMAKE_FIND_PACKAGE_NAME}_FIND_QUIETLY)
      set(quiet_arg QUIET)
    endif()
    set(required_arg)
    if(${CMAKE_FIND_PACKAGE_NAME}_FIND_REQUIRED)
      set(required_arg REQUIRED)
    endif()

    find_package(${dep} ${version} ${exact_arg} ${quiet_arg} ${required_arg})
    if (NOT ${dep}_FOUND)
      set(${CMAKE_FIND_PACKAGE_NAME}_NOT_FOUND_MESSAGE "${CMAKE_FIND_PACKAGE_NAME} could not be found because dependency ${dep} could not be found.")
      set(${CMAKE_FIND_PACKAGE_NAME}_FOUND False)
      return()
    endif()
    set(required_arg)
    set(quiet_arg)
    set(exact_arg)
  endif()
endmacro()

####################################################################################

set_and_check(GLFW3_INCLUDE_DIR "${PACKAGE_PREFIX_DIR}/include")
set_and_check(GLFW3_LIBRARY_DIR "${PACKAGE_PREFIX_DIR}/lib")

find_library(GLFW3_LIBRARY "glfw3" HINTS ${GLFW3_LIBRARY_DIR})


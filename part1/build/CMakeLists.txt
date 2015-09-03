cmake_minimum_required( VERSION 2.6.0 )

# Project name
project( Part1 )

# Set extra compiler flags
if( UNIX )
  set( CMAKE_CXX_FLAGS "-W -Wall -std=c++0x" )
endif( UNIX )

# Add sources
aux_source_directory( ../src Part1_SRCS )
aux_source_directory( ../../external/cgtk Part1_SRCS )

# Where to find headers
include_directories( ../src )
include_directories( ../../external/cgtk )

# Required libraries are added to this variable
set( requiredLibs )

# Optional libraries are added to this variable
set( optionalLibs )

# Where to look for extra modules used to find libraries
set( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/modules/" )

# Where to look for external libraries (freeglut, GLEW) when running CMake
# on Windows
if( WIN32 )
  include_directories( $ENV{ASSIGNMENT3_ROOT}/external/include )
  set( CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} $ENV{ASSIGNMENT3_ROOT}/external/lib )
  MESSAGE( STATUS "CMAKE_LIBRARY_PATH: " ${CMAKE_LIBRARY_PATH} )
endif( WIN32 )

# OpenGL (GL, GLU, GLEXT)
find_package( OpenGL REQUIRED )
if( OPENGL_FOUND )
  include_directories( SYSTEM ${OPENGL_INCLUDE_DIR} )
  set( requiredLibs ${requiredLibs} ${OPENGL_LIBRARIES} )
endif( OPENGL_FOUND )

# GLFW
add_subdirectory( $ENV{ASSIGNMENT3_ROOT}/external/glfw ${CMAKE_CURRENT_BINARY_DIR}/glfw )
include_directories( SYSTEM $ENV{ASSIGNMENT3_ROOT}/external/glfw/include )

# GLEW
if( WIN32 )
  find_library( GLEW_LIBRARIES glew32.lib )
  set( requiredLibs ${requiredLibs} ${GLEW_LIBRARIES} )
endif( WIN32 )
if( UNIX )
  find_package( GLEW REQUIRED )
  if( GLEW_FOUND )
    include_directories( ${GLEW_INCLUDE_DIR} )
    set( requiredLibs ${requiredLibs} ${GLEW_LIBRARY} )
  endif( GLEW_FOUND )
endif( UNIX )

# GLM
include_directories( $ENV{ASSIGNMENT3_ROOT}/external/glm )

# AntTweakBar (uncomment this section if you want to set up a tweakbar)
 include_directories( $ENV{ASSIGNMENT3_ROOT}/external/AntTweakBar/include )
 if( WIN32 )
   find_library( ANTTWEAKBAR_LIBRARIES AntTweakBar.lib AntTweakBar64.lib
                 PATHS $ENV{ASSIGNMENT3_ROOT}/external/AntTweakBar/lib )
   set( requiredLibs ${requiredLibs} ${ANTTWEAKBAR_LIBRARIES} )
 endif( WIN32 )
 if( UNIX )
   link_directories( $ENV{ASSIGNMENT3_ROOT}/external/AntTweakBar/lib )
   set( requiredLibs ${requiredLibs} AntTweakBar )
 endif( UNIX )

# Create build files for executable
add_executable( part1 ${Part1_SRCS} )

# Link against libraries
target_link_libraries( part1 glfw ${requiredLibs} ${GLFW_LIBRARIES} ${optionalLibs} )

# Install executable
install(PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/part1 DESTINATION bin)

# Build type
set( CMAKE_BUILD_TYPE Release )

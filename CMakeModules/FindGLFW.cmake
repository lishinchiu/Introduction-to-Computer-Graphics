FIND_PATH(GLFW_INCLUDE_DIR GLFW/glfw3.h DOC "Path to GLFW include directory."
  HINTS
  $ENV{GLFW_ROOT}
  PATH_SUFFIX include
  PATHS
  common
  /usr/include/
  /usr/local/include/
  /usr/include/GLFW
  /usr/local/include/GLFW
  ${GLFW_ROOT_DIR}/include/ 
)

FIND_LIBRARY(GLFW_LIBRARY DOC "Absolute path to GLFW library."
  NAMES glfw GLFW.lib glfw3
  HINTS
  $ENV{GLFW_ROOT}
  PATH_SUFFIXES lib/x64 release debug
  PATHS
  common/GLFW
  /usr/lib
  /usr/lib64
  /usr/lib/x86_64-linux-gnu
  /usr/lib/arm-linux-gnueabihf
  /usr/local/lib
  /usr/local/lib64
  /sw/lib
  /opt/local/lib
  ${GLFW_ROOT_DIR}/lib-msvc100
  ${GLFW_ROOT_DIR}/lib-msvc110
  ${GLFW_ROOT_DIR}/lib-msvc120
  ${GLFW_ROOT_DIR}/lib
)

SET(GLFW_FOUND 0)
IF(GLFW_LIBRARY AND GLFW_INCLUDE_DIR)
  SET(GLFW_FOUND 1)
  #message(STATUS "GLFW found!")
ENDIF(GLFW_LIBRARY AND GLFW_INCLUDE_DIR)

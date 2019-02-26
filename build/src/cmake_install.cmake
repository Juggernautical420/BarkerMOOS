# Install script for directory: /Users/jasonbarker/moos-ivp-jbbarker/src

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "None")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("/Users/jasonbarker/moos-ivp-jbbarker/build/src/lib_behaviors-test/cmake_install.cmake")
  include("/Users/jasonbarker/moos-ivp-jbbarker/build/src/pXRelayTest/cmake_install.cmake")
  include("/Users/jasonbarker/moos-ivp-jbbarker/build/src/pExampleApp/cmake_install.cmake")
  include("/Users/jasonbarker/moos-ivp-jbbarker/build/src/pPrimeFactor/cmake_install.cmake")
  include("/Users/jasonbarker/moos-ivp-jbbarker/build/src/pOdometry/cmake_install.cmake")
  include("/Users/jasonbarker/moos-ivp-jbbarker/build/src/pCommunicationAngle_jbbarker/cmake_install.cmake")

endif()


#--------------------------------------------------------
# The CMakeLists.txt for:                    lib_traffic
# Author(s):                                Jason Barker
#--------------------------------------------------------

SET(SRC 
  Manifest.cpp
  ManifestSet.cpp
  Populator_ManifestSet.cpp
  TrafficObject.cpp
  TrafficScheme.cpp
  PopulatorTest.cpp
  MBUtils.cpp
  FileBuffer.cpp)

ADD_LIBRARY(traffic ${SRC})


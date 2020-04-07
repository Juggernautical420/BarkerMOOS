#--------------------------------------------------------
# The CMakeLists.txt for:                       pTrafficPopulate
# Author(s):                              Jason Barker
#--------------------------------------------------------

SET(SRC
  TrafficPopulate.cpp
  TrafficPopulate_Info.cpp
  main.cpp
)

ADD_EXECUTABLE(pTrafficPopulate ${SRC})

TARGET_LINK_LIBRARIES(pTrafficPopulate
   ${MOOS_LIBRARIES}
   apputil
   mbutil
   geometry
   traffic
   m
   pthread)


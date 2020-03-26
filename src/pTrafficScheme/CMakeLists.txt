#--------------------------------------------------------
# The CMakeLists.txt for:                       pTrafficScheme
# Author(s):                              Jason Barker
#--------------------------------------------------------

SET(SRC
  TrafficScheme.cpp
  TrafficScheme_Info.cpp
  main.cpp
)

ADD_EXECUTABLE(pTrafficScheme ${SRC})

TARGET_LINK_LIBRARIES(pTrafficScheme
   ${MOOS_LIBRARIES}
   apputil
   mbutil
   m
   pthread)


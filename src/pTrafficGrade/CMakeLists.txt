#--------------------------------------------------------
# The CMakeLists.txt for:                       pTrafficGrade
# Author(s):                              Jason Barker
#--------------------------------------------------------

SET(SRC
  TrafficGrade.cpp
  TrafficGrade_Info.cpp
  main.cpp
)

ADD_EXECUTABLE(pTrafficGrade ${SRC})

TARGET_LINK_LIBRARIES(pTrafficGrade
   ${MOOS_LIBRARIES}
   apputil
   mbutil
   m
   pthread)


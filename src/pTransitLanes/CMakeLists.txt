#--------------------------------------------------------
# The CMakeLists.txt for:                       pTransitLanes
# Author(s):                              Jason Barker
#--------------------------------------------------------

SET(SRC
  TransitLanes.cpp
  TransitLanes_Info.cpp
  main.cpp
)

ADD_EXECUTABLE(pTransitLanes ${SRC})

TARGET_LINK_LIBRARIES(pTransitLanes
   ${MOOS_LIBRARIES}
   apputil
   mbutil
   m
   geometry
   pthread)


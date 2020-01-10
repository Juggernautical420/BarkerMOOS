#--------------------------------------------------------
# The CMakeLists.txt for:                       pTransitAlpha
# Author(s):                              Jason Barker
#--------------------------------------------------------

SET(SRC
  TransitAlpha.cpp
  TransitAlpha_Info.cpp
  main.cpp
)

ADD_EXECUTABLE(pTransitAlpha ${SRC})

TARGET_LINK_LIBRARIES(pTransitAlpha
   ${MOOS_LIBRARIES}
   apputil
   mbutil
   m
   geometry
   pthread)


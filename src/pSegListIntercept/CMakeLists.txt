#--------------------------------------------------------
# The CMakeLists.txt for:                       pSegListIntercept
# Author(s):                              Jason Barker
#--------------------------------------------------------

SET(SRC
  SegListIntercept.cpp
  SegListIntercept_Info.cpp
  main.cpp
)

ADD_EXECUTABLE(pSegListIntercept ${SRC})

TARGET_LINK_LIBRARIES(pSegListIntercept
   ${MOOS_LIBRARIES}
   geometry
   apputil
   mbutil
   m
   intercept
   pthread)


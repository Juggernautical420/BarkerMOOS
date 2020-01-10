#--------------------------------------------------------
# The CMakeLists.txt for:                       pSegPassing
# Author(s):                              Jason Barker
#--------------------------------------------------------

SET(SRC
  SegPassing.cpp
  SegPassing_Info.cpp
  main.cpp
)

ADD_EXECUTABLE(pSegPassing ${SRC})

TARGET_LINK_LIBRARIES(pSegPassing
   ${MOOS_LIBRARIES}
   apputil
   mbutil
   m
   pthread)


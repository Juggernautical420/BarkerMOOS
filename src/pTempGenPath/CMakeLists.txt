#--------------------------------------------------------
# The CMakeLists.txt for:                       pTempGenPath
# Author(s):                              Jason Barker
#--------------------------------------------------------

SET(SRC
  TempGenPath.cpp
  TempGenPath_Info.cpp
  main.cpp
)

ADD_EXECUTABLE(pTempGenPath ${SRC})

TARGET_LINK_LIBRARIES(pTempGenPath
   ${MOOS_LIBRARIES}
   apputil
   mbutil
   m
   geometry
   pthread)


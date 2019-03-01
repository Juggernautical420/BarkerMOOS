#--------------------------------------------------------
# The CMakeLists.txt for:                       pCommunicationAngle_jbbarker
# Author(s):                              Jason Barker
#--------------------------------------------------------

SET(SRC
  CommunicationAngle_jbbarker.cpp
  CommunicationAngle_jbbarker_Info.cpp
  main.cpp
  AngleCalcs.cpp
)

ADD_EXECUTABLE(pCommunicationAngle_jbbarker ${SRC})

TARGET_LINK_LIBRARIES(pCommunicationAngle_jbbarker
   ${MOOS_LIBRARIES}
   mbutil
   m
   pthread)


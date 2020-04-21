#--------------------------------------------------------
# The CMakeLists.txt for:                       pTSSCompliance
# Author(s):                              Jason Barker
#--------------------------------------------------------

SET(SRC
  TSSCompliance.cpp
  TSSCompliance_Info.cpp
  main.cpp
)

ADD_EXECUTABLE(pTSSCompliance ${SRC})

TARGET_LINK_LIBRARIES(pTSSCompliance
   ${MOOS_LIBRARIES}
   apputil
   mbutil
   m
   traffic
   geometry
   pthread)


#--------------------------------------------------------
# The CMakeLists.txt for:                       pSegDetailing
# Author(s):                              Jason Barker
#--------------------------------------------------------

SET(SRC
  SegDetailing.cpp
  SegDetailing_Info.cpp
  main.cpp
)

ADD_EXECUTABLE(pSegDetailing ${SRC})

TARGET_LINK_LIBRARIES(pSegDetailing
   ${MOOS_LIBRARIES}
   apputil
   mbutil
   m
   pthread)


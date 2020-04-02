#--------------------------------------------------------
# The CMakeLists.txt for:                     app_buildtss
# Author(s):                                Jason Barker
#--------------------------------------------------------


SET(SYSTEM_LIBS m)

SET(SRC 
  main.cpp
  )

ADD_EXECUTABLE(buildtss ${SRC})

TARGET_LINK_LIBRARIES(buildtss 
  mbutil
  traffic
  geometry
  ${SYSTEM_LIBS}
)
  


